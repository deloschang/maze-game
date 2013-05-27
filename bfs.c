//file: bfs.c
//
//Breadth first search implementation

#include "amazing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "header.h"
#include "amazing_client.h"

void find_path(matrix* mat,XYPOS* start,XYPOS* goal,int path[]){
     queue* container=malloc(sizeof(queue));
     queue* q=malloc(sizeof(queue));
     MALLOC_CHECK(q);
     MALLOC_CHECK(container);
     cell* first=init_cell(start->xPos*2,start->yPos*2,0,0,NULL);
     cell* con_first=init_cell(start->xPos*2,start->yPos*2,0,0,NULL);
     q->head=first;
     container->head=con_first;
     
     int adj_x;
     int adj_y;
     int wall_x;
     int wall_y;
     int distance=0;
     cell* v;
     int is_found=0;
     while (q->head!=NULL){
	v=dequeue(q);
        //distance+=1;
	//printf("exploring (%d,%d)\n",v->x,v->y);
        if (v->x==goal->xPos*2 && v->y==goal->yPos*2){
              //cell* g=init_cell(adj_x,adj_y,0,0,v);
              //int* path=construct_path(v);
              construct_path(v,path);
	      is_found=1;
              //free_queue(container);
	      //free_queue(q);
	      //free(q);
              //free(container);
              //return path;
        }
	     
	//for (int i=0;i<4;i++){
		
	int order[4]={0,1,2,3};
	int temp;
	for (int k=0;k<100;k++){
	   int r1=rand()%4;
	   int r2=rand()%4;
	   temp=order[r1];
	   order[r1]=order[r2];
	   order[r2]=temp;
	}

	for (int i=0;i<4;i++){
	    // printf("order[%d] %d\n",k,order[k]);
	  //}
	  //shuffle(&order);

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
	  //printf("checking coordinates (%d,%d)\n",adj_x,adj_y);
	  if (adj_x>=0 && adj_y>=0 && wall_x>=0 && wall_y>=0 &&
		adj_x<mat->column && wall_x<mat->column && adj_y<mat->row &&
		wall_y<mat->row){
	      if (mat->matrix[wall_y][wall_x]!='1' && 
				mat->matrix[wall_y][wall_x]!='_'){
	         if (contains(container,adj_x,adj_y)==0){
		     //printf("checked\n");
	             cell* g=init_cell(adj_x,adj_y,v->dist+1,0,v);
                     cell* con_g=init_cell(adj_x,adj_y,v->dist+1,0,v);
		     enqueue(container,con_g);
		     enqueue(q,g);
	          }else{
			//printf("already visited\n");
		  }
		
	      }else{
		  //printf("wall\n");
	      }
	   }else{
		//printf("out of bounds\n");
	   }

	}     
     }
     if (!is_found){
	printf("path not found, shouldnt happend\n");
	//path[0]=rand() % 4;
	path[0]=8;
	exit(3);
     }
     //printf("freeing\n");
     //free_queue(container);
     //free_queue(q);
     //free(q);
     //free(container);
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
    cell* sentinel=c;
    if (sentinel->parent==NULL){
	temp_path[0]=8;
	printf("ASSIGNING 8 to PATH *******************\n");
	//path=&temp_path[0];
	//return path;
    }else{
       while (sentinel->parent!=NULL){
       //while (counter>=0){
	  cell* p=sentinel->parent;
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
           //printf("path coordiantes (%d,%d)\n",sentinel->x,sentinel->y);
	   //printf("path move: %d\n",temp_path[counter+1]);
	   sentinel=sentinel->parent;
         }
         //printf("path coordinates (%d,%d)\n",sentinel->x,sentinel->y);
	 //printf("path move: %d\n",path[counter+1]);
         //printf("returning path\n");
	 
	 //path=&temp_path[0];
	 print_path(temp_path,distance);
         //return path;
     }
}

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
    //printf("enqued\n");
}

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


void free_queue(queue* q){
    cell* c=q->head;

    while (c->next!=NULL){
	c=c->next;
	free(c->prev);
    }
    free(c);

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
