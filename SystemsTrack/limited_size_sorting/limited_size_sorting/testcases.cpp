#include "testcases.h"
#include "divide_and_sort.h"
#include "solutions.h"
#include <stdlib.h>

void divideAndSortTestcase1() {
	char *filename = "data.txt";
	int maxBlockSize = 4;
	int numberOfResultantFiles;
	divideAndSort(filename, maxBlockSize, &numberOfResultantFiles);
}

void sortWithLimitedMemoryTestcase1() {
	char *filename = "data.txt";
	int maxBlockSize = 16000;
	int numberOfResultantFiles;
	divideAndSort(filename, maxBlockSize, &numberOfResultantFiles);
	mergeFiles(numberOfResultantFiles);
}

void sortByUsingBitVectorsTestCase1() {
	sortUsingBitVectors("data.txt");
}