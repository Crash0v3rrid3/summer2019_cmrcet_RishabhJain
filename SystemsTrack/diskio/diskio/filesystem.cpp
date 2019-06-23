#include "filesystem.h"
#include "diskio.h"
#include <stdio.h>
#include "sanity_wrappers.h"
#include <string.h>
#include <stdlib.h>

int isFileSystemMounted(FileSystem *fs) {
	if (!fs->metadata || !fs->filetable) {
		printf("Please mount filesystem first!\n");
		return 0;
	}
	return 1;
}

unsigned int getOrSetblockSize(unsigned int bs) {
	static unsigned int blockSize = bs;

	if (bs)  {					// Set block size, if bs != 0
		blockSize = bs;
	}

	return blockSize;
}

/*char *getOrSetHarddiskFilename(char *harddiskname) {
	//static char *harddiskfile = 
}*/

void *readBytes(unsigned int numberOfBytes, unsigned int blockNumber) {
	void *buf = (void *)calloc(numberOfBytes, 1), *diskData;
	unsigned int bytesToRead = numberOfBytes, bytesRead = 0, intermediateBytes;
	unsigned int blockSize = getOrSetblockSize(0);

	while (bytesToRead > 0) {
		intermediateBytes = ((bytesToRead > blockSize) ? blockSize : bytesToRead);
		diskData = readBlock(blockNumber);
		memcpy(buf, diskData, intermediateBytes);
		bytesRead += intermediateBytes;
		bytesToRead -= (bytesToRead > blockSize) ? blockSize : bytesToRead;
		blockNumber += 1;

		free(diskData);
		diskData = NULL;
	}

	return buf;
}

void writeBytes(void *buf, unsigned int numberOfBytes, unsigned int blockNumber) {
	unsigned int blockSize = getOrSetblockSize(0);

	while (numberOfBytes >= blockSize) {
		writeBlock(buf, blockNumber);
		blockNumber++;
		numberOfBytes -= blockSize;
		buf = ((char *)buf + blockSize);
	}

	if (numberOfBytes) {
		void *tmp = (void *)calloc(blockSize, 1);
		memcpy(tmp, buf, numberOfBytes);
		writeBlock(tmp, blockNumber);
	}
}

void updateMetadataInFS(FileSystem *fs) {
	unsigned int totalNumberOfMetadataBytes = (sizeof(Metadata) - 4 + fs->metadata->totalNumberOfBlocks),
				totalNumberOfFileTableBytes = (4 + fs->filetable->numberOfFiles * sizeof(FileRecord));
	void *buf = (void *)calloc(totalNumberOfFileTableBytes + totalNumberOfMetadataBytes, 1);	// For filetable
	//Copy metadata
	memcpy(buf, 
		fs->metadata, 
		totalNumberOfMetadataBytes);

	// copy filetable
	memcpy((char *)buf + totalNumberOfMetadataBytes, 
		fs->filetable, 
		totalNumberOfFileTableBytes);

	// TODO: Modify byte vector

	writeBytes(buf, totalNumberOfFileTableBytes + totalNumberOfMetadataBytes, 0);
}

void mount(FileSystem *fs, char *harddiskFileName, unsigned int newBlockSize) {
	getOrSetblockSize(newBlockSize);
	int numberOfBlocks = init(harddiskFileName, newBlockSize);
	void *buf = readBytes(LENGTH_OF_STATIC_CONTENT_IN_METADATA, 0);
	unsigned int bytesToRead;

	fs->metadata = (Metadata *)calloc(LENGTH_OF_STATIC_CONTENT_IN_METADATA, 1);
	memcpy(fs->metadata, buf, LENGTH_OF_STATIC_CONTENT_IN_METADATA);

	free(buf);
	buf = NULL;

	if (fs->metadata->magicNumber != MAGIC_NUMBER || fs->metadata->blockSize != newBlockSize) {
		fs->metadata->totalNumberOfBlocks = numberOfBlocks;
		printf("Disk is corrupted!\n");
		format(fs, newBlockSize);
	}	else	{
		// Number of Bytes to read for byte table
		bytesToRead = LENGTH_OF_STATIC_CONTENT_IN_METADATA
					+ fs->metadata->totalNumberOfBlocks;
		//Number of bytesToRead for fileTable
		bytesToRead += 4;	// # of files

		buf = readBytes(bytesToRead, LENGTH_OF_STATIC_CONTENT_IN_METADATA / newBlockSize);
		fs->metadata = (Metadata *)realloc(fs->metadata, LENGTH_OF_STATIC_CONTENT_IN_METADATA + numberOfBlocks);

		// Copy byte vector
		memcpy(fs->metadata->byteVector, (char *)buf + LENGTH_OF_STATIC_CONTENT_IN_METADATA, numberOfBlocks);

		// Copy number of files
		unsigned int numberOfFiles = *(int *)((char *)buf + LENGTH_OF_STATIC_CONTENT_IN_METADATA + numberOfBlocks);

		//free(buf);
		buf = NULL;

		//Read FileRecords
		bytesToRead = LENGTH_OF_STATIC_CONTENT_IN_METADATA
			+ fs->metadata->totalNumberOfBlocks
			+ 4
			+ numberOfFiles * sizeof(FileRecord);

		buf = readBytes(bytesToRead, (LENGTH_OF_STATIC_CONTENT_IN_METADATA / newBlockSize));

		// Allocate memory
		fs->filetable = (FileTable *)calloc(4 + numberOfFiles * sizeof(FileRecord), 1);
		fs->filetable->numberOfFiles = numberOfFiles;

		//Copy to filetable
		memcpy(fs->filetable->files, 
			(char *)buf + LENGTH_OF_STATIC_CONTENT_IN_METADATA + numberOfFiles, 
			4 + numberOfFiles * sizeof(FileTable));
	}
}

long long findNextFreeBlock(FileSystem *fs) {
	unsigned int itr;

	for (itr = DATA_START_BLOCK; itr < fs->metadata->totalNumberOfBlocks; ++itr) {
		if (fs->metadata->byteVector[itr])
			return itr;
	}

	return -1;
}

unsigned int isFileAllocatable(FileSystem *fs, FileRecord file) {
	return (file.numberOfBlocks < fs->metadata->numberOfEmptyBlocks);
}

void addFileEntry(FileSystem *fs, FileRecord file) {
	if (fs->filetable->numberOfFiles > 0) {
		fs->filetable = (FileTable *)realloc(fs->filetable, 4 + (fs->filetable->numberOfFiles + 1) * sizeof(FileRecord));
	}	else	{
		fs->filetable = (FileTable *)calloc(4 + sizeof(FileRecord), 1);
	}
	memcpy(fs->filetable->files + fs->filetable->numberOfFiles, &file, sizeof(FileRecord));
	fs->filetable->numberOfFiles++;
}

unsigned int *findNFreeBlocks(FileSystem *fs, unsigned int N) {
	unsigned int itr, *result = (unsigned int *)calloc(N, sizeof(unsigned int)), *resultPtr = result;
	for (itr = DATA_START_BLOCK; itr < fs->metadata->totalNumberOfBlocks; --itr) {
		if (!N) break;
		if (fs->metadata->byteVector[itr]) {
			*resultPtr++ = itr;
			N--;
		}
	}
	if (!N)
		return result;
}

void setByteVectors(FileSystem *fs, unsigned int blockNumber, unsigned int flag) {
	unsigned int itr;

	fs->metadata->byteVector[blockNumber] = flag;
}

int copyToFS(FileSystem *fs, char *sourceFileName, char *destFileName) {
	if (!isFileSystemMounted(fs)) {
		return 0;
	}
	FILE *sourceFile;
	FileRecord file;
	unsigned int blockSize = getOrSetblockSize(0), itr;
	unsigned int *fileBlocks;
	void *buf;

	// initialize source file
	didFileOpen(fopen_s(&sourceFile, sourceFileName, "rb"));

	//initialize destinaton file
	// Name of file
	strncpy(file.filename, destFileName, 20);
	fseek(sourceFile, 0, SEEK_END);
	file.size = ftell(sourceFile);
	file.numberOfBlocks = (file.size / blockSize) + (file.size % blockSize > 0);

	//Check if free space is available
	if (!isFileAllocatable(fs, file)) return 0;
	fileBlocks = findNFreeBlocks(fs, file.numberOfBlocks + 1);
	file.startBlockOrDataBlock = *fileBlocks;
	
	//Add file to file table
	addFileEntry(fs, file);

	if (file.numberOfBlocks == 1) {	
		buf = calloc(blockSize, 1);

		// Seek back
		wasSeekSuccessful(fseek(sourceFile, 0, SEEK_SET));
	
		//Read file 
		checkEOF(fread(buf, file.size, 1, sourceFile));

		//Write File
		writeBlock(buf, file.startBlockOrDataBlock);

		//Flag to not empty
		setByteVectors(fs, file.startBlockOrDataBlock, 0);

		//Clean up
		free(buf);
		buf = NULL;
	}	else	{
		buf = (void *)calloc(file.numberOfBlocks * sizeof(unsigned int), 1);
		
		memcpy(buf, fileBlocks + 1, file.numberOfBlocks * sizeof(unsigned int));
		writeBytes(buf, file.numberOfBlocks * sizeof(unsigned int), file.startBlockOrDataBlock);
		setByteVectors(fs, file.startBlockOrDataBlock, 1);
		
		for (itr = 1; itr <= file.numberOfBlocks; ++itr) {
			fread(buf, blockSize, 1, sourceFile);
			writeBlock(buf, fileBlocks[itr]);
			setByteVectors(fs, fileBlocks[itr], 0);
		}
	}

	updateMetadataInFS(fs);

	fclose(sourceFile);
}

int searchFileInFileTable(FileSystem *fs, char *name) {
	unsigned int itr;

	for (itr = 0; itr < fs->filetable->numberOfFiles; ++itr) {
		if (!strcmp(fs->filetable->files[itr].filename, name)) {
			return itr;
		}
	}

	return itr;
}

void copyFromFS(FileSystem *fs, char *sourceFileName, char *destFileName) {
	if (!isFileSystemMounted(fs)) {
		return;
	}
	int fileID = searchFileInFileTable(fs, sourceFileName);

	if (fileID < 0) {
		printf("File doesn't exitst!\n");
		return;
	}

	FileRecord *sourceFile = &fs->filetable->files[fileID];
	FILE *destFile;
	unsigned int blockSize = getOrSetblockSize(0);
	didFileOpen(fopen_s(&destFile, destFileName, "wb+"));
	unsigned int bytesToWrite, itr, *blocks;
	void *buf;

	if (sourceFile->numberOfBlocks == 1) {
		buf = readBlock(sourceFile->startBlockOrDataBlock);
		checkBytesWritten(fwrite(buf, sourceFile->size, 1, destFile));

		free(buf);
		buf = NULL;
	}	else	{
		blocks = (unsigned int *)readBlock(sourceFile->startBlockOrDataBlock);
		bytesToWrite = sourceFile->size;
		for (itr = 0; itr < sourceFile->numberOfBlocks; ++itr) {
			buf = readBlock(blocks[itr]);
			//if (bytesToWrite < blockSize)
			checkBytesWritten(fwrite(buf, ((bytesToWrite < blockSize) ? bytesToWrite : blockSize), 1, destFile));
			bytesToWrite -= blockSize;
		}
	}

	fclose(destFile);
}

void format(FileSystem *fs, unsigned int blockSize) {
	/*if (!isFileSystemMounted(fs)) {
		return;
	}*/
	//Confirm
	char op;
	printf("Are you sure? ");
	fflush(stdin);
	scanf("%c", &op);
	if (op == 'y' || op == 'Y') {
		unsigned int numberOfBlocks = fs->metadata->totalNumberOfBlocks;
		if (fs->metadata) {
			free(fs->metadata);
			fs->metadata = NULL;
		}

		// Set Valid data
		fs->metadata = (Metadata *)calloc(sizeof(Metadata) + numberOfBlocks - 4, 1);
		fs->metadata->magicNumber = MAGIC_NUMBER;
		fs->metadata->blockSize = blockSize;
		memset(fs->metadata->byteVector, 1, numberOfBlocks);
		fs->metadata->totalNumberOfBlocks = numberOfBlocks;
		fs->metadata->numberOfEmptyBlocks = numberOfBlocks - DATA_START_BLOCK;
		fs->filetable = (FileTable *)calloc(sizeof(FileTable), 1);
		fs->filetable->numberOfFiles = 0;

		void *buf = (void *)calloc(sizeof(Metadata) + numberOfBlocks - 4 + 4, 1);
		memcpy(buf, fs->metadata, sizeof(Metadata) + numberOfBlocks - 4);

		updateMetadataInFS(fs);
	}	else	{
		exit(1);
	}
}

void list(FileSystem *fs) {
	if (!isFileSystemMounted(fs)) {
		return;
	}
	unsigned int itr = 0;

	for (; itr < fs->filetable->numberOfFiles; ++itr) {
		puts(fs->filetable->files[itr].filename);
	}
}

int deleteFile(FileSystem *fs, char *filename) {
	if (!isFileSystemMounted(fs)) {
		return 0;
	}
	unsigned int itr;
	int fileID = searchFileInFileTable(fs, filename);

	if (fileID < 0) {
		printf("File doesn't exitst!\n");
		return 0;
	}

	memcpy(fs->filetable->files + fs->filetable->numberOfFiles - 1, fs->filetable->files + fileID, sizeof(FileRecord));
	fs->filetable->numberOfFiles--;
	updateMetadataInFS(fs);

	return 1;
}

void printDebugInformation(FileSystem *fs) {
	unsigned int itr;

	printf("Block Size: %u\n", fs->metadata->blockSize);
	printf("Total number of blocks: %u\n", fs->metadata->totalNumberOfBlocks);
	printf("Number of empty blocks: %u\n", fs->metadata->numberOfEmptyBlocks);
	printf("Number of files: %u\n", fs->filetable->numberOfFiles);

	for (itr = 0; itr < fs->filetable->numberOfFiles; ++itr) {
		printf("[\"%s\", %u, %u, %u]\n", 
			fs->filetable->files[itr].filename, 
			fs->filetable->files[itr].size, 
			fs->filetable->files[itr].numberOfBlocks, 
			fs->filetable->files[itr].startBlockOrDataBlock);
	}
}

void unmount(FileSystem *fs) {
	if (fs->metadata) {
		free(fs->metadata);
		fs->metadata = NULL;
	}

	if (fs->filetable) {
		free(fs->filetable);
		fs->filetable = NULL;
	}
}