#include "file_system.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS

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
	int itr;
	File *file = NULL;

	//Open file
	didFileOpen(fopen_s(&(filesystem->file), filename, "rb+"));

	wasSeekSuccessful(fseek(filesystem->file, 0, 0));
	// Read number of files -> 4 bytes
	checkEOF(fread(&(filesystem->numberOfFiles), sizeof(unsigned int), 1, filesystem->file));
	// Read free space -> 4 bytes
	checkEOF(fread(&(filesystem->freeSpace), sizeof(unsigned int), 1, filesystem->file));
	if (filesystem->numberOfFiles == 0) {
		filesystem->freeSpace = FILESYSTEM_SIZE;
	}

	if (filesystem->numberOfFiles > 0) {
		filesystem->fileTable = (File **)calloc(filesystem->numberOfFiles, sizeof(File *));
		for (itr = 0; itr < filesystem->numberOfFiles; ++itr) {
			// Load file table
			//Create new File entry
			file = (File *)calloc(1, sizeof(File));
			// Read it
			checkEOF(fread(file, sizeof(File), 1, filesystem->file));
			// Save it in file table
			filesystem->fileTable[itr] = file;
		}
	}
}

int findBestFit(FileSystem *filesystem, File *file) {
	return -1;
}

int isFileAllocatable(FileSystem *filesystem, File *file) {
	int itr;

	if (file->length < filesystem->freeSpace) {
		return 1;
	}

	return 0;
}

int addNewFileToFileSystem(FileSystem *filesystem, File *file, FILE *sourceFile) {
	if (!isFileAllocatable(filesystem, file)) {
		return 0;
	}
	int bestFit = findBestFit(filesystem, file);
	void *buf = calloc(file->length + 1, 1);
	wasSeekSuccessful(fseek(sourceFile, 0, 0));
	checkEOF(fread(buf, 1, file->length, sourceFile));

	if (bestFit == -1) {
		if (filesystem->fileTable) {
			filesystem->fileTable = (File **)realloc(filesystem->fileTable, (filesystem->numberOfFiles + 1) * sizeof(File *));
		}	else	{
			filesystem->fileTable = (File **)calloc(1, sizeof(File *));
		}

		// Read data from source file and add to filesystem
		if (filesystem->numberOfFiles > 0) {
			file->offset = filesystem->fileTable[filesystem->numberOfFiles - 1]->offset - file->length;
		}
		else	{
			file->offset = FILESYSTEM_SIZE - file->length;
		}

		// Add file to filetable
		filesystem->fileTable[filesystem->numberOfFiles] = file;
		wasSeekSuccessful(fseek(filesystem->file, 4 + 4 + filesystem->numberOfFiles * sizeof(File), 0));
		checkBytesWritten(fwrite(file, sizeof(File), 1, filesystem->file));
		filesystem->numberOfFiles++;

		wasSeekSuccessful(fseek(filesystem->file, file->offset, 0));
		checkBytesWritten(fwrite(buf, 1, file->length, filesystem->file));


		//Decrement free space
		filesystem->freeSpace -= file->length;
		
	}	else	{

	}

	return 1;
}

void *getFileDataFromName(FileSystem *filesystem, char *fileName, int *length) {
	void *buf = NULL;
	int itr;

	for (itr = 0; itr < filesystem->numberOfFiles; ++itr) {
		if (!strcmp(filesystem->fileTable[itr]->name, fileName) && filesystem->fileTable[itr]->flag) {
			*length = filesystem->fileTable[itr]->length;
			buf = (void *)calloc(filesystem->fileTable[itr]->length, 1);

			wasSeekSuccessful(fseek(filesystem->file, filesystem->fileTable[itr]->offset, 0));
			fread(buf, 1, filesystem->fileTable[itr]->length, filesystem->file);

			return buf;
		}
	}

	return NULL;
}

void copyFileFromDisk(FileSystem *filesystem, char *sourceName, char *destinationName) {
	FILE *source = NULL;
	File *destFile = (File *)calloc(1, sizeof(File));
	strncpy(destFile->name, destinationName, sizeof(destFile->name));
	destFile->flag = 1;

	didFileOpen(fopen_s(&source, sourceName, "rb+"));
	wasSeekSuccessful(fseek(source, 0, SEEK_END));
	destFile->length = ftell(source);

	if (source)
		if (!addNewFileToFileSystem(filesystem, destFile, source)) {
			printf("You're out of storage space, clean!\n");
		}

	fclose(source);
}

void copyFileToDisk(FileSystem *filesystem, char *sourceName, char *destinationName) {
	FILE *dest = NULL;
	int length;

	didFileOpen(fopen_s(&dest, destinationName, "wb+"));
	void *buf = getFileDataFromName(filesystem, sourceName, &length);
	checkBytesWritten(fwrite(buf, 1, length, dest));

	fclose(dest);
}

void deleteFile(FileSystem *filesystem, char *filename) {
	int itr;

	for (itr = 0; itr < filesystem->freeSpace; ++itr) {
		if (!strncmp(filename, filesystem->fileTable[itr]->name, sizeof(filesystem->fileTable[itr]->name))) {
			filesystem->fileTable[itr]->flag = 0;
			wasSeekSuccessful(fseek(filesystem->file, sizeof(File) * itr, 0));
			checkBytesWritten(fwrite(filesystem->fileTable[itr], sizeof(File), 1, filesystem->file));
			return;
		}
	}
}

void listFiles(FileSystem filesystem) {
	int itr;
	for (itr = 0; itr < filesystem.numberOfFiles; ++itr) {
		if (filesystem.fileTable[itr]->flag) {
			printf("%s\n", filesystem.fileTable[itr]->name);
		}
	}
}

void formatFileSystem(FileSystem *filesystem) {
	filesystem->freeSpace = FILESYSTEM_SIZE;
	filesystem->numberOfFiles = 0;
	wasSeekSuccessful(fseek(filesystem->file, 0, 0));
	unsigned char val = 0;
	long long totalBytes = FILESYSTEM_SIZE;
	size_t block = 65534;

	while (totalBytes > 0) {
		checkBytesWritten(fwrite(&val, 1, block, filesystem->file));
		totalBytes -= block;
	}

	checkBytesWritten(fwrite(&val, 1, totalBytes - block, filesystem->file));
}


void closeFileSystem(FileSystem *filesystem) {
	wasSeekSuccessful(fseek(filesystem->file, 0, 0));
	checkBytesWritten(fwrite(&(filesystem->numberOfFiles), sizeof(unsigned int), 1, filesystem->file));
	checkBytesWritten(fwrite(&(filesystem->freeSpace), sizeof(unsigned int), 1, filesystem->file));
	fclose(filesystem->file);
}