#include "parser.h"
#include "excel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getRowColumnFromString(char *cell, unsigned int *result) {
	result[0] = cell[1] - '1';
	result[1] = cell[0] - 'A';
}

char *cloneStr(char *str) {
	char *result = (char *)malloc(strlen(str));
	strcpy(result, str);
	return result;
}

int evaluateExpression(Excel sheet, Cell cell) {
	unsigned int ans1, ans2, row, column;
	char buf1[165], buf2[16], op;
	if (cell.mode) {
		sscanf(cell.expression, "%s %c %s", buf1, &op, buf2);
		if (!(buf1[0] >= '0' && buf1[0] <= '9')) {
			column = buf1[0];
			row = atoi(buf1 + 1);
			column -= 'A';
			row -= 1;
			get(sheet, row, column, &ans1);
		}		else		{
			ans1 = atoi(buf1);
		}
		
		if (buf2[0] >= 'A' && buf2[0] <= 'Z') {
			column = buf2[0];
			row = atoi(buf2 + 1);
			column -= 'A';
			row -= 1;
			get(sheet, row, column, &ans2);
		}		else		{
			ans2 = atoi(buf2);
		}
		
		switch (op) {
		case '+': return ans1 + ans2;
		case '-': return ans1 - ans2;
		case '*': return ans1 * ans2;
		case '/': return ans1 / ans2;
		default: return ans1;
		}
	}	else	{
		return cell.data;
	}
}

void copyTokenToArgument(char *dest, char *src) {
	strncpy(dest, src, 16);
}

int parseInput(char *input, size_t inputLen, char *arg1, char *arg2, char *arg3, int *op) {
	char *capitalizedInput = input;
	//_strupr_s(input, inputLen);
	char *token = strtok(input, " ");
	int numberOfTokensMustGive;

	copyTokenToArgument(arg1, token);

	if (!strcmp(token, GET_COMMAND)) {
		*op = GET;
		// Number of parameters GET accepts
		numberOfTokensMustGive = 1;
		// Parse args
		token = strtok(NULL, " ");
		if (!token)
			return FAIL_PARSE;
		copyTokenToArgument(arg2, token);
		numberOfTokensMustGive--;

	}	else if (!strcmp(token, SET_COMMAND))	{
		*op = SET;
		numberOfTokensMustGive = 2;

		// Parse cell
		token = strtok(NULL, " =");
		if (!token)
			return FAIL_PARSE;
		
		copyTokenToArgument(arg2, token);
		numberOfTokensMustGive--;
		
		// Parse value
		token = strtok(NULL, " ");
		if (!token)
			return FAIL_PARSE;
		if (token[0] != '=')
			return FAIL_PARSE;
		token = strtok(NULL, " ");

		if (!token)
			return FAIL_PARSE;
		copyTokenToArgument(arg3, token);
		numberOfTokensMustGive--;

		if (!(arg3[0] >= '1' && arg3[0] <= '9')) {
			token = strtok(NULL, " ");
			
			if (token) {
				strcat(arg3, " ");
				strcat(arg3, token);

				token = strtok(NULL, " ");
				if (!token)
					return FAIL_PARSE;
				strcat(arg3, " ");
				strcat(arg3, token);
			}
		}

	} 	else if (!strcmp(token, PRINT_COMMAND))	{
		*op = PRINT;
		numberOfTokensMustGive = 0;
	}	else if (!strcmp(token, IMPORT_COMMAND))	{
		*op = IMPORT;
		numberOfTokensMustGive = 1;

		token = strtok(NULL, " ");
		if (!token)
			return FAIL_PARSE;
		numberOfTokensMustGive--;
		copyTokenToArgument(arg2, token);

	}	else if (!strcmp(token, EXPORT_COMMAND))	{
		*op = EXPORT;
		numberOfTokensMustGive = 1;

		token = strtok(NULL, " ");
		if (!token)
			return FAIL_PARSE;
		numberOfTokensMustGive--;
		copyTokenToArgument(arg2, token);
	}	else if (!strcmp(token, SAVE_COMMAND))	{
		*op = SAVE;
		numberOfTokensMustGive = 0;
	}	else	{
		return FAIL_PARSE;
	}
	token = strtok(NULL, " ");
	while (token) {
		numberOfTokensMustGive--;
		token = strtok(NULL, " ");
	}

	return (numberOfTokensMustGive) ? FAIL_PARSE : SUCCESS_PARSE;
}

unsigned int *parseRowColumn(char *cell) {
	unsigned int *result = (unsigned int *)malloc(sizeof(int) * 2);
	getRowColumnFromString(cell, result);

	return result;
}