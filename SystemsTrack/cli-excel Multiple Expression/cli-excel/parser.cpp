#include "parser.h"
#include "excel.h"
#include "stack.h"
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

int isOperator(char *token) {
	switch (token[0]) {
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case '^': return 1;
	default: return 0;
	}

	return 0;
}

int isCell(char *token) {
	return (token[0] >= 'A' && token[0] <= 'Z');
}

int isLiteral(char *token) {
	int itr = 0;

	while (token[itr]) {
		if (!(token[itr] >= '0' && token[itr] <= '9')) {
			return 0;
		}
		itr++;
	}

	return 1;
}

char **tokenize(char *infix, int *tokens) {
	int length = strlen(infix);
	char *ptr = infix, **result = (char **)calloc(length, sizeof(char **));
	int itr = 0, resultItr = 0, tokenItr = 0;

	while (*ptr) {
		resultItr = 0;
		itr = 0;
		while (itr < length && ptr[itr] == ' ') {
			itr += 1;
		}

		if (ptr[itr]) {
			if (ptr[itr] >= 'A' && ptr[itr] <= 'Z') {
				result[tokenItr] = (char *)calloc(length, 1);
				while (ptr[itr] >= 'A' && ptr[itr] <= 'Z')
					result[tokenItr][resultItr++] = ptr[itr++];
				while (ptr[itr] >= '0' && ptr[itr] <= '9')	// Fix
					result[tokenItr][resultItr++] = ptr[itr++];
				ptr += itr;
			}	else if (ptr[itr] >= '0' && ptr[itr] <= '9')	{
				result[tokenItr] = (char *)calloc(length, 1);
				while (ptr[itr] >= '0' && ptr[itr] <= '9')
					result[tokenItr][resultItr++] = ptr[itr++];
				ptr += itr;
			}		else	{
				result[tokenItr] = (char *)calloc(2, 1);
				result[tokenItr][0] = ptr[itr];
				ptr += itr + 1;
			}
		}
		tokenItr++;
	}

	*tokens = tokenItr;

	return result;
}

int evaluateExpression(Excel sheet, Cell cell) {
	if (cell.mode) {
		char **tokens = (char **)calloc(5, sizeof(char *));
		char *token = strtok(cloneStr(cell.expression), " "), *op1, *op2, buf[32];
		int size = 0, itr;
		unsigned int rowColumn[2], ans1, ans2;
		Stack s;

		while (token) {
			tokens[size++] = token;
			token = strtok(NULL, " ");
		}

		for (itr = 0; itr < size; ++itr) {
			if (isOperator(tokens[itr])) {
				op1 = peek(s);
				pop(&s);
				op2 = peek(s);
				pop(&s);
				if (isCell(op1)) {
					getRowColumnFromString(op1, rowColumn);
					get(sheet, rowColumn[0], rowColumn[1], &ans1);
				}	else	{
					ans1 = atoi(op1);
				}

				if (isCell(op2)) {
					getRowColumnFromString(op2, rowColumn);
					get(sheet, rowColumn[0], rowColumn[1], &ans2);
				}
				else	{
					ans2 = atoi(op2);
				}
				switch (tokens[itr][0]) {
				case '+': sprintf(buf, "%d", ans1 + ans2); break;
				case '-': sprintf(buf, "%d", ans1 - ans2); break;
				case '%': sprintf(buf, "%d", ans2 % ans1); break;
				case '/': sprintf(buf, "%d", ans2 / ans1); break;
				case '*': sprintf(buf, "%d", ans2 * ans1); break;
				}
				push(&s, buf);
			}		else	{
				push(&s, tokens[itr]);
			}
		}
		return atoi(peek(s));
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