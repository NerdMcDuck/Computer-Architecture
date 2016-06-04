#include "mystery.h"
#include <stdio.h>
#include <stdlib.h>

int add(int x, int y){
	return (x+y);
}

long int fib(long n){
	int prev = 0, next = 1, temp, i;
	if(n <= 0 || n > 46){
		return 0;
	}
	for(i = 0; i <= n; i++){
		if(i == 1 || i == 2){
			temp = 1;
		} else {
			temp = add(prev,next);
			prev = next;
			next = temp;
		}
	}
	return temp;
}

int main(int argc, char **argv){

	long input = atoi(argv[1]);
	long num = fib(input);
	printf("Value: %ld\n", num);
	
return 0; 
}