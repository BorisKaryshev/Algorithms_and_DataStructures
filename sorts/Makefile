default_target: all

CC = gcc
PROG = main
OBJS = ./objs/main.o ./objs/sorting_algorithms.o ./objs/array_t.o ./objs/trees.o
OL = 0

all: $(OBJS)
	$(CC) $(OBJS) -o $(PROG)

./objs/main.o: main.c sort.h ./lib/trees.h
	$(CC) -O$(OL) -c main.c -o ./objs/main.o

./objs/sorting_algorithms.o: sorting_algorithms.c sort.h
	$(CC) -O$(OL) -c sorting_algorithms.c -o ./objs/sorting_algorithms.o

./objs/array_t.o: array_t.c sort.h
	$(CC) -O$(OL) -c array_t.c -o ./objs/array_t.o

./objs/trees.o: ./lib/trees.c ./lib/trees.h
	$(CC) -O$(OL) -c ./lib/trees.c -o ./objs/trees.o

clean:
	rm -r objs $(PROG)

$(shell mkdir -p objs)
