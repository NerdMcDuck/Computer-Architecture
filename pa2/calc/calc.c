#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/*add function
*Takes in two arguments 
*returns the sum of the args
*/
long int add(long int arg1, long int arg2, char num1Sign, char num2Sign){
	long int res;
	
	if(num1Sign != '-' && num2Sign != '-'){
		res = arg1 + arg2;
	}else if(num1Sign == '-' && num2Sign != '-'){
		res = arg2 - arg1;
	}else if(num1Sign != '-' && num2Sign == '-'){
		res = arg1 - arg2;
	}else if(num1Sign == '-' && num2Sign == '-'){
		res = arg1 + arg2;
		res = res * -1;
	}else{
		fprintf(stderr, "ERROR: INVALID EITHER '-' OR BLANK\n");
	}
	
	if(res < 0){
		res = res *-1;
		printf("-");
	}else if(res == 0){
		printf("0\n");
		exit(EXIT_SUCCESS);
	}

	return res;
}

/* Subtract function
* returns the difference of two arguments
*/
long int sub(long int arg1, long int arg2, char num1Sign, char num2Sign ){
	long int res;

	if(num1Sign != '-' && num2Sign != '-'){
		res = arg1 - arg2;
	}else if(num1Sign != '-' && num2Sign == '-'){
		res = arg1 + arg2;
	}else if(num1Sign == '-' && num2Sign != '-'){
		res = arg1 + arg2;
		res *= -1;
	}else if(num1Sign == '-' && num2Sign == '-'){
		if(arg1 > arg2){
			res = arg1 - arg2;
			res *= -1;
		}else {
			res = arg2 - arg1;
		}
	}else {
		fprintf(stderr, "ERROR: INVALID EITHER '-' OR BLANK\n");
	}

	if(res < 0){
		res = res *-1;
		printf("-");
	}else if(res == 0){
		printf("0\n");
		exit(EXIT_SUCCESS);
	}
	return res;
}
/*multiply function
* returns the product of the arguments
*/
long int multiply(long int arg1, long int arg2, char num1Sign, char num2Sign){
	long int res; 

	if(num1Sign != '-' && num2Sign != '-'){
		res = arg1 * arg2;
	}else if((num1Sign == '-' && num2Sign != '-') || (num1Sign != '-' && num2Sign == '-')){
		res = arg1 * arg2;
		res *= -1;
	}else if(num1Sign == '-' && num2Sign == '-'){
		res = arg1 * arg2;
	}else{
		fprintf(stderr, "ERROR: INVALID EITHER '-' OR BLANK\n");
	}

	if(res < 0){
		res = res *-1;
		printf("-");
	}else if(res == 0){
		printf("0\n");
		exit(EXIT_SUCCESS);
	}
	return res;
}

/*
*Trim all the leading zeros
*Called only by intTochar function
*returns a trimmed string
*/
char *trim(char *str){
	int n;

	if((n = strspn(str, "0")) != 0 && str[n] != '\0'){
		return &str[n];
	}

	return str;
}


/* Int to char conversion 
*/
char *intTochar(int x){
	int y = x;
	int size = 0;
	char *str;
	
	while(y>0){
		size++;
		y--;
	}
	
	str = malloc(size+1);
	
	while(size-- > 0){
		
		str[size] = x%10 + '0';
		x = x/10;
		str[strlen(str)] = '\0';
		
	}

	return trim(str);
}

/*
 Converts a decimal character to an int
 returns an INT representation of the char
 */
long int DEC_ASCII_INT(char *str){
    
    long int res = 0; 
   
    if(str[0] == '-'){
    	str += 2;
    } else
    str++;
    
    while(*str){
        
        switch(*str){
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            res *= 10;
            res += *str -'0';
            break; 
        default:
            fprintf(stderr,"ERROR: NOT A DECIMAL STRING [%c]\n",*str);
            exit(EXIT_FAILURE);
        }
        str++;
    }
    
    return res;
}

/*converting Octal int to decimal int
called by OCTAL_ASCII_INT
*/
long int OCTAL_to_DEC(long int octal){
	long int dec = 0;
	int i = 0;
	while(octal != 0){
		dec += (octal % 10) * pow(8,i++);
		octal = octal/10;
	}

	return dec;
}

/*
Converts an octal character to an int
returns an INT representation of char
*/
long int OCTAL_ASCII_INT(char *str){
	long int res = 0; 

	if(str[0] == '-'){
    	str += 2;
    } else
    str++;

    while(*str){

    	switch(*str){
		case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
	        res *= 10;
	        res += *str - '0';
	        break;
      	default:
	      	fprintf(stderr,"ERROR: NOT AN OCTAL [%c]\n", *str);
	      	exit(EXIT_FAILURE);
    	}
     str++;
    }
    return OCTAL_to_DEC(res);
}


/*converts a HEX character to a DEC integer
* returns an integer representation of a hex
*/
long int HEX_ASCII_INT(char *str){
	long int res = 0;
	int dec = 0;
	int size = strlen(str);

	if(str[0] == '-'){
		size -= 2;
		str += 2;
	}else
	size -= 1;
	str++;

	
	size--;
	while(*str){

		switch(*str){
		case '0':
			dec = 0;
			break;
		case '1':
			dec = 1;
			break;
		case '2':
			dec = 2;
			break;
        case '3':
        	dec = 3;
        	break;
        case '4':
        	dec = 4;
        	break;
        case '5':
        	dec = 5;
        	break;
        case '6':
        	dec = 6;
        	break;
        case '7':
        	dec = 7;
        	break;
        case '8':
        	dec = 8;
        	break;
        case '9': 
        	dec = 9;
        	break;  
        case 'a':
        case 'A':
        	dec = 10;
        	break;
        case 'b':
        case 'B':
        	dec = 11;
        	break;
        case 'c':
        case 'C':
        	dec = 12;
        	break;
        case 'd':
        case 'D':
        	dec = 13;
        	break;
        case 'e':
        case 'E':
        	dec = 14;
        	break;
        case 'f':
        case 'F':
        	dec = 15;
        	break;
        default:
        	fprintf(stderr, "ERROR: NOT A HEX CHAR [%c]\n", *str);
        	exit(EXIT_FAILURE);
		}
		res += dec * pow(16, size);
		size--;
		str++;
	}
	
	return res;

}

/* converts Binary to dec
 * Called ONLY by BIN_ASCII_INT
 * returns a decimal
 */
long int Bin_toDEC(long int bin){
	long int dec, index, rem;
	dec = 0; index = 0; rem = 0;

	while(bin != 0){
		rem = bin%10;

		bin = bin/10;
		dec += rem*pow(2,index);
		index++;
	}

	return dec;
}

/*converts Binary ascii to int
*Calls on Bin_toDEC 
*Passes it the newly formed int
* returns a decimal
*/
long int BIN_ASCII_INT(char *str){
	long int res = 0;
	
	if(str[0] == '-'){
		str += 2;
	}else 
		str++;
	while(*str){

		switch(*str){
		case '0':
		case '1':
			res *= 10;
			res += *str -'0';
			break;
		default:
			fprintf(stderr, "ERROR: NOT A BINARY CHAR [%c]\n", *str);
			exit(EXIT_FAILURE);
		}
		str++;
	}

	return Bin_toDEC(res);
}

long int inputBase(char *str){
	if(str[0] == '-'){
		str++;
	} 

	switch(*str){
	case 'b':
		return (BIN_ASCII_INT(str));
		break;
	case 'd':
		return (DEC_ASCII_INT(str));
		break;
	case 'o':
		return (OCTAL_ASCII_INT(str));
		break;
	case 'x':
		return (HEX_ASCII_INT(str));
		break;
	default:
		fprintf(stderr, "ERROR: NOT A VALID BASE\n");
		exit(EXIT_FAILURE);
	}
return 0;
}

int toBin(long int dec){
	long int res = 0; 
	int digit = 1, i = 32, j;
	long int temp;
	temp = dec;
if(temp <= 1000){
	while(dec){
		res += (dec%2)*digit;
		dec /= 2;
		digit *= 10;
	}
} else {
	while(i >= 0){
		j = dec >> i;
		if(j & 1){
			printf("1");
		} else {
			printf("0");
		}
	i--;
	}
	printf("\n");
	exit(EXIT_SUCCESS);
}
	 	
	return res;
}

/*converts from decimal to octal
*returns an octal
*/
int toOctal(long int dec){
	long int res = 0; 
	int digit = 1;
	while(dec){
		res += (dec%8)*digit;
		dec /= 8;
		digit *= 10;
	}
	 
	return res;
}

/*converts from decimal to hex
*prints out a hex
*doesn't return anything
*/
void toHex(long int dec){
	int rem, i, pos = 1;
	char hexNum[60];
	while(dec!= 0){
		rem = dec % 16;

		if(rem < 10){
			rem += 48;
		}else {
			rem += 55;
		}
		hexNum[pos++] = rem;
		dec /= 16;
	}
	for(i = pos -1; i > 0; i--){
		printf("%c", hexNum[i]);
	}
}



/*Determine what format the result should be outputed
*takes in argv[4] 
*calles the appropiate function to convert to
*/
long int outputBase(long int num, char *output){
	/*
	if(num == 0){
		printf("0\n");
		exit(EXIT_SUCCESS);
	}
	*/
	switch(*output){
	case 'd':
		printf("d");
		return num;
		break;
	case 'b':
		printf("b");
		return toBin(num);
		
	case 'o':
		printf("o");
		return toOctal(num);
		break;
		
	case 'x':
		printf("x");
		toHex(num);
		break;
	default:
		fprintf(stderr, "ERROR: NOT A VALID OUTPUTBASE\n");
		exit(EXIT_FAILURE);
	}
return 0;
}

int main(int argc, char **argv){
	long int SUM, SUB, PROD, res;
	char *op = argv[1], *num1 = argv[2], *num2 = argv[3], *outBase = argv[4];
	
	if(argc != 5){
		fprintf(stderr,"ERROR: Not a valid number of arguments\n");
		exit(EXIT_FAILURE);
	}
    
    switch(*op){
	case '+':
		SUM = add(inputBase(num1), inputBase(num2),num1[0],num2[0]);
		res = outputBase(SUM, outBase); 
		return printf("%s\n",intTochar(res));
	case '-':
		SUB = sub(inputBase(num1), inputBase(num2),num1[0],num2[0]); 
		res = outputBase(SUB, outBase);
		return printf("%s\n", intTochar(res));
	case '*':
		PROD = multiply(inputBase(num1), inputBase(num2),num1[0],num2[0]);
		res =  outputBase(PROD, outBase);
		return printf("%s\n", intTochar(res));
	default:
		fprintf(stderr, "ERROR: NOT A VALID OPERATOR %c\n",*op);
    }

	return 0;
}
