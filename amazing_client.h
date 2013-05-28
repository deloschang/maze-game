//#if defined (__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)

//#else

#define DIM 1000
#define MAX_MOVE_DIM 200000

typedef struct _matrix {
     int row;
     int column;
     char matrix[DIM][DIM];
} matrix;

//DATA STRUCTURES

// Matrix. This data structure holds a 2-dimensional array that represents the maze, and the dimensions 
// of the maze.   
typedef struct _matrix {
     int row;
     int column;
     char matrix[DIM][DIM];
} matrix;

// Cell. This is a data structure that holds the coordinates of a position on the map, a pointer to the 
// parent cell (the previous position), the distance between the cell and the goal (start?), and pointers
// to the next and previous cells in the queue.
typedef struct _cell{
     int x;
     int y;
     struct _cell* parent;
     struct _cell* next;
     struct _cell* prev;
     int dist;
     int is_visited;
}cell;

// The queue is a FIFO data structure with a pointer to the cell at the front of the queue.
typedef struct _queue{
     cell* head;
}queue;

//PROTOTYPES used by amazing_client.c

// Converts the shared map into the matrix data structure required for graphics, and returns a pointer to
// the matrix map.
matrix* convert_map();

// Marks a wall on the map at the position of the wall. '1' on the map represents a wall.
void mark_as_wall(XYPOS* wall);

// Calculates the goal coordinates given the Avatar coordinates and returns them. 
XYPOS* get_goal(XYPOS* start);

// Updates the shared map with new positions of the Avatars
void update_shared_map(XYPOS* old,XYPOS* updated,int x[],int y[],int counter);

// Graphics function that renders the maze.
void render_maze();

// Frees the shared memory resources used by amazing_client.c
void free_shared_memory();

// Prints the converted map in ASCII format in STDOUT
void print_converted_map();

// Prints the shared map in ASCII format in STDOUT
void print_shared_map();

// Gets turn id from the shared memory resource if necessary
int get_turn_id();

//updates turn id to the shared memory resource
void update_turn_id(int new_turn);

// Graphics function that renders the maze.
void render_maze();

// Graphics Function that updates the graphics
void update_graphics();

// Graphics function that starts the graphics and renders the maze
void* start_graphics(void* ptr);


//PROTOTYPES: bfs.c related functions

// Enqueues the queue with the newly created cell object
void enqueue(queue* q,cell* c);

// Dequeues the first entry in the queue and returns a pointer to it, or returns NULL if the queue is
// empty;
cell* dequeue(queue* q);

// Finds the shortest path between the XYPOS start and end on the map, and writes the sequence of moves 
// into path (the array). Takes the map (in matrix form), the start and end XYPOS, and an empty array to
// write the path into as inputs. 
void find_path(matrix* mat,XYPOS* start,XYPOS* end,int path[]);

// Given a cell and an empty array, writes each move that the avatar should make into the array, starting
// at index 0.
//(cell contains a parent so it's easy to backtrack how path
//was created and recreate it)
void construct_path(cell* c,int path[]);

// Intializes a cell structure with the values of the parameters and returns a pointer to it.
cell* init_cell(int x,int y,int d,int mark,cell* parent);

// Frees all of the elements in the queue
void free_queue(queue* q);

// Frees all of the elements and their parents in the queue
void free_queue_advanced(queue* q1,queue* q2);

// Checks whether the queue contains some specific coordinates already. Returns 1 if it does, 0 otherwise
int contains(queue* q,int x,int y);

// Function to print the path (for debugging purposes);
void print_path(int path[],int length);
