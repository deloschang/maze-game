// DEFINES

// the max dimensions of an aspect of the max (REWORD)
#define MAX_DIM 400

//PROTOTYPES: functions that will be used in AMStartup.c

//function to retrieve Ip number of given host (stratton in our case);
int retrieveIP(char* host,char* IP);

//function to check the validity of the arguments
int check_arguments(int argc,char* host,char* avatars,char* diff);

//function to retrieve the arguments from the command line
void retrieve_arguments(int argc,char* argv[],char* host,char* avatars,
								char* diff);
//creates a new shared memory resource and initialize it with some default map values
void create_shared_map(int w,int h);

//function that returns shared map from the shared memory resource                           
shared_map* get_shared_map();


//DATA STRUCTURES

//shared map data structure that will be stored in shared memory segment
typedef struct _shared_map{
    int row;
    int col;
    int map[MAX_DIM][MAX_DIM]; // the map
    int turnID;                // the turn number

} shared_map;

