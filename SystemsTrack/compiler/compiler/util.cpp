#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

char *cloneStr(char *str) {
	char *res = (char *)calloc(strlen(str), 1);
	strcpy(res, str);
	return res;
}

char *getFileData(char *filename) {
	FILE *inputFile;
	unsigned int length;
	char *data;

	fopen_s(&inputFile, filename, "r");
	fseek(inputFile, 0, SEEK_END);
	length = ftell(inputFile);
	data = (char *)calloc(length, 1);

	fseek(inputFile, 0, SEEK_SET);

	fread(data, 1, length, inputFile);

	fclose(inputFile);
	return data;
}