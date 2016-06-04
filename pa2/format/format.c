#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* take a binary string character
* returns an unsigned int in twos complement
*/

unsigned int toInt(char *binary){
	int i=0, length = 32, index, carry = 1, traversal = 0;
	char temp;
	char *bin = malloc(sizeof(char)*32);
	unsigned int dec = 0;

/* populate a char array */
	while(*binary){

		switch(*binary){
		case '0':
			bin[i] = '0';
			break;
		case '1':
			bin[i] = '1';
			break;
		default:
			fprintf(stderr, "NOT BINARY\n");
			exit(EXIT_FAILURE);
		}
	binary++;
	i++;
	}
/* takes the complement of it */
	for(index = 0; index < length; index++){
		temp = bin[index];
		if((temp & '1') == '1'){
			bin[index] = '0';
		}else{
			bin[index] = '1';
		}
	}

/* adds 1 to negated binary string, convert to twos complement */	
	for(index = length-1; index > 0; index--){
		temp = bin[index];
		if(temp == '1' && carry == 1){
			bin[index] = '0';
		}else if(temp == '0' && carry == 1){
			bin[index] = '1';
			carry = 0;
		}else{
			continue;
		}
	}
	
/*converts the twos complement binary to decimal */
	while(bin[traversal]){
		dec = dec |(bin[traversal] == '1') << traversal;
		traversal++;
	} 

return dec;
}

/* take a binary string character
* returns an a float in IEEE 754 single precision
*/
long int toFloat(char *binary){
	return 0;
}

int main(int argc, char **argv){
	char *binary = argv[1];
	char *s = argv[2];
	
	if(argc != 3){
		fprintf(stderr,"ERROR: Not a valid number of arguments\n");
		exit(EXIT_FAILURE);
	}
	
	switch(*s){
	case 'i':
		return printf("%d\n",toInt(binary));
	case 'f':
		return toFloat(binary);
	default:
		fprintf(stderr,"ERROR: Not a valid type\n");
		exit(EXIT_FAILURE);
	}

}