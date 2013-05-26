# This is the makefile for the query engine

CC = gcc
CFLAGS = -Wall -pedantic -std=c99
CFLAGS1 = -Wall 
PKGFLAGS = `pkg-config --cflags gtk+-2.0 --libs gtk+-2.0`
SVIDFLAGS = -D_SVID_SOURCE=1 

EXEC4 = startup
OBJS4 = AMStartup.o 
SRCS4 = AMStartup.c 

EXEC2 = graphics
OBJS2 = graphics.o
SRCS2 = graphics.c 

#EXEC3 = algo
#OBJS3 = algorithm.o
#SRCS3 = algorithm.c 

EXEC = amazing_client
OBJS = amazing_client.o bfs.o sem1.o graphics.o
SRCS = amazing_client.c bfs.c sem1.c graphics.c 

# Future library details
#UTILDIR=../utils/
#UTILFLAG=-ltseutil
#UTILLIB=$(UTILDIR)libtseutil.a
#UTILC=$(UTILDIR)hash.c $(UTILDIR)file.c $(UTILDIR)index.c
#UTILH=$(UTILC:.c=.h)

# Commands start with TAB not spaces

# Compile startup and Amazing client
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(SVIDFLAGS) -o $(EXEC) $(OBJS) $(PKGFLAGS)
	$(CC) $(CFLAGS) -o $(EXEC4) $(OBJS4) $(PKGFLAGS)

	# test run
	./startup -n 2 -d 0 -h stratton.cs.dartmouth.edu

	make clean
	./cleansharedmem.sh

$(OBJS): $(SRCS) 
	$(CC) $(CFLAGS) $(SVIDFLAGS) -c $(SRCS) $(PKGFLAGS)
	$(CC) $(CFLAGS) -c $(SRCS4) $(PKGFLAGS)

# Amazing Client
#$(EXEC4): $(OBJS4)   
	#$(CC) $(CFLAGS) $(SVIDFLAGS) -o $(EXEC4) $(OBJS4) $(PKGFLAGS)

#$(OBJS4): $(SRCS4) 
	#$(CC) $(CFLAGS) -c $(SRCS4) $(PKGFLAGS)

#=====================================================#
#unit: $(SRCS2) 
	#$(CC) $(CFLAGS) -c $(SRCS2) 
	#$(CC) $(CFLAGS) -o $(EXEC2) $(OBJS2) -L$(UTILDIR) $(UTILFLAG)
#unit2: $(SRCS2) 
	#$(CC) $(CFLAGS) -g -ggdb -c $(SRCS2)
	#$(CC) $(CFLAGS) -g -ggdb -o $(EXEC2) $(OBJS2) -L$(UTILDIR) $(UTILFLAG)
	#gdb --args queryengine_test

# testing graphics
$(EXEC2): $(OBJS2)
	$(CC) $(CFLAGS) -o $(EXEC2) $(OBJS2) $(PKGFLAGS)
	./graphics
	make clean

#$(OBJS2): $(SRCS2) 
	#$(CC) $(CFLAGS) -c $(SRCS2) $(PKGFLAGS)



debug: $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -c $(SRCS2) $(PKGFLAGS)
	#$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(OBJS) -L$(UTILDIR) $(UTILFLAG) #future library
	$(CC) $(CFLAGS) -g -ggdb -o $(EXEC2) $(OBJS2) $(PKGFLAGS)
	gdb --args graphics

debug2: $(SRCS)
	$(CC) $(CFLAGS) $(SVIDFLAGS) -g -ggdb -c $(SRCS) $(PKGFLAGS)
	$(CC) $(CFLAGS) -g -ggdb -c $(SRCS4) $(PKGFLAGS)
	$(CC) $(CFLAGS) $(SVIDFLAGS) -g -ggdb -o $(EXEC) $(OBJS) $(PKGFLAGS)
	$(CC) $(CFLAGS) -g -ggdb -o $(EXEC4) $(OBJS4) $(PKGFLAGS)

	gdb --args ./startup -n 3 -d 0 -h stratton.cs.dartmouth.edu
	make clean
	./cleansharedmem.sh

#debug2: $(SRCS)
	#$(CC) $(CFLAGS) -g -ggdb -c $(SRCS)
	#$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(OBJS) -L$(UTILDIR) $(UTILFLAG)
	#gdb --args indexer ../crawler_dir/data/ index.dat index.dat index_new.dat

valgrind: $(OBJS)
	$(CC) $(CFLAGS) $(SVIDFLAGS) -g -ggdb -c $(SRCS) $(PKGFLAGS)
	$(CC) $(CFLAGS) -g -ggdb -c $(SRCS4) $(PKGFLAGS)
	$(CC) $(CFLAGS) $(SVIDFLAGS) -g -ggdb -o $(EXEC) $(OBJS) $(PKGFLAGS)
	$(CC) $(CFLAGS) -g -ggdb -o $(EXEC4) $(OBJS4) $(PKGFLAGS)

	valgrind --trace-children=yes --tool=memcheck --leak-check=full ./startup -n 2 -d 0 -h stratton.cs.dartmouth.edu > valgrind.txt
	make clean
	./cleansharedmem.sh

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
	#rm -f core.*
	#rm -f vgcore.*
	rm -f .nfs*
	rm -f graphics
	rm -f startup
	rm -f algo
	rm -f amazing_client

cleanlog:
	rm -f *.log
