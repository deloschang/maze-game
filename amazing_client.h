#if defined (__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)

#else

    union semun{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
    };
#endif
