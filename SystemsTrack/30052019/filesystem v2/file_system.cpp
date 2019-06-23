#include "file_system.h"
#include "disk.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define _CRT_SECURE_NO_WARNINGS

unsigned int totalBlocksRequiredForStoringFile(unsigned int length) {
	return floor((double)(length / BLOCK_SIZE + 1));
}

void checkEOF(int ret) {
	if (ret == EOF) exit(1);
}

void checkBytesWritten(unsigned int ret) {
	if (ret == 0)  {
		printf("Inavlid number of bytes written\n");
		exit(1);
	}
}

void didFileOpen(errno_t ret) {
	if (ret) {
		printf("File couldn't open\n");
		exit(1);
	}
}

void wasSeekSuccessful(int ret) {
	if (ret) {
		printf("Seek was not successful\n");
		exit(1);
	}
}

void initFileSystem(FileSystem *filesystem, char *filename) {
	unsigned int itr;
	File *file = NULL;
	//Open file
	didFileOpen(fopen_s(&(filesystem->file), filename, "rb+"));

	// Verify Magic Number
	void *block = readBlock(filesystem, (unsigned int)0);
	if (MAGIC_NUMBER != *(int *)block) {
		printf("Hard disk is corrupted!\n");
		formatFileSystem(filesystem);
	}	else {
		memcpy(&filesystem->magicNumber, block, sizeof(FileSystem) - sizeof(FILE *));
	}

	
}

unsigned int findNextFreeBlock(FileSystem *filesystem, File *file) {
	unsigned int itr, itr2;

	for (itr = 0; itr < sizeof filesystem->freeBlockFlags; ++itr) {
		if (filesystem->freeBlockFlags[itr]) {	//Naive, no best oR worst fit
			for (itr2 = itr; itr2 < sizeof filesystem->freeBlockFlags && itr2 < file->numberOfBlocks && filesystem->freeBlockFlags[itr2]; ++itr2);
			if (itr2 == file->numberOfBlocks)
			return itr + DATA_START_BLOCK_OFFSET;
		}
	}
}

void addFileEntry(FileSystem *filesystem, File *file) {
	int offsetToStoreFileEntry = METADATA_TABLE_BLOCK_OFFSET + filesystem->numberOfFiles * sizeof(File), itr;

	// Read data from source file and add to filesystem
	if (filesystem->numberOfFiles > 0) {
		file->offset = findNextFreeBlock(filesystem, file);
	}
	else	{
		file->offset = DATA_START_BLOCK_OFFSET;
	}

	// Add file to filetable
	memcpy(&filesystem->fileTable[filesystem->numberOfFiles], file, sizeof(File));
	filesystem->numberOfFiles++;
	
	for (itr = 0; itr < file->numberOfBlocks; ++itr) {
		filesystem->freeBlockFlags[itr + file->offset - DATA_START_BLOCK_OFFSET] = 0;
	}

	void *buf = (void *)calloc(1, BLOCK_SIZE);

	memcpy(buf, &filesystem->magicNumber, sizeof(FileSystem) - sizeof(FILE *));
	writeBlock(filesystem, 0, buf);
	free(buf);
	buf = NULL;
}

int findBestFit(FileSystem *filesystem, File *file) {
	return -1;
}

int isFileAllocatable(FileSystem *filesystem, File *file) {
	int itr;

	if (BLOCK_SIZE * filesystem->numberOfFreeBlocks >= file->length) {
		return 1;
	}

	return 0;
}

int addNewFileToFileSystem(FileSystem *filesystem, File *file, FILE *sourceFile) {
	if (!isFileAllocatable(filesystem, file)) {
		return 0;
	}
	void *buf = (void *)calloc(1, BLOCK_SIZE);
	unsigned int itr, length;
	file->offset = findNextFreeBlock(filesystem, file);
	
	addFileEntry(filesystem, file);

	wasSeekSuccessful(fseek(sourceFile, 0, 0));
	// Write block
	for (itr = 0; itr < file->numberOfBlocks; ++itr) {
		memset(buf, 0, BLOCK_SIZE);
		length = fread(buf, 1, BLOCK_SIZE, sourceFile);
		writeBlock(filesystem, file->offset + itr, buf);
	}

	free(buf);
	buf = NULL;

	return 1;
}

void copyFileToDisk(FileSystem *filesystem, char *sourceName, char *destinationName) {
	FILE *source = NULL;
	File *destFile = (File *)calloc(1, sizeof(File));
	strncpy(destFile->name, destinationName, sizeof(destFile->name));

	didFileOpen(fopen_s(&source, sourceName, "rb+"));
	wasSeekSuccessful(fseek(source, 0, SEEK_END));
	destFile->length = ftell(source);
	destFile->numberOfBlocks = floor((double)(destFile->length / BLOCK_SIZE) + 1);

	if (source)
		if (!addNewFileToFileSystem(filesystem, destFile, source)) {
			printf("You're out of storage space, clean!\n");
		}

	fclose(source);
}

int getFileFromName(FileSystem *filesystem, char *name) {
	unsigned int itr;

	for (itr = 0; itr < filesystem->numberOfFiles; ++itr) {
		if (!strcmp(name, filesystem->fileTable[itr].name)) {
			return itr;
		}
	}
}

void copyFileFromDisk(FileSystem *filesystem, char *sourceName, char *destinationName) {
	FILE *dest = NULL;
	unsigned int length, itr;
	void *buf = NULL;
	File *file = &filesystem->fileTable[getFileFromName(filesystem, sourceName)];
	length = file->length;

	didFileOpen(fopen_s(&dest, destinationName, "wb+"));


	for (itr = 0; itr < file->numberOfBlocks; ++itr) {
		buf = readBlock(filesystem, itr + file->offset);
		length = (itr == (file->numberOfBlocks - 1)) ? file->length % BLOCK_SIZE : BLOCK_SIZE;
		checkBytesWritten(fwrite(buf, 1, length, dest));
		free(buf);
		buf = NULL;
	}

	fclose(dest);
}

void deleteFile(FileSystem *filesystem, char *filename) {
	int itr, fileId = getFileFromName(filesystem, filename);
	void *buf = (void *)calloc(1, BLOCK_SIZE);

	if (fileId < 0) return;

	for (itr = 0; itr < filesystem->fileTable[fileId].numberOfBlocks; ++itr) {
		filesystem->freeBlockFlags[itr + filesystem->fileTable[fileId].offset - DATA_START_BLOCK_OFFSET] = 1;
	}

	memcpy(filesystem->fileTable + fileId, filesystem->fileTable + filesystem->numberOfFiles - 1, sizeof(File));
	filesystem->numberOfFiles--;
	memcpy(buf, &filesystem->magicNumber, sizeof(FileSystem) - sizeof(FILE *));
	writeBlock(filesystem, 0, buf);

	free(buf);
	buf = NULL;
}

void listFiles(FileSystem filesystem) {
	int itr;
	for (itr = 0; itr < filesystem.numberOfFiles; ++itr) {
		printf("%s\n", filesystem.fileTable[itr].name);
	}
}

void formatFileSystem(FileSystem *filesystem) {
	char op;
	unsigned int itr = MAGIC_NUMBER;
	printf("You want to format? ");
	scanf("%c", &op);

	if (op == 'y' || op == 'Y') {
		void *buf = (void *)calloc(1, BLOCK_SIZE);
		for (itr = 0; itr < TOTAL_BLOCKS; ++itr) {
			writeBlock(filesystem, itr, buf);
		}

		filesystem->numberOfFreeBlocks = TOTAL_BLOCKS;
		filesystem->magicNumber = MAGIC_NUMBER;
		memset(filesystem->fileTable, 0, sizeof(filesystem->fileTable));
		filesystem->numberOfFiles = 0;
		filesystem->numberOfFreeBlocks = TOTAL_BLOCKS;
		memset(filesystem->freeBlockFlags, 1, filesystem->numberOfFreeBlocks);
		memcpy(buf, &filesystem->magicNumber, sizeof(FileSystem) - sizeof(FILE *));
		
		writeBlock(filesystem, 0, buf);
		
		free(buf);
		buf = NULL;
	}
}

void printDebugInformation(FileSystem filesystem) {
	int itr;

	printf("Number of files in disk: %u\n", filesystem.numberOfFiles);
	for (itr = 0; itr < filesystem.numberOfFiles; ++itr) {
		printf("[\"%s\", %u, %u, %u]\n", filesystem.fileTable[itr].name,
			filesystem.fileTable[itr].offset,
			filesystem.fileTable[itr].length,
			(int)(filesystem.fileTable[itr].length / BLOCK_SIZE) + ((filesystem.fileTable[itr].length % BLOCK_SIZE) > 0));										
	}

	printf("Number of free blocks: %u\n", filesystem.numberOfFreeBlocks);
}


void closeFileSystem(FileSystem *filesystem) {
	fclose(filesystem->file);
}