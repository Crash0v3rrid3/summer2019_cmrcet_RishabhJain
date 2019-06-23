#include <stdio.h>
#ifndef _sanity_wrappers_h
#define _sanity_wrappers_h

//open
void didFileOpen(errno_t ret);
//for seek
void wasSeekSuccessful(int ret);
//read
void checkEOF(int ret);
//write
void checkBytesWritten(unsigned int ret);

#endif