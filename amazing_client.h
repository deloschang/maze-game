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
