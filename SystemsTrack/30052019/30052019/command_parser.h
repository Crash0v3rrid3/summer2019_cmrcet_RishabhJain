#ifndef _command_parser_h
#define _command_parser_h
#define _CRT_SECURE_NO_WARNINGS
#define COPY_FROM_DISK_COMMAND "COPYTODISK"
#define COPY_TO_DISK_COMMAND "COPYFROMDISK"
#define DELETE_COMMAND "DELETE"
#define LIST_COMMAND "LIST"
#define FORMAT_COMMAND "FORMAT"
#define EXIT_COMMAND "EXIT"

enum op { INVALID_INPUT = -1, COPY_FROM_DISK, COPY_TO_DISK, DELETE, LIST, FORMAT, EXIT };

// takes in command input, parses it and stores args in the 2d array, return type of command
int parseInput(char *, void ***, int *);

#endif