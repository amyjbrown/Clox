# Source
CC = gcc
SRC = $(wildcard .c)
OBJ = $(SRC:.c=.o)
EXE = clox
CFLAGS = -Wall -g -pedantic -g

# Compile and assemble C source files
%.o: %.c
	$(CC) -c $(CFLAGS) $*.c

# Link all OBJs
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ)

clean:
	rm -f $(OBJ) 