#include "excel.h"
#include "parser.h"
#include "queue.h"
#include "stack.h"
#include "expression_parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int isCell(char *token) {
	char *ptr = token;
	int flag = 0;
	while (*ptr && *ptr <= 'Z' && *ptr >= 'A'){
		ptr++;
		flag = 1;
	}

	while (*ptr && *ptr <= '9' && *ptr >= '1') {
		ptr++;
		flag = 2;
	}

	return !(*ptr) && (flag == 2);
}

int get(Excel sheet, unsigned int row, unsigned int column, Cell *cell) {
	if (row >= sheet.maxRow) {
		cell->data = 0;
		cell->type = NULL_DATA;
		return SUCCESS_GET;
	}

	if (column >= sheet.table[row].maxColumn) {
		cell->data = 0;
		cell->type = NULL_DATA;
		return SUCCESS_GET;
	}

	switch (sheet.table[row].row[column].type) {
	case INT_DATA: cell->data = sheet.table[row].row[column].data;
		cell->type = INT_DATA;
		break;
	case STRING_DATA: cell->data = sheet.table[row].row[column].data;
		cell->type = STRING_DATA;
		break;
	case EXPRESSION_DATA: cell->data = (int *)malloc(sizeof(int));
		*(int *)(cell->data) = evaluateExpression(sheet, sheet.table[row].row[column]);
		cell->type = EXPRESSION_DATA;
		break;
	case NULL_DATA: 
		cell->type = NULL_DATA;
	}

	return SUCCESS_GET;
}

int typeOfInput(char *expression) {
	int length = strlen(expression), itr = 0;
	char *tmp = cloneStr(expression);
	char *token = strtok(tmp, " ");

	while (token) {
		for (itr = 0; token[itr]; ++itr) {
			if (!(token[itr] >= '0' && token[itr] <= '9'))
				return EXPRESSION_DATA;
		}
		token = strtok(NULL, " ");
	}

	return INT_DATA;
}

int set(Excel *sheet, unsigned int row, unsigned int column, char *input) {
	unsigned int itr;
	unsigned int type = typeOfInput(input), value;
	int numberOfTokens = 0;
	void *data = NULL;
	char **tokens = NULL;

	switch (type) {
	case STRING_DATA: data = input; type = STRING_DATA; break;
	case EXPRESSION_DATA: 
		if (!validateExpression(*sheet, input, row, column, &tokens, &data, &numberOfTokens)) {
			data = cloneStr(input);
			type = STRING_DATA;
		}
		break;
	case INT_DATA: data = (void *)malloc(sizeof(int));
		*(int *)data = atoi(input);
		break;
	}
	if (row >= sheet->maxRow) {
		sheet->table = (Row *)realloc(sheet->table, sizeof(Row) * (row + 1));
		for (itr = sheet->maxRow; itr < row; ++itr) {
			sheet->table[itr].maxColumn = 0;
			sheet->table[itr].row = NULL;
		}
		sheet->maxRow = row + 1;
		sheet->table[row].row = (Cell *)calloc(column + 1, sizeof(Cell));
		sheet->table[row].maxColumn = column + 1;

	} else if (column >= sheet->table[row].maxColumn) {
		if (sheet->table[row].row) {
			sheet->table[row].row = (Cell *)realloc(sheet->table[row].row, sizeof(Cell) * (column + 1));
			for (itr = sheet->table[row].maxColumn; itr < column; ++itr) {
				sheet->table[row].row[itr].type = NULL_DATA;
				sheet->table[row].row[itr].data = 0;
			}
		}
		else
			sheet->table[row].row = (Cell *)calloc(column + 1, sizeof(Cell));
		sheet->table[row].maxColumn = column + 1;
	}

	sheet->table[row].row[column].type = type;
	sheet->table[row].row[column].data = data;

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

void initializeRowColumnItrToBeginingOfData(Excel sheet, unsigned int *row, unsigned int *column) {
	*row = 0;
	*column = 0;
}

void printSheet(Excel sheet) {
	unsigned int rowItr, columnItr;
	Cell cell;
	printHeadings(10);
	initializeRowColumnItrToBeginingOfData(sheet, &rowItr, &columnItr);
	for (; rowItr < sheet.maxRow; ++rowItr) {
		printf("%d\t|", rowItr + 1);
		for (columnItr = 0; columnItr < sheet.table[rowItr].maxColumn; ++columnItr) {
			get(sheet, rowItr, columnItr, &cell);
			switch (cell.type) {
			case INT_DATA: fprintf(stdout, "\t%d\t|", *(int*)cell.data); break;
			case STRING_DATA:
			case EXPRESSION_DATA: fprintf(stdout, "\t%s\t|", *(int*)cell.data); break;
			}
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
	int tmp;
	if (fp == NULL) return FAIL_IMPORT;
	char buff[1024], *token;
	sheet->maxRow = 0;
	sheet->table = (Row *)calloc(MAX_ROWS, sizeof(Row));

	while (fgets(buff, sizeof(buff), fp)) {
		token = strtok(buff, ",");
		sheet->table[sheet->maxRow].row = (Cell *)calloc(MAX_COLUMNS, sizeof(Cell));
		sheet->table[sheet->maxRow].maxColumn = 0;
		while (token) {
			tmp = strlen(token);
			if (token[tmp - 1] == '\n')
				token[tmp - 1] = 0;

			set(sheet, sheet->maxRow, sheet->table[sheet->maxRow].maxColumn, token);
			sheet->table[sheet->maxRow].maxColumn++;
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
	Cell cell;

	if (descFile == NULL) return FAIL_IMPORT;
	fseek(descFile, 0, 0);

	for (rowItr = 0; rowItr < sheet.maxRow; ++rowItr) {
		for (columnItr = 0; columnItr < sheet.table[rowItr].maxColumn; ++columnItr) {
			get(sheet, rowItr, columnItr, &cell);
			switch (sheet.table[rowItr].row[columnItr].type) {
			case INT_DATA: fprintf(descFile, "%u", *(int *)cell.data); break;
			case STRING_DATA: fprintf(descFile, "%s", (char *)cell.data); break;
			case EXPRESSION_DATA: fprintf(descFile, "%s", (char *)cell.data); break;
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