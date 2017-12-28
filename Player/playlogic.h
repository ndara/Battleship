#ifndef PLAYLOGIC_H
#define PLAYLOGIC_H

#include "battleship.h"

int getFD(const char *arg);
void readMessage(int fd, int *cur);
void initBoard(char board[][SIZE]);
void initAircraftCarrier(char board[][SIZE]);
void initBattleship(char board[][SIZE]);
void initDestroyer(char board[][SIZE]);
void initSubmarine(char board[][SIZE]);
void initPatrolBoat(char board[][SIZE]);
void printBoard(char board[][SIZE]);
void sendBoard(char board[][SIZE], int fd);
void nextShot(int *i, int *j, int *flag);
void sendShot(int *i, int *j, Shot *s, int fd, int *flag);

#endif
