default_target: all

CC = gcc
PROG = main
OBJS = ./objs/main.o ./objs/sorting_algorithms.o ./objs/array_t.o
OL = 3

all: $(OBJS)
	@$(CC) $(OBJS) -o $(PROG)

./objs/main.o: main.c sort.h
	@$(CC) -O$(OL) -c main.c -o ./objs/main.o

./objs/sorting_algorithms.o: sorting_algorithms.c sort.h
	@$(CC) -O$(OL) -c sorting_algorithms.c -o ./objs/sorting_algorithms.o

./objs/array_t.o: array_t.c sort.h
	@$(CC) -O$(OL) -c array_t.c -o ./objs/array_t.o

clean:
	@rm -r objs $(PROG)

$(shell mkdir -p objs)