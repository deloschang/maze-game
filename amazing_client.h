//#if defined (__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)

//#else

 //   union semun{
//	int val;
//	struct semid_ds *buf;
//	unsigned short int *array;
//	struct seminfo *__buf;
  //  };
//#endif

#define DIM 1000
#define MAX_MOVE_DIM 200000

typedef struct _matrix {
     int row;
     int column;
     char matrix[DIM][DIM];
} matrix;


//ALl of the  amazing client functions

//converts map into the structure required for graphics
matrix* convert_map();

//marks a wall in the map
void mark_as_wall(XYPOS* wall);

//gets the goal coordinates for the specific
XYPOS* get_goal(XYPOS* start);

//updates shared map with new info
void update_shared_map(XYPOS* old,XYPOS* updated,int x[],int y[],int counter);

//graphics function to render the maze
void render_maze();

//frees shared memory resources
void free_shared_memory();

//prints our shared map in ASCII format
void print_converted_map();
void print_shared_map();

//gets turn id from the shared memory resource if necessary
int get_turn_id();

//updates turn id to the shared memory resource
void update_turn_id(int new_turn);

//function to update the graphics
void update_graphics();


//ALl bfs related functions
//and its data structures such as queues, cell ,etc (all used in bfs.c)

typedef struct _cell{
     int x;
     int y;
     struct _cell* parent;
     struct _cell* next;
     struct _cell* prev;
     int dist;
     int is_visited;
}cell;

typedef struct _queue{
     cell* head;
}queue;

//enqueues the queue with the newly created cell object
void enqueue(queue* q,cell* c);

//dequeues the first entry in the queue
cell* dequeue(queue* q);

//finds path between two positions
void find_path(matrix* mat,XYPOS* start,XYPOS* end,int path[]);

//given a cell constructs a path
//(cell contains a parent so it's easy to backtrack how path
//was created and recreate it)
void construct_path(cell* c,int path[]);

//cell structure initializer
cell* init_cell(int x,int y,int d,int mark,cell* parent);

//frees all of the elements in the queue
void free_queue(queue* q);

//frees all of the elements and their parents in the queue
void free_queue_advanced(queue* q1,queue* q2);

//checkes whether queue contains some specific coordinates already
int contains(queue* q,int x,int y);

//function to print the path (for debugging purposes);
void print_path(int path[],int length);

