#if defined (__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)

#else
// DATA STRUCTURES 
// The semaphore data strucuture that is used to control and synchronize the shared map.
     union semun{
        int val;       //the value of the semaphore                
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
     };
#endif

// PROTOTYPES used by sem1.c

// Initializes the semaphore by setting its value to 1. 
int set_semvalue(int sem_id);

// Deletes the semaphore
void del_semvalue(int sem_id);

// Sets the sempahore to false, and if the semaphore was already false, suspend this task.
int semaphore_p(int sem_id);

// Sets the semaphore to true and resumes another task, if another task is waiting.
int semaphore_v(int sem_id);
