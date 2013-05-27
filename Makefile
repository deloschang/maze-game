CC = gcc
CFLAGS = -Wall -pedantic -std=c99
PKGFLAGS = `pkg-config --cflags gtk+-2.0 --libs gtk+-2.0`
SVIDFLAGS = -D_SVID_SOURCE=1 

EXEC = amazing_client
OBJS = amazing_client.o bfs.o sem1.o graphics.o
SRCS = amazing_client.c bfs.c header.h sem1.c semun.h graphics.c 

EXEC2 = AMStartup
OBJS2 = AMStartup.c

#amazing_client: amazing_client.c amazing_client.h bfs.c header.h sem1.c semun.h graphics.c 
	#$(CC) $(CFLAGS) -g -D_SVID_SOURCE=1 -o amazing_client amazing_client.c bfs.c sem1.c graphics.c $(PKGFLAGS)
	#./AMStartup -n 3 -d 3 -h kancamagus.cs.dartmouth.edu

#AMStartup: AMStartup.c AMStartup.h header.h
	#$(CC) $(CFLAGS) -D_SVID_SOURCE=1 -o AMStartup AMStartup.c 

# Compile startup and Amazing client
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(SVIDFLAGS) -o $(EXEC2) $(OBJS2)
	$(CC) $(CFLAGS) $(SVIDFLAGS) -o $(EXEC) $(OBJS) $(PKGFLAGS)
	#$(CC) $(CFLAGS) -o $(EXEC4) $(OBJS4) $(PKGFLAGS)

	# test run
	#./startup -n 2 -d 0 -h stratton.cs.dartmouth.edu
	#./startup -n 2 -d 0 -h kancamagus.cs.dartmouth.edu

	# make clean
	#./cleansharedmem.sh

$(OBJS): $(SRCS) 
	$(CC) $(CFLAGS) $(SVIDFLAGS) -c $(SRCS) $(PKGFLAGS)
	#$(CC) $(CFLAGS) -c $(SRCS4) $(PKGFLAGS)

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
	rm -f AMStartup
	rm -f algo
	rm -f amazing_client

cleanlog:
	rm -f *.log
