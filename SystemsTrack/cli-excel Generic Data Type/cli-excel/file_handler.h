#include <stdio.h>
#include "excel.h"
#ifndef _file_handler_h
#define _file_handler_h

typedef struct _ExcelFile  {
	unsigned int mode : 1;
	FILE *fp;
} ExcelFile;

// Takes 2 args, filename, Excel sheet and updates sheet
void readLineFromExcelFile(char *, size_t);
int writeDataToExcelFile(char *, size_t, Excel);

// status codes
#define SUCCESS_READ 1
#define FAIL_READ 0
#define SUCCESS_WRITE 1
#define FAIL_WRITE 0

#endif