#include "divide_and_sort.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int min(int *start, int n) {
	int min = 0, itr;

	for (itr = 1; itr < n; ++itr) {
		if (start[itr] < start[min]) {
			min = itr;
		}
	}

	return min;
}

void swap(int *a, int *b) {
	int t = *a;
	*a = *b;
	*b = t;
}

void sort(int *start, int size) {
	int itr, tmp;

	for (itr = 0; itr < size; ++itr) {
		tmp = min(start + itr, size - itr) + itr;
		if (start[itr] > start[tmp])
			swap(&start[itr], &start[tmp]);
	}
}

void writeToFile(int *data, int numberOfValues, int filenumber) {
	char buf[16], filename[32] = "file-";
	sprintf(buf, "%d.txt", filenumber);
	strcat(filename, buf);
	FILE *file = fopen(filename, "w");

	while (numberOfValues--) {
		fprintf(file, "%d\n", *data++);
	}

	fclose(file);
}

int numberOfNValues(int *arr, int size, int val) {
	int itr, result = 0;
	
	for (itr = 0; itr < size; ++itr) {
		if (arr[itr] == val) 
			result += 1;
	}

	return result;
}

void divideAndSort(char *filename, int maxBlockSize, int *numberOfResultantFiles) {
	FILE *file = fopen(filename, "r");
	int *buff = (int *)malloc(sizeof(int) * maxBlockSize), itr = 0, blockNum = 0;
	char tmp[16];


	while (EOF != fscanf(file, "%d", buff + itr)) {
		itr += 1;
		if (itr == maxBlockSize) {
			sort(buff, itr);
			writeToFile(buff, itr, blockNum);
			itr = 0;
			blockNum++;
		}
	}

	if (itr > 0) {
		sort(buff, itr);
		writeToFile(buff, itr, blockNum);
	}

	*numberOfResultantFiles = blockNum;
}