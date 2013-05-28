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
#include "utils/amazing.h"
#include "utils/amazing_client.h"
#include <unistd.h>
#include "utils/header.h"
#include <sys/sem.h>
#include "utils/semun.h"
#include <gtk/gtk.h> // for gtk functionality
#include <pthread.h>

/**
 * start_graphics
 * Starts the graphics by rendering the maze
 **/ 
void* start_graphics(void *ptr){
    render_maze();
    return NULL;
}

extern char data2[DIM][DIM];
extern int data_row_length;
extern int data_column_length;


//amazing client that starts avatar process and guides it through the 
//connection with the server to solve the maze using boosted bfs algorithm.
int main(int argc,char* argv[]){

    int avatar_id=atoi(argv[1]);
    int sem_id; //id for the semaphore

    //creating a  semaphore (only avatar 0 does it so that there wouldnt 
    //be duplicate semahores)
    if (avatar_id==0){
        sem_id=semget((key_t)1324,1,0666 | IPC_CREAT);
        if (!set_semvalue(sem_id)){
            printf("failed to initialize semaphore\n");
            exit(1);
        }

        // Start updating the graphics
        update_graphics();

        gtk_init(&argc, &argv);

        pthread_t t1;
        int iret1 = pthread_create(&t1, NULL, start_graphics, NULL);
        if (iret1) {
            fprintf(stderr,
                    "pthread_create failed, rc=%d\n",iret1);
            exit(iret1);
        }
        

    }else{//other avatars get it after avatar 0 creates it
        system("sleep 1");
        sem_id=semget((key_t)1324,1,0666);
    }

    //paramater passed from AMStartup
    int nAvatars=atoi(argv[2]);
    //int maze_diff=atoi(argv[3]);  unused paramater
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
    BZERO(sendline,sizeof(AM_MESSAGE));
    AM_MESSAGE* recvline=malloc(sizeof(AM_MESSAGE));
    BZERO(recvline,sizeof(AM_MESSAGE));
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

    //constructing AM_AVATAR_READY message and sending it to the server
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

    //extracting paramaters from the server message
    int turn_num=ntohl(recvline->msg.avatar_turn.TurnId);
    XYPOS* init_pos=malloc(sizeof(XYPOS));
    init_pos->xPos=ntohl(recvline->msg.avatar_turn.Pos[avatar_id].xPos);
    init_pos->yPos=ntohl(recvline->msg.avatar_turn.Pos[avatar_id].yPos);

    //using the semaphore to modify our shared map (segment of shared memory)

    if (!semaphore_p(sem_id)){
        exit(1);
    }
    sleep(rand() % 2);
    int empty_param[AM_MAX_AVATAR]={0,0,0};//value doesnt matter
    update_shared_map(NULL,init_pos,empty_param,empty_param,0);

    if (!semaphore_v(sem_id)){//releasing the semaphore
        exit(1);
    } 

    printf("%d\n",avatar_id);
    if (avatar_id==0){//trying to make so that avatar 0 would be ahead of all
        //of the other avatars to begin with

    }else{
        sleep(1*avatar_id);
    }



    XYPOS* goal=NULL;

    int condition=1;//condition that regulates the loop to solve the maze
    int test_counter=0;
    XYPOS* cur_pos=malloc(sizeof(XYPOS));
    cur_pos->xPos=init_pos->xPos;
    cur_pos->yPos=init_pos->yPos;//initializing current position
    free(init_pos);
    XYPOS* new_pos=malloc(sizeof(XYPOS));


    int path[MAX_MOVE_DIM];//this variable will 
    //store integers representing which path
    //the avatar should take
    int path_count=0;

    //main loop where we solve the maze
    while (condition){ 

        if (turn_num==avatar_id){//only making a move if it's avatar's turn

            //grabbing semaphore to access shared memory
            if (!semaphore_p(sem_id)){
                printf("semp failed\n");
                exit(1);
            }

            matrix* mat=convert_map();

            //resetting the path for the avatar
            for (int l=0;l<MAX_MOVE_DIM;l++){
                path[l]=-5;
            }
            //get_goal serves as a function to determine optimal
            //goal point. It will usually pick the middle point between
            //two closest avatars. However in case they are close to each other
            //some fixed point is returned
            goal=get_goal(cur_pos);
            find_path(mat,cur_pos,goal,path);//calling bfs function to
            //construct a path
            path_count=0;

            free(mat);
            free(goal);

            int next_move;

            next_move=path[path_count];

            //sending message to the server to move
            BZERO(sendline,sizeof(AM_MESSAGE));
            BZERO(recvline,sizeof(AM_MESSAGE));
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

            //in the case we solved the maze we append the log file
            //with required paramaters
            if (ntohl(recvline->message_type)==AM_MAZE_SOLVED){

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

                condition=0;//and set this variable to 0 so that maze
                //solving algorithm would stop

                //updating the map for the very last time
                int x_list[AM_MAX_AVATAR];
                int y_list[AM_MAX_AVATAR];
                int count=0;
                for (int i=0;i<nAvatars;i++){

                    if (i!=avatar_id){
                        x_list[count]=ntohl(recvline->msg.
                                avatar_turn.Pos[i].xPos);
                        y_list[count]=ntohl(recvline->msg.
                                avatar_turn.Pos[i].yPos);
                        count++;
                    }
                }
                update_shared_map(cur_pos,new_pos,x_list,y_list
                        ,count);

            }

            if (ntohl(recvline->message_type)==AM_TOO_MANY_MOVES
                    || ntohl(recvline->message_type)==AM_SERVER_TIMEOUT){
                condition=0;
            }

            if (ntohl(recvline->message_type)==AM_AVATAR_TURN && condition==1){
                //extracting coordinated of the new position from the
                //server message
                new_pos->xPos=ntohl(recvline->msg.avatar_turn.
                        Pos[avatar_id].xPos);
                new_pos->yPos=ntohl(recvline->msg.avatar_turn.
                        Pos[avatar_id].yPos);
                //checking if there was a wall in between positions we 
                //transitioned
                if (new_pos->xPos==cur_pos->xPos
                        && new_pos->yPos==cur_pos->yPos && next_move!=8){

                    XYPOS* wall=malloc(sizeof(XYPOS));
                    wall->xPos=cur_pos->xPos*2;
                    wall->yPos=cur_pos->yPos*2;
                    //figuring out where the wall was and updating our
                    //shared map with this knowledge
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

                }else {//in case of a valid move (not a wall)


                    int x_cords[AM_MAX_AVATAR];
                    int y_cords[AM_MAX_AVATAR];
                    int counter=0;
                    //loop to make sure we don't woverwrite
                    //other avatar positions when we update our shared map
                    for (int i=0;i<nAvatars;i++){

                        if (i!=avatar_id){
                            x_cords[counter]=ntohl(recvline->msg.
                                    avatar_turn.Pos[i].xPos);
                            y_cords[counter]=ntohl(recvline->msg.
                                    avatar_turn.Pos[i].yPos);
                            counter++;
                        }
                    }
                    update_shared_map(cur_pos,new_pos,x_cords,y_cords
                            ,counter);



                }
                // New position so update the server
                update_graphics();

                //updating our current position
                cur_pos->xPos=new_pos->xPos;
                cur_pos->yPos=new_pos->yPos;

                //constructing a message to log file
                char next_move_str[50];
                BZERO(next_move_str,50);
                snprintf(next_move_str,50,"%d",next_move);
                char coord[50];
                BZERO(coord,50);
                snprintf(coord,50,"%d",cur_pos->xPos);
                char log_file_msg[AM_MAX_MESSAGE];
                BZERO(log_file_msg,AM_MAX_MESSAGE);

                strncpy(log_file_msg,"echo ' ",AM_MAX_MESSAGE);
                strncat(log_file_msg,"Avatar used this move: ",AM_MAX_MESSAGE);
                strncat(log_file_msg,next_move_str,AM_MAX_MESSAGE);
                strncat(log_file_msg,"' >> ",AM_MAX_MESSAGE);
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
            }else{
                printf("Maze Solved!!\n");
            }
            //after we updated our share map and did everything else
            //releasing the semaphore so that other process could access 
            //shared map and modify it
            if (!semaphore_v(sem_id)){
                printf("semv failed\n");
                exit(1);
            }

        }else{//in case it wasn't that avatar's turn to move


            //still listening for the server to extract new turn id
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



        }
        //sleep(1);   
        test_counter++;


    }
    //just finished solving the maze
    //need to free all of the resources ONCE
    //let avatar 0 sleep so that other processes could finish using those
    //resources, and then free everything
    if (avatar_id==0){
        sleep(nAvatars);
        del_semvalue(sem_id);
    }

    //freeing messages and variables
    free(recvline);
    free(sendline);
    free(new_pos);
    free(cur_pos);

    //once again only avatar 0 frees shared memory
    if (avatar_id==0){
        print_converted_map();
        free_shared_memory();
    }
    return 0;

}

//function to get turn id from the shared memory segment if necessary
int get_turn_id(){
    shared_map* sh_map=get_shared_map();
    int id= sh_map->turnID;
    return id;
}

//function to update turn id in the shared memory segment
void update_turn_id(int new_turnID){
    shared_map* sh_map=get_shared_map();
    sh_map->turnID=new_turnID;
}

//prints shared map in ASCII (for debugging purposes)
void print_shared_map(){
    shared_map* sh_map=get_shared_map();
    for (int i=0;i<sh_map->row;i++){
        for (int j=0;j<sh_map->col;j++){
            printf("%d",sh_map->map[i][j]);
        }
        printf("\n");
    }



}
//function that prints the map in the format used by graphics file
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


//function that converts shared map from shared memory segment into
//the map format used by the graphics (int[][] --> char[][] )
matrix* convert_map(){
    shared_map* sh_map=get_shared_map();
    matrix* mat=malloc(sizeof(matrix));
    MALLOC_CHECK(mat);
    mat->row=sh_map->row;
    mat->column=sh_map->col;
    for (int i=0;i<sh_map->row;i++){
        for (int j=0;j<sh_map->col;j++){
            mat->matrix[i][j]='\0';
            if (i % 2==0 && j % 2==0){
                if (sh_map->map[i][j]==7){
                    mat->matrix[i][j]='A';
                }else{
                    mat->matrix[i][j]='E';
                }
            }else if (i % 2==0 && j % 2!=0){
                if (sh_map->map[i][j]==1){
                    mat->matrix[i][j]='1';
                }else{
                    mat->matrix[i][j]='0';
                }
            }else if (i % 2!=0 && j % 2==0){
                if (sh_map->map[i][j]==1){
                    mat->matrix[i][j]='_';
                }else{
                    mat->matrix[i][j]='0';
                }
            }else if (i % 2!=0 && j % 2!=0){
                mat->matrix[i][j]='Z';
            }	     

        }
    }

    return mat;


}


//function that adds a wall to our shared map resource
void mark_as_wall(XYPOS* wall){
    shared_map* sh_map=get_shared_map();
    sh_map->map[(wall->yPos)][(wall->xPos)]=1;
}


//function that calculates the goal for the avatar
//there are coupel of cases:
//most often the function find closes avatar to the given start position
//(excluding himself) and computes middle point in that path
//if the avatars are clustered together then
//the same avatar will be used as a goal for all of the other avatars
XYPOS* get_goal(XYPOS* start_pos){
    matrix* mat=convert_map();
    int path[MAX_MOVE_DIM];
    XYPOS* avatar_list[10];
    int index=0;
    for (int i=0;i<mat->row;i++){
        for (int j=0;j<mat->column;j++){
            if (mat->matrix[i][j]=='A'){//signifies location of the avatar

                XYPOS* av=malloc(sizeof(XYPOS));
                MALLOC_CHECK(av);
                av->xPos=j/2;//converting to the server format coordinates
                av->yPos=i/2;
                avatar_list[index]=av;
                index++;
            }
        }
    }
    //free(mat);
    //portion of the function to compute max and min distances between
    //the avatars
    int max_dist=-10000;
    int min_dist=10000;
    int min_path[MAX_MOVE_DIM];
    //int min_avatar;
    //int max_avatar;

    int counter=0;//variable that will measure the distance between 2 avatars
    for (int i=0;i<index;i++){
        //resetting the path
        for (int k=0;k<MAX_MOVE_DIM;k++){
            path[k]=-5;
        }
        //only updating this if the avatar is not equal to itself or
        //other avatar that is in the same position already
        if (avatar_list[i]->yPos != start_pos->yPos ||
                avatar_list[i]->xPos != start_pos->xPos){
            find_path(mat,start_pos,avatar_list[i],path);
            counter=0;
            while (path[counter]!=-5 && counter<MAX_MOVE_DIM){
                counter++;//counting the distance between two avatrs
            }
            if (counter<min_dist){//updating new min distance
                //if condition was sattisfied
                min_dist=counter;
                //min_avatar=i;
                for (int k=0;k<MAX_MOVE_DIM;k++){
                    min_path[k]=-5;
                }
                for (int k=0;k<counter;k++){
                    min_path[k]=path[k];
                }
            }
            //updating new max distance
            if (counter>max_dist){
                max_dist=counter;
                //max_avatar=i;
            }
        }

    }

    //finally initializing what goal should be
    XYPOS* goal=malloc(sizeof(XYPOS));
    goal->xPos=start_pos->xPos;
    goal->yPos=start_pos->yPos;

    free(mat);
    //if max_dist<7 that means all of the avatars are in the cluster
    if (max_dist<10){ 
        //assigning the coordinates as a goal that should be in the middle
        //of all of the avatars
        goal->xPos=avatar_list[index/2]->xPos;
        goal->yPos=avatar_list[index/2]->yPos;

        for (int j=0;j<index;j++){
            free(avatar_list[j]);
        }
        return goal;
    }

    //computing the middle point between the shortest path we just 
    //calculated
    //values in the path signify the direction to go
    int size=min_dist/2+1;
    if (size==0){
        size++;
    }
    for (int i=0;i<size;i++){
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
    //freeing all of the resources
    for (int i=0;i<index;i++){
        free(avatar_list[i]);
    }
    return goal;

}

//function that updates shared map resource with the new positions
void update_shared_map(XYPOS* old, XYPOS *new, int x_cords[],
        int y_cords[],int counter){

    //case for the very first update right after initialization of avatars
    if (old==NULL || counter==0){
        shared_map* sh_map=get_shared_map();
        sh_map->map[(new->yPos)*2][(new->xPos)*2]=7;//7 signifies avatar's
        //location
    }else{
        shared_map* sh_map=get_shared_map();
        int is_avatar_there=0;//condition to check whether we are not
        //overwriting other avatar's location
        for (int i=0;i<counter;i++){
            if (old->xPos==x_cords[i] &&
                    old->yPos==y_cords[i]){
                is_avatar_there=1;
            }
        }
        if (is_avatar_there){
            sh_map->map[(old->yPos)*2][(old->xPos)*2]=7;
        }else{
            sh_map->map[old->yPos*2][old->xPos*2]=9;//9 is just a regular 
            //entry in the map that's
            //not occupied by avatar
        }

        sh_map->map[(new->yPos)*2][(new->xPos)*2]=7;//upating new postion

        //NOTE: we need to multiple by 2 because our representation of the
        //map is doube the size of the original map
    }

}

//function that retrieves shared map from the shared memory segment
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


// Called each time to update the graphics map
void update_graphics(){

    // Convert the shared map into the 2D char array in matrix
    // struct form
    matrix* graphics_map = convert_map();

    data_row_length = graphics_map->row;
    data_column_length = graphics_map->column;

    // Move the 2D char array map into the global 2D char array
    // Graphics will use this to render
    for (int i = 0; i < graphics_map->row; i++){
        for (int j = 0; j < graphics_map->column; j++){
            data2[i][j] = graphics_map->matrix[i][j];
        }
    }
}

//function to free shared memory segments
void free_shared_memory(){
    void *shared_memory=(void*)0;

    int shmid;
    shmid=shmget((key_t)1323,sizeof(shared_map),0666);

    if (shmid ==-1){
        printf("failed to get shared memory\n");
        exit(1);

    }

    shared_memory=shmat(shmid,(void*)0,0);

    if (shmdt(shared_memory)==-1){
        exit(1);
    }

    if (shmctl(shmid,IPC_RMID,0)==-1){
        exit(1);
    }

}

