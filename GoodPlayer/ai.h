#ifndef AI_H
#define AI_H

#include "battleship.h"

int withinBounds(int row, int col);
int isOccupied(char board[][SIZE], int x, int y);
int pickDir(char board[][SIZE], int x, int y, int ship, int shipSize);
int tryRight(char board[][SIZE], int x, int y, int ship, int shipSize);
int tryDown(char board[][SIZE], int x, int y, int ship, int shipSize);
int tryUp(char board[][SIZE], int x, int y, int ship, int shipSize);
int tryLeft(char board[][SIZE], int x, int y, int ship, int shipSize);
void initShip(char board[][SIZE], int ship, int shipSize);
void randomizeBoard(char board[][SIZE]);
void removeShip(char board[][SIZE], int ship);

#endif
