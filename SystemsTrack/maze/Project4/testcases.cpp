#include <stdio.h>
#include <stdlib.h>
#include "maze_without_jumps.h"
#include "maze_with_jumps.h"


// Helper function
void printMaze(int **maze, int rows, int columns) {
	int rowItr, columnItr;
	printf("Given Maze: \n");
	for (rowItr = 0; rowItr < rows; ++rowItr) {
		for (columnItr = 0; columnItr < columns; ++columnItr) {
			printf("%d ", maze[rowItr][columnItr]);
		}
		puts("");
	}
}

void printSolution(int **solution, int numberOfPointsInSolution) {
	int itr;
	if (!solution) {
		printf("Can't find a path!\n");
	}
	else	{
		for (itr = numberOfPointsInSolution - 1; itr >= 0; --itr) {
			printf("%d, %d\n", solution[itr][0], solution[itr][1]);
		}
	}
}

void mazeWithoutJumpsTestCase1() {
	int maze[5][5] = {  { 1, 1, 1, 1, 0 },
						{ 1, 0, 1, 0, 1 },
						{ 1, 0, 1, 1, 1 },
						{ 0, 1, 0, 1, 1 },
						{ 1, 0, 1, 1, 1 }  };
	int rows = 5, columns = 5, itr, startRow = 0, startColumn = 0, endRow = 4, endColumn = 4, numberOfPointsInSolution;
	int *pass[5];
	for (itr = 0; itr < rows; ++itr) {
		pass[itr] = maze[itr];
	}
	int **solution = findPathFromStartToEndWithoutJumps(pass, rows, columns, startRow, startColumn, endRow, endColumn, &numberOfPointsInSolution);

	printMaze(pass, rows, columns);
	printSolution(solution, numberOfPointsInSolution);
}

void mazeWithJumpsTestCase1() {
	int maze[5][5] = {  { 1, 1, 0, 1, 0 },
						{ 3, 0, 0, 1, 1 },
						{ 1, 0, 0, 1, 2 },
						{ 0, 1, 0, 1, 0 },
						{ 2, 0, 0, 1, 1 } };
	int rows = 5, columns = 5, itr, startRow = 0, startColumn = 0, endRow = 4, endColumn = 4, numberOfPointsInSolution;
	int *pass[5];
	for (itr = 0; itr < rows; ++itr) {
		pass[itr] = maze[itr];
	}

	int **solution = findPathFromStartToEndWithJumps(pass, rows, columns, startRow, startColumn, endRow, endColumn, &numberOfPointsInSolution);

	printMaze(pass, rows, columns);
	printSolution(solution, numberOfPointsInSolution);
}