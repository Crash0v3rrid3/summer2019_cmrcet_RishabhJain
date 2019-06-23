#ifndef _maze_without_jumps_h
#define _maze_without_jumps__h

int **findPathFromStartToEndWithoutJumps(int **, int, int, int, int, int, int, int *);
void addCellToPath(int **, int *, int, int);
int isCellValid(int **, int **, int, int, int, int);
void moveDown(int, int, int *, int *, int);
void moveUp(int, int, int *, int *, int );
void moveRight(int, int, int *, int *, int);
void moveLeft(int, int, int *, int *, int);

#endif