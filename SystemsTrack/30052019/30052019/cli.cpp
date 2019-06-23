#include "cli.h"
#include "command_parser.h"
#include "file_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int startCli() {
	FileSystem filesystem = {0, 0, 0, 0};
	int typeOfCommand, numberOfArguments;
	int startDay, workingHoursStartTime, workingHoursEndTime, itr1, itr2, workingDays[7] = { 0 };
	char command[512];
	void **args;

	printf("cwd: \n");
	system("dir");
	
	printf("Enter filename: ");
	fgets(command, 512, stdin);
	command[strlen(command) - 1] = 0;
	initFileSystem(&filesystem, command);

	while (1) {
		fflush(stdin);
		printf(COMMAND_PROMPT);
		scanf("%[^\n]s", command);
		typeOfCommand = parseInput(command, &args, &numberOfArguments);
		
		switch (typeOfCommand) {
		case COPY_FROM_DISK: copyFileFromDisk(&filesystem, (char *)args[0], (char *)args[1]); break;
		case COPY_TO_DISK: copyFileToDisk(&filesystem, (char *)args[0], (char *)args[1]); break;
		case DELETE: deleteFile(&filesystem, (char *)args[0]); break;
		case LIST: listFiles(filesystem); break;
		case FORMAT: formatFileSystem(&filesystem);
		case EXIT: 	closeFileSystem(&filesystem);  exit(0);
		default: printf("Invalid input\n");
		}
	}
}