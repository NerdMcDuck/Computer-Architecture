#include "nCr.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>
#include <string.h>

int main(int argc, char **argv){
	if(argc != 2){
		fprintf(stderr, "\x1b[3;31;31mERROR: Not enough arguments or too many arguments\x1b[0m \n");	
		exit(EXIT_FAILURE);
	}
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	
	char str1[3];
	char str2[3];
	strcpy(str1, argv[1]);
	strcpy(str2, "-h");

	if(strcmp(str1,str2) == 0){
		printf("\x1b[3;31;33mUsage: formula <Positive Integer>\x1b[0m\n");
		exit(EXIT_SUCCESS);
	}

	char *ptr = argv[1];
	int n = atoi(argv[1]);
	int k; 
	int res = 0;

	if(n <0){
		fprintf(stderr, "\x1b[3;31;31mERROR: Negative Number Inputed!\x1b[0m \n");	
		exit(EXIT_FAILURE);
	}

	if(n > 12){
		fprintf(stderr, "\x1b[3;31;31m0\x1b[0m \n");
		exit(EXIT_FAILURE);
	}

	if(!isdigit(*ptr)){
		fprintf(stderr, "\x1b[3;31;31mNaN\x1b[0m \n");
		exit(EXIT_FAILURE);
	}

	if(n == 0){
		printf("\x1b[3;31;32m(1 + x)^%d = \x1b[0m \x1b[3;31;35m 1 \x1b[0m \n", n);
		gettimeofday(&end, NULL);
		long t = end.tv_usec - begin.tv_usec;
		printf("\n\x1b[3;31;32mTime Required =\x1b[0m \x1b[3;31;35m%ld microseconds\x1b[0m\n", t);
		exit(EXIT_SUCCESS);
	}

	printf("\x1b[3;31;32m(1 + x)^%d = \x1b[0m \x1b[3;31;35m 1 +\x1b[0m ", n);
		
	for(k = 1; k <= n; k++){
		res = nCr(n,k);
		printf("\x1b[3;31;35m%d*x^%d\x1b[0m", res, k);

		if(k != n){
			printf("\x1b[3;31;35m + \x1b[0m");
		}
	}
	printf("\n");
	gettimeofday(&end, NULL);
	long t = end.tv_usec - begin.tv_usec;
	printf("\n\x1b[3;31;32mTime Required =\x1b[0m \x1b[3;31;35m%ld microseconds\x1b[0m\n", t);
return 0;
}