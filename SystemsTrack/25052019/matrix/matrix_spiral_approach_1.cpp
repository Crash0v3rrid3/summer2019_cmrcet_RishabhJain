#include "matrix_spiral_approach_1.h"
#include <stdio.h>

int *matrixSpiralApproach1(int **matrix, int m, int n) {
    int *result = (int *)malloc(sizeof(int) * m * n), i = 0;
    int rowItr = 0, columnItr = 0;
    int right = n - 1, left = 0, top = 0, down = m - 1;

    while(right >= left && top <= down) {
        // Move Right
        while (columnItr <= right) {
            result[i++] = matrix[rowItr][columnItr];
            columnItr++;
        }
        columnItr--;
        rowItr++;
        right--;

        // Move Down
        while(rowItr <= down) {
            result[i++] = matrix[rowItr][columnItr];
            rowItr++;
        }
        rowItr--;
        columnItr--;
        down--;

        //Move Left
        while(columnItr >= left) {
            result[i++] = matrix[rowItr][columnItr];
            columnItr--;
        }
        columnItr++;
        rowItr--;
        left--;

        //Move Up
        while(rowItr > top) {
            result[i++] = matrix[rowItr][columnItr];
            rowItr--;
        }
        rowItr++;
        columnItr++;
        top++;
    }

    return result;
}
