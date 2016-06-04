#include "nCr.h"

int  Factorial(int n){

	if(n == 0){

		return 1;

	} else {

		return(n*Factorial(n-1));
	}

return 0;
}

int nCr(int n, int r){
	int result;

		result = Factorial(n)/(Factorial(r)*Factorial(n-r));
		
	return result;
}

