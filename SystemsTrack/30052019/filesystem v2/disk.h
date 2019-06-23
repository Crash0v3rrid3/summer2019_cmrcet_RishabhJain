#ifndef _filesystem_abstraction_h
#define _filesystem_abstraction_h

void *readBlock(FileSystem *filesystem, unsigned int blockNumber);
void writeBlock(FileSystem *filesystem, int blockNumber, void *buf);

#endif