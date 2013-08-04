CC = gcc
CFLAGS = -g -Wall `pkg-config fuse --cflags --libs`

all : fbfs

fbfs : fbfs.o log.o
	$(CC) $(CFLAGS) -o fbfs fbfs.o log.o

fbfs.o : fbfs.c
	$(CC) $(CFLAGS) -c fbfs.c

log.o : log.c
	$(CC) $(CFLAGS) -c log.c

.PHONY : clean

clean : 
	@rm -f *.o fbfs

