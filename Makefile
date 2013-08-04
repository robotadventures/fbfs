include Makefile.inc

all : fbfs

fbfs : fbfs.o log.o libgraph.a
	$(CC) $(CFLAGS) $(LIBS) -o fbfs fbfs.o log.o libgraph.a

libgraph.a : force_look
	@cd libgraph; $(MAKE) $(MFLAGS)

fbfs.o : fbfs.c
	$(CC) $(CFLAGS) -c fbfs.c

log.o : log.c
	$(CC) $(CFLAGS) -c log.c

.PHONY : clean

force_look :
	@true

clean : 
	@rm -f *.o fbfs
	@cd libgraph; $(MAKE) $(MFLAGS) clean

