/*
 * tokenizer.c
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
    
	int strlength; //the length of the string
    int strIndex; //Index of the string to be used later i.e argv[1][strIndex]
    char * strCpy; //coping the original string so as to not modify the original
    char *currChar; //current character 
};

typedef struct TokenizerT_ TokenizerT;
	//token state
	enum Tstate {TS_State0, TS_Octal, TS_Hex, TS_Digit, TS_Float, TS_MAL, Zero};
/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 * 
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

 //trim leading and trailing white space
 //got help for this one
void * trimWhiteSpace(char * str) { 
	char *ptr = str;
	int length = strlen(ptr);

	//trim trailing white space
	while(isspace(ptr[length - 1])) {
		ptr[--length] = 0;
	}

	//trim leading white space
	while(*ptr && isspace(*ptr)){
		++ptr;
		--length;
	}


	memmove(str, ptr, length + 1);
	return 0;
}

TokenizerT *TKCreate( char * ts ) {
	
    //Allocate memory for the token
    TokenizerT *TKsp = (TokenizerT*)malloc(sizeof(TokenizerT));
	
	//get length of token and set a pointer to it
	TKsp->strlength = strlen(ts);

	//check if ts is NULL
	if(strlen(ts) == 0) {
		return NULL;
	}

	//create a temporary copy of the string
	char *tempCopy = ts;

	//copy it to the temp string
	strcpy(tempCopy, ts);

	//trim the white space from it
	trimWhiteSpace(tempCopy);

	//Space allocation for the new copy 
	TKsp -> strCpy = (char *)malloc(strlen(tempCopy)+1); //space allocation

	strcpy(TKsp -> strCpy, tempCopy); //doing the actual copy ref for me(delete later): strcpy(dest, src)
	

	//allocate space for tracking the current character ptr
	TKsp -> currChar = (char *)malloc(strlen(ts) + 1);

	//index of currChar
	TKsp -> strIndex = 0;
	//printf("test: %s\n", TKsp -> strCpy); //just tests
	
	
  return TKsp;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk ) {
	
	//delete string
	free(tk -> strCpy);
	
	//delete current character
	free(tk -> currChar);

	//delete token
	free(tk); 

}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( TokenizerT * tk ) {


	//The next token that will be returned
	char * next_tok  = (char *)malloc(sizeof(strlen(tk -> strCpy))+1);
	strcpy(next_tok, tk -> strCpy);

	//there is no next token, end of string
	if(next_tok == '\0'){
		return 0;
	}
	

	//increase the index - also fixes a certain infinite loop ;)
	tk -> strIndex = (tk -> strIndex) + strlen(next_tok) +1;
	
	//printf("next_tok %s\n" , next_tok);

  return next_tok;
}
//ascii values: a-f(97-102), A-F(65-70), 0-9(48-57)
//TS_State0 first digit is 0
int Ts_State0(const char *a){

	//if 0 is just entered 
	if(strlen(a) == 1 && a[0] == '0'){
		return Zero; 
	}
	//pointer to each character in a
	const char *tkptr = a;

	//OCTAL 
	if(a[1] != 'x' && a[1] != 'X' && a[1] != 88 && a[1] != 120){
		
		//Go through the character array for Octals
		while (isdigit(*tkptr) && *tkptr != 56 && *tkptr != 57){
			//Increase the pointer
	   		tkptr++;
	        
	   		//End of string and/or absorbing state has been reached
	    	if(*tkptr == '\0'){

	            return TS_Octal;
	    	}
	    }
	} //end Octal
	
    //if heading towards a HEX value
    if(tkptr[1]== 'x' || tkptr[1] == 'X'){
    	size_t i;
    	//printf("length: %d\n", strlen(tkptr)); //prints length of the pointer for debugging
    	
    	//go through each character in the string pointed to by tkptr
    	for(i = 2; i < strlen(tkptr); i++){
    		
    		//printf("tkptr: %c\n", tkptr[i]); //debugging

    		//assign each char to hexx
    		char hexx = tkptr[i];

    		//check if the current char is hexx
    		if(isxdigit(hexx) == 0){

    			//printf("Entered: %c\n",hexx); //debugging
    			//not hex
    			return TS_MAL;
    		}
    		//continue with the loop
    	}

    	//All characters are hex 
    	return TS_Hex; 
	} //end hex

	//Floating Point number
	if(*tkptr == '.') { //the start of a floating point number
		size_t i;
		//lets go through the string
		for (i = 1; i < strlen(tkptr); i++){ //starts after the '0.'
			const char floating = tkptr[i];
		//if 'e' or 'E' or '+' or '-' are located right in front of the period
		if(tkptr[1] == 'E' || tkptr[1] =='e' || tkptr[1] =='+' || tkptr[1] =='-'){
			
			return TS_MAL;
		}

		/* supposed to stop something like 0.2+1 from outputting. But outputs a warning though
			if(*tkptr == '+' || *tkptr == '-' || *tkptr == 'E' && *tkptr == 'e'){
			printf("tkptr %c\n",tkptr[1]);
			return TS_MAL;
		}*/
		
			//check if contains letters/punctuations  
			if(isdigit(floating) == 0 && tkptr[i] != 'e' && tkptr[i] != 'E' && tkptr[i] != '+' && tkptr[i] != '-'){
				//malformed
				return TS_MAL;
			} 
			//check if string contains 'e', 'E', '+', '-'

			if(floating == 'e' || floating == 'E'){

				//if contains e or E but does not have the +-
				if((tkptr[i+1] != '+') && (tkptr[i+1] != '-')){

					return TS_MAL;
				} 
				//contains +-
				size_t j;
				for(j = i+2; j <strlen(tkptr); j++){

					//checks all the digits after +-
					if(!isdigit(tkptr[j])){
						
						return TS_MAL;
					}
					//continue Jloop
				}
			}
			//continue outer loop
		}
		return TS_Float;
	}

printf("Hey: %c\n", tkptr[1]);
	return 0; //no reason for this to be reached
} 	


	int Ts_Digit(const char *d){
		size_t i;

		//pointer to each character in a
		const char *tkptr = d;
		int j;
		//go through each character in the string pointed to by tkpt
		for(i = 0; i < strlen(tkptr); i++){

			char digit = tkptr[i];

			//check if it's a digit
			if(isdigit(digit) == 0){
				//if encounters none digit, first checks to see if it was the start of a float
				if(tkptr[i] == '.'){
					 j = i;
					break; //exit loop
				}
				printf("Entered2 \n");
				//not a digit
				return TS_MAL;
			}
			//continue loop

		}

		//Floating Point number
	if(tkptr[j] == '.') { //the start of a floating point number
		
		size_t i = j;
		//lets go through the string
		for (i = j+1; i < strlen(tkptr); i++){ //starts after the '0.'
			const char floating = tkptr[i];
		//if 'e' or 'E' or '+' or '-' are located right in front of the period
		if(tkptr[j+1] == 'E' || tkptr[j+1] =='e' || tkptr[j+1] =='+' || tkptr[j+1] =='-'){
			
			return TS_MAL;
		}

			//check if contains letters/punctuations  
			if(isdigit(floating) == 0 && tkptr[i] != 'e' && tkptr[i] != 'E' && tkptr[i] != '+' && tkptr[i] != '-'){
				//malformed
				return TS_MAL;
			} 
			//check if string contains 'e', 'E', '+', '-'

			if(floating == 'e' || floating == 'E'){

				//if contains e or E but does not have the +-
				if((tkptr[i+1] != '+') && (tkptr[i+1] != '-')){

					return TS_MAL;
				} 
				//contains +-
				size_t j;
				for(j = i+2; j <strlen(tkptr); j++){

					//checks all the digits after +-
					if(!isdigit(tkptr[j])){
						
						return TS_MAL;
					}
					//continue Jloop
				}
			}
			//continue outer loop
		}
		return TS_Float;
	}

	//The value is a valid Integer
	return TS_Digit;


	return 0;
	}
	/*
	 *Returns the state of the character passed by the caller
	 */
 	 int getState(const char *a){
 		size_t i; 

 		//get characters from token
 		//Uses first character of token to decide which state to go with
 		for(i = 0; i<strlen(a); i++){
 			//whitespace
 			if(isspace(a[i])){
 				continue;
 			}
 			//if token starts with 0
 			if(a[0] == '0'){ //TS_State0
 				//calls the Ts_State fucntion passing it the character string
 				return Ts_State0(a);

 				//ascii 1-9 is 49 - 57
 			} else if(a[0] >= 49 && a[0] <= 57) {
 				//printf("char: %d\n", a[0]);

 				return Ts_Digit(a);
 			}

 			else {
 				//Not a valid input
 				return TS_MAL;
 			}

 		}

 		return 0;
 	}


/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
   
 //check the number of arguments and/or if no arguments were passed
	if(argc <= 1){

		char *error = "ERROR! Not enough arguments";
		printf("\x1b[3;31;31m  %s \x1b[0m \n" , error);
		
		return 0;
	}
    
	//Get the first argument, copy it and tokenize it
	char * str = argv[1];
	TokenizerT * strTK = TKCreate(str); //creates a token of the copied string
	
	//iterate through argv and get the next token by calling TKGetNextToken

    while( strTK ->strIndex < (strTK -> strlength)){

    	strTK -> currChar = TKGetNextToken(strTK); //assign it to currChar
    	
    	switch(getState(strTK -> currChar)) //get that state of each token
    	{
    		case TS_Digit : 

    			printf("\x1b[3;31;32m Decimal  \x1b[0m %s\n", strTK -> currChar);
    			break;

    		case TS_Float : 

    			printf("\x1b[3;31;32m Float  \x1b[0m %s\n", strTK -> currChar);
    			break;

    		case TS_Hex :

    			printf("\x1b[3;31;32m Hex  \x1b[0m %s\n", strTK -> currChar);
    			break;

    		case TS_Octal :

    			printf("\x1b[3;31;32m Octal  \x1b[0m %s\n", strTK -> currChar);
    			break;
    		case Zero : 
    			printf("\x1b[3;31;32m Zero Value  \x1b[0m %s\n", strTK -> currChar);
    			break;

    		default :
    			printf("\x1b[3;31;31m MALFORMED INPUT:  [%s] \x1b[0m \n", strTK -> currChar);

    	}


    }

    //free up memory
    TKDestroy(strTK);
	
  return 0;
}


