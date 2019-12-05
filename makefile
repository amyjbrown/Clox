# Source
CC = gcc
SRC = $(wildcard *.c)
EXE = clox.exe
CFLAGS = -Wall -g -pedantic -g

debug:
	$(CC) $(CFLAGS) $(SRC) -o $(EXE)

# TODO make a general C builder
