#include "cli.h"
#include <string.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS

char *strupper(char *str) {
	unsigned int itr;
	char *result = (char *)calloc(strlen(str), 1);

	for (itr = 0; str[itr]; ++itr) {
		if (str[itr] <= 'z' && str[itr] >= 'a') {
			result[itr] = str[itr] - 'a' + 'A';
		}	else	{
			result[itr] = str[itr];
		}
	}
	result[itr] = 0;

	return result;
}

int parseInput(char **commands, unsigned int numberOfCommands, unsigned int *numberOfArgumentsForEachCommand, char *input, void ***args, unsigned int *numberOfArgs) {
	unsigned int itr1 = 0, itr2;
	char *token = strtok(input, " ");
	token = strupper(token);

	for (itr1 = 0; itr1 < numberOfCommands; ++itr1) {
		if (!strcmp(token, strupper(commands[itr1]))) {
			*numberOfArgs = numberOfArgumentsForEachCommand[itr1];
			args[0] = (void **)calloc(*numberOfArgs, sizeof(void **));
			for (itr2 = 0; itr2 < numberOfArgumentsForEachCommand[itr1]; ++itr2) {
				token = strtok(NULL, " ");
				if (!token) return -1;
				args[0][itr2] = (void *)calloc(strlen(token), 1);
				strcpy((char *)args[0][itr2], token);
			}
			break;
		}

	}

	return itr1;
}