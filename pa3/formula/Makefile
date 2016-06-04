CC = gcc
all: formula

formula: formula.o
	$(CC) -o formula -m32 -Wall formula.c nCr.s

formula.o: formula.c
	$(CC) -c formula.c
	
clean: 
	rm -rf *.o formula
