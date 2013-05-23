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
//#include <sys/sem.h>

//static int set_semvalue(void);
//static void del_semvalue(void);
//static int semaphore_p(void);
//static int semaphore_v(void);

//static int sem_id;

int main(int argc,char* argv[]){
    int avatar_id=atoi(argv[1]);
    int nAvatars=atoi(argv[2]);
    int maze_diff=atoi(argv[3]);

    char server_ip[AM_MAX_MESSAGE];
    BZERO(server_ip,AM_MAX_MESSAGE);
    strncpy(server_ip,argv[4],AM_MAX_MESSAGE);

    int maze_port=atoi(argv[5]);
    
    char log_file[AM_MAX_MESSAGE];
    BZERO(log_file,AM_MAX_MESSAGE);
    strncpy(log_file,argv[6],AM_MAX_MESSAGE);

    //establishing server connection

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

    int turn_num=ntohl(recvline->msg.TurnId);
    XYPOS init_pos=recvline->msg.Pos[avatar_id];

    //add semaphore (not using for the moment)
    
    //int i;
    //int pause_time;
    //char op_char='O';

    //sem_id=semget((key_t)1324,1,0666 | IPC_CREAT);

    //--------
    int counter=turn_num;
    for (int i=0;i<AM_MAX_AVATAR;i++){
	if (counter==avatar_id){
	     update_shared_map(NULL,init_pos);
	}
	counter++;
    } 

    XYPOS* goal=NULL;
    matrix* mat=NULL;
    //goal=get_average();
    //update_shared_map(init_pos);
    int condition=1;
    XYPOS* cur_pos=init_pos;

    //main loop to solve the maze
    while (condition){

	if (turn_num==avatar_id){
	   if (mat!=NULL){
		free(mat);	
	   }
	   matrix* mat=convert_map();
	   if (goal!=NULL){
	       free(goal);
	   }
	   goal=get_average();

	   int next_move=find_move(cur_pos,goal,mat);
	   if (next_move==-1){
	        printf("No valid move is possible. Exiting\n");
	        exit(1);
	   }

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
		FILE fp*=fopen(log_file,"a");
                if (fp!=NULL){
		    fprintf(fp,"%d,"recvline->msg.maze_solved.Difficulty);
		    fprintf(fp,"%d," recvline->msg.maze_solved.nAvatars);
		    fprintf(fp,"%d," recvline->msg.maze_solved.nMoves);
		    fprintf(fp,"%d," recvline->msg.maze_solved.Hash);
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
	        XYPOS* new_pos=recvline->msg.avatar_turn.Pos[avatar_id];
	        if (new_pos->xPos==cur_pos->xPos
			&& new_pos->yPos==cur_pos->yPos){
		    //mark as wall
		    XYPOS* wall=cur_pos;
		    if (next_move==M_WEST){
		         wall->xPos=wall->xPos-1;
		         mark_as_wall(wall);
		    }else if (next_move==M_NORTH){
		        wall->yPos+=-1;
		        mark_as_wall(wall);
		    }else if (next_move==M_SOUTH){
		        wall->yPos=wall->yPos+1;
  		        mark_as_wall(wall);
		    }else if (next_move==M_EAST){
		        wall->xPos+=1;
		        mark_as_wall(wall);
		    }
	         }else{
	            update_shared_map(cur_pos,new_pos);
		    //ALSO UPDATE GRAPHICS (add later)
		    cur_pos=new_pos;
	         }
	         char coord[50];
	         BZERO(coord,50);
	         snprintf(coord,50,"%d",cur_pos->xPos);
	         char log_file_msg[AM_MAX_MESSAGE];
	         BZERO(log_file_msg,AM_MAX_MESSAGE);
	         strncpy(log_file_msg,"echo ",AM_MAX_MESSAGE);
	         strncat(log_file_msg,"Avatar ",AM_MAX_MESSAGE);
	         strncat(log_file_msg,argv[1],AM_MAX_MESSAGE);
	         strncat(log_file_msg," moved to (",AM_MAX_MESSAGE);
	         strncat(log_file_msg,coord,AM_MAX_MESSAGE);
	         strncat(log_file_msg,",",AM_MAX_MESSAGE);
	
	         BZERO(coord,50);
	         snprintf(coord,50,"%d",cur_pos->yPos);
	    
	         strncat(log_file_msg,coord,AM_MAX_MESSAGE);
	         strncat(log_file_msg,") >>",AM_MAX_MESSAGE);
	         strncat(log_file_msg,log_file,AM_MAX_MESSAGE);
	         system(log_file_msg);
	    
	         turn_num=ntohl(recvline->msg.avatar_turn.TurnId);
	    }else{
		printf("shouldnt happen\n");
	    }

	}else{
            if (recv(sockfd,recvline,sizeof(AM_MESSAGE),0)==0){
               printf("Couldnt receive message from the server\n");
               return 1;

            }

	    if (ntohl(recvline->message_type)==AM_AVATAR_TURN){
		turn_num=ntohl(recvline->msg.avatar_turn.TurnId);
	    }

	    if (ntohl(recvline->message_type)==AM_MAZE_SOLVED ||
	       ntohl(recvline->message_type)==AM_SERVER_TIMEOUT ||
		ntohl(recvline->message_type)==AM_AVATAR_TOO_MANY_MOVES){
		condition=0;
	    }
	



	}

	



    }

    free(goal);
    free(mat);
    free(recvline);
    free(sendline);

    
    //free shared map
    free_shared_memory();
    return 0;

}

matrix* convert_map(){
    shared_map* sh_map=get_shared_map();
    matrix* mat=malloc(sizeof(matrix));
    MALLOC_CHECK(mat);
    mat->row=sh_map->row;
    mat->col=sh_map->col;
    char child_mat[sh_map->row][sh_map->col];
    for (int i=0;i<sh_map->row;i++){
	BZERO(child_mat[i],sh_map->col);
	for (int j=0;j<sh_map->col;j++){
	    if (i % 2==0 && j % 2==0){
		if (sh_map->map[i][j]==7){
		     child_mat[i][j]='.';
		}else{
		     child_mat[i][j]='E';
		}
	    }else if (i % 2==0 && j % 2!=0){
		if (sh_map->map[i][j]==1){
		     child_mat[i][j]='1';
		}else{
		     child_mat[i][j]='0';
		}
	    }else if (i % 2!=0 && j % 2==0){
		if (sh_map->map[i][j]==1){
		     child_mat[i][j]='_';
		}else{
		     child_mat[i][j]='0';
		}
	    }else if (i % 2!=0 && j % 2!=0){
		child_mat[i][j]='Z';
	    }	     

	}
    }
    mat->matrix=(char**)child_mat;
    return mat;


}

void mark_as_wall(XYPOS* wall){
     shared_map* sh_map=get_shared_map();
     sh_map->map[(wall->yPos)*2][(wall->xPos)*2]=1;
}

XYPOS* get_average(){
    shared_map* sh_map=get_shared_map();
    int sum_x=0;
    int sum_y=0;
    int count=0;
    for (int i=0;sh_map->row;i++){
	for (int j=0;j<sh_map->col;j++){
	     if (sh_map[i][j]==7){
		sum_x+=j;
		sum_y+=i;
		count++;
	     }
	 }
    }
    XYPOS* goal=malloc(sizeof(XYPOS));
    MALLOC_CHECK(goal);
    goal->xPos=sum_x/count/2;
    goal->yPos=sum_y/count/2;
    return goal;

}


void update_shared_map(XYPOS* old, XYPOS *new){
     if (old==NULL){
         shared_map* sh_map=get_shared_map();
         sh_map->map[(new->yPos)*2][(new->xPos)*2]=7;
     }else{
	 shared_map* sh_map=get_shared_map();
	 sh_map->map[(old->yPos)*2][(old->xPos)*2]=3;
	 sh_map->map[(new->yPos)*2][(new->xPos)*2]=7;
     }

}


shared_map* get_shared_map(){
    void *shared_memory=(void*)0;
    shared_map *sh_map;

    int shmid;
    shmid=shmget((key_t)1323,sizeof(shared_map),0666);// | IPC_CREAT);

    if (shmid ==-1){
        printf("failed to get shared memory\n");
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
    shared_map *sh_map;

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

    if (shmct1(schmid,IPC_RMID,0)==-1){
	printf("shmct1 failed\n");
	exit(1);
    }

}
