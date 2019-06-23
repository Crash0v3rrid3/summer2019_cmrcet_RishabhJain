#include <stdio.h>
#include "compiler.h"
#include "filesystem.h"

int main() {
	FILE *actualFile;
	FileSystem fs = {0, 0};
	unsigned int length;
	void *sourceData, *finalCode = NULL;
	mount(&fs, "fs.hdd", 16384);								// Please create fs.hdd before execution!

	sourceData = copyFromFS(&fs, "source.mrnd", &length);
	finalCode = compile((char *)sourceData, &length);
	copyToFS(&fs, finalCode, length, "mrnd.executable");

	return 0;
}