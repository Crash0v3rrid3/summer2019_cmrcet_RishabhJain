#include "excel.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int get(Excel sheet, unsigned int row, unsigned int column, unsigned int *result) {
	if (row >= sheet.maxRow) {
		*result = 0;
		return SUCCESS_GET;
	}

	if (column >= sheet.table[row].maxColumn) {
		*result = 0;
		return SUCCESS_GET;
	}

	if (!sheet.table[row].row[column].mode)
		*result = sheet.table[row].row[column].data;
	else
		*result = evaluateExpression(sheet, sheet.table[row].row[column]);

	return SUCCESS_GET;
}

char *createCopy(char *str) {
	char *result = (char *)malloc(strlen(str));
	strcpy(result, str);
	return result;
}

int set(Excel *sheet, unsigned int row, unsigned int column, int mode, unsigned int value, char *expression) {
	unsigned int itr;
	if (row >= sheet->maxRow) {
		sheet->table = (Row *)realloc(sheet->table, sizeof(Row) * (row + 1));
		for (itr = sheet->maxRow; itr < row; ++itr) {
			sheet->table[itr].maxColumn = 0;
			sheet->table[itr].row = NULL;
		}
		sheet->maxRow = row + 1;
		sheet->table[row].row = (Cell *)calloc(column + 1, sizeof(Cell));
		sheet->table[row].maxColumn = column + 1;

		sheet->table[row].row[column].mode = mode;
		if (sheet->table[row].row[column].mode)
			sheet->table[row].row[column].expression = createCopy(expression);
		else
			sheet->table[row].row[column].data = value;

		return SUCCESS_SET;
	}

	if (column >= sheet->table[row].maxColumn) {
		if (sheet->table[row].row)
			sheet->table[row].row = (Cell *)realloc(sheet->table[row].row, sizeof(Cell) * (column + 1));
		else
			sheet->table[row].row = (Cell *)calloc(column + 1, sizeof(Cell));
		sheet->table[row].maxColumn = column + 1;

		sheet->table[row].row[column].mode = mode;
		if (sheet->table[row].row[column].mode)
			sheet->table[row].row[column].expression = createCopy(expression);
		else
			sheet->table[row].row[column].data = value;
		return SUCCESS_SET;
	}

	sheet->table[row].row[column].mode = mode;
	if (sheet->table[row].row[column].mode)
		sheet->table[row].row[column].expression = createCopy(expression);
	else
		sheet->table[row].row[column].data = value;
	return SUCCESS_SET;
}

void printHeadings(int n) {
	char c = 'A';
	int itr;
	printf("\t");

	for (itr = 0; itr < n; ++itr,++c) {
		printf("\t%c\t", c);
		if (itr != n - 1)
			printf("|");
	}
	puts("");
	printf("\t");
	for (itr = 0; itr < n; ++itr, ++c) {
		printf("\t_\t", c);
		if (itr != n - 1)
			printf("|");
	}
	puts("");
}

void printSheet(Excel sheet) {
	unsigned int rowItr, columnItr, tmp;
	printHeadings(10);
	for (rowItr = 0; rowItr < sheet.maxRow; ++rowItr) {
		printf("%d\t|", rowItr + 1);
		for (columnItr = 0; columnItr < sheet.table[rowItr].maxColumn; ++columnItr) {
			get(sheet, rowItr, columnItr, &tmp);
			fprintf(stdout, "\t%d\t|", tmp);
		}

		for (; columnItr < MAX_COLUMNS; ++columnItr) {
			fprintf(stdout, "\t0\t|");
		}
		fputc('\n', stdout);
	}

	for (; rowItr < MAX_ROWS; ++rowItr) {
		printf("%d\t|", rowItr + 1);

		for (columnItr = 0; columnItr < MAX_COLUMNS; ++columnItr) {
			fprintf(stdout, "\t0\t|");
		}
		fputc('\n', stdout);
	}
}

int isStringCellOrExpression(char *str) {
	unsigned int itr = 0;

	for (; str[itr]; ++itr) {
		if (!(str[itr] >= '0' && str[itr] <= '9'))
			return 1;
	}

	return 0;
}

int import(Excel *sheet, char *filename, size_t length) {
	FILE *fp = fopen(filename, "r");
	int tmp;
	if (fp == NULL) return FAIL_IMPORT;
	char buff[1024], *token;
	sheet->maxRow = 0;
	sheet->table = (Row *)calloc(MAX_ROWS, sizeof(Row));

	while (fgets(buff, sizeof(buff), fp)) {
		token = strtok(buff, ",");
		sheet->table[sheet->maxRow].row = (Cell *)calloc(MAX_COLUMNS, sizeof(Cell));
		sheet->table[sheet->maxRow].maxColumn = 0;
		// Assuming max column will be 10!
		while (token) {
			tmp = strlen(token);
			if (token[tmp - 1] == '\n')
				token[tmp - 1] = 0;
			if (!isStringCellOrExpression(token)) {
				sheet->table[sheet->maxRow].row[sheet->table[sheet->maxRow].maxColumn].mode = 0;
				sheet->table[sheet->maxRow].row[sheet->table[sheet->maxRow].maxColumn++].data = atoi(token);
			}		else		{
				sheet->table[sheet->maxRow].row[sheet->table[sheet->maxRow].maxColumn].mode = 1;
				sheet->table[sheet->maxRow].row[sheet->table[sheet->maxRow].maxColumn++].expression = cloneStr(token);
			}
			token = strtok(NULL, ",");
		}
		sheet->maxRow++;
	}

	fclose(fp);
	return SUCCESS_IMPORT;
}

int export(Excel sheet, char *filename, size_t length) {
	FILE *descFile = fopen(filename, "w+");
	int columnItr = 0, rowItr;
	unsigned int tmp;
	if (descFile == NULL) return FAIL_IMPORT;
	fseek(descFile, 0, 0);

	for (rowItr = 0; rowItr < sheet.maxRow; ++rowItr) {
		for (columnItr = 0; columnItr < sheet.table[rowItr].maxColumn; ++columnItr) {
			if (sheet.table[rowItr].row[columnItr].mode)
				fprintf(descFile, "%s", sheet.table[rowItr].row[columnItr].expression);
			else {
				get(sheet, rowItr, columnItr, &tmp);
				fprintf(descFile, "%u", tmp);
			}
			fputc(',', descFile);
		}

		if (columnItr == MAX_COLUMNS) {
			fseek(descFile, ftell(descFile) - 1, 0);
		}

		for (; columnItr < MAX_COLUMNS; ++columnItr) {
			fputc('0', descFile);
			if (columnItr != MAX_COLUMNS - 1)
				fputc(',', descFile);
		}
		fputc('\n', descFile);
	}

	for (; rowItr < MAX_ROWS; ++rowItr) {
		
		for (columnItr = 0; columnItr < MAX_COLUMNS; ++columnItr) {
			fputc('0', descFile);
			fputc(',', descFile);
		}
		fseek(descFile, ftell(descFile) - 1, 0);
		fputc('\n', descFile);
	}

	fclose(descFile);
	return SUCCESS_EXPORT;
}

int saveSheet(Excel sheet, char* filename) {
	return export(sheet, filename, strlen(filename));
}