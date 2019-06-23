#include <stdio.h>
#ifndef _file_system_h
#define _file_system_h

#define FILESYSTEM_SIZE 104857600
#define FILE_NAME_LENGTH 20
#define BLOCK_SIZE (16 * 1024)
#define MAGIC_NUMBER 0x444E524D
#define METADATA_TABLE_BLOCK_OFFSET 4
#define DATA_START_BLOCK_OFFSET (1)
#define TOTAL_BLOCKS 6000


typedef struct {
	char name[FILE_NAME_LENGTH];
	unsigned int offset;
	unsigned int numberOfBlocks;
	unsigned int length;
} File;

typedef struct {
	FILE *file;
	unsigned int magicNumber;
	File fileTable[32];
	unsigned int numberOfFiles;
	unsigned int numberOfFreeBlocks;
	unsigned char freeBlockFlags[TOTAL_BLOCKS - 3];
} FileSystem;

void initFileSystem(FileSystem *, char *filename);
void copyFileFromDisk(FileSystem *, char *sourceName, char *destinationName);
void copyFileToDisk(FileSystem *, char *sourceName, char *destinationName);
void deleteFile(FileSystem *, char *filename);
void listFiles(FileSystem);
void formatFileSystem(FileSystem *);
void printDebugInformation(FileSystem);
void closeFileSystem(FileSystem *filesystem);

// Wrapper functions
void checkEOF(int ret);
void checkBytesWritten(unsigned int ret);
void didFileOpen(errno_t ret);
void wasSeekSuccessful(int ret);

#endif