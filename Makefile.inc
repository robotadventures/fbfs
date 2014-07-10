CC = gcc
CFLAGS = -g -Wall $(shell pkg-config --cflags json libcurl fuse)
LIBS = $(shell pkg-config --libs json libcurl fuse) -lgraph

