#ifndef _diskio_h
#define _diskio_h

unsigned int init(char *harddisk, unsigned int blockSize);
void *readBlock(unsigned int blockNumber);
void writeBlock(void *buf, unsigned int blockNumber);

#endif