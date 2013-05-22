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
//#include <unistd.h>
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

    //add semaphore
    int i;
    int pause_time;
    char op_char='O';

    sem_id=semget((key_t)1324,1,0666 | IPC_CREAT);
    update_shared_map(init_pos);

    free(recvline);
    free(sendline);
    //free shared map

}


