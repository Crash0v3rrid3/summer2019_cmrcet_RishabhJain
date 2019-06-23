#include <stdlib.h>
#include "maze_without_jumps.h"

void addCellToPath(int **solution, int *numberOfPointsInSolution, int curRow, int curColumn) {
	solution[*numberOfPointsInSolution] = (int *)malloc(sizeof(int) * 2);
	solution[*numberOfPointsInSolution][0] = curRow;
	solution[*numberOfPointsInSolution][1] = curColumn;
	(*numberOfPointsInSolution)++;
}

void moveLeft(int curRow, int curColumn, int *nextRow, int *nextColumn, int jump) {
	*nextColumn = curColumn + jump;
	*nextRow = curRow;
}

void moveRight(int curRow, int curColumn, int *nextRow, int *nextColumn, int jump) {
	*nextColumn = curColumn + jump;
	*nextRow = curRow;
}

void moveUp(int curRow, int curColumn, int *nextRow, int *nextColumn, int jump) {
	*nextColumn = curColumn;
	*nextRow = curRow - jump;
}

void moveDown(int curRow, int curColumn, int *nextRow, int *nextColumn, int jump) {
	*nextColumn = curColumn;
	*nextRow = curRow + jump;
}

int isCellValid(int **maze, int **visited, int curRow, int curColumn, int maxRow, int maxColumn) {
	if (curColumn >= maxColumn || curRow >= maxRow || curRow < 0 || curColumn < 0) return 0;
	if (visited[curRow][curColumn]) return 0;
	if (maze[curRow][curColumn] <= 0) return 0;
	return 1;
}

int traverseWithoutJumps(int **maze, int **visited, int curRow, int curColumn, int endRow, int endColumn, int **solution, int *numberOfPointsInSolution, int maxRow, int maxColumn, void (*op[])(int, int, int *, int *, int), int numberOfOperations) {
	if (curColumn == endColumn && endRow == curRow) {
		addCellToPath(solution, numberOfPointsInSolution, curRow, curColumn);
		return 1;
	}

	if (!isCellValid(maze, visited, curRow, curColumn, maxRow, maxColumn))
		return 0;
	
	int tmp, nextRow, nextColumn, itr, ***localBestSolution = (int ***)calloc(numberOfOperations, sizeof(int ***)), *numberOfPointsInLocalBestSolution = (int *)calloc(numberOfOperations, sizeof(int)), min = -1;
	visited[curRow][curColumn] = 1;

	for (itr = 0; itr < numberOfOperations; ++itr) {
		localBestSolution[itr] = (int **)calloc(maxRow * 2, sizeof(int *));
		for (tmp = 0; tmp < maxRow; ++tmp)
			localBestSolution[itr][tmp] = (int *)calloc(maxColumn, sizeof(int));
	}

	for (itr = 0; itr < numberOfOperations; ++itr) {
		op[itr](curRow, curColumn, &nextRow, &nextColumn, 1);
		tmp = traverseWithoutJumps(maze, visited, nextRow, nextColumn, endRow, endColumn, localBestSolution[itr], numberOfPointsInLocalBestSolution + itr, maxRow, maxColumn, op, numberOfOperations);
		if (tmp) {
			if (min == -1)
				min = itr;
			else if (numberOfPointsInLocalBestSolution[min] > numberOfPointsInLocalBestSolution[itr])
				min = itr;
		}
	}
	
	//addCellToPath(solution, numberOfPointsInSolution, curRow, curColumn);
	if (min == -1) 
		return 0;

	for (itr = 0; itr < numberOfPointsInLocalBestSolution[min]; ++itr) 
		addCellToPath(solution, numberOfPointsInSolution, localBestSolution[min][itr][0], localBestSolution[min][itr][1]);
	addCellToPath(solution, numberOfPointsInSolution, curRow, curColumn);

	visited[curRow][curColumn] = 0;
	return 1;
}

int **findPathFromStartToEndWithoutJumps(int **maze, int rows, int columns, int startRow, int startColumn, int endRow, int endColumn, int *finalnumberOfPointsInSolution) {
	void (*op[])(int, int, int *, int *, int) = { moveLeft, moveRight, moveUp, moveDown };
	int numberOfOperations = 4;
	int **solution = (int **)calloc(rows + columns, sizeof(int *)), numberOfPointsInSolution = 0, tmp;
	int **visited = (int **)calloc(rows, sizeof(int *));

	for (tmp = 0; tmp < rows; ++tmp) {
		visited[tmp] = (int *)calloc(columns, sizeof(int));
	}

	tmp = traverseWithoutJumps(maze, visited, startRow, startColumn, endRow, endColumn, solution, &numberOfPointsInSolution, rows, columns, op, numberOfOperations);
	*finalnumberOfPointsInSolution = numberOfPointsInSolution;
	return numberOfPointsInSolution ? solution : NULL;
}