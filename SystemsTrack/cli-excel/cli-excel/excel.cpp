#include "excel.h"
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

	*result = sheet.table[row].row[column];

	return SUCCESS_GET;
}

int set(Excel *sheet, unsigned int row, unsigned int column, unsigned int value) {
	unsigned int itr;
	if (row >= sheet->maxRow) {
		sheet->table = (Row *)realloc(sheet->table, sizeof(Row) * (row + 1));
		for (itr = sheet->maxRow; itr < row; ++itr) {
			sheet->table[itr].maxColumn = 0;
			sheet->table[itr].row = NULL;
		}
		sheet->maxRow = row + 1;
		sheet->table[row].row = (unsigned int *)calloc(column + 1, sizeof(unsigned int));
		sheet->table[row].maxColumn = column + 1;
		sheet->table[row].row[column] = value;

		return SUCCESS_SET;
	}

	if (column >= sheet->table[row].maxColumn) {
		if (sheet->table[row].row)
			sheet->table[row].row = (unsigned int *)realloc(sheet->table[row].row, sizeof(unsigned int) * (column + 1));
		else
			sheet->table[row].row = (unsigned int *)calloc(column + 1, sizeof(unsigned int));
		sheet->table[row].maxColumn = column + 1;
		sheet->table[row].row[column] = value;
		return SUCCESS_SET;
	}

	sheet->table[row].row[column] = value;
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
	unsigned int rowItr, columnItr;
	printHeadings(10);
	for (rowItr = 0; rowItr < sheet.maxRow; ++rowItr) {
		printf("%d\t|", rowItr + 1);
		for (columnItr = 0; columnItr < sheet.table[rowItr].maxColumn; ++columnItr) {
			fprintf(stdout, "\t%d\t|", sheet.table[rowItr].row[columnItr]);
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

int import(Excel *sheet, char *filename, size_t length) {
	FILE *fp = fopen(filename, "r");
	int column = 0;
	if (fp == NULL) return FAIL_IMPORT;
	char buff[32], *token;
	sheet->maxRow = 0;
	sheet->table = (Row *)calloc(10, sizeof(Row));

	while (fgets(buff, sizeof(buff), fp)) {
		//sheet->table = 
		token = strtok(buff, ",");
		sheet->table[sheet->maxRow].maxColumn = 0;
		sheet->table[sheet->maxRow].row = (unsigned int *)calloc(10, sizeof(unsigned int));
		// Assuming max column will be 10!
		while (token) {
			sheet->table[sheet->maxRow].row[sheet->table[sheet->maxRow].maxColumn++] = atoi(token);
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
	if (descFile == NULL) return FAIL_IMPORT;
	fseek(descFile, 0, 0);

	for (rowItr = 0; rowItr < sheet.maxRow; ++rowItr) {
		for (columnItr = 0; columnItr < sheet.table[rowItr].maxColumn; ++columnItr) {
			fprintf(descFile, "%d", sheet.table[rowItr].row[columnItr]);
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
	return SUCCESS_IMPORT;
}