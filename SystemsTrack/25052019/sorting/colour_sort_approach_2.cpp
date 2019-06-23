#include "colour_sort_approach_2.h"

void colorSortApproach2(int *arr, int size) {
    int zeros = 0, ones = 0, itr;

    for(itr = 0; itr < size; ++itr) {
        if(arr[itr])
            ones++;
        else
            zeros++;
    }

    for(itr = 0; zeros > 0; zeros--, itr++)
        arr[itr] = 0;
    for(; ones > 0; itr++, ones--)
        arr[itr] = 1;
}