default_target: all

CC = gcc
OL = 0
PROG = main
OBJS = ./objs/main.o ./objs/structs.o

all: $(OBJS)
	@$(CC) $(OBJS) -o $(PROG)

./objs/main.o: main.c structs.h
	@$(CC) -O$(OL) -c main.c -o ./objs/main.o


./objs/structs.o: structs.c structs.h
	@$(CC) -O$(OL) -c structs.c -o ./objs/structs.o

clean:
	@rm -r objs
	@rm $(PROG)

$(shell mkdir -p objs)