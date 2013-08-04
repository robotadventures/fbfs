CC = gcc
CFLAGS = -g -Wall $(shell pkg-config --cflags json-c libcurl fuse)
LIBS = $(shell pkg-config --libs json-c libcurl fuse) -lgraph

