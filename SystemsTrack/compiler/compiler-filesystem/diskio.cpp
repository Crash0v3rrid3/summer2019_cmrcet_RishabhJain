#include "diskio.h"
#include "sanity_wrappers.h"
#include <stdlib.h>
#include <stdio.h>

static FILE *disk = NULL;
static unsigned int maxBlockSize = 0;

// helpers start
unsigned int offsetToSeekTo(unsigned int blockID) {
	return blockID * maxBlockSize;
}
//helpers end

// Disk io related functions
unsigned int init(char *harddisk, unsigned int blockSize) {
	if (disk) {
		fclose(disk);
		disk = NULL;
		maxBlockSize = 0;
	}
	didFileOpen(fopen_s(&disk, harddisk, "rb+"));
	maxBlockSize = blockSize;
	wasSeekSuccessful(fseek(disk, 0, SEEK_END));

	return ftell(disk) / blockSize;
}

void *readBlock(unsigned int blockID) {
	void *buf = (void *)calloc(maxBlockSize, 1);

	if (disk) {
		wasSeekSuccessful(fseek(disk, offsetToSeekTo(blockID), SEEK_SET));
		checkEOF(fread(buf, maxBlockSize, 1, disk));
		return buf;				// Free explicitly!
	}

	return NULL;
}

void writeBlock(void *buf, unsigned int blockID) {
	if (disk) {
		wasSeekSuccessful(fseek(disk, offsetToSeekTo(blockID), SEEK_SET));
		checkBytesWritten(fwrite(buf, maxBlockSize, 1, disk));
		fflush(disk);
	}
}