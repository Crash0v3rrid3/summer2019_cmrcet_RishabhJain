#ifndef _excel_h
#define _excel_h
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#define MAX_ROWS 10
#define MAX_COLUMNS 10
typedef struct _Row {
	unsigned int *row;
	unsigned int maxColumn;
} Row;
typedef struct _Excel {
	//unsigned int **table;
	Row *table;
	unsigned int maxRow;
} Excel;

// Takes 4 args, Excel sheet, cell row, column and place to store the value, returns status code
int get(Excel, unsigned int, unsigned int, unsigned int *);
// Takes 4 args, Excel sheet, cell row, column and what value to store, returns status code
int set(Excel *, unsigned int, unsigned int, unsigned int);
// Takes only one arg and prints sheet it to stdout
void printSheet(Excel);
// Takes two args sheet, filename
int import(Excel *, char *, size_t);
// Takes two args sheet, filename
int export(Excel, char *, size_t);

// status codes
#define SUCCESS_GET 1
#define FAIL_GET 0
#define SUCCESS_SET 1
#define FAIL_SET 0
#define SUCCESS_EXPORT 1
#define FAIL_EXPORT 1
#define SUCCESS_IMPORT 1
#define FAIL_IMPORT 0

#endif