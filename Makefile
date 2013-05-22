# This is the makefile for the query engine

CC = gcc
CFLAGS = -Wall -pedantic -std=c99
CFLAGS1 = -Wall 
PKGFLAGS = `pkg-config --cflags gtk+-2.0 --libs gtk+-2.0`

EXEC = graphics
LDFLAGS = 

# query engine details
OBJS = graphics.o
SRCS = graphics.c 

# Future library details
#UTILDIR=../utils/
#UTILFLAG=-ltseutil
#UTILLIB=$(UTILDIR)libtseutil.a
#UTILC=$(UTILDIR)hash.c $(UTILDIR)file.c $(UTILDIR)index.c
#UTILH=$(UTILC:.c=.h)

# Commands start with TAB not spaces
$(EXEC): $(OBJS)
	#$(CC) $(CFLAGS) -o $(EXEC) $(OBJS) -L$(UTILDIR) $(UTILFLAG)  # future library 
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS) $(PKGFLAGS)
	./graphics
	make clean
$(OBJS): $(SRCS) 
	$(CC) $(CFLAGS) -c $(SRCS) $(PKGFLAGS)

#unit: $(SRCS2) 
	#$(CC) $(CFLAGS) -c $(SRCS2) 
	#$(CC) $(CFLAGS) -o $(EXEC2) $(OBJS2) -L$(UTILDIR) $(UTILFLAG)
#unit2: $(SRCS2) 
	#$(CC) $(CFLAGS) -g -ggdb -c $(SRCS2)
	#$(CC) $(CFLAGS) -g -ggdb -o $(EXEC2) $(OBJS2) -L$(UTILDIR) $(UTILFLAG)
	#gdb --args queryengine_test

debug: $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -c $(SRCS) $(PKGFLAGS)
	#$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(OBJS) -L$(UTILDIR) $(UTILFLAG) #future library
	$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(OBJS) $(PKGFLAGS)
	gdb --args graphics

#debug2: $(SRCS)
	#$(CC) $(CFLAGS) -g -ggdb -c $(SRCS)
	#$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(OBJS) -L$(UTILDIR) $(UTILFLAG)
	#gdb --args indexer ../crawler_dir/data/ index.dat index.dat index_new.dat

valgrind: $(OBJS)
	$(CC) $(CFLAGS) -g -ggdb -c $(SRCS) $(PKGFLAGS)
	#$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(OBJS) -L$(UTILDIR) $(UTILFLAG) #future library
	$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(OBJS) $(PKGFLAGS)
	valgrind --tool=memcheck --leak-check=full ./graphics

valgrindgdb: $(OBJS)
	$(CC) $(CFLAGS) -g -ggdb -c $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(OBJS) -L$(UTILDIR) $(UTILFLAG)
	valgrind --tool=memcheck --leak-check=full --vgdb-error=0 ./queryengine ../indexer_dir/index.dat ../crawler_dir/data

$(UTILLIB): $(UTILC) $(UTILH)
	cd $(UTILDIR); make;

clean:
	rm -f *~
	rm -f *#
	rm -f ./prs
	rm -f *.o
	rm -f *.gch
	rm -f core.*
	rm -f vgcore.*
	rm -f .nfs*
	rm -f graphics

cleanlog:
	rm -f *log.*


