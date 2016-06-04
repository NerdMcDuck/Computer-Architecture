CC = gcc
CCFLAGS = -Wall -lm
all: y86emul

y86emul: y86emul.o
	$(CC) $(CCFLAGS) -o y86emul y86emul.c

y86emul.o: y86emul.c y86emul.h
	$(CC) $(CCFLAGS) -c y86emul.c
	
clean: 
	rm -rf *.o y86emul