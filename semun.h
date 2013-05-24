#if defined (__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)

#else

     union semun{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
     };
#endif
int set_semvalue(int sem_id);
void del_semvalue(int sem_id);
int semaphore_p(int sem_id);
int semaphore_v(int sem_id);
