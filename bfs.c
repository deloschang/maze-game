//file: bfs.c
//
//Breadth first search implementation with some modifications
//Modifications include choosing random direction for the search
//BFS is implemented using queue data structure which contains
//cell data structures. Cell data structure represents a grid on the maze

#include "amazing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "graphics.h"
#include "amazing_client.h"
#include "header.h"

//function that finds and constructs a path (stores it into path array) from
//start to goal positions
void find_path(matrix* mat,XYPOS* start,XYPOS* goal,int path[]){
     queue* container=malloc(sizeof(queue));
     queue* q=malloc(sizeof(queue));
     MALLOC_CHECK(q);
     MALLOC_CHECK(container);
     cell* first=init_cell(start->xPos*2,start->yPos*2,0,0,NULL);
     cell* con_first=init_cell(start->xPos*2,start->yPos*2,0,0,NULL);
     q->head=first;
     container->head=con_first;

     queue* deq_list=malloc(sizeof(queue));
     deq_list->head=NULL;     

     int adj_x;
     int adj_y;
     int wall_x;
     int wall_y;
     cell* v;
     int is_found=0;
     while (q->head!=NULL){
	v=dequeue(q);
        enqueue(deq_list,v);

	//found a goal
        if (v->x==goal->xPos*2 && v->y==goal->yPos*2){
              construct_path(v,path);
	      is_found=1;
              free_queue(container);
  	      free_queue_advanced(q,deq_list);
	      //free_queue_advanced(q);
	      //free_queue_advanced(deq_list);
	      return;   
        }
	     
	//function to mix the order of
	//direct exploration
		
	int order[4]={0,1,2,3};
	int temp;
	for (int k=0;k<100;k++){
	   int r1=rand()%4;
	   int r2=rand()%4;
	   temp=order[r1];
	   order[r1]=order[r2];
	   order[r2]=temp;
	}

	//exploring each possible direction
	for (int i=0;i<4;i++){
	
	  //adj_x,adj_y signify next entry position
	  //while wall_x,wall_y illustrate walls associated with the
	  //transition between current and next entry
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
	  //checking for boundary cases
	  if (adj_x>=0 && adj_y>=0 && wall_x>=0 && wall_y>=0 &&
		adj_x<mat->column && wall_x<mat->column && adj_y<mat->row &&
		wall_y<mat->row){
	      if (mat->matrix[wall_y][wall_x]!='1' && 
				mat->matrix[wall_y][wall_x]!='_'){//wall
	         if (contains(container,adj_x,adj_y)==0){//if cell wasnt 
							//explored yet
	             cell* g=init_cell(adj_x,adj_y,v->dist+1,0,v);
                     cell* con_g=init_cell(adj_x,adj_y,v->dist+1,0,NULL);
		     enqueue(container,con_g);
		     enqueue(q,g);
		     v->is_visited++;//updating for freeing purposes
	          }
		
	      }
	   }

       }     
     }
	
     //this should never happen since this is a perfect maze
     if (!is_found){
	printf("path not found, shouldnt happend\n");
	//exit(3);
	path[0]=rand() % 4;
     }


}

//function that constructs a path given a cell
//cell c has a reference to its parent so its easy to backtrack the
//original path and put it into int array temp_path
void construct_path(cell* c,int temp_path[]){
    int distance=c->dist;
    int counter=distance-1;
    cell* sentinel=c;

    if (sentinel->parent==NULL){
	temp_path[0]=8;
    }else{
	//looping through all of the parents and constructing the path
	//as we go along (NOTE the path is constructed in a reverse order
	//and we will need to reverse it later)
       while (sentinel->parent!=NULL){
	  cell* p=sentinel->parent;
	   if (sentinel->y==p->y && sentinel->x+2==p->x){//west
	       temp_path[counter]=0;
	       counter--;
	   }else if (sentinel->y==p->y && sentinel->x-2==p->x){//east
               temp_path[counter]=3;
               counter--;
           }else  if (sentinel->x==p->x && sentinel->y+2==p->y){//north
               temp_path[counter]=1;
               counter--;
           }else if (sentinel->x==p->x && sentinel->y-2==p->y){//south
               temp_path[counter]=2;
               counter--;
           }
	   sentinel=sentinel->parent;
         }
     }         
	 //DONE!
	 //path is stored into the array
	 //print_path(temp_path,distance-1);
  
}

//enqueues the cell into a queue as a last entry (FIFO structure)
void enqueue(queue* q, cell* c){
    cell* sentinel=q->head;
    if (sentinel==NULL){
	q->head=c;
    }else{
       while (sentinel->next!=NULL){
	   sentinel=sentinel->next;
       }
       sentinel->next=c;
       c->prev=sentinel;
    }
}


//dequeue the queue (first cell in the queue removed)
cell* dequeue(queue* q){
   cell* for_return=q->head;
   if (for_return->next==NULL){
        q->head=NULL;
	return for_return;
   }else{
      q->head=for_return->next;
      for_return->next->prev=NULL;
      for_return->next=NULL;
      
   }
   return for_return;
}

//function to check whether given coordinates have already been explored
int contains(queue* q,int xPos,int yPos){
     cell* sentinel=q->head;
     if (sentinel!=NULL){
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
     }else{
	return 0;
     }

}

//function to free the queue data structure
void free_queue(queue* q){
    cell* c=q->head;

    while (c->next!=NULL){
	c=c->next;
	free(c->prev);
    }
    free(c);
    free(q);

}

//cell constructor
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

//function to print the path (for debugginf purposes)
void print_path(int path[],int length){
    printf("Path: ");
    fflush(stdout);
  
    for (int i=0;i<length;i++){
	printf("%d;",path[i]);
    }
    printf("\n");

}

void free_queue_advanced(queue* q1,queue* q2){

  queue* container=malloc(sizeof(queue));
  container->head=NULL;

  cell* free_list[200000];
  int free_index=0;
  queue* q;
  for (int i=0;i<2;i++){
    if (i==0){
	q=q1;
    }else{
	q=q2;
    }


    if (q!=NULL){
    
        cell* c=q->head;
   
       if (c!=NULL){

           while (c!=NULL){

	       cell* p_sent=c->parent; 

	       cell* cont_c;
	   
               if (contains(container,c->x,c->y)==0){
		   cont_c=init_cell(c->x,c->y,0,0,NULL);
	 	   enqueue(container,cont_c);
		   free_list[free_index]=c;
		   free_index++;
	       }
       
	       while (p_sent!=NULL){
	      

                   if (contains(container,p_sent->x,p_sent->y)==0){
                       cont_c=init_cell(p_sent->x,p_sent->y,0,0,NULL);
                       enqueue(container,cont_c);
                       free_list[free_index]=p_sent;
                       free_index++;
                   }

	           p_sent=p_sent->parent;
		   
	       }

	       c=c->next;
          }

      }
    }

  }
  
  free_queue(container);
  for (int i=0;i<free_index;i++){
      free(free_list[i]);
  }
 
  free(q1);
  free(q2);



}


