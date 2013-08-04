include ../Makefile.inc

all : ../libgraph.a

../libgraph.a : graph.o util.o user.o graph_session.o like.o
	@ar rv ../libgraph.a graph.o util.o user.o graph_session.o like.o

graph.o : graph.c
	$(CC) $(CFLAGS) -c graph.c

graph_session.o : graph_session.c
	$(CC) $(CFLAGS) -c graph_session.c

util.o : util.c
	$(CC) $(CFLAGS) -c util.c

user.o : user.c
	$(CC) $(CFLAGS) -c user.c

like.o : like.c
	$(CC) $(CFLAGS) -c like.c

clean :
	@rm -rf *.o graph
