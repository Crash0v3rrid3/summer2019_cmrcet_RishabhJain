#include <stdlib.h>

void performOpWhenInBoundry(int *columnItr, int *rowItr, int op) {
    switch(op) {
        case 0:
        case 1:
        case 2:
        case 3:
    }
}

int checkInBoundry(int columnItr, int rowItr, int right, int left, int top, int down, int op) {
    switch(op) {
        case 0:
        case 1:
        case 2:
        case 3:
    }
    return 0;
}

void performPostOperation(int *columnItr, int *rowItr, int *right, int *left, int *top, int *down, int op) {
    switch(op) {
        case 0:
        case 1:
        case 2:
        case 3:
    }
}

int *matrixSpiralApproach2(int **matrix, int m, int n) {
    int *result = (int *)malloc(sizeof(int) * m * n), i = 0;
    int rowItr = 0, columnItr = 0;
    int right = n - 1, left = 0, top = 0, down = m - 1;
    int op = 0;
    // op == 0 Move right
    // op == 1 Move Down
    // op == 2 Move Left
    // op == 3 Move up

    while(right >= left && top <= down) {
        while(checkInBoundry(columnItr, rowItr, right, left, top, down, op))
            performOpWhenInBoundry(&columnItr, &rowItr, op);
        performPostOperation(&columnItr, &rowItr, &right, &left, &top, &down, op);
    }
}