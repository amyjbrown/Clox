# Makefile for building Clox
# TBD as I make things work out better!

##### Variables #####
Compiler = gcc
Source = main.c chunk.c debug.c linearray.c memory.c value.c vm.c
Target = clox.exe
Flags = -g -Wall

##### Commands #####
clox :
	$(Compiler) $(Source) -o $(Target)

debug :
	$(Compiler) $(Flags) $(Source) -o clox-debug.exe