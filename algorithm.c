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
#include "header.h"
#include "amazing_client.h"
#include <string.h>
//int heuristic_function(XYPOS *start, XYPOS *end, matrix *map);
//int valid_pos(XYPOS *pos, matrix *map);
//int check_move(XYPOS *pos, XYPOS *end, int direction, matrix *map);
//int find_move(XYPOS *start, XYPOS *end, matrix *map);
//int main (){
 
  //XYPOS *test;
 // test = (XYPOS *)malloc(sizeof(XYPOS));
 // test->xPos = 1;
 // test->yPos = 1;

  //XYPOS *end;
 // end = (XYPOS *)malloc(sizeof(XYPOS));
 // end->xPos = 4;
 // end->yPos = 3;
  
 // matrix *map = (matrix *)malloc(sizeof(matrix));
 // map->row = 9;
 // map->column = 9;
  //map->matrix = A;

  
 // int a = -1;
 // a = find_move(test,end,map);
  
 // printf ("direction is %d\n", a);

 // free(test);
  //free(end);
  //free(map);

//}


int find_move (XYPOS *start, XYPOS *end, matrix *map){
  int score=0;
  int best_move;
  int best_score = 10000;//fix
  
  if (start->xPos==end->xPos && start->yPos==end->yPos){//checking current spot
        return 8;
  }
  int score_array[10];
  int score_counter=0;
  //check all directions (0 = W, 1 = N, 2 = S, 3 = E)
  for (int i = 0; i < 4; i ++) { 
    score = check_move(start, end, i, map);
    printf ("score = %d\n", score);
    if (score < best_score && score >= 0) {
      best_score = score;
      best_move = i;
      if (score_counter>0){
          memset(score_array,0,sizeof(int)*score_counter);
          score_counter=0;
          score_array[score_counter]=i;
      }
    }else if (score==best_score){
      score_counter++;
      score_array[score_counter]=i;
    }
  } 

  //if (best_score==0){
    //return 8;
  //}
  
  if (best_score == 10000)
    return -1;
  else
    if (score_counter>0){
	return (rand() % (score_counter+1));
    }else{
        return best_move;
    }
  
}

int heuristic_function(XYPOS *start, XYPOS *end, matrix *map) {
  XYPOS* sentinel=malloc(sizeof(XYPOS));
  sentinel->xPos=start->xPos;
  sentinel->yPos=start->yPos;
  int cost=0;
  //int nearness=0;
  while (sentinel->xPos!=end->xPos*2){
	if (sentinel->xPos<end->xPos*2){
	    if (map->matrix[sentinel->yPos][sentinel->xPos+1]=='1'){
		cost+=10;
	    }

	    if (map->matrix[sentinel->yPos][sentinel->xPos]=='V'){
		cost+=4;
	    }
	    sentinel->xPos+=2;
	}else{
	    if (map->matrix[sentinel->yPos][sentinel->xPos-1]=='1'){
		cost+=10;
	    }

            if (map->matrix[sentinel->yPos][sentinel->xPos]=='V'){
                cost+=4;
            }


	    sentinel->xPos-=2;
	}
  }

  sentinel->xPos=start->xPos;
  sentinel->yPos=start->yPos;
  while (sentinel->yPos!=end->yPos*2){
        if (sentinel->yPos<end->yPos*2){
            if (map->matrix[sentinel->yPos+1][sentinel->xPos]=='_'){
                cost+=6;
            }
            sentinel->yPos+=2;
        }else{
            if (map->matrix[sentinel->yPos-1][sentinel->xPos]=='_'){
                cost+=6;
            }
            sentinel->yPos-=2;
        }
  }
  free(sentinel);


  return (cost+abs( start->xPos - end->xPos*2 )+
			 abs(start->yPos - end->yPos*2 ));
    
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
  else if (direction ==3) {
    move->xPos = move->xPos +2;
    wallx = move->xPos - 1;
    wally = move->yPos;
  }

  if (valid_pos(move, map) == 0) {
    free(move);
    return -1;
  }
  if (map->matrix[wally][wallx] == '_' || map->matrix[wally][wallx] == '1') {
    free(move);
    return -1;
  }
  else {
    int score = heuristic_function(move, end, map);
    free (move);
    return score;
  }
  
}
