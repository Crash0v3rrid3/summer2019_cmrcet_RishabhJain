#include "solutions.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct _flag {
	unsigned char flag : 1;
} Flag;


void sortUsingBitVectors(char *filename) {
	int itr = 0;
	Flag *flags = (Flag *)calloc(sizeof(Flag), 1000001);
	unsigned int num;
	FILE *file = fopen(filename, "r");

	printf("%u\n", sizeof(Flag));

	while (itr < 9) {
		while (EOF != fscanf(file, "%u", &num)) {
			if (num >= (1000000 * itr + 1) && num <= (1000000 * (itr + 1))) {
				flags[num % 1000000].flag = 1;
			}
		}

		for (num = 1; num <= 1000000; ++num) {
			if (flags[num].flag) {
				printf("%u\n", num + itr * 1000000);
			}
			flags[num].flag = 0;
		}
		itr += 1;
		rewind(file);
	}

	free(flags);
	fclose(file);
}