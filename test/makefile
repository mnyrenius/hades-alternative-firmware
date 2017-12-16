
PROJECT = hades-test

CC = gcc
CFLAGS = -Wall -O3 --std=c11 -g -DDEBUG

INCLUDE = -I../src\

SRC = $(wildcard *.c) ../src/ringbuffer.c ../src/notemem.c ../src/turing.c

unittest:
	mkdir -p bin
	$(CC) $(CFLAGS) $(INCLUDE) $(SRC) -o bin/$(PROJECT)
	./bin/$(PROJECT)

targettest:
	py.test -vs test.py

clean:
	@rm -fr bin
	@rm -fr __pycache__
	@rm -fr .cache
