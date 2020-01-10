SHELL=/bin/sh

CC=gcc
CFLAGS=-Wall -Werror -Wextra -pedantic -std=c99
LDFLAGS=-lm
LIB=$(wildcard lib/*.c)
SRC=$(LIB) maze.c
OBJ=${SRC:.c=.o}

all:maze

maze: $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

maze-debug: $(SRC)
	$(CC) $(CFLAGS) -DDEBUG -o $@ $^ $(LDFLAGS)

clean:
	rm -v *.o lib/*.o maze

.PHONY: all clean lib
