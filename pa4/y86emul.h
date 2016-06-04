#ifndef Y86EMUL_H
#define Y86EMUL_H

//Converts a byte in unsigned int form to a char string that holds the chars
char * Byte_to_Char(unsigned int byte)
{
	char hex[16]={"0123456789abcdef"};
	char * byteChar = calloc(1,3);
	
	int i = 1;
	while(byte)
	{
		byteChar[i] = hex[byte % 16];
		byte = byte/16;
		i--;
	}
	
	if(byteChar[0] == '\0')
	{
		byteChar[0] = '0';
	}
	
	if(byteChar[1] == '\0')
	{
		byteChar[1] = '0';
	}
	
	return byteChar; 
}

//Converts hex string to an unsigned integer which will then be converted to a char
//and stored into the y86memory
unsigned int hextobyte(char * byte)
{
	int i;
	unsigned int sum = 0;
	for(i = strlen(byte) - 1; i >= 0; i--)
	{
		char digit = byte[i];
		if(digit > '9')
		{
			sum += (digit - 'a' + 10) * pow(16, strlen(byte) - (i+1)) ;
		}
		else
		{
			sum += (digit - '0') * pow(16, strlen(byte) - (i+1));
		}
	}
	return sum;
}

//Converts hex string to an unsigned decimal long
unsigned long HEX_TO_DEC(char * hex)
{
	int j;
	unsigned long dec = 0;
	for(j = strlen(hex) - 1; j >= 0; j--)
	{
		char digit = hex[j];
		if(digit > '9')
		{
			dec += (digit - 'a' + 10) * pow(16, strlen(hex) - (j+1)) ;
		}
		else
		{
			dec += (digit - '0') * pow(16, strlen(hex) - (j+1));
		}
	}	
	
	return dec;
}


//Converts from  2's complement
void twoscomp(char * input)
{
	//Subtract one from the input
	int index;
	for(index = strlen(input) - 1; index >= 0; index--)
	{
		if(input[index] == '1')
		{
			input[index] = '0';
			break;
		}
		else if(input[index] == '0')
		{
			input[index] = '1';
		}
		else
		{
			printf("NaN\n");
			exit(EXIT_FAILURE);
		}
	}
	
	//Complement the digits
	int index2;
	for(index2 = 0; index2 < strlen(input); index2++)
	{
		if(input[index2] == '0')
		{
			input[index2] = '1';
		}
		else if(input[index2] == '1')
		{
			input[index2] = '0';
		}
		else
		{
			printf("NaN\n");
			exit(EXIT_FAILURE);
		}
	}
}

//Converts hex string to a signed decimal long
long HEX_TO_SDEC(char * hex)
{
	//convert to binary to check for two's complement
	char* binary[16] = {"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"};
	
	int index, isNeg = 0;
	char* bin = (char*) calloc(1,strlen(hex) * 4 + 1);
	for(index = 0; index < strlen(hex); index++)
	{
		char digit = hex[index];
		if(digit > '9')
		{
			strcat(bin, binary[digit - 'a' + 10]) ;
		}
		else
		{
			strcat(bin, binary[digit - '0']);
		}
	}
	
	if(bin[0] == '1')
	{
		twoscomp(bin);
		isNeg = 1;
	}
	
	//convert from bin to dec
	int index2;
	long sum = 0;
	for(index2 = strlen(bin) - 1; index2 >= 0; index2--)
	{
		sum += (bin[index2] - '0') * pow(2, strlen(bin) - (index2+1));
	}		
	
	if(isNeg == 1)
		return -1 * sum;
	else
		return sum;
}


//Converts the 32 bit address from little endian to the proper value
unsigned long getAddress(unsigned long pc, unsigned char * y86memory)
{
	char bigEndian[9];
	
	int index;
	for(index = 0; index < 9; index++)
	{
		bigEndian[index] = '\0';
	}
	
	strcat(bigEndian, Byte_to_Char(y86memory[pc + 3]));
	strcat(bigEndian, Byte_to_Char(y86memory[pc + 2]));
	strcat(bigEndian, Byte_to_Char(y86memory[pc + 1]));
	strcat(bigEndian, Byte_to_Char(y86memory[pc]));
	
return HEX_TO_DEC(bigEndian);
}

long getValue(unsigned long pc, unsigned char * y86memory)
{
	char bigEndian[9];
	
	int index;
	for(index = 0; index < 9; index++)
	{
		bigEndian[index] = '\0';
	}
	
	strcat(bigEndian, Byte_to_Char(y86memory[pc + 3]));
	strcat(bigEndian, Byte_to_Char(y86memory[pc + 2]));
	strcat(bigEndian, Byte_to_Char(y86memory[pc + 1]));
	strcat(bigEndian, Byte_to_Char(y86memory[pc]));

	return HEX_TO_SDEC(bigEndian);
}

//Execution functions
void halt();
void Execute();
void rrmovl();
void irmovl();
void rmmovl();
void mrmovl();
void addl();
void subl();
void andl();
void xorl();
void mull();
void cmpl();
void jmp();
void jle();
void jl();
void je();
void jne();
void jge();
void jg();
void call();
void ret();
void pushl();
void popl();
void readb();
void readl();
void writeb();
void writel();
void movsbl();

#endif