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
	unsigned int *coordinates;
	Cell cell;

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
					statusCode = get(sheet, coordinates[0], coordinates[1], &cell);
					if (statusCode == SUCCESS_GET) {
						switch (cell.type) {
						case INT_DATA: 
						case EXPRESSION_DATA: printf("%d\n", *(int *)cell.data); break;
						case STRING_DATA: printf("%s\n", (char *)cell.data); break;
						case NULL_DATA: printf("0\n"); break;
						}
					}
					break;
				case SET: 
					coordinates = parseRowColumn(arg2);
					statusCode = set(&sheet, coordinates[0], coordinates[1], arg3);
					if (statusCode == FAIL_SET) {
						
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
