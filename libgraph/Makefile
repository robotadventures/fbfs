CC = gcc
CFLAGS = $(shell pkg-config --cflags json-c libcurl) # -fPIC
LIBS = $(shell pkg-config --libs json-c libcurl)

all : graph.o util.o user.o graph_session.o post.o

graph: graph.o util.o user.o graph_session.o post.o
	$(CC) $(CFLAGS) $(LIBS) -o graph graph.o util.o user.o graph_session.o post.o

graph.o: graph.c
	$(CC) $(CFLAGS) -c graph.c

graph_session.o: graph_session.c
	$(CC) $(CFLAGS) -c graph_session.c

util.o : util.c
	$(CC) $(CFLAGS) -c util.c

user.o : user.c
	$(CC) $(CFLAGS) -c user.c

post.o : post.c
	$(CC) $(CFLAGS) -c post.c

clean :
	@rm -rf *.o graph
