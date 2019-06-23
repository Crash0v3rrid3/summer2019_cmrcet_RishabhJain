#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"
#include "filesystem.h"

char *cloneStr(char *str) {
	char *res = (char *)calloc(strlen(str), 1);
	strcpy(res, str);
	return res;
}