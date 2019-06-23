#include "expression_parser.h"
#include "excel.h"
#include "stack.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int _validateExpression(Excel sheet, char *expression, int sourceRow, int sourceColumn) {
	// validate recursive expression
	char **tokens = (char **)calloc(5, sizeof(char *));
	char *token = strtok(cloneStr(expression), " ");
	unsigned int rowColumn[2], size = 0, itr;

	while (token) {
		tokens[size++] = token;
		token = strtok(NULL, " ");
	}

	for (itr = 0; itr < size; ++itr) {
		if (isCell(tokens[itr])) {
			getRowColumnFromString(tokens[itr], rowColumn);
			if (rowColumn[0] == sourceRow && rowColumn[1] == sourceColumn) return 0;
			if (rowColumn[0] < sheet.maxRow && rowColumn[1] < sheet.table[rowColumn[0]].maxColumn && sheet.table[rowColumn[0]].row[rowColumn[1]].type == EXPRESSION_DATA)
				if (sheet.table[rowColumn[0]].row[rowColumn[1]].type == INT_DATA && !_validateExpression(sheet, (char *)sheet.table[rowColumn[0]].row[rowColumn[1]].data, sourceRow, sourceColumn))
					return 0;
		}
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
			ptr += 1;
		}

		if (*ptr) {
			if (ptr[itr] >= 'A' && ptr[itr] <= 'Z') {
				result[tokenItr] = (char *)calloc(length, 1);
				while (ptr[itr] >= 'A' && ptr[itr] <= 'Z')
					result[tokenItr][resultItr++] = *ptr++;
				while (ptr[itr] >= '0' && ptr[itr] <= '9')	// Fix
					result[tokenItr][resultItr++] = *ptr++;
				//ptr += itr;
			}
			else if (ptr[itr] >= '0' && ptr[itr] <= '9')	{
				result[tokenItr] = (char *)calloc(length, 1);
				while (ptr[itr] >= '0' && ptr[itr] <= '9')
					result[tokenItr][resultItr++] = *ptr++;
				//ptr += itr;
			}	else	{
				result[tokenItr] = (char *)calloc(2, 1);
				result[tokenItr][0] = *ptr++;
				//ptr += itr + 1;
			}
			tokenItr++;
		}
	}

	*tokens = tokenItr;

	return result;
}

void definePriority(unsigned char *priority) {
	priority[')'] = priority['('] = 255;
	priority['/'] = priority['*'] = priority['%'] = 250;
	priority['+'] = priority['-'] = 245;
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

char *infixToPostfix(char *infix, char **tokens, int numberOfTokens) {
	char *postFix = (char *)calloc(strlen(infix) * 2, 1);
	Stack s = { 0 };
	int itr = 0;
	unsigned char priority[256];

	definePriority(priority);

	for (; itr < numberOfTokens; ++itr) {
		if (isCell(tokens[itr]) || isLiteral(tokens[itr])) {
			strcat(postFix, tokens[itr]);
			strcat(postFix, " ");
		}
		else	{
			if (peek(s)) {
				if (tokens[itr][0] == ')') {
					while (peek(s) && peek(s)[0] != '(') {
						strcat(postFix, peek(s));
						pop(&s);
						strcat(postFix, " ");
					}
					pop(&s);
				}
				else	{
					while (peek(s) && peek(s)[0] != '(' && priority[tokens[itr][0]] < priority[peek(s)[0]]) {
						strcat(postFix, peek(s));
						pop(&s);
						strcat(postFix, " ");
					}
					push(&s, tokens[itr]);
				}
			}
			else	{
				push(&s, tokens[itr]);
			}
		}
	}

	while (peek(s)) {
		strcat(postFix, peek(s));
		pop(&s);
		strcat(postFix, " ");
	}

	return peek(s) 
		?  NULL
		: postFix;
}

int validateExpression(Excel sheet, char *expression, int sourceRow, int sourceColumn, char ***tokens, void **data, int *numberOfTokens) {
	// Check whether expression is correct and parenthesis are balanced 

	int itr, length = strlen(expression), parenthesis = 0;

	for (itr = 0; expression[itr]; ++itr) {
		if (expression[itr] == '(') parenthesis++;
		else if (expression[itr] == ')') parenthesis--;
	}

	if (parenthesis)
		return 0;

	*tokens = tokenize(expression, numberOfTokens);
	for (itr = 0; itr < *numberOfTokens; ++itr) {
		if (!(isCell((*tokens)[itr]) || isLiteral((*tokens)[itr]) || isOperator((*tokens)[itr])))
			return 0;
	}
	*data = (void *)infixToPostfix(expression, *tokens, *numberOfTokens);
	if (!(*data))
		return 0;

	if (!_validateExpression(sheet, expression, sourceRow, sourceColumn))
		return 0;

	return 1;
}

int isOperator(char *token) {
	switch (token[0]) {
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case '(':
	case ')':
	case '^': return 1;
	default: return 0;
	}
}