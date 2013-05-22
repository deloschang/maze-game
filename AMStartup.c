//File: AMStartup.c
//
//Description: Sends AM_INITIALIZE message to the server. After the server
//responds, the program extracts MazePort value from that reply.Then it creates
//a new log file Amazing_$USER_N_D.log where N and D depicts number of avatars
//and difficulty of the maze respectively.Finally it calls the amazing client
//with the required parameters such as starting avatar id number, number of
//avatars, difficulty, IP address, MazePort and name of the log file that
//was created earlier.
//
//Pseudo Code:
//
//1) Checks validity of the arguments
//
//2)Constructs and sends AM_INITIALIZE message to the server
//
//3)Gets the response from the server and retrieves MazePort from the message
//
//4)Creates a log file
//
//5)Launches Amazing client program

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include "amazing.h"
#include "AMStartup.h"
#include "header.h"
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/shm.h>

int main(int argc,char* argv[]){

    char host_name[AM_MAX_MESSAGE];
    BZERO(host_name,AM_MAX_MESSAGE);
    char num_avatars_str[AM_MAX_MESSAGE];
    BZERO(num_avatars_str,AM_MAX_MESSAGE);
    char maze_difficulty_str[AM_MAX_MESSAGE];
    BZERO(maze_difficulty_str,AM_MAX_MESSAGE);

    int num_avatars;
    int maze_difficulty;

    retrieve_arguments(argc,argv,host_name,num_avatars_str,
						maze_difficulty_str);
    int check=check_arguments(argc,host_name,num_avatars_str,
					maze_difficulty_str);
    if (check!=0){
	printf("-n Usage: [nAvatars (0-10)]\n");
	printf("-d [maze difficulty (0-9)]\n");
	printf("-h [hostname (stratton.cs.dartmouth.edu)]\n");
	//printf("No switches\n");
	//printf("Arguments must come in this order\n");
	return 1;
    }

    //printf("%s\n",host_name);
    //printf("%s\n",num_avatars_str);
    //printf("%s\n",maze_difficulty_str);
    //char test_char[AM_MAX_MESSAGE];
    //BZERO(test_char,AM_MAX_MESSAGE);
    //retrieveIP(host_name,test_char);
    //printf("%s\n",test_char);
    //return 1;//testing

    num_avatars=atoi(num_avatars_str);
    maze_difficulty=atoi(maze_difficulty_str);
    //printf("%d\n",num_avatars);
    //printf("%d\n",maze_difficulty);
    //return 1;

    int sockfd;
    struct sockaddr_in servaddr;
    //char sendline[AM_MAX_MESSAGE], recvline[AM_MAX_MESSAGE];
    
    //constructing AM_INITIALIZE message
    AM_MESSAGE* sendline=malloc(sizeof(AM_MESSAGE));
    MALLOC_CHECK(sendline);
    sendline->message_type=htonl(AM_INITIALIZE);
    //struct initialize* init=malloc(sizeof(initialize));
    //MALLOC_CHECK(initialize);
    //init->nAvatars=num_avatars;
    //init->Difficulty=maze_difficulty;
    //sendline->initialize=init;
    sendline->msg.initialize.nAvatars=htonl(num_avatars);
    sendline->msg.initialize.Difficulty=htonl(maze_difficulty);


    AM_MESSAGE* recvline=malloc(sizeof(AM_MESSAGE));
    MALLOC_CHECK(recvline);

    //creating a socket
    if ((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
	printf("Failed to create the socket\n");
	return 1;
    }

    //retrieve server IP from the hostname
    char serverIP[AM_MAX_MESSAGE];
    BZERO(serverIP,AM_MAX_MESSAGE);
    check=retrieveIP(host_name,serverIP);
    if (check!=0){
 	return 1;
    }

    //creation of the socket
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(serverIP);
    servaddr.sin_port=htons(AM_SERVER_PORT);

    //establishing connection with the server
    if (connect(sockfd,(struct sockaddr *) &servaddr,sizeof(servaddr))<0){
	printf("Could not connect to the server\n");
	return 1;
    }

    //sending initialization message to the server
    send(sockfd,sendline,sizeof(AM_MESSAGE),0); //need to add more params
    if (recv(sockfd,recvline,sizeof(AM_MESSAGE),0)==0){
	printf("Server terminated\n");
	free(sendline);
	return 1;
    }

    if (ntohs(recvline->message_type) & AM_ERROR_MASK){
	printf("Corrupted message from the server\n");
	return 1;
    }

    free(sendline);

    //retrieving MazePort
    unsigned int  maze_port=ntohl(recvline->msg.initialize_ok.MazePort);
    char MazePort[AM_MAX_MESSAGE];
    BZERO(MazePort,AM_MAX_MESSAGE);
    snprintf(MazePort,AM_MAX_MESSAGE,"%d",maze_port);
 
    //getting maze dimensions
    unsigned int width=ntohl(recvline->msg.initialize_ok.MazeWidth);
    unsigned int height=ntohl(recvline->msg.initialize_ok.MazeHeight);
    
    
    //printf("%s\n",serverIP);
    //printf("%d\n",ntohl(recvline->message_type));
    //printf("%d\n",AM_INITIALIZE);
    //printf("%d\n",width);
    //printf("%d\n",height);
    //printf("%d\n",maze_port);


    //Creating shared map object

    //create_shared_map(width,height);

    //done with the recvline message
    free(recvline);   
    //return 1;//testing
    //preparing to create a log file
    char logFileCommand[AM_MAX_MESSAGE];
    BZERO(logFileCommand,AM_MAX_MESSAGE);
    char fileName[AM_MAX_MESSAGE];
    BZERO(fileName,AM_MAX_MESSAGE);
    char *userName;

    userName=getenv("USER"); //getting user name
    if (userName==NULL){
	printf("failed to retrieve user name\n");
	return 1;
    }else{
	strncpy(fileName,"Amazing_",AM_MAX_MESSAGE);
	strncat(fileName,userName,AM_MAX_MESSAGE);
	strncat(fileName,"_",AM_MAX_MESSAGE);
	strncat(fileName,num_avatars_str,AM_MAX_MESSAGE);
	strncat(fileName,"_",AM_MAX_MESSAGE);
	strncat(fileName,maze_difficulty_str,AM_MAX_MESSAGE);
	strncat(fileName,".log",AM_MAX_MESSAGE);
    }

    //creating a log file
    strncpy(logFileCommand,"echo -n "" > ",AM_MAX_MESSAGE);
    strncat(logFileCommand,fileName,AM_MAX_MESSAGE);
    system(logFileCommand);


    char date[AM_MAX_MESSAGE];
    BZERO(date,AM_MAX_MESSAGE);
    time_t rawtime;
    struct tm *timeinfo;
    
    //getting current data and time
    time(&rawtime);
    timeinfo=localtime(&rawtime);
    strftime(date,AM_MAX_MESSAGE,"%Y-%m-%d-%H-%M-%S",timeinfo);
    

    //adding first line to the log file
    BZERO(logFileCommand,AM_MAX_MESSAGE);
    strncpy(logFileCommand,"echo ",AM_MAX_MESSAGE);
    strncat(logFileCommand,userName,AM_MAX_MESSAGE);
    strncat(logFileCommand,", ",AM_MAX_MESSAGE);
    strncat(logFileCommand,MazePort,AM_MAX_MESSAGE);
    strncat(logFileCommand,", ",AM_MAX_MESSAGE);
    strncat(logFileCommand,date,AM_MAX_MESSAGE);
    strncat(logFileCommand,";",AM_MAX_MESSAGE);
    strncat(logFileCommand," >> ",AM_MAX_MESSAGE);
    strncat(logFileCommand,fileName,AM_MAX_MESSAGE);
    system(logFileCommand);

    return 1;
    //launching amazing client
    int avatar_id=0;
    for (int i=0;i<num_avatars;i++){
        char amazingCommand[AM_MAX_MESSAGE];
        BZERO(amazingCommand,AM_MAX_MESSAGE);
	char avatar_id_str[AM_MAX_MESSAGE];
	BZERO(avatar_id_str,AM_MAX_MESSAGE);
	snprintf(avatar_id_str,AM_MAX_MESSAGE,"%d",avatar_id);
        strncpy(amazingCommand,"./amazing_client ",AM_MAX_MESSAGE);
	strncat(amazingCommand,avatar_id_str,AM_MAX_MESSAGE);
	strncat(amazingCommand," ",AM_MAX_MESSAGE);
        strncat(amazingCommand,num_avatars_str,AM_MAX_MESSAGE);
        strncat(amazingCommand," ",AM_MAX_MESSAGE);
        strncat(amazingCommand,maze_difficulty_str,AM_MAX_MESSAGE);
        strncat(amazingCommand," ",AM_MAX_MESSAGE);
        strncat(amazingCommand,serverIP,AM_MAX_MESSAGE);
        strncat(amazingCommand," ",AM_MAX_MESSAGE);
        strncat(amazingCommand,MazePort,AM_MAX_MESSAGE);
        strncat(amazingCommand," ",AM_MAX_MESSAGE);
        strncat(amazingCommand,fileName,AM_MAX_MESSAGE);
        system(amazingCommand);
	avatar_id++;
    }
    return 0;



}

void create_shared_map(int w,int h){
    void *shared_memory=(void*)0;
    shared_map *sh_map;

    int shmid;
    shmid=shmget((key_t)1323,sizeof(shared_map),0666 | IPC_CREAT);
 
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
    
    for (int i=0;i<2*w-1;i++){
	for (int j=0;j<2*h-1;j++){
	    if (i % 2!=0 && j % 2 !=0){
		sh_map->map[i][j]=3;
	    }else if (i % 2!=0 && j % 2==0){
		sh_map->map[i][j]=9;
	    }else if (i % 2==0 && j % 2!=0){
		sh_map->map[i][j]=9;
	    }else if (i % 2==0 && j % 2==0){
		sh_map->map[i][j]=2;
	    }

	}
    }

}

void retrieve_arguments(int argc,char* argv[],char* host,
					char* avatars,char* diff){

   //int c;
   //char switch_str[50];
   //BZERO(switch_str,50);
   //char optarg[AM_MAX_MESSAGE];
   //BZERO(optarg,AM_MAX_MESSAGE);
   //while ((c=getopt(argc,argv,"ndh:"))!=-1){
   for (int i=0;i<argc;i++){
	//switch (c){
	  //  case 'n':
	  if (strncmp(argv[i],"-n",50)==0 && i+1<argc){
		strncpy(avatars,argv[i+1],AM_MAX_MESSAGE);
	  }
	    //    break;

	  //  case 'd':
	  if (strncmp(argv[i],"-d",50)==0 && i+1<argc){
		strncpy(diff,argv[i+1],AM_MAX_MESSAGE);
	  }
	//	break;

	  //  case 'h':
	  if (strncmp(argv[i],"-h",50)==0 && i+1<argc){
		strncpy(host,argv[i+1],AM_MAX_MESSAGE);
	  }
	//	break;

	  //  default:
	//	printf("Wrong switch\n");
	//	exit(1);
	//}

   }


}

int check_arguments(int argc,char* host,char* avatars,char* diff){
    if (argc!=7){
	printf("Wrong number of arguments (4 needed) \n");
	return 1;
    }

        if (atoi(avatars)==0 && avatars[0]!='0'){
	     printf("argument must be numeric\n");
	     return 1;
        }

	if (atoi(avatars)>=10){
	    printf("arguments cant exceed 10\n");
	    return 1;
        }

	if (atoi(avatars)<=1){
	    printf("There must be at least 2 avatars\n");
	    return 1;
	}


        if (atoi(diff)==0 && diff[0]!='0'){
             printf("argument must be numeric\n");
             return 1;
        }

        if (atoi(diff)>=10){
            printf("arguments cant exceed 10\n");
            return 1;
        }

        if (atoi(diff)<0){
            printf("Maze difficulty must be positive\n");
  	    return 1;
	}




    if (strncmp(host,"stratton.cs.dartmouth.edu",AM_MAX_MESSAGE)!=0){
	printf("You must use 'stratton.cs.dartmouth.edu' server\n");
        return 1;
    }
    return 0;

}


int retrieveIP(char* hostname,char* IP){
    struct hostent* host=gethostbyname(hostname);
    if (host==NULL){
	printf("error while getting the host \n");
	return 1;
    }

    //struct in_addr* address=(struct in_addr*)host->h_addr;
    //char *ip_address=inet_ntoa(address);
    struct in_addr address;//=(struct in_addr)host->h_addr_list[0];
    memcpy(&address,host->h_addr_list[0],sizeof(struct in_addr));
    //char *ip_address;=inet_ntoa(address);
    BZERO(IP,AM_MAX_MESSAGE);
    strncpy(IP,inet_ntoa(address),AM_MAX_MESSAGE);
    return 0;
    



}

