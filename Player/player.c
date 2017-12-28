#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "battleship.h"
#include "playlogic.h"

int main(int argc, char *argv[]) {
   int readFD, writeFD, i, j, flag, cur;
   char board[SIZE][SIZE];
   Shot shot, oppShot; 
  
   if(argc != 3) {
      fprintf(stderr, "Usage: player readFD writeFD\n");
      exit(EXIT_FAILURE);
   }

   readFD = getFD(argv[1]); 
   writeFD = getFD(argv[2]); 
   while(1) {
      readMessage(readFD, &cur);
      if(NEW_GAME == cur) {
         i = 0;
         j = 0;
         flag = 0;
         initBoard(board);
         sendBoard(board, writeFD);
      }

      if(SHOT_REQUEST == cur) {
         sendShot(&i, &j, &shot, writeFD, &flag);         
      }

      if(SHOT_RESULT == cur) {
         /*add strategy*/ 
      }

      if(OPPONENTS_SHOT == cur) {
         /* add strategy*/ 
         read(readFD, &oppShot, sizeof(Shot));
      }

      if(MATCH_OVER == cur) {
         break;
      }
   }
   return 0;
}
