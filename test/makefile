
PROJECT = hades-test

CC = gcc
CFLAGS = -Wall -O3 --std=c11 -g

INCLUDE = -I../src\

SRC = $(wildcard *.c) ../src/ringbuffer.c ../src/notemem.c

test:
	mkdir -p bin
	$(CC) $(CFLAGS) $(INCLUDE) $(SRC) -o bin/$(PROJECT)
	./bin/$(PROJECT)

clean:
	@rm -fr bin
