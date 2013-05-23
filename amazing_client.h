//#if defined (__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)

//#else

 //   union semun{
//	int val;
//	struct semid_ds *buf;
//	unsigned short int *array;
//	struct seminfo *__buf;
  //  };
//#endif

matrix* convert_map();
void mark_as_wall(XYPOS* wall);
XYPOS* get_average();
void update_shared_map();
shared_map* get_shared_map();
void free_shared_memory();
