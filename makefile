CC = gcc -Wall
OBJECTS = source/main.o source/dictlib.o

diccionario: all clean

all: $(OBJECTS)
	$(CC) $(OBJECTS) -o diccionario

source/main.o:	source/dictlib.h
source/dictlib.o: source/dictlib.h

.PHONY: clean
clean:
	rm -rf source/*.o