#include "excel.h"
#ifndef _expression_parser_h
#define _expression_parser_h

char *infixToPostfix(char *, char **, int);
int validateExpression(Excel, char *, int, int, char ***, void **, int *);

#endif