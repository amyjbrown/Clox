# simplest possible makefile I can make

clox:
	gcc main.c chunk.c debug.c linearray.c memory.c value.c -o clox.exe
