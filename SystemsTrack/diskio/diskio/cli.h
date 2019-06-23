#ifndef _cli_h
#define _cli_h
#define COMMAND_PROMPT ">>>"

int parseInput(char **commands, unsigned int numberOfCommands, unsigned int *numberOfArgumentsForEachCommand, char *input, void ***args, unsigned int *numberOfArgs);


#endif