#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#include "gameflow.h"
#include "battleship.h"

void initRecords(Records *r) {
   r->wins = 0;
   r->draws = 0;
   r->losses = 0;
}

void pickGameWinner(int gameNum, int d, int play1win, int play2win,
   Records *r1, Records *r2, Stats *s1, Stats *s2, 
   char *player1, char *player2)
{
   if(play1win && play2win) {
      (r1->draws)++;
      (r2->draws)++;
      if(d) {
         printf("Game %d: Draw between %s and %s in %d shots\n\n",
            gameNum, player1, player2, s1->shots);
      }
   }
   else if(play1win == 0 && play2win == 0) {
      (r1->losses)++;
      (r2->losses)++;
      if(d) {
         printf("Game %d: Neither %s nor %s won within %d shots\n\n",
            gameNum, player1, player2, MAX_SHOTS);
      }
   }
   else if(play1win && play2win == 0) {
      (r1->wins)++;
      (r2->losses)++;
      if(d) {
         printf("Game %d: %s won in %d shots, %s had %d hits and %d sinks\n\n",
            gameNum, player1, s1->shots, player2, s2->hits, 5 - s2->sinks);
      }
   }
   else if(play1win == 0 && play2win) {
      (r1->losses)++;
      (r2->wins)++;
      if(d) {
         printf("Game %d: %s won in %d shots, %s had %d hits and %d sinks\n\n",
            gameNum, player2, s2->shots, player1, s1->hits, 5 - s1->sinks);
      }
   }
   else {
      fprintf(stderr, "The winner is undetermined\n");
   }
}

void pickMatchWinner(int play1win, int play2win, Records *r1, Records *r2,
   char *player1, char *player2) 
{
   if(r1->wins == 0 && r1->draws == 0
      && r2->wins == 0 && r2->draws == 0)
   {  
      printf("Match Results: Neither %s nor %s won (%d, %d, %d)\n",
         player1, player2, r1->wins, r1->draws, r1->losses);
   }
   else if(r1->wins == r2->wins && r1->draws == r2->draws 
      && r1->losses == r2->losses) 
   {
      printf("Match Results: Draw between %s and %s (%d, %d, %d)\n",
         player1, player2, r1->wins, r1->draws, r1->losses);
   }
   else if(r1->wins > r2->wins) {
      printf("Match Results: %s is the winner (%d, %d, %d)\n",
         player1, r1->wins, r1->draws, r1->losses);
   }
   else if(r2->wins > r1->wins) {
      printf("Match Results: %s is the winner (%d, %d, %d)\n",
         player2, r2->wins, r2->draws, r2->losses);
   }
   else {
      fprintf(stderr, "Unexpected result\n");
   }
}

void playMatch(int g, int d, char *player1, char *player2,
   int readFD1, int writeFD1, int readFD2, int writeFD2,
   char board1[][SIZE], char board2[][SIZE], Stats *s1, Stats *s2,
   Records *r1, Records *r2) 
{
   int i, play1win, play2win, outMsg;
   for(i = 0; i < g; i++) {
      play1win = 0;
      play2win = 0;
      
      initStats(s1);
      initStats(s2);
      
      getBoards(writeFD1, writeFD2, readFD1, readFD2, board1, board2, 
         &play1win, &play2win, player1, player2);
      if(play1win == -1 && play2win == 0) {
         (r1->losses)++;
         (r2->wins)++;
         if(d) {
            printf("Game %d: %s won due to %s being disqualified\n\n",
               i + 1, player2, player1);
         }
         continue;
      }
      else if(play2win == -1 && play1win == 0) {
         (r1->wins)++;
         (r2->losses)++;
         if(d) {
            printf("Game %d: %s won due to %s being disqualified\n\n",
               i + 1, player1, player2);
         }
         continue;
      }
      else if(play1win == -1 && play2win == -1) {
         (r1->losses)++;
         (r2->losses)++;
         if(d)
            printf("Game %d: Both %s and %s were disqualified\n\n",
               i + 1, player1, player2);
         continue;
      }
      else {
         playGame(d, player1, player2, readFD1, writeFD1, readFD2, writeFD2,
            board1, board2, s1, s2, &play1win, &play2win);
         pickGameWinner(i + 1, d, play1win, play2win, r1, r2, s1, s2, 
            player1, player2);
      }
   }
   outMsg = MATCH_OVER;
   write(writeFD1, &outMsg, sizeof(int));
   write(writeFD2, &outMsg, sizeof(int));
   pickMatchWinner(play1win, play2win, r1, r2, player1, player2);
}

void playGame(int d, char *player1, char *player2, int readFD1, int writeFD1, 
   int readFD2, int writeFD2, char board1[][SIZE], char board2[][SIZE], 
   Stats *s1, Stats *s2, int *play1win, int *play2win) 
{
   int totalShots = 0;
   while(totalShots < MAX_SHOTS && *play1win == 0 && *play2win == 0) {

      shotProcess(board2, s1, play1win, readFD1, writeFD1, writeFD2,
         d, player1);

      shotProcess(board1, s2, play2win, readFD2, writeFD2, writeFD1, 
         d, player2);

      totalShots++;
   }
}

void shotProcess(char oppboard[][SIZE], Stats *mystats, int *playWin,
   int myRead, int myWrite, int oppWrite, int d, char *player) 
{
   int outMsg;
   char result;
   Shot* shot = malloc(sizeof(Shot));

   outMsg = SHOT_REQUEST;   
   write(myWrite, &outMsg, sizeof(int));
   read(myRead, shot, sizeof(Shot));
   result = shotResult(oppboard, mystats, shot, playWin);

   if(d) {
      if(result == HIT)
         printf("%16s: shot[%2d][%2d],  HIT!, ", 
            player, shot->row, shot->col);
      else if(result == MISS)
         printf("%16s: shot[%2d][%2d],  Miss, ", 
            player, shot->row, shot->col);
      else if(result == SINK)
         printf("%16s: shot[%2d][%2d], SINK!, ",
            player, shot->row, shot->col);
      else {
         fprintf(stderr, "unexpected result from shot\n");
         exit(EXIT_FAILURE);
      }

      printf("shots:%3d, misses:%2d, hits:%2d, sunk:%d\n",
         mystats->shots, mystats->misses, mystats->hits, 5 - mystats->sinks);
   }

   outMsg = SHOT_RESULT;
   write(myWrite, &outMsg, sizeof(int));
   outMsg = result;
   write(myWrite, &outMsg, sizeof(int));

   outMsg = OPPONENTS_SHOT;
   write(oppWrite, &outMsg, sizeof(int));
   write(oppWrite, shot, sizeof(Shot));
   
   free(shot);
}

void getBoards(int wfd1, int wfd2, int rfd1, int rfd2, 
   char board1[][SIZE], char board2[][SIZE],
   int *play1win, int *play2win, char *player1, char *player2) 
{
   int validB1, validB2, outMsg = NEW_GAME;

   write(wfd1, &outMsg, sizeof(int));
   readBoard(rfd1, board1);
   validB1 = verifyBoard(board1);

   write(wfd2, &outMsg, sizeof(int));
   readBoard(rfd2, board2);
   validB2 = verifyBoard(board2);

   if(validB1 == 0) {
      *play1win = -1;
      printf("%s disqualified: Invalid board\n", player1);
   }
   if(validB2 == 0) {
      *play2win = -1;
      printf("%s disqualified: Invalid board\n", player2);
   }
}

void readBoard(int fd, char board[][SIZE]) {
   char cur;
   int i, j;
   for(i = 0; i < SIZE; i++) {
      for(j = 0; j < SIZE; j++) {
         read(fd, &cur, sizeof(char));
         board[i][j] = cur;
      }
   }
}

void initStats(Stats* stats) {
   stats->aircraft_carrier = SIZE_AIRCRAFT_CARRIER;
   stats->battleship = SIZE_BATTLESHIP;
   stats->destroyer = SIZE_DESTROYER;
   stats->submarine = SIZE_SUBMARINE;
   stats->patrol_boat = SIZE_PATROL_BOAT;
   stats->shots = 0;
   stats->hits = 0;
   stats->misses = 0;
   stats->sinks = NUMBER_OF_SHIPS;
}

char shotResult(char board[][SIZE], Stats* stats, Shot* s, int *winner) {
   char result, target;
   (stats->shots)++;
   if(withinBounds(s->row, s->col) == 0) {
      (stats->misses)++;
      return MISS;
   }
   target = board[s->row][s->col];
   if(target == OPEN_WATER) {
      (stats->misses)++;
      return MISS;
   }
   else if(target == HIT) {
      (stats->hits)++;
      return HIT;
   }
   else if(target == SINK) {
      return SINK;
   }
   else {
      result = shootShip(board, target, stats, winner, s);
      return result;
   }
}

int isWinner(Stats* s) {
   if(s->aircraft_carrier == 0 && s->battleship == 0 && s->destroyer == 0 && 
      s->submarine == 0 && s->patrol_boat == 0)
      return 1;
   return 0;
}

char shootShip(char board[][SIZE], char target, Stats* stats, int *winner,
   Shot* s)
{
   unsigned sunk = 0;
   (stats->hits)++;

   if(target == AIRCRAFT_CARRIER) {
      (stats->aircraft_carrier)--;
      if(stats->aircraft_carrier == 0)
         sunk = 1;
   }
   else if(target == BATTLESHIP) {
      (stats->battleship)--;
      if(stats->battleship == 0)
         sunk = 1;
   }
   else if(target == DESTROYER) {
      (stats->destroyer)--;
      if(stats->destroyer == 0)
         sunk = 1;
   }
   else if(target == SUBMARINE) {
      (stats->submarine)--;
      if(stats->submarine == 0) 
         sunk = 1;
   }
   else if(target == PATROL_BOAT) {
      (stats->patrol_boat)--;
      if(stats->patrol_boat == 0)
         sunk = 1;
   }

   if(sunk) {
      board[s->row][s->col] = SINK;
      (stats->sinks)--;
      if(isWinner(stats)) {
         *winner = 1;
      }
      return SINK;
   }
   else {
      board[s->row][s->col] = HIT;
      return HIT;
   }
}
