#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "battleship.h"
#include "playlogic.h"
#include "ai.h"

int getFD(const char *arg) {
   int fd;

   if (1 != sscanf(arg, "%d", &fd)) {
      fprintf(stderr, "Bad fd argument\n");
      exit(EXIT_FAILURE);
   }
   return fd;
}

void readMessage(int fd, int *cur) {
   int intSize = sizeof(int);
   if (intSize != read(fd, cur, intSize)) {
      fprintf(stderr, "read failure in %s at line %d\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
   }
}

void initBoard(char board[][SIZE]) {
   int i, j;
   for(i = 0; i < SIZE; i++) {
      for(j = 0; j < SIZE; j++) {
         board[i][j] = OPEN_WATER;
      }
   }
   /*
   initAircraftCarrier(board);
   initBattleship(board);
   initDestroyer(board);
   initSubmarine(board);
   initPatrolBoat(board);
   */
}

void initBoardran(char board[][SIZE]) {
   int i, j;
   for(i = 0; i < SIZE; i++) {
      for(j = 0; j < SIZE; j++) {
         board[i][j] = OPEN_WATER;
      }
   }
   
   initAircraftCarrier(board);
   initBattleship(board);
   initDestroyer(board);
   initSubmarine(board);
   initPatrolBoat(board);
}

void initAircraftCarrier(char board[][SIZE]) {
   int i;
   for(i = 0; i < SIZE_AIRCRAFT_CARRIER; i++) {
      board[1][i + 1] = AIRCRAFT_CARRIER;
   }
}

void initBattleship(char board[][SIZE]) {
   int i;
   for(i = 0; i < SIZE_BATTLESHIP; i++) {
      board[2][i + 1] = BATTLESHIP;
   }
}

void initDestroyer(char board[][SIZE]) {
   int i;
   for(i = 0; i < SIZE_DESTROYER; i++) {
      board[3][i + 1] = DESTROYER;
   }
}

void initSubmarine(char board[][SIZE]) {
   int i;
   for(i = 0; i < SIZE_SUBMARINE; i++) {
      board[4][i + 1] = SUBMARINE;
   }
}

void initPatrolBoat(char board[][SIZE]) {
   int i;
   for(i = 0; i < SIZE_PATROL_BOAT; i++) {
      board[5][i + 1] = PATROL_BOAT;
   }
}

void printBoard(char board[][SIZE]) {
   int i, j;
   for(i = 0; i < SIZE; i++) {
      for(j = 0; j < SIZE; j++) {
         printf("%4d", board[i][j]);
      }
      printf("\n");
   }
}

void sendBoard(char board[][SIZE], int fd) {
   char buffer[5];
   int i, j;
   for(i = 0; i < SIZE; i++) {
      for(j = 0; j < SIZE; j++) {
         sprintf(buffer, "%c", board[i][j]);
         if(write(fd, buffer, sizeof(char)) != 1) {
            fprintf(stderr, "we have a problem\n");
            exit(EXIT_FAILURE);
         }
      }
   }
}

void nextShot(int *i, int *j, int *flag) {
   if(*i == 0 && *j == 0 && *flag == 0) {
      *flag = 1;
      return;
   }
   (*j) += 2;
   if(*j >= SIZE) {
      (*i)++;
      if(*i % 2 == 0)
         *j = 0;
      else
         *j = 1;
   }
   if(*i == SIZE) {
      *j = 0;
      (*i) = 0;
   }
}

int validShot(char board[][SIZE], int row, int col) {
   if(withinBounds(row, col) && board[row][col] == 0)
      return 1;
   else
      return 0;
}

void sendShot(int *i, int *j, Shot *s, int fd, int *flag, char board[][SIZE]) {
   nextShot(i, j, flag);
   
   while(validShot(board, *i, *j) == 0) {
      nextShot(i, j, flag);
   }

   s->row = *i;
   s->col = *j;
   write(fd, s, sizeof(Shot));
   write(fd, NULL, 1); 
}

