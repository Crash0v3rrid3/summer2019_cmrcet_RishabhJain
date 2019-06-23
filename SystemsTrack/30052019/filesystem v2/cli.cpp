#include "cli.h"
#include "command_parser.h"
#include "file_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HARDDISK "harddisk.hdd"

int startCli() {
	FileSystem filesystem;
	int typeOfCommand, numberOfArguments;
	char command[512];
	void **args;

	printf("cwd: \n");
	system("dir");
	
	initFileSystem(&filesystem, HARDDISK);

	while (1) {
		fflush(filesystem.file);
		fflush(stdin);
		printf(COMMAND_PROMPT);
		scanf("%[^\n]s", command);
		typeOfCommand = parseInput(command, &args, &numberOfArguments);
		
		switch (typeOfCommand) {
		case COPY_FROM_DISK: copyFileToDisk(&filesystem, (char *)args[0], (char *)args[1]); break;
		case COPY_TO_DISK: copyFileFromDisk(&filesystem, (char *)args[0], (char *)args[1]); break;
		case DELETE: deleteFile(&filesystem, (char *)args[0]); break;
		case LIST: listFiles(filesystem); break;
		case FORMAT: formatFileSystem(&filesystem);
		case DEBUG: printDebugInformation(filesystem); break;
		case EXIT: 	closeFileSystem(&filesystem);  exit(0);
		default: printf("Invalid input\n");
		}
	}
}