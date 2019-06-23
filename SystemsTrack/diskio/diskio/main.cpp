#include <stdio.h>
#include <stdlib.h>
#include "cli.h"
#include "filesystem.h"

void diskIOTestCase1() {
	int blockSize;
	char *harddisk = "10mb.hdd";
}

int main() {
	char command[512];
	int numberOfCommands = 8, commandID;
	char *commands[8] = { "MOUNT", "COPYTOFS", "COPYFROMFS", "FORMAT", "LS", "DELETE", "DEBUG", "EXIT" };
	unsigned int numberOfArgumentsForEachCommand[8] = { 2, 2, 2, 1, 0, 1, 0, 0 }, numberOfArguments;
	void **args;
	FileSystem fs = { 0, 0 };
	
	while (1) {
		fflush(stdin);
		printf(">>> ");
		scanf("%[^\n]s", command);

		commandID = parseInput(commands, numberOfCommands, numberOfArgumentsForEachCommand, command, &args, &numberOfArguments);
		switch (commandID) {
		case 0: mount(&fs, (char *)args[0], atoi((char *)args[1])); break;
		case 1: copyToFS(&fs, (char *)args[0], (char *)args[1]); break;
		case 2: copyFromFS(&fs, (char *)args[0], (char *)args[1]); break;
		case 3: format(&fs, atoi((char *)args[0])); break;
		case 4: list(&fs); break;
		case 5: deleteFile(&fs, (char *)args[0]); break;
		case 6: printDebugInformation(&fs); break;
		case 7: exit(0);
		case -1: printf("Invalid command\n");
		}
	}
	
}