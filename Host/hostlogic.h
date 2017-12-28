#ifndef HOSTLOGIC_H
#define HOSTLOGIC_H

#include "battleship.h"

typedef struct {
   unsigned aircraft_carrier, battleship, destroyer, submarine, patrol_boat;
   unsigned shots, hits, misses, sinks;
} Stats;

typedef struct {
	unsigned wins, draws, losses;
} Records;

char* basename(char *argv[], int playArg);
void validateFlag(char *arg, int *g, int *d);
int checkFlags(int argc, char *argv[], int *d, int *play1Arg, int *play2Arg);
int withinBounds(int row, int col);
int adjacencyCheck(char board[][SIZE], int row, int col, 
   int ship, int shipSize);
int verifyShip(char board[][SIZE], int ship, int shipSize);
int validCell(char c);
int verifyBoard(char board[][SIZE]);

#endif
