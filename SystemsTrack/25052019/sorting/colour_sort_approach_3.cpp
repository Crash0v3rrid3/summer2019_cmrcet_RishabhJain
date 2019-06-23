#include "colour_sort_approach_3.h"

void colorSortApproach3(int *arr, int size) {
    int start = 0, end = size - 1;

    while(start < end) {
        if(arr[start]) {
            while(end >= 0 && arr[end]) end--;
            arr[end] = 1;
            arr[start] = 0;
        }
        start++;
    }
}