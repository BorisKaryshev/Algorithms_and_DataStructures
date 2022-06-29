default_target: all

CC = gcc
OL = 
PROG = main
OBJS = ./objs/main.o ./objs/structs.o ./objs/trees.o

all: $(OBJS)
	$(CC) $(OBJS) -o $(PROG)

./objs/main.o: main.c structs.h trees.h
	$(CC) $(OL) -c main.c -o ./objs/main.o


./objs/structs.o: structs.c structs.h
	$(CC) $(OL) -c structs.c -o ./objs/structs.o

./objs/trees.o: trees.c trees.h
	$(CC) $(OL) -c trees.c -o ./objs/trees.o

clean:
	rm -r objs
	rm $(PROG)

$(shell mkdir -p objs)