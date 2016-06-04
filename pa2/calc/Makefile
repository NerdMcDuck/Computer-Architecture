CC  = gcc 
CCFLAGS = -ansi -Wall -pedantic -lm
all : calc 

calc : calc.o 
	$(CC) $(CCFLAGS) -o calc calc.o

calc.o : calc.c 
	$(CC) $(CCFLAGS) -c calc.c 

clean: 
	rm -rf *.o calc 


	
