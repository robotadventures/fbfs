CC = gcc
CFLAGS = -Ijson-c -ljson-c -lcurl

all : graph

graph: graph.o util.o user.o graph_session.o
	$(CC) $(CFLAGS) -o graph graph.o util.o user.o graph_session.o

graph.o: graph.c
	$(CC) $(CFLAGS) -c graph.c

graph_session.o: graph_session.c
	$(CC) $(CFLAGS) -c graph_session.c

util.o : util.c
	$(CC) $(CFLAGS) -c util.c

user.o : user.c
	$(CC) $(CFLAGS) -c user.c

clean :
	@rm -rf *.o graph
