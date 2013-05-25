//#if defined (__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)

//#else

 //   union semun{
//	int val;
//	struct semid_ds *buf;
//	unsigned short int *array;
//	struct seminfo *__buf;
  //  };
//#endif

#define DIM 200

typedef struct _matrix {
     int row;
     int column;
     char matrix[DIM][DIM];
} matrix;

//#if defined (__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)

//#else

  //   union semun{
//	int val;
//	struct semid_ds *buf;
//	unsigned short int *array;
//	struct seminfo *__buf;
  //   };
//#endif

//amazing client functions
matrix* convert_map();
void mark_as_wall(XYPOS* wall);
XYPOS* get_average();
void update_shared_map();
void render_maze();
//shared_map* get_shared_map();
void free_shared_memory();
void print_converted_map();
void print_shared_map();
int get_turn_id();
void update_turn_id(int new_turn);


//algorithm functions
int heuristic_function(XYPOS *start, XYPOS *end, matrix *map);
int valid_pos(XYPOS *pos, matrix *map);
int check_move(XYPOS *pos, XYPOS *end, int direction, matrix *map);
int find_move(XYPOS *start, XYPOS *end, matrix *map);


//typedef struct _shared_map{
  //  int row;
   // int col;
   // int map[MAX_DIM][MAX_DIM];

//} shared_map;
//


//bfs related functions
//and data structures

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

void enqueue(queue* q,cell* c);
cell* dequeue(queue* q);
void find_path(matrix* mat,XYPOS* start,XYPOS* end,int path[]);
void construct_path(cell* c,int path[]);
cell* init_cell(int x,int y,int d,int mark,cell* parent);
void free_queue();
int contains(queue* q,int x,int y);
//void print_path(matrix* mat,XYPOS* s,XYPOS* e);
void print_path(int path[],int length);
void shuffle(int* array);



