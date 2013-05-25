//file: amazing_client.c
//
//Description: this program acts as an avatar process. It first sends 
//AM_AVATAR_READY message to the server containing their avatar id.
//Then each avatar will extract #turn variable sent by the server and their
//own coordinates. Each avatar will update their location in the shared map
//based on their turn. Then each avatar will move until they all meet each
//other
//
//Pseudo Code:
//
//1)send AM_AVATAR_READY message to the server
//
//2)extract information from the message server
//
//3)update shared map with the initial coordinates
//
//4)while maze not solved
//
//5)call the heuristic to get next move
//
//6)update shared map
//
//7)update graphics window
//
//8) free all of the memory
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "AMStartup.h"
#include <sys/shm.h>
#include "amazing.h"
#include "amazing_client.h"
#include <unistd.h>
#include "header.h"
//#include "algorithm.h"
#include <sys/sem.h>
#include "semun.h"

//semaphore related functions
//static int set_semvalue(int sem_id);
//static void del_semvalue(int sem_id);
//static int semaphore_p(int sem_id);
//static int semaphore_v(int sem_id);
//semaphore id
//static int sem_id;

int main(int argc,char* argv[]){
    //extern matrix* data;
    //printf("amazing client started\n");

    int avatar_id=atoi(argv[1]);

    int sem_id;
    //getting semaphore
    if (avatar_id==0){
        sem_id=semget((key_t)1324,1,0666 | IPC_CREAT);
	if (!set_semvalue(sem_id)){
	     printf("failed to initialize semaphore\n");
	     exit(1);
	}
    }else{
	system("sleep 1");
	sem_id=semget((key_t)1324,1,0666);
    }

    

    //int avatar_id=atoi(argv[1]);
    int nAvatars=atoi(argv[2]);
    int maze_diff=atoi(argv[3]);

    char server_ip[AM_MAX_MESSAGE];
    BZERO(server_ip,AM_MAX_MESSAGE);
    strncpy(server_ip,argv[4],AM_MAX_MESSAGE);
    //printf("%s\n",server_ip);

    int maze_port=atoi(argv[5]);
    //printf("%d\n",maze_port);
    
    char log_file[AM_MAX_MESSAGE];
    BZERO(log_file,AM_MAX_MESSAGE);
    strncpy(log_file,argv[6],AM_MAX_MESSAGE);
    //printf("%s\n",log_file);
    //fflush(stdout);
    //establishing server connection
    //printf("hi");
    int sockfd;
    struct sockaddr_in servaddr;
    AM_MESSAGE* sendline=malloc(sizeof(AM_MESSAGE));
    AM_MESSAGE* recvline=malloc(sizeof(AM_MESSAGE));
    MALLOC_CHECK(sendline);
    MALLOC_CHECK(recvline);
    
    if ((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
	printf("Problem in creating a socket\n");
	return 1;

    }

    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(server_ip);
    servaddr.sin_port=htons(maze_port);// ?
    
    if (connect(sockfd,(struct sockaddr*) &servaddr,sizeof(servaddr))<0){
	printf("Couldnt connect to the server\n");
	return 1;
    }

    sendline->message_type=htonl(AM_AVATAR_READY);
    sendline->msg.avatar_ready.AvatarId=htonl(avatar_id);

    send(sockfd,sendline,sizeof(AM_MESSAGE),0);
    
    if (recv(sockfd,recvline,sizeof(AM_MESSAGE),0)==0){
	printf("Couldnt receive message from the server\n");
	return 1;

    }

    if (ntohl(recvline->message_type) & AM_ERROR_MASK){
	printf("Wrong message format\n");
	return 1;
    }

    int turn_num=ntohl(recvline->msg.avatar_turn.TurnId);
    XYPOS* init_pos=malloc(sizeof(XYPOS));
    //init_pos=&(recvline->msg.avatar_turn.Pos[avatar_id]);
    init_pos->xPos=ntohl(recvline->msg.avatar_turn.Pos[avatar_id].xPos);
    init_pos->yPos=ntohl(recvline->msg.avatar_turn.Pos[avatar_id].yPos);

    //add semaphore (not using for the moment)
    //printf("message %d\n",ntohl(recvline->message_type));
    //printf("avatar id %d\n",avatar_id);
    //printf("x %d\n",init_pos->xPos);
    //printf("y  %d\n",init_pos->yPos);
    //printf("turn num %d\n",turn_num);
    //printf("-----\n");
    //return 1;
    //int i;
    //int pause_time;
    //char op_char='O';

    //sem_id=semget((key_t)1324,1,0666 | IPC_CREAT);

    //--------
    //int counter=turn_num;
    //for (int i=0;i<nAvatars;i++){
//	if (counter==avatar_id){
//	     update_shared_map(NULL,init_pos);
//	}else{
//	     system("sleep 1");
//	}
//	counter++;
  //  }
    if (!semaphore_p(sem_id)){
	printf("semaphore p failed\n");
	exit(1);
    }
    sleep(rand() % 2);
    update_shared_map(NULL,init_pos);

    if (!semaphore_v(sem_id)){
	printf("semaphore v failed\n");
	exit(1);
    } 

    printf("%d\n",avatar_id);
    if (avatar_id==0){
       //system("sleep 1");
       //print_converted_map();
       //print_shared_map();
       //printf("hi\n");
       //return 1;
    }else{
	system("sleep 1");
    }

    //FOR GRAPHICS
    //if (avatar_id==0){
    //	data=convert_map();
    //    render_maze();
    //}

    XYPOS* goal=NULL;
    matrix* mat=NULL;
    //goal=get_average();
    //update_shared_map(init_pos);
    int condition=1;
    int test_counter=0;
    XYPOS* cur_pos=malloc(sizeof(XYPOS));
    cur_pos=init_pos;
    //printf("avatars %d initial position (%d,%d)\n",avatar_id,init_pos->xPos,
//							init_pos->yPos);
    XYPOS* new_pos=malloc(sizeof(XYPOS));
    XYPOS* prev;
    //main loop to solve the maze
    
    //testing bfs
    //
    int path[100];
    int path_count=0;
    //condition=0;
    //if (avatar_id==0){
    if (!semaphore_p(sem_id)){
        printf("semaphore p failed\n");
        exit(1);
    }

    //condition=0;
    //goal=malloc(sizeof(XYPOS));
    //goal->xPos=2;
    //goal->yPos=2;
    matrix* m=convert_map();
    //path=find_path(m,cur_pos,goal);
    //find_path(m,cur_pos,goal,path);
    //print_path(m,cur_pos,goal);

    if (!semaphore_v(sem_id)){
        printf("semaphore v failed\n");
        exit(1);
    }
    //}




    while (condition && test_counter<15){

	if (turn_num==avatar_id){
	   if (mat!=NULL){
		//free(mat);	
	   }

	   if (!semaphore_p(sem_id)){
		printf("semp failed\n");
		exit(1);
   	   }
	   printf("avatar's turn: %d\n",avatar_id);
	   matrix* mat=convert_map();
	   //if (goal!=NULL){
	       //free(goal);
	   //}
	   //printf("getting average %d\n",avatar_id);
	   //goal=get_average();
	   for (int l=0;l<100;l++){
		path[l]=-5;
	   }
	   goal=get_goal();
	   //goal=malloc(sizeof(XYPOS));
           //goal->xPos=2;
	   //goal->yPos=2;
	
	   printf("goal for avatar  %d is (%d,%d)\n",avatar_id,
				goal->xPos*2,goal->yPos*2);
	   
	   find_path(mat,cur_pos,goal,path);
	   path_count=0;
	   //int next_move=find_move(cur_pos,goal,mat);
	   //if (next_move==-1){
	     //   printf("No valid move is possible. Exiting\n");
	       // exit(1);
	   //}else{
	//	printf("Calculated move for av %d %d\n",
	//		avatar_id,next_move);
    	  // }
    	  int next_move;
    	  //if (avatar_id==1){
	  if (path_count>=0){
    	      next_move=path[path_count];
	      path_count++;
	  
          }else{
 	       next_move=8;
	  }
	   printf("Next move for avatar: %d is %d\n",avatar_id,next_move);
	   sendline->message_type=htonl(AM_AVATAR_MOVE);
	   sendline->msg.avatar_move.AvatarId=htonl(avatar_id);
	   sendline->msg.avatar_move.Direction=htonl(next_move);

           send(sockfd,sendline,sizeof(AM_MESSAGE),0);

            if (recv(sockfd,recvline,sizeof(AM_MESSAGE),0)==0){
               printf("Couldnt receive message from the server\n");
               return 1;

            }

            if (ntohl(recvline->message_type) & AM_ERROR_MASK){
                printf("Wrong message format\n");
                return 1;
            }


	    if (ntohl(recvline->message_type)==AM_MAZE_SOLVED){
		//char maze_solved[AM_MAX_MESSAGE];
		//BZERO(maze_solved,AM_MAX_MESSAGE);
		//strncpy(maze_solved,
		FILE* fp=fopen(log_file,"a");
                if (fp!=NULL){
		    fprintf(fp,"%d,",ntohl(
			recvline->msg.maze_solved.Difficulty));
		    fprintf(fp,"%d,", 
			ntohl(recvline->msg.maze_solved.nAvatars));
		    fprintf(fp,"%d,", 
			ntohl(recvline->msg.maze_solved.nMoves));
		    fprintf(fp,"%d,", 
			ntohl(recvline->msg.maze_solved.Hash));
		    fclose(fp);
		}else{
		    printf("failed to open log file\n");
		    exit(1);
		}
		condition=0;

	    }

	    if (ntohl(recvline->message_type)==AM_TOO_MANY_MOVES
		|| ntohl(recvline->message_type)==AM_SERVER_TIMEOUT){
		condition=0;
	    }

	    if (ntohl(recvline->message_type)==AM_AVATAR_TURN && condition==1){
	        //XYPOS* new_pos=malloc(sizeof(XYPOS));
                //new_pos=&(recvline->msg.avatar_turn.Pos[avatar_id]);
                prev=cur_pos;
		//turn_num=ntohl(recvline->msg.avatar_turn.TurnId);
                new_pos->xPos=ntohl(recvline->msg.avatar_turn.
					Pos[avatar_id].xPos);
		new_pos->yPos=ntohl(recvline->msg.avatar_turn.
					Pos[avatar_id].yPos);
 		printf("previous position for avatar %d (%d,%d)\n",
			avatar_id,cur_pos->xPos,cur_pos->yPos);
		printf("from the server for avatar %d (%d,%d)\n",
			avatar_id,new_pos->xPos,new_pos->yPos);
		fflush(stdout);
	        if (new_pos->xPos==cur_pos->xPos
			&& new_pos->yPos==cur_pos->yPos && next_move!=8){
		    //mark as wall
		    XYPOS* wall=malloc(sizeof(XYPOS));
		    wall->xPos=cur_pos->xPos*2;
		    wall->yPos=cur_pos->yPos*2;
		    if (next_move==M_WEST){
		         wall->xPos=wall->xPos-1;
		         mark_as_wall(wall);
		    }else if (next_move==M_NORTH){
		        wall->yPos=wall->yPos-1;
		        mark_as_wall(wall);
		    }else if (next_move==M_SOUTH){
		        wall->yPos=wall->yPos+1;
  		        mark_as_wall(wall);
		    }else if (next_move==M_EAST){
		        wall->xPos=wall->xPos+1;
		        mark_as_wall(wall);
		    }
		    free(wall);
		    //free(path);
 		    //mat=convert_map();
		    //print_converted_map();
 		    //path=find_path(mat,cur_pos,goal);
 		    //find_path(mat,cur_pos,goal,path);
		    //if (path==NULL){
		//	printf("failed to find path\n");
			//exit(1);
		//	path_count=-1;
		//	next_move=8;
		  //  }else{
			//printf("hi\n");
			//fflush(stdout);
			//print_path(path);
	       	    //path_count=0;
		    //}
		    //path_count=0;
		    //printf("avatar %d ran into wall\n",avatar_id);
	         }else {
		    //if (next_move!=8){
	                update_shared_map(cur_pos,new_pos);
		    //}
                    //free(cur_pos);
		    //cur_pos=new_pos;
		 }
		 cur_pos->xPos=new_pos->xPos;
		 cur_pos->yPos=new_pos->yPos;
	         
		 if (cur_pos->xPos==goal->xPos && cur_pos->yPos==goal->yPos){
		     path_count=-1;
		     next_move=8;
		 }
		     
		 //free(new_pos);

		 //FOR GRAPHICS
		 //free(data);
		 //data=convert_map();
		 char av_id_str[50];
		 BZERO(av_id_str,50);
		 snprintf(av_id_str,50,"%d",avatar_id);
	         char coord[50];
	         BZERO(coord,50);
	         snprintf(coord,50,"%d",cur_pos->xPos);
	         char log_file_msg[AM_MAX_MESSAGE];
	         BZERO(log_file_msg,AM_MAX_MESSAGE);

		 strncpy(log_file_msg,"echo ' ",AM_MAX_MESSAGE);
		 strncat(log_file_msg,"Avatars move: ",AM_MAX_MESSAGE);
		 strncat(log_file_msg,av_id_str,AM_MAX_MESSAGE);
		 strncat(log_file_msg," >> ",AM_MAX_MESSAGE);
		 strncat(log_file_msg,log_file,AM_MAX_MESSAGE);
		 system(log_file_msg);


		 BZERO(log_file_msg,AM_MAX_MESSAGE);
	         strncpy(log_file_msg,"echo '",AM_MAX_MESSAGE);
	         strncat(log_file_msg,"Avatar ",AM_MAX_MESSAGE);
	         strncat(log_file_msg,argv[1],AM_MAX_MESSAGE);
	         strncat(log_file_msg," moved to (",AM_MAX_MESSAGE);
	         strncat(log_file_msg,coord,AM_MAX_MESSAGE);
	         strncat(log_file_msg,",",AM_MAX_MESSAGE);
	
	         BZERO(coord,50);
	         snprintf(coord,50,"%d",cur_pos->yPos);
	    
	         strncat(log_file_msg,coord,AM_MAX_MESSAGE);
	         strncat(log_file_msg,")' >>",AM_MAX_MESSAGE);
	         strncat(log_file_msg,log_file,AM_MAX_MESSAGE);
	         system(log_file_msg);
	    
	         turn_num=ntohl(recvline->msg.avatar_turn.TurnId);
		 //update_turn_id(turn_num);
		 //printf("next turn changes from %d to %d\n",
		//	avatar_id,turn_num);
		 printf("avatar %d moved from to (%d,%d)\n",
	avatar_id,cur_pos->xPos,cur_pos->yPos);
		//fflush(stdout);
		 //printf("goal is (%d,%d)\n",goal->xPos,goal->yPos);
		 

		  
	    }else{
		printf("Maze Solved!!\n");
	    }

	    if (!semaphore_v(sem_id)){
		printf("semv failed\n");
		exit(1);
	    }

	}else{
	    //system("sleep 1")
	
            //if (!semaphore_p(sem_id)){
              //  printf("semaphore_p failed\n");
                //exit(1);
	    //}

	    sleep(2);

            if (recv(sockfd,recvline,sizeof(AM_MESSAGE),0)==0){
                printf("%d Couldnt receive message from the server\n",
						avatar_id);
                return 1;

            }

	    if (ntohl(recvline->message_type)==AM_AVATAR_TURN){
		turn_num=ntohl(recvline->msg.avatar_turn.TurnId);
	    }

	    if (ntohl(recvline->message_type)==AM_MAZE_SOLVED ||
	        ntohl(recvline->message_type)==AM_SERVER_TIMEOUT ||
		ntohl(recvline->message_type)==AM_TOO_MANY_MOVES){
		condition=0;
	    }
	    //system("sleep 1");
	    //turn_num++;
	    //if (turn_num==nAvatars){
	    //    turn_num=0;
	    //}
	



	}
	//printf("turn num %d\n",turn_num);
	//if (!semaphore_p(sem_id)){
	  //  printf("semaphore_p failed\n");
	    //exit(1);
	    //turn_num=get_turn_id();
	//}

	//turn_num=get_turn_id();


	//if (!semaphore_v(sem_id)){
	  // printf("semaphore v failed\n");
	  // exit(1);
	//}
        sleep(1);
	    


	test_counter++;
        //system("sleep 1");


    }
    if (avatar_id==0){
        sleep(2);
        del_semvalue(sem_id);
    }

    //free(goal);
    //free(mat);
    free(recvline);
    free(sendline);
    //free(data);
    //free(new_pos);
    //free(cur_pos);
    
    //free shared map
    if (avatar_id==0){
	print_converted_map();
        free_shared_memory();
    }else{
	//system("sleep 1");
    }
    printf("%d\n",avatar_id);
    return 0;

}

int get_turn_id(){
   shared_map* sh_map=get_shared_map();
   int id= sh_map->turnID;
   return id;
}

void update_turn_id(int new_turnID){
   shared_map* sh_map=get_shared_map();
   sh_map->turnID=new_turnID;
}

void print_shared_map(){
    shared_map* sh_map=get_shared_map();
    for (int i=0;i<sh_map->row;i++){
	for (int j=0;j<sh_map->col;j++){
	     printf("%d",sh_map->map[i][j]);
        }
	printf("\n");
    }



}

void print_converted_map(){
    matrix* mat=convert_map();
    for (int i=0;i<mat->row;i++){
	for (int j=0;j<mat->column;j++){
	     printf("%c",mat->matrix[i][j]);
	}
	printf("\n");

    }
    free(mat);

}

matrix* convert_map(){
    shared_map* sh_map=get_shared_map();
    matrix* mat=malloc(sizeof(matrix));
    MALLOC_CHECK(mat);
    mat->row=sh_map->row;
    mat->column=sh_map->col;
    char child_mat[sh_map->row][sh_map->col];
    for (int i=0;i<sh_map->row;i++){
	BZERO(child_mat[i],sh_map->col);
        BZERO(mat->matrix[i],sh_map->col);
	for (int j=0;j<sh_map->col;j++){
	    if (i % 2==0 && j % 2==0){
		if (sh_map->map[i][j]==7){
		     child_mat[i][j]='A';
		     mat->matrix[i][j]='A';
		//}else if (sh_map->map[i][j]==9){
		  //   mat->matrix[i][j]='V';
		}else{
		     child_mat[i][j]='E';
		     mat->matrix[i][j]='E';
		}
	    }else if (i % 2==0 && j % 2!=0){
		if (sh_map->map[i][j]==1){
		     child_mat[i][j]='1';
		     mat->matrix[i][j]='1';
		}else{
		     child_mat[i][j]='0';
		     mat->matrix[i][j]='0';
		}
	    }else if (i % 2!=0 && j % 2==0){
		if (sh_map->map[i][j]==1){
		     child_mat[i][j]='_';
		     mat->matrix[i][j]='_';
		}else{
		     child_mat[i][j]='0';
		     mat->matrix[i][j]='0';
		}
	    }else if (i % 2!=0 && j % 2!=0){
		child_mat[i][j]='Z';
		mat->matrix[i][j]='Z';
	    }	     

	}
    }
    //mat->matrix=child_mat;
    return mat;


}

void mark_as_wall(XYPOS* wall){
     shared_map* sh_map=get_shared_map();
     sh_map->map[(wall->yPos)][(wall->xPos)]=1;
     printf("marking wall at %d,%d\n",wall->xPos,wall->yPos);
}


XYPOS* get_goal(){
     printf("Calculating goal\n");
     //shared_map* sh_map=get_shared_map();
     matrix* mat=convert_map();
     int path[100];
     XYPOS* avatar_list[10];
     int index;
     for (int i=0;i<mat->row;i++){
	for (int j=0;j<mat->column;j++){
	      if (mat->matrix[i][j]=='A'){
		printf("Found avatar %d\n",index);
		XYPOS* av=malloc(sizeof(XYPOS));
		av->xPos=j/2;
		av->yPos=i/2;
	        avatar_list[index]=av;
		index++;
	      }
	}
     }
     int min_dist=10000;
     int min_path[100];
     int min_avatar1;
     //int min_avatar2;
     int counter=0;
     for (int i=0;i<index;i++){
	for (int j=i+1;j<index;j++){
	     for (int k=0;k<100;k++){
		path[k]=-5;
	     }
	     printf("looking for a path between (%d,%d) and (%d,%d)\n",
		avatar_list[i]->xPos,avatar_list[i]->yPos,
		avatar_list[j]->xPos,avatar_list[j]->yPos);
	     find_path(mat,avatar_list[i],avatar_list[j],path);
	     counter=0;
	     while (path[counter]!=-5 && counter<100){
		counter++;
	     }
	     if (counter<min_dist && counter!=0){
		min_dist=counter;
		min_avatar1=i;
		//min_avatar2=j;
		for (int k=0;k<100;k++){
		    min_path[k]=-5;
		}
		for (int k=0;k<counter;k++){
		    min_path[k]=path[k];
		}
	     }
	}
     }

     XYPOS* start=avatar_list[min_avatar1];
     XYPOS* goal=malloc(sizeof(XYPOS));
     goal->xPos=start->xPos;
     goal->yPos=start->yPos;
     if (counter==1){
	//goal->xPos=start->xPos;
	//goal->yPos=start->yPos;
	return goal;
     }else{
        for (int i=0;i<counter/2;i++){
	    if (min_path[i]==0){
		goal->xPos-=1;	
	    }else if (min_path[i]==1){
		goal->yPos-=1;
	    }else if (min_path[i]==2){
		goal->yPos+=1;
	    }else if (min_path[i]==3){
		goal->xPos+=1;
	    }
	}
     }
     for (int i=0;i<index;i++){
	free(avatar_list[i]);
     }
     return goal;

}

XYPOS* get_average(){
    //printf("inside get average\n");
    shared_map* sh_map=get_shared_map();
    //printf("got shared map inside get avg function\n");
    int sum_x=0;
    int sum_y=0;
    int count=0;
    //printf("%d,%d\n",sh_map->row,sh_map->col);
    for (int i=0;i<sh_map->row;i++){
        //printf("outer\n");
	for (int j=0;j<sh_map->col;j++){
	     //printf("%d",sh_map->map[i][j]);
	     if (sh_map->map[i][j]==7){
		//printf("if\n");
		sum_x+=j;
		sum_y+=i;
		count++;
	     }
	 }
    }
    XYPOS* goal=malloc(sizeof(XYPOS));
    MALLOC_CHECK(goal);
    //printf("count %d\n",count);
    goal->xPos=sum_x/count/2;
    goal->yPos=sum_y/count/2;
    //printf("inside get average\n");
    //printf("goal :(%d,%d)\n",goal->yPos,goal->xPos);
    return goal;

}


void update_shared_map(XYPOS* old, XYPOS *new){
     if (old!=NULL){
       //printf("Update Map:\n"); 
       //printf("%d,%d\n",old->xPos,old->yPos);
       //printf("%d,%d\n",new->xPos,new->yPos);
     }
     if (old==NULL){
         shared_map* sh_map=get_shared_map();
         sh_map->map[(new->yPos)*2][(new->xPos)*2]=7;
     }else{
	 shared_map* sh_map=get_shared_map();
	 sh_map->map[(old->yPos)*2][(old->xPos)*2]=9;
	 sh_map->map[(new->yPos)*2][(new->xPos)*2]=7;
     }

}


shared_map* get_shared_map(){
    void *shared_memory=(void*)0;
    shared_map *sh_map;

    int shmid;
    shmid=shmget((key_t)1323,sizeof(shared_map),0666);// | IPC_CREAT);

    if (shmid ==-1){
        printf("failed to get shared memory inside get_shared_map\n");
        exit(1);

    }

    shared_memory=shmat(shmid,(void*)0,0);
    if (shared_memory == (void*)-1){
        printf("shmat command failed\n");
        exit(1);
    }

    sh_map=(shared_map*)shared_memory;
 
    return sh_map;
}

void free_shared_memory(){
    void *shared_memory=(void*)0;
    //shared_map *sh_map;

    int shmid;
    shmid=shmget((key_t)1323,sizeof(shared_map),0666);// | IPC_CREAT);

    if (shmid ==-1){
        printf("failed to get shared memory\n");
        exit(1);

    }

    shared_memory=shmat(shmid,(void*)0,0);

    if (shmdt(shared_memory)==-1){
	printf("shmdt failed\n");
	exit(1);
    }

    if (shmctl(shmid,IPC_RMID,0)==-1){
	printf("shmctl failed\n");
	exit(1);
    }

}



/* The function set_semvalue initializes the semaphore using the SETVAL command in a
 semctl call. We need to do this before we can use the semaphore. 

static int set_semvalue(void)
{
    union semun sem_union;

    sem_union.val = 1;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) return(0);
    return(1);
}  */

/* The del_semvalue function has almost the same form, except the call to semctl uses
 the command IPC_RMID to remove the semaphore's ID. 

static void del_semvalue(void)
{
    union semun sem_union;
    
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
        fprintf(stderr, "Failed to delete semaphore\n");
} */

/* semaphore_p changes the semaphore by -1 (waiting). 

static int semaphore_p(void)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = -1; // P() 
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_p failed\n");
        return(0);
    }
    return(1);
} */

/* semaphore_v is similar except for setting the sem_op part of the sembuf structure to 1,
 so that the semaphore becomes available. 

static int semaphore_v(void)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; // V() 
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_v failed\n");
        return(0);
    }
    return(1);
} */


