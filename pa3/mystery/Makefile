CC = gcc
all: mystery

mystery: mystery.o
	$(CC) -Wall -o mystery mystery.c

mystery.o: mystery.c
	$(CC) -c mystery.c
	
clean: 
	rm -rf *.o mystery
