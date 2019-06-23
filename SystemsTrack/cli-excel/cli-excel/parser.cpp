#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	result[1] = cell[0] - 'A';
	result[0] = atoi(cell + 1) - 1;

	return result;
}