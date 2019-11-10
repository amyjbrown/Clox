# Makefile for building Clox
# TBD as I make things work out better!

##### Variables #####
Compiler = gcc
Source = main.c chunk.c compiler.c console.c debug.c linearray.c memory.c value.c scanner.c vm.c
Target = clox.exe
Flags = -g -Wall

##### Commands #####
clox :
	$(Compiler) $(Source) -o $(Target)

debug :
	$(Compiler) $(Flags) $(Source) -o clox-debug.exe

tree :
	$(Compiler) -MH $(Source)