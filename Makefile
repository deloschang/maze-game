CC = gcc
CFLAGS = -Wall -pedantic -std=c99
PKGFLAGS = `pkg-config --cflags gtk+-2.0 --libs gtk+-2.0`
SVIDFLAGS = -D_SVID_SOURCE=1 

# amazing client details
EXEC = amazing_client
OBJS = amazing_client.o 
SRCS = amazing_client.c 

# startup details
EXEC2 = AMStartup
OBJS2 = AMStartup.c

# unit test details
EXEC3 = unit_test
OBJS3 = unit_test.o 
SRCS3 = unit_test.c 

UTILDIR=utils/
UTILFLAG=-ltseutil
UTILLIB=$(UTILDIR)libtseutil.a
UTILC=$(UTILDIR)bfs.c $(UTILDIR)graphics.c $(UTILDIR)sem1.c
UTILH=$(UTILC:.c=.h)

#amazing_client: amazing_client.c amazing_client.h bfs.c header.h sem1.c semun.h graphics.c 
	#$(CC) $(CFLAGS) -g -D_SVID_SOURCE=1 -o amazing_client amazing_client.c bfs.c sem1.c graphics.c $(PKGFLAGS)
	#./AMStartup -n 3 -d 3 -h kancamagus.cs.dartmouth.edu

#AMStartup: AMStartup.c AMStartup.h header.h
	#$(CC) $(CFLAGS) -D_SVID_SOURCE=1 -o AMStartup AMStartup.c 

# Compile startup and Amazing client
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(SVIDFLAGS) -o $(EXEC2) $(OBJS2) -L$(UTILDIR) $(UTILFLAG)
	$(CC) $(CFLAGS) $(SVIDFLAGS) -o $(EXEC) $(OBJS) $(PKGFLAGS) -L$(UTILDIR) $(UTILFLAG)

$(OBJS): $(SRCS) 
	$(CC) $(CFLAGS) $(SVIDFLAGS) -c $(SRCS) $(PKGFLAGS) -L$(UTILDIR) $(UTILFLAG)

unit: $(SRCS3) 
	$(CC) $(CFLAGS) $(SVIDFLAGS) -c $(SRCS3) $(PKGFLAGS) -L$(UTILDIR) $(UTILFLAG)
	$(CC) $(CFLAGS) $(SVIDFLAGS) -o $(EXEC3) $(OBJS3) $(PKGFLAGS) -L$(UTILDIR) $(UTILFLAG)

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
	rm -f unit_test

cleanlog:
	rm -f *.log
