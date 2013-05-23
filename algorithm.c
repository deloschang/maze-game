/*
 *
 *
 *
 *
 */
#include "amazing.h"
#include <stdlib.h>
#include "algorithm.h"
#include <stdio.h>

int heursitic_function(XYPOS *start, XYPOS *end, matrix *map);
int valid_pos(XYPOS *pos, matrix *map);
int check_move(XYPOS *pos, XYPOS *end, int direction, matrix *map);
int find_move(XYPOS *start, XYPOS *end, matrix *map);
int main (){
 
  XYPOS *test;
  test = (XYPOS *)malloc(sizeof(XYPOS));
  test->xPos = 1;
  test->yPos = 1;

  XYPOS *end;
  end = (XYPOS *)malloc(sizeof(XYPOS));
  end->xPos = 4;
  end->yPos = 3;
  
  matrix *map = (matrix *)malloc(sizeof(matrix));
  map->row = 9;
  map->column = 9;
  //map->matrix = A;

  
  int a = -1;
  a = find_move(test,end,map);
  
  printf ("direction is %d\n", a);

  free(test);
  free(end);
  free(map);

}


int find_move (XYPOS *start, XYPOS *end, matrix *map){
  int score=0;
  int best_move;
  int best_score = 10000;//fix
  
  
  //check all directions (0 = W, 1 = N, 2 = S, 3 = E)
  for (int i = 0; i < 4; i ++) { 
    score = check_move(start, end, i, map);
    printf ("score = %d\n", score);
    if (score < best_score && score >= 0) {
      best_score = score;
      best_move = i;
    }
  } 
  
  if (best_score == 10000)
    return -1;
  else
    return best_move;
  
}

int heuristic_function(XYPOS *start, XYPOS *end, matrix *map) {
  return (abs(start->xPos - end->xPos * 2) + abs(start->yPos - end->yPos * 2));
    
}

int valid_pos(XYPOS *pos, matrix *map) {
  if (pos->xPos < 0 || pos->yPos < 0  ) {
    return 0;
  }
  if (pos->xPos >= map->column || pos->yPos >= map->row){
    return 0;
  }
  return 1;
}

int check_move(XYPOS *pos, XYPOS *end, int direction, matrix *map){
  XYPOS *move;
  move = (XYPOS *)malloc(sizeof(XYPOS));
  move->xPos = pos->xPos * 2;
  move->yPos = pos->yPos * 2;
  int wallx;
  int wally;
  char A[9][9] = {
    { 'E', '1', 'E', '0', 'E', '0', 'E', '0', 'E' } ,
    { '0', 'Z', '0', 'Z', '0', 'Z', '_', 'Z', '0' } ,
    { 'E', '0', 'E', '1', 'E', '1', 'E', '1', 'E' } ,
    { '_', 'Z', '_', 'Z', '0', 'Z', '0', 'Z', '_' } ,
    { 'E', '0', 'E', '0', 'E', '1', 'E', '0', 'E' } ,
    { '0', 'Z', '_', 'Z', '_', 'Z', '_', 'Z', '0' } ,
    { 'E', '0', 'E', '1', 'E', '0', 'E', '0', 'E' } ,
    { '0', 'Z', '0', 'Z', '0', 'Z', '_', 'Z', '_' } ,
    { 'E', '1', 'E', '0', 'E', '0', 'E', '0', 'E' } ,
  };

  if (direction == 0){
    move->xPos = move->xPos - 2;
    wallx = move->xPos + 1; 
    wally = move->yPos;
  } 
  else if (direction == 1) {
    move->yPos = move->yPos - 2;
    wallx = move->xPos;
    wally = move->yPos + 1;
  }
  else if (direction == 2) {
    move->yPos = move->yPos + 2;
    wallx = move->xPos;
    wally = move->yPos - 1;
  }
  else {
    move->xPos = move->xPos +2;
    wallx = move->xPos - 1;
    wally = move->yPos;
  }

  if (valid_pos(move, map) == 0) {
    free(move);
    return -1;
  }
  if (A[wally][wallx] == '_' || A[wally][wallx] == '1') {
    free(move);
    return -1;
  }
  else {
    int score = heuristic_function(move, end, map);
    free (move);
    return score;
  }
  
}
