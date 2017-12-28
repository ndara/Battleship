#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#include "hostlogic.h"
#include "battleship.h"

char* basename(char *argv[], int playArg) {
   char *basename;
   int i, lastSlash = -1;
   int c = 0;
   for(i = 0; i < strlen(argv[playArg]); i++) {
      if(argv[playArg][i] == '/')
         lastSlash = i;
   }
   basename = malloc(strlen(argv[playArg]));
   if(basename == NULL) {
      fprintf(stderr, "malloc failed\n");
      exit(EXIT_FAILURE);
   }
   for(i = lastSlash + 1; i < strlen(argv[playArg]); i++) {
      basename[c] = argv[playArg][i];
      c++; 
   }
   basename[c] = '\0';
   return basename;
}

int checkFlags(int argc, char *argv[], int *d, int *play1Arg, int *play2Arg) {
   int i, g = -1;
   int players = 0;
   for(i = 1; i < argc; i++) {
      if(argv[i][0] == '-') {
         validateFlag(argv[i], &g, d);   
      }
      else {
         players++;
         if(players == 1)
            *play1Arg = i;
         if(players == 2)
            *play2Arg = i;
      }
   }
   if(players != 2) {
      fprintf(stderr, "Usage: battleship [-gN|d] player1 player2 \n");
      exit(EXIT_FAILURE);
   }

   if(g == -1)
      g = 3;
   return g;
}

void validateFlag(char *arg, int *g, int *d) {
   char temp1, temp2;
   if(strlen(arg) < 2) {
      fprintf(stderr, "Usage: battleship [-gN|d] player1 player2 \n");
      exit(EXIT_FAILURE);
   }
   if(strlen(arg) == 2 && arg[1] == 'd')
      *d = 1;
   else if(arg[1] == 'g') {
      if(strlen(arg) < 3 || isdigit(arg[2]) == 0) {
         fprintf(stderr, "Usage: battleship [-gN|d] player1 player2 \n");
         exit(EXIT_FAILURE);
      }
      else {
         sscanf(arg, "%c %c %d", &temp1, &temp2, g);
         if(*g == 0) {
            fprintf(stderr, "Usage: battleship [-gN|d] player1 player2 \n");
            exit(EXIT_FAILURE);
         }
      }
   }
   else {
      fprintf(stderr, "Usage: battleship [-gN|d] player1 player2 \n");
      exit(EXIT_FAILURE);
   }
}

int withinBounds(int row, int col) {
   if(row < SIZE && row >= 0 && col < SIZE && col >= 0)
      return 1;
   return 0;
}

int adjacencyCheck(char board[][SIZE], int row, int col, 
   int ship, int shipSize) 
{
   int rowSt = row + 1;
   int colSt = col + 1;
   int curSize = 1;
   while(withinBounds(rowSt, col) && curSize < shipSize
      && board[rowSt][col] == ship) 
   {
      curSize++;
      rowSt++;
   }
   if(curSize == shipSize)
      return 1;
   curSize = 1;
   while(withinBounds(row, colSt) && curSize < shipSize
      && board[row][colSt] == ship) 
   {
      curSize++;
      colSt++;
   }
   if(curSize == shipSize)
      return 1;
   else
      return 0;
}

int verifyShip(char board[][SIZE], int ship, int shipSize) {
   int i, j, ct, flag, row, col, success = 1;
   ct = 0;
   flag = 1;
   for(i = 0; i < SIZE; i++) {
      for(j = 0; j < SIZE; j++) {
         if(board[i][j] == ship) {
            if(flag) {
               row = i;
               col = j;
               flag = 0;
            }
            ct++;
         }
      }
   }
   if(ct != shipSize)
      return 0;
   success = adjacencyCheck(board, row, col, ship, shipSize);
   return success;
}

int validCell(char c) {
   if(c == OPEN_WATER || c == AIRCRAFT_CARRIER || c == BATTLESHIP
      || c == DESTROYER || c == SUBMARINE || c == PATROL_BOAT)
      return 1;
   return 0;
}

int verifyBoard(char board[][SIZE]) {
   int i, j, a, b, d, s, p;
   for(i = 0; i < SIZE; i++) {
      for(j = 0; j < SIZE; j++) {
         if(validCell(board[i][j]) == 0)
            return 0;
      }
   }
   a = verifyShip(board, AIRCRAFT_CARRIER, SIZE_AIRCRAFT_CARRIER);
   b = verifyShip(board, BATTLESHIP, SIZE_BATTLESHIP);
   d = verifyShip(board, DESTROYER, SIZE_DESTROYER);
   s = verifyShip(board, SUBMARINE, SIZE_SUBMARINE);
   p = verifyShip(board, PATROL_BOAT, SIZE_PATROL_BOAT);
   if(a && b && d && s && p)
      return 1;
   else
      return 0; 
}

