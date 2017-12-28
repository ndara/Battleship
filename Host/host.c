#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "hostlogic.h"
#include "gameflow.h"
#include "battleship.h"

int main(int argc, char *argv[]) {
   pid_t pid1, pid2;
   int play1Arg, play2Arg, g, d;
   int pipe1[2], pipe2[2], pipe3[2], pipe4[2];
   char *player1, *player2;
   char readFD1[10], readFD2[10], writeFD1[10], writeFD2[10];
   char board1[SIZE][SIZE], board2[SIZE][SIZE];
   Stats s1, s2;
   Records r1, r2;

   d = 0;
   g = checkFlags(argc, argv, &d, &play1Arg, &play2Arg);
   player1 = basename(argv, play1Arg);
   player2 = basename(argv, play2Arg);

   pipe(pipe1);
   pipe(pipe2);
   pipe(pipe3);
   pipe(pipe4);

   sprintf(readFD1, "%d", pipe1[0]);
   sprintf(writeFD1, "%d", pipe2[1]);
   sprintf(readFD2, "%d", pipe3[0]);
   sprintf(writeFD2, "%d", pipe4[1]);

   if((pid1 = fork()) < 0) {
      fprintf(stderr, "Fork failed\n");
      exit(EXIT_FAILURE);
   }
   else if(pid1 == 0) {
      close(pipe1[1]);
      close(pipe2[0]);

      execl(argv[play1Arg], player1, readFD1, writeFD1, NULL);
      fprintf(stderr, "Execl failed\n");
      exit(EXIT_FAILURE);
   }
   else {
      if((pid2 = fork()) < 0) {
         fprintf(stderr, "Fork failed\n");
         exit(EXIT_FAILURE);
      }
      else if(pid2 == 0) {
         close(pipe3[1]);
         close(pipe4[0]);

         execl(argv[play2Arg], player2, readFD2, writeFD2, NULL);
         fprintf(stderr, "Execl failed\n");
         exit(EXIT_FAILURE);
      }
   }
   close(pipe1[0]);
   close(pipe2[1]);
   close(pipe3[0]);
   close(pipe4[1]);
   /* play match */
   initRecords(&r1);
   initRecords(&r2);
   playMatch(g, d, player1, player2, pipe2[0], pipe1[1], pipe4[0], pipe3[1],
      board1, board2, &s1, &s2, &r1, &r2);

   waitpid(pid1, NULL, 0); 
   waitpid(pid2, NULL, 0); 

   free(player1);
   free(player2);
   exit(EXIT_SUCCESS);
}
