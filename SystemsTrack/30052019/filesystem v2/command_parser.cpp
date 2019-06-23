#include "command_parser.h"
#include <string.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS

int parseInput(char *input, void ***args, int *numberOfArgs) {
	char *token = strtok(input, " ");
	toke																																		                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       n = _
		strupr(token);
	
	if (!strcmp(token, COPY_TO_DISK_COMMAND)) {

		args[0] = (void **)calloc(2, sizeof(void  *));

		token = strtok(NULL, " ");
		args[0][0] = (void *)calloc(strlen(token), 1);
		strcpy((char *)args[0][0], token);

		token = strtok(NULL, " ");
		args[0][1] = (void *)calloc(strlen(token), 1);
		strcpy((char *)args[0][1], token);

		return COPY_TO_DISK;
	}	else if (!strcmp(token, COPY_FROM_DISK_COMMAND)) {

		args[0] = (void **)calloc(2, sizeof(char *));

		token = strtok(NULL, " ");
		args[0][0] = (void *)calloc(strlen(token), 1);
		strcpy((char *)args[0][0], token);

		token = strtok(NULL, " ");
		args[0][1] = (void *)calloc(strlen(token), 1);
		strcpy((char *)args[0][1], token);

		return COPY_FROM_DISK;
	}	else if (!strcmp(token, DELETE_COMMAND)) {
		token = strtok(NULL, " ");
		*args = (void **)calloc(1, sizeof(char *));
		**args = (void *)calloc(strlen(token), 1);
		strcpy((char *)**args, token);
		return DELETE;
	}	else if (!strcmp(token, LIST_COMMAND)) {
		return LIST;
	}	else if (!strcmp(token, FORMAT_COMMAND)) {
		return FORMAT;
	}	else if (!strcmp(token, EXIT_COMMAND)) {
		return EXIT;
	}	else if (!strcmp(token, DEBUG_COMMAND)) {
		return DEBUG;
	}	else	{
		return INVALID_INPUT;
	}


}