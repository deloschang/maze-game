//functions that will be used in AMStartup.c

int retrieveIP(char* host,char* IP);
int check_arguments(int argc,char* host,char* avatars,char* diff);
void retrieve_arguments(int argc,char* argv[],char* host,char* avatars,
								char* diff);

void create_shared_map(int w,int h);
//shared_map* get_shared_map();

#define MAX_DIM 200

typedef struct _shared_map{
    int row;
    int col;
    int map[MAX_DIM][MAX_DIM];
    int turnID;

} shared_map;

shared_map* get_shared_map();
