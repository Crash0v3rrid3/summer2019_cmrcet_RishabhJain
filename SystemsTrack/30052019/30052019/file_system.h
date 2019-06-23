#include <stdio.h>
#ifndef _file_system_h
#define _file_system_h
#define _CRT_SECURE_NO_WARNINGS
#define FILESYSTEM_SIZE 104857600


typedef struct {
	char name[32];
	unsigned int offset;
	unsigned int length;
	unsigned char flag : 1;
} File;

typedef struct {
	FILE *file;
	unsigned int numberOfFiles;
	unsigned int freeSpace;
	File **fileTable;
} FileSystem;

void initFileSystem(FileSystem *, char *filename);
void copyFileFromDisk(FileSystem *, char *sourceName, char *destinationName);
void copyFileToDisk(FileSystem *, char *sourceName, char *destinationName);
void deleteFile(FileSystem *, char *filename);
void listFiles(FileSystem);
void formatFileSystem(FileSystem *);
void closeFileSystem(FileSystem *filesystem);

#endif