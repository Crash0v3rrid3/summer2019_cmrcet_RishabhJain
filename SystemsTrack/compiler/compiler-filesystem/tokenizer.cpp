#include <string.h>
#include <stdlib.h>
#include "tokenizer.h"

char **tokenize(char *line, unsigned int *numberOfArgs) {
	unsigned int itr1 = 0;
	char **result = (char **)calloc(5, sizeof(char *));
	char *token = strtok(line, " ,\t");

	while (token) {
		result[itr1] = (char *)calloc(strlen(token), 1);
		strcpy(result[itr1], token);
		itr1++;
		token = strtok(NULL, " ,:\t");
	}	

	return result;
}