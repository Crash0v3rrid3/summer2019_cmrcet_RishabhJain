#ifndef _filesystem_h
#define _filesystem_h
#define LENGTH_OF_STATIC_CONTENT_IN_METADATA 16
#define MAGIC_NUMBER 0x444E524D
#define DATA_START_BLOCK 4

typedef struct {
	char filename[20];
	unsigned int size;
	unsigned int numberOfBlocks;
	unsigned int startBlockOrDataBlock;
} FileRecord;

typedef struct {
	unsigned int magicNumber;
	unsigned int blockSize;
	unsigned int totalNumberOfBlocks;
	unsigned int numberOfEmptyBlocks;
	char byteVector[4];
} Metadata;

typedef struct {
	unsigned int numberOfFiles;
	FileRecord files[1];
} FileTable;

typedef struct {
	Metadata *metadata;
	FileTable *filetable;
} FileSystem;

void mount(FileSystem *fs, char *harddiskFileName, unsigned int blockSize);
int copyToFS(FileSystem *fs, char *sourceFileName, char *destFileName);
void copyFromFS(FileSystem *fs, char *sourceFileName, char *destFileName);
void format(FileSystem *fs, unsigned int blockSize);
void list(FileSystem *fs);
int deleteFile(FileSystem *fs, char *filename);
void printDebugInformation(FileSystem *fs);
void unmount(FileSystem *fs);

#endif