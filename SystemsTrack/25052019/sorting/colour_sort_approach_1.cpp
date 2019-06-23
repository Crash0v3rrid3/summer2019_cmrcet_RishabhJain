#include "colour_sort_approach_1.h"

void colorSortApproach1(int *arr, int size) {
    int ones = 0, itr;

    for(itr = 0; itr < size; ++itr) {
        ones += arr[itr];
    }

    for(itr = size - 1; ones > 0; --ones, itr--)
        arr[itr] = 1;
    for(; itr >= 0; itr--)
        arr[itr] = 0;
}