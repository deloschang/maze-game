CC = gcc
CFLAGS = -Wall -pedantic -std=c99

amazing_client: amazing_client.c amazing_client.h bfs.c header.h sem1.c semun.h graphics.c
	$(CC) $(CFLAGS) -g -D_SVID_SOURCE=1 -o amazing_client amazing_client.c bfs.c sem1.c 

AMStartup: AMStartup.c AMStartup.h header.h
	$(CC) $(CFLAGS) -g -D_SVID_SOURCE=1 -o AMStartup AMStartup.c


clean:
	rm -f *.o
