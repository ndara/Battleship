#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "battleship.h"
#include "ai.h"

void randomizeBoard(char board[][SIZE]) {
   int i, j;
   for(i = 0; i < SIZE; i++) {
      for(j = 0; j < SIZE; j++) {
         board[i][j] = OPEN_WATER;
      }
   }
   initShip(board, PATROL_BOAT, SIZE_PATROL_BOAT);
   initShip(board, AIRCRAFT_CARRIER, SIZE_AIRCRAFT_CARRIER);
   initShip(board, BATTLESHIP, SIZE_BATTLESHIP);
   initShip(board, DESTROYER, SIZE_DESTROYER);
   initShip(board, SUBMARINE, SIZE_SUBMARINE);
}

void initShip(char board[][SIZE], int ship, int shipSize) {
   int rx, ry, success = 0;
   srand(time(NULL));
   while(!success) {
      rx = rand() % SIZE;
      ry = rand() % SIZE;
      success = pickDir(board, rx, ry, ship, shipSize);
   }
}

int isOdd(int x) {
   return (x % 2);
}

int pickDir(char board[][SIZE], int x, int y, int ship, int shipSize) {
   int success = 0;
   if(isOdd(x) && isOdd(y)) {
      success = tryRight(board, x, y, ship, shipSize);
      if(!success)
         success = tryLeft(board, x, y, ship, shipSize);
      if(!success)
         success = tryDown(board, x, y, ship, shipSize);
      if(!success)
         success = tryUp(board, x, y, ship, shipSize);
   }
   else if(isOdd(x) && isOdd(y) == 0) {
      success = tryLeft(board, x, y, ship, shipSize);
      if(!success)
         success = tryUp(board, x, y, ship, shipSize);
      if(!success)
         success = tryRight(board, x, y, ship, shipSize);
      if(!success)
         success = tryDown(board, x, y, ship, shipSize);
   }
   else if(isOdd(x) == 0 && isOdd(y)) {
      success = tryDown(board, x, y, ship, shipSize);
      if(!success)
         success = tryRight(board, x, y, ship, shipSize);
      if(!success)
         success = tryUp(board, x, y, ship, shipSize);
      if(!success)
         success = tryLeft(board, x, y, ship, shipSize);
   }
   else {
      success = tryUp(board, x, y, ship, shipSize);
      if(!success)
         success = tryDown(board, x, y, ship, shipSize);
      if(!success)
         success = tryLeft(board, x, y, ship, shipSize);
      if(!success)
         success = tryRight(board, x, y, ship, shipSize);
   }
   return success; 
}


int withinBounds(int row, int col) {
   if(row < SIZE && row >= 0 && col < SIZE && col >= 0)
      return 1;
   return 0;
}

int isOccupied(char board[][SIZE], int x, int y) {
   if(board[x][y] == OPEN_WATER)
      return 0;
   return 1;
}

int tryRight(char board[][SIZE], int x, int y, int ship, int shipSize) {
   int s = 0;
   while(s != shipSize) {
      if(withinBounds(x, y) && isOccupied(board, x, y) == 0) {
         board[x][y] = ship;
         y++;
      }
      else {
         removeShip(board, ship);
         return 0;
      }
      s++;
   }
   return 1;
}

int tryDown(char board[][SIZE], int x, int y, int ship, int shipSize) {
   int s = 0;
   while(s != shipSize) {
      if(withinBounds(x, y) && isOccupied(board, x, y) == 0) {
         board[x][y] = ship;
         x++;
      }
      else {
         removeShip(board, ship);
         return 0;
      }
      s++;
   }
   return 1;
}

int tryLeft(char board[][SIZE], int x, int y, int ship, int shipSize) {
   int s = 0;
   while(s != shipSize) {
      if(withinBounds(x, y) && isOccupied(board, x, y) == 0) {
         board[x][y] = ship;
         y--;
      }
      else {
         removeShip(board, ship);
         return 0;
      }
      s++;
   }
   return 1;
}

int tryUp(char board[][SIZE], int x, int y, int ship, int shipSize) {
   int s = 0;
   while(s != shipSize) {
      if(withinBounds(x, y) && isOccupied(board, x, y) == 0) {
         board[x][y] = ship;
         x--;
      }
      else {
         removeShip(board, ship);
         return 0;
      }
      s++;
   }
   return 1;
}

void removeShip(char board[][SIZE], int ship) {
   int i, j;
   for(i = 0; i < SIZE; i++) {
      for(j = 0; j < SIZE; j++) {
         if(board[i][j] == ship)
            board[i][j] = OPEN_WATER;
      }
   }
}

