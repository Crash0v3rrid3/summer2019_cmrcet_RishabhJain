#include "matrix_transpose.h"

int **matrixTranspose(int **matrix, int m, int n) {
    int **result = (int **)malloc(sizeof(int *) * m);
    int rowItr, columnItr;

    for(columnItr = 0; columnItr < n; ++columnItr) {
        result[columnItr] = (int *)malloc(sizeof(int) * n);
        for(rowItr = 0; rowItr < m; ++rowItr)
            result[columnItr][rowItr] = matrix[rowItr][columnItr];
    }

    return result;
}
