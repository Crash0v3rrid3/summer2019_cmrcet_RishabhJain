#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "parser.h"
#include "excel.h"

void startCli() {
	char buf[256];									// Command cannot exceed 255 characters
	char arg1[16], arg2[16], arg3[256], *filename = NULL;
	size_t length;
	Excel sheet;
	sheet.maxRow = 0;
	sheet.table = NULL;
	int op, statusCode;
	unsigned int *coordinates, result;

	while (1) {
		printf("> ");
		fgets(buf, 256, stdin);						// Read utmost 256 bytes
		length = strlen(buf);						
		buf[length - 1] = 0;							// Eliminate \n
		length -= 1;								// Reduce length

		statusCode = parseInput(buf, length, arg1, arg2, arg3, &op);
		if (statusCode == SUCCESS_PARSE) {	
			switch (op) {
				case GET: coordinates = parseRowColumn(arg2);
					statusCode = get(sheet, coordinates[0], coordinates[1], &result);
					if (statusCode == SUCCESS_GET) {
						printf("%u\n", result);
					}
					break;
				case SET: 
					coordinates = parseRowColumn(arg2);
					if ((arg3[0] >= '1' && arg3[0] <= '9')) {
						result = atoi(arg3);
						statusCode = set(&sheet, coordinates[0], coordinates[1], 0, result, NULL);
					}
					else
						statusCode = set(&sheet, coordinates[0], coordinates[1], 1, 0, arg3);
					if (statusCode == SUCCESS_SET) {
						
					}
					break;
				case PRINT:	printSheet(sheet);	break;
				case IMPORT: statusCode = import(&sheet, arg2, strlen(arg2));
						if (statusCode == FAIL_IMPORT) {
							printf("Invalid filename\n");
						}	else	{
							filename = cloneStr(arg2);
						}
						break;
				case EXPORT: statusCode = export(sheet, arg2, strlen(arg2));
						if (statusCode == FAIL_EXPORT) {
							printf("\n");
						}
						break;
				case SAVE: 
					if (filename)
						statusCode = saveSheet(sheet, filename);
					else	{
						printf("Nothing to save");
					}
				case EXIT:
					return;
			}
		}	else	{
			printf("Invalid Command\n");
		}
	}
}
