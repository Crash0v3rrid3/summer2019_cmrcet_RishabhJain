#include "sanity_wrappers.h"
#include <stdio.h>
#include <stdlib.h>

void didFileOpen(errno_t ret) {
	if (ret) {
		printf("Couldn't open file, errorno: %u\n", ret);
		exit(1);
	}
}

void wasSeekSuccessful(int ret) {
	if (ret) {
		printf("Seek was not successful\n");
		exit(1);
	}
}

void checkBytesWritten(unsigned int ret) {
	if (!ret) {
		printf("Inavlid number of bytes written\n");
		exit(1);
	}
}

void checkEOF(int ret) {
	if (ret == EOF) {
		printf("Reached EOF while reading file\n");
		exit(1);
	}
}