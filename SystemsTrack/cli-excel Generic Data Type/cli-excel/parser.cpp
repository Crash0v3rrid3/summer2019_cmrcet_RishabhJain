#include "parser.h"
#include "excel.h"
#include "stack.h"
#include "expression_parser.h"
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
	if (cell.type == EXPRESSION_DATA) {
		char *op1, *op2, buf[32];
		int size = 0, itr;
		char **tokens = tokenize(cloneStr((char *)cell.data), &size);
		unsigned int rowColumn[2], ans1, ans2;
		Cell cell;
		Stack s = { 0 };

		for (itr = 0; itr < size; ++itr) {
			if (isOperator(tokens[itr])) {
				op1 = peek(s);
				pop(&s);
				op2 = peek(s);
				pop(&s);
				if (op1 && isCell(op1)) {
					getRowColumnFromString(op1, rowColumn);
					get(sheet, rowColumn[0], rowColumn[1], &cell);
					if (cell.type == INT_DATA)
					ans1 = *(int *)cell.data;
				}	else	{
					ans1 = atoi(op1);
				}

				if (op2 && isCell(op2)) {
					getRowColumnFromString(op2, rowColumn);
					get(sheet, rowColumn[0], rowColumn[1], &cell);
					if (cell.type == INT_DATA)
						ans2 = *(int *)cell.data;
				}
				else	{
					ans2 = atoi(op2);
				}
				if (op1 && op2) {
					switch (tokens[itr][0]) {
					case '+': sprintf(buf, "%d", ans1 + ans2); break;
					case '-': sprintf(buf, "%d", ans1 - ans2); break;
					case '%': sprintf(buf, "%d", ans2 % ans1); break;
					case '/': sprintf(buf, "%d", ans2 / ans1); break;
					case '*': sprintf(buf, "%d", ans2 * ans1); break;
					}
				}	else	{
					sprintf(buf, "%d", (op1) ? ans1 : ans2);
				}
				push(&s, buf);
			}		else	{
				push(&s, tokens[itr]);
			}
		}
		if (isCell(peek(s))) {
			getRowColumnFromString(peek(s), rowColumn);
			get(sheet, rowColumn[0], rowColumn[1], &cell);
			switch (cell.type) {
			case INT_DATA: ans1 = *(int*)cell.data; break;
			case NULL_DATA: ans1 = 0; break;
			default: break;
			}
		}	else	{
			ans1 = atoi(peek(s));
		}
		
		return ans1;
	}	else	{
		switch (cell.type) {
		case INT_DATA: return *(int *)cell.data;
		case STRING_DATA: 
		case NULL_DATA: return 0;
		}
	}
}

void copyTokenToArgument(char *dest, char *src) {
	strncpy(dest, src, 16);
}

int parseInput(char *input, size_t inputLen, char *arg1, char *arg2, char *arg3, int *op) {
	char *capitalizedInput = input;
	//_strupr_s(input, inputLen);
	char *token = strtok(capitalizedInput, " ");
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
		if (!token) return FAIL_PARSE;
		if (token[0] != '=')
			copyTokenToArgument(arg3, token);
		else
			arg3[0] = 0;
		numberOfTokensMustGive--;

		token = strtok(NULL, " ");
		while (token) {
			strcat(arg3, " ");
			strcat(arg3, token);
			token = strtok(NULL, " ");
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
	}	else if (!strcmp(token, EXIT_COMMAND))	{
		*op = EXIT;
		return SUCCESS_PARSE;
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