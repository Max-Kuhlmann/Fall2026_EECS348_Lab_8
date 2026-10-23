# This is a comment line
CC=g++
# CFLAGS will be the options passed to the compiler.
CFLAGS=-std=c++17 -Wall
OBJECTS=Lab8.cpp
all: calculator

calculator: $(OBJECTS)
	$(CC) $(OBJECTS) -o calculator

%.o: %.c
	$(CC) $(CFLAGS) $<

clean:
	rm -rf *.o calculator

