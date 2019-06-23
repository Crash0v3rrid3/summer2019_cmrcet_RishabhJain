#include <stdio.h>
#include "string_compress.h"

unsigned long long findNumberOfRepetitions(char *start) {
    unsigned long long result = 1;
    char c = *start++;

    while (*start++ == c)
        result++;

    return result;
}

int writeIntToStr(unsigned long long num, char *str) {
    char buf[32];
    int length;

    length = snprintf(buf, 32, "%llu", num);
    strcpy(str, buf);

    return length;
}

char *compress(char *str) {
    unsigned long long length = strlen(str), tmp;
    char *resultPtr = str;
    char *strPtr = str;

    while (*strPtr) {
        tmp = findNumberOfRepetitions(strPtr);  // Find number of repetitions of the character
        *resultPtr++ = *strPtr;                 // copy single character to result
        if (tmp > 1)
            resultPtr += writeIntToStr(tmp, resultPtr); // Copy number of occurences
        strPtr += tmp;                                  // Move to next character
    }
    *resultPtr = 0;

    return str;//resultStr;
}
