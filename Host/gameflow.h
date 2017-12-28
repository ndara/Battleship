#ifndef GAMEFLOW_H
#define GAMEFLOW_H

#include "hostlogic.h"
#include "battleship.h"

void initRecords(Records *r);
void shotProcess(char oppboard[][SIZE], Stats *mystats, int *playWin,
   int myRead, int myWrite, int oppWrite, int d, char *player);
void pickGameWinner(int gameNum, int d, int play1win, int play2win, 
   Records *r1, Records *r2, Stats *s1, Stats *s2,
   char *player1, char *player2);
void pickMatchWinner(int play1win, int play2win, Records *r1, Records *r2,
   char *player1, char *player2);
void playGame(int d, char *player1, char *player2, int readFD1, int writeFD1, 
   int readFD2, int writeFD2, char board1[][SIZE], char board2[][SIZE], 
   Stats *s1, Stats *s2, int *play1win, int *play2win);
void playMatch(int g, int d, char *player1, char *player2,
   int readFD1, int writeFD1, int readFD2, int writeFD2,
   char board1[][SIZE], char board2[][SIZE], Stats *s1, Stats *s2,
   Records *r1, Records *r2);
void getBoards(int wfd1, int wfd2, int rfd1, int rfd2, 
   char board1[][SIZE], char board2[][SIZE],
   int *play1win, int *play2win, char *player1, char *player2);
void readBoard(int fd, char board[][SIZE]);
void initStats(Stats* stats);
int isWinner(Stats* s);
char shootShip(char board[][SIZE], char target, Stats* stats, int *winner,
   Shot* s);
char shotResult(char board[][SIZE], Stats* stats, Shot* s, int *winner);

#endif
