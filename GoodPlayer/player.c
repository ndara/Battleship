#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "battleship.h"
#include "playlogic.h"
#include "ai.h"

void destroy(char oppBoard[][SIZE], int fd,  Shot *og, Shot *s, int *ct,
   int *right, int *left, int *down, int *up) 
{

   if(*right && validShot(oppBoard, og->row, og->col + *ct)) {
      s->row = og->row;
      s->col = og->col + *ct;
      write(fd, s, sizeof(Shot));
      (*ct)++;
      return;
   }
   else {
      *right = 0;
      *ct = 1;
   }

   if(*left && validShot(oppBoard, og->row, og->col - *ct)) {
      s->row = og->row;
      s->col = og->col - *ct;
      write(fd, s, sizeof(Shot));
      (*ct)++;
      return;
   }
   else {
      *left = 0;
      *ct = 1;
   }

   if(*down && validShot(oppBoard, og->row + *ct, og->col)) {
      s->row = og->row + *ct;
      s->col = og->col;
      write(fd, s, sizeof(Shot));
      (*ct)++;
      return;
   }
   else {
      *down = 0;
      *ct = 1;
   }

   if(*up && validShot(oppBoard, og->row - *ct, og->col)) {
      s->row = og->row + *ct;
      s->col = og->col;
      write(fd, s, sizeof(Shot));
      (*ct)++;
      return;
   }
   else {
      *up = 0;
      *ct = 1;
      fprintf(stderr, "HERE\n");
   }
}

int main(int argc, char *argv[]) {
   int readFD, writeFD;
   int i, j, flag, cur, result, hunt; 
   int ct, right, left, up, down;
   char board[SIZE][SIZE], oppBoard[SIZE][SIZE];
   Shot shot, oppShot, hit;
  
   if(argc != 3) {
      fprintf(stderr, "Usage: player readFD writeFD\n");
      exit(EXIT_FAILURE);
   }

   readFD = getFD(argv[1]); 
   writeFD = getFD(argv[2]); 

   initBoard(oppBoard);
   
   while(1) {
      readMessage(readFD, &cur);
      if(NEW_GAME == cur) {
         i = 0;
         j = 0;
         flag = 0;
         hunt = 0;
         /*printBoard(oppBoard);
         printf("\n");
         */
         initBoard(oppBoard);
         /*randomizeBoard(board);*/
         initBoardran(board);
         fprintf(stderr, "\n");
         sendBoard(board, writeFD);
      }

      if(SHOT_REQUEST == cur) {
         if(hunt) {
            destroy(oppBoard, writeFD, &hit, &shot, &ct, &right, &left, &down, &up);
         }
         else {
            sendShot(&i, &j, &shot, writeFD, &flag, oppBoard);
         }
      }

      if(SHOT_RESULT == cur) {
         read(readFD, &result, sizeof(int));
         oppBoard[shot.row][shot.col] = result;
         if(result == HIT && hunt == 0) {
            hunt = 1;
            right = 1;
            left = 1;
            down = 1;
            up = 1;
            hit = shot;
            ct = 1;
         }
         if(result == SINK && hunt == 1)
            hunt = 0;
         if(result == MISS && hunt == 1) {
            if(right == 1)
               right = 0;
            else if(left == 1)
               left = 0;
            else if(down == 1)
               down = 0;
            else {
               up = 0;
               hunt = 0;
            }
            ct = 1;
         }
      }

      if(OPPONENTS_SHOT == cur) {
         read(readFD, &oppShot, sizeof(Shot));
      }

      if(MATCH_OVER == cur) {
         break;
      }
   }
      
   return 0;
}
