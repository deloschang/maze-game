//file: bfs.c
//
// Breadth first search implementation

#include "amazing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graphics.h"
#include "amazing_client.h"

int track = 0;
int parent_track = 0;

void find_path(matrix* mat,XYPOS* start,XYPOS* goal,int path[]){
  int     adj_x, adj_y, wall_x, wall_y;
  cell*   v;
  int     is_found=0;

  // Set up the basic queueing structure for the BFS
  // Container will be a queue that contains all the nodes
  // that have been visited already.
  queue* container=malloc(sizeof(queue));

  // Q is the main queue that will be a real-time source
  // of which paths to take. It will cross-reference the 
  // container queue to skip nodes that have already been
  // visited
  queue* q=malloc(sizeof(queue));

  
  MALLOC_CHECK(q);
  MALLOC_CHECK(container);

  // Initial cells to function as the head of the queue
  cell* first=init_cell(start->xPos*2,start->yPos*2,0,0,NULL);
  cell* con_first=init_cell(start->xPos*2,start->yPos*2,0,0,NULL);

  q->head=first;
  container->head=con_first;


  while (q->head != NULL){
    v = dequeue(q);

    // Check if the x and y coordinates match the goal.
    // If so, construct the path to the goal
    printf("before, checking %d %d \n", v->x, v->y);
    if ( (v->x == goal->xPos*2) && (v->y == goal->yPos*2) ){
    ("after\n");
      construct_path(v,path);
      is_found=1;


      free_queue(container);
      free(container); container = NULL;

      if (q->head != NULL){
        free_queue_container(q);
        free(q); q = NULL;
        /*cell* prev = v;*/
        /*while ( prev->parent != NULL){*/
          /*cell* temp = prev->parent;*/

          /*free(prev);*/
          /*prev = NULL;*/

          /*prev = temp;*/
        /*}*/
        /*free(prev);*/
        /*prev = NULL;*/
        /*free(v->parent);*/
        /*v->parent = NULL;*/

        free(v);
        v = NULL;

        printf("I'm not empty\n");

      } else {

        printf("I'm empty \n");

        cell* prev = v;
        while ( prev->parent != NULL){
          cell* temp = prev->parent;

          free(prev);
          prev = NULL;

          prev = temp;
        }

        free(prev);
        v = NULL;

        free(q);
        q = NULL;
      }


      return;
    }

    int order[4]={0,1,2,3};
    int temp;

    // Randomly decide which path to go towards.
    // This is more efficient than *ALWAYS* picking
    // which direction to move
    for (int k=0; k<100; k++){
      int r1 = rand()%4;
      int r2 = rand()%4;
      temp = order[r1];
      order[r1] = order[r2];
      order[r2] = temp;
    }

    // Depending on the order, determine the next
    // direction. wall_x is the position of the wall
    // that should be checked. 
    for (int i=0;i<4;i++){

      if (order[i]==0){
        adj_x=v->x-2;
        adj_y=v->y;
        wall_x=v->x-1;
        wall_y=v->y;

      }else if (order[i]==1){
        adj_x=v->x;
        adj_y=v->y-2;
        wall_x=v->x;
        wall_y=v->y-1;

      }else if (order[i]==2){
        adj_x=v->x;
        adj_y=v->y+2;
        wall_x=v->x;
        wall_y=v->y+1;

      }else if (order[i]==3){
        adj_x=v->x+2;
        adj_y=v->y;
        wall_x=v->x+1;
        wall_y=v->y;
      }

      printf("checking coordinates (%d,%d)\n",adj_x,adj_y);
      if (adj_x>=0 && adj_y>=0 && wall_x>=0 && wall_y>=0 &&
          adj_x<mat->column && wall_x<mat->column && adj_y<mat->row &&
          wall_y<mat->row){

        // Make sure the anticipated wall position is not 
        // a wall by reference the matrix for '1' and'_'
        if (mat->matrix[wall_y][wall_x] != '1' && 
            mat->matrix[wall_y][wall_x] != '_'){

          if ( contains(container,adj_x,adj_y)==0 ){
            printf("you got queueueueued! \n");
            // g is the path should be explored, to be
            // enqueued.

            // first make a copy of the parent
            /*cell* parent_copy = NULL;*/
            /*parent_copy = copyCell(parent_copy, v);*/

            cell* g=init_cell(adj_x, adj_y, v->dist+1,0, v);
            // for container, no need for parent
            cell* con_g=init_cell(adj_x, adj_y, v->dist+1,0, NULL);

            /*parent_copy = NULL;*/

            enqueue(container,con_g);

            v->is_visited++;
            enqueue(q,g);
          } else {
            printf("already visited %d %d \n", adj_x, adj_y);
          }

        }else{
          printf("wall %d %d \n", wall_x, wall_y);
        }
      }else{
        //printf("out of bounds\n");
      }
    }     

    /*if (v != NULL){*/
      /*free(v);*/
      /*v = NULL;*/
    /*}*/

  }

  if (!is_found){
    printf("path not found, shouldnt happen\n");
    path[0] = 8;
  }

  // Clean up the BFS by cleaning up the BFS queue
  // and the container which contains all the 
  // nodes that have been visited
  /*free_queue_container(container);*/

  /*free_queue(q);*/
  /*free(q);*/

  //return NULL;
}


void construct_path(cell* c,int temp_path[]){
  int distance=c->dist;
  printf("Distance %d\n",distance);
  //int temp_path[distance];
  //int* path=malloc((distance)*sizeof(int));
  //int* path;
  //memset(path,0,distance*sizeof(int));
  int counter=distance-1;
  //int counter=0;
  cell* sentinel = c;
  if (sentinel->parent==NULL){
    temp_path[0]=8;
    //path=&temp_path[0];
    //return path;
  } else {
    while (sentinel->parent!=NULL){
      //while (counter>=0){
      cell* p = sentinel->parent;
      //printf("exploring parent  (%d,%d)\n",p->x,p->y);
      if (sentinel->y==p->y && sentinel->x+2==p->x){//west
        temp_path[counter]=0;
        counter--;
      }else
        if (sentinel->y==p->y && sentinel->x-2==p->x){//east
          temp_path[counter]=3;
          counter--;
        }else
          if (sentinel->x==p->x && sentinel->y+2==p->y){//north
            temp_path[counter]=1;
            counter--;
          }else
            if (sentinel->x==p->x && sentinel->y-2==p->y){//south
              temp_path[counter]=2;
              counter--;
            }
      printf("path coordiantes (%d,%d)\n",sentinel->x,sentinel->y);
      printf("path move: %d\n",temp_path[counter+1]);
      sentinel=sentinel->parent;
    }

    printf("path coordinates (%d,%d)\n",sentinel->x,sentinel->y);
    //printf("path move: %d\n",path[counter+1]);
    printf("returning path\n");

    //path=&temp_path[0];
    print_path(temp_path,distance);
    //return path;
    }
  }

  /**
   * enqueue
   * This function will enqueue a cell into the main 
   * queue for BFS
   **/
  void enqueue(queue* q, cell* c){
    cell* sentinel=q->head;
    if (sentinel==NULL){
      q->head=c;
    }else{

      while (sentinel->next != NULL){
        sentinel = sentinel->next;
      }

      sentinel->next = c;
      c->prev = sentinel;
    }
  }

  /**
   * copyCell
   **/
  cell* copyCell(cell* copy, cell* orig){
    copy=malloc(sizeof(cell));
    MALLOC_CHECK(copy);

    copy->x = orig->x;
    copy->y = orig->y;
    copy->dist = orig->dist;
    copy->is_visited = orig->is_visited;
    copy->parent = orig->parent;
    copy->next = orig->next;
    copy->prev = orig->prev;

    return copy;
  }


  /**
   * dequeue
   * This function will dequeue the queue in FIFO manner
   **/ 
  cell* dequeue(queue* q){
    cell* for_return = NULL;
    for_return = copyCell(for_return, q->head);

    /*cell* for_return = q->head;*/
    // If the head is the only element in the queue
    // do not change any links
    if ( for_return->next == NULL ){

      free(q->head);
      q->head=NULL;



      return for_return;

    // If there are links to change, replace the head with
    // the next element in the queue and change the links
    // in constant time
    } else {

      /*if  ((q->head)->parent != NULL){*/
        /*free( (q->head)->parent);*/
        /*(q->head)->parent = NULL;*/
      /*}*/


      free(q->head); 
      q->head = NULL;

      q->head = for_return->next;
      for_return->next->prev=NULL;
      for_return->next=NULL;
    }

    return for_return;
  }

  /**
   * contains
   * This function will check if the q contains 
   * the given xPos and yPos.
   **/
  int contains(queue* q,int xPos,int yPos){
    cell* sentinel=q->head;

    while (sentinel->next!=NULL){
      if (sentinel->x==xPos && sentinel->y==yPos){
        return 1;
      }
      sentinel=sentinel->next;
    }
    if (sentinel->x==xPos && sentinel->y==yPos){
      return 1;
    }

    return 0;

  }


  /**
   * free_queue
   * This function will free all the memory within the main queue
   **/
  void free_queue_container(queue* q){
    cell* prev;
    cell* temp;

    cell* c=q->head;

    // free all parents of the queue
    while (c->next!=NULL){
      prev = c;
      c = c->next;

      printf("prev is %d %d\n", prev->x, prev->y);
      printf("c is %d %d\n", c->x, c->y);

      while ( prev->parent != NULL){
        printf("freeing inner parents \n");
        temp = prev->parent;

        if ( prev->is_visited == 0){
          free(prev);
          BZERO(prev, sizeof(cell));
          prev = NULL;

        } else {
          prev->is_visited--;
        }

        prev = temp;
      }
      printf("parent track done\n");

      printf("prev is %d %d\n", prev->x, prev->y);
      printf("c is %d %d\n", c->x, c->y);

      free(prev);
      BZERO(prev, sizeof(cell));
      prev = NULL;

      printf("%d \n", track);
    }
    printf("most parents done, one left\n");

    // free the parents of c
    prev = c;
    while ( prev->parent != NULL){
      temp = prev->parent;
      free(prev);
      prev = NULL;

      prev = temp;
    }

    free(prev);
    prev = NULL;

  }

  /**
   * free_queue
   * This function will free all the memory within the main queue
   **/
  void free_queue(queue* q){
    cell* c=q->head;

    while (c->next!=NULL){
      c=c->next;
      free(c->prev);
      c->prev = NULL;
    }

    if (c != NULL){
      free(c);
      c = NULL;
    }
  }

  cell* init_cell(int xpos,int ypos,int d,int is_marked,cell* p){
    cell* c=malloc(sizeof(cell));
    MALLOC_CHECK(c);

    c->x=xpos;
    c->y=ypos;
    c->dist=d;
    c->is_visited=is_marked;
    c->parent=p;
    c->next=NULL;
    c->prev=NULL;

    return c;
  }

  //void print_path(matrix* mat,XYPOS* s,XYPOS* e){
  void print_path(int path[],int length){
    //int *path=find_path(mat,s,e);
    //int* sentinel=path;
    printf("Path: ");
    fflush(stdout);
    //for (int i=0;i<(sizeof(path)/sizeof(int));i++){
    for (int i=0;i<length;i++){
      printf("%d;",path[i]);
    }
    printf("\n");

  }

  void shuffle(int* array){
    int temp;
    for (int i=0;i<100;i++){
      int r1=rand() % 3;
      int r2=rand() % 3;
      temp=array[r1];
      array[r1]=array[r2];
      array[r2]=temp;
    }
  }

  int main(){
    // Call render_maze() once and it will rerender the global
    // map struct (data) every 1 sec.
    static char Array[9][9] = {
      // 0    1    2    3    4    5    6    7    8
      { 'E', '1', 'E', '0', 'E', '0', 'E', '0', 'E' } , // 0
      { '0', 'Z', '0', 'Z', '0', 'Z', '_', 'Z', '0' } , // 1
      { 'E', '0', 'E', '1', 'E', '1', 'E', '1', 'E' } , // 2
      { '_', 'Z', '_', 'Z', '0', 'Z', '0', 'Z', '_' } , // 3
      { 'E', '0', 'E', '0', 'E', '1', 'E', '0', 'E' } , // 4
      { '0', 'Z', '_', 'Z', '_', 'Z', '_', 'Z', '0' } , // 5
      { 'E', '0', 'E', '1', 'E', '0', 'E', '0', 'E' } , // 6
      { '0', 'Z', '0', 'Z', '0', 'Z', '_', 'Z', '_' } , // 7
      { 'E', '1', 'E', '0', 'E', '0', 'E', '0', 'E' } , // 8
    };

    matrix* data = malloc(sizeof(matrix));
    data->row = 9;
    data->column = 9;

    for (int i = 0; i < data->row; i++){
      for (int j = 0; j < data->column; j++){
        data->matrix[i][j] = Array[i][j];
      }
    }

    XYPOS* cur_pos = malloc(sizeof(XYPOS));
    cur_pos->xPos = 0;
    cur_pos->yPos = 0;
    XYPOS* goal = malloc(sizeof(XYPOS));
    goal->xPos = 4;
    goal->yPos = 4;

    int path[100];

    find_path(data, cur_pos, goal, path);

    free(cur_pos);
    free(goal);
    free(data);
  }
