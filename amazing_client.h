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

#define min(x,y)   ((x)>(y))?(y):(x)

//! Zero out the 2D array
#define BZEROARRAY(array,n,m)  memset(array, 0, sizeof(array[0][0]) * m * n)
#define convertBack(n)  (n+1)/2
#define convertForward(n)  (2*n)+1

//! Print arg together with the source file name and the current line number.
#define LOG(s)  printf("[%s:%d]%s\n", __FILE__, __LINE__, s)

//! Check whether arg is NULL or not. Quit this program if it is NULL.
#define MYASSERT(s)  if (!(s))   {                                      \
    printf("General Assert Error at %s:line%d\n", __FILE__, __LINE__);  \
    exit(-1); \
  }

//! Check whether arg is NULL or not on a memory allocation. Quit this program if it is NULL.
#define MALLOC_CHECK(s)  if ((s) == NULL)   {                     \
    printf("No enough memory at %s:line%d ", __FILE__, __LINE__); \
    perror(":");                                                  \
    exit(-1); \
  }

//! Set memory space starts at pointer \a n of size \a m to zero. 
#define BZERO(n,m)  memset(n, 0, m)

typedef struct _matrix {
     int row;
     int column;
     char matrix[DIM][DIM];
     //char** matrix;
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
XYPOS* get_goal();
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



