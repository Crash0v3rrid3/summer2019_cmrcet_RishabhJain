#include "file_system.h"
#include "disk.h"
#include <stdlib.h>

unsigned int getOffsetBasedOnBlockNumber(unsigned int blockNumber) {
	return blockNumber * BLOCK_SIZE;
}

void *readBlock(FileSystem *filesystem, unsigned int blockNumber) {
	void *buf = (void *)calloc(1, BLOCK_SIZE);
	wasSeekSuccessful(fseek(filesystem->file, getOffsetBasedOnBlockNumber(blockNumber), SEEK_SET));

	fread(buf, 1, BLOCK_SIZE, filesystem->file);

	return buf;
}

void writeBlock(FileSystem *filesystem, int blockNumber, void *buf) {
	wasSeekSuccessful(fseek(filesystem->file, getOffsetBasedOnBlockNumber(blockNumber), SEEK_SET));
	// Mark block as occupied
	checkBytesWritten(fwrite(buf, 1, BLOCK_SIZE, filesystem->file));
}