#include "solutions.h"
#include <stdio.h>

FILE *openFile(int filenumber, char *mode) {
	FILE *file;
	char filename[32];
	sprintf(filename, "file-%d.txt", filenumber);
	file = fopen(filename, mode);

	return file;
}

void mergeFiles(int numberOfFiles) {
	int n = 0, num1, num2;
	char filename[32];
	FILE *file1, *file2, *resultFile;
	int k = numberOfFiles;
	file1 = openFile(n, "r");
	n += 1;
	file2 = openFile(n, "r");
	n += 1;

	while (n < numberOfFiles) {
		resultFile = openFile(++numberOfFiles, "w");
		
		file1 = openFile(n, "r");
		n++;
		file2 = openFile(n, "r");
		n++;
		
		fscanf(file1, "%d", &num1);
		fscanf(file2, "%d", &num2);
		while (1) {
			if (num1 < num2) {
				fprintf(resultFile, "%d\n", num1);
				if (EOF == fscanf(file1, "%d", &num1)) break;
			}	else {
				fprintf(resultFile, "%d\n", num2);
				if (EOF == fscanf(file2, "%d", &num2)) break;
			}
		}

		while (EOF != fscanf(file1, "%d", &num1)) {
			fprintf(resultFile, "%d\n", num1);
		}

		while (EOF != fscanf(file2, "%d", &num2)) {
			fprintf(resultFile, "%d\n", num2);
		}

		fclose(file1);
		fclose(file2);
		fclose(resultFile);
	}
}