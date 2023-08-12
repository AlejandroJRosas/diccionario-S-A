CC = gcc -Wall
OBJECTS = source/main.o source/dictlib.o

diccionario: all clear

all: $(OBJECTS)
	$(CC) $(OBJECTS) -o diccionario

source/main.o:	source/dictlib.h
source/dictlib.o: source/dictlib.h

.PHONY: clear
clear:
	rm -rf source/*.o