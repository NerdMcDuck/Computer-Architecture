CC  = gcc 
CCFLAGS = -ansi -Wall -pedantic 
all : format 

format : format.o 
	$(CC) $(CCFLAGS) -o format format.o

format.o : format.c 
	$(CC) $(CCFLAGS) -c format.c 

clean: 
	rm -rf *.o format


	
