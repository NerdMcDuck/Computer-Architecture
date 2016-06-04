#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "y86emul.h"


unsigned int pc; //program counter
unsigned long memSize; //stores the size of the allocated memory
unsigned char * y86memory; //memory of the y86 program
int startInstructions; //start point of the instructions
int endInstructions; //last address of instructions list

int ZF = 0, SF = 0, OF = 0; 
/* ZF zero flag
 * SF sign flag
 * OF overflow flag
 */
unsigned long registers[8]; //create an array for the registers 
 /*
	%eax 0
	%ecx 1
	%edx 2
	%ebx 3
	%esp 4
	%ebp 5
	%esi 6
	%edi 7
 */

int main(int argc, char * argv[])
{
	if(argc != 2)
	{
		fprintf(stderr,"\x1b[3;31;31mERROR! Not the correct amount of arguments\x1b[0m\n" );
		exit(EXIT_FAILURE);
	}

	if(strcmp(argv[1],"-h") == 0)
	{
		printf("\x1b[3;31;33mUsage: y86emul <y86 input file>\n<y86 input file> is the name of the input file\x1b[0m\n ");
		exit(EXIT_SUCCESS);
	}

	FILE * fp;
	char buff[800];
	fp = fopen(argv[1], "r+");

	if(fp == NULL)
	{
		fprintf(stderr, "\x1b[3;31;31mERROR! Could not open file:\x1b[3;31;32m %s\x1b[0m\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	/*Reads in the first 5 characters of each line
	 * Checks for the .size and .text directives
	 * returns error if missing either directive or has more than one of each
	 */

	int sizectr = 0, textctr = 0;

	while(fgets(buff, 6, fp) != NULL)
	{

	if( strcmp(buff, ".size") == 0)
	{
        
		sizectr++;

	}
	else if(strcmp(buff, ".text") == 0)
	{
		textctr++;
	}

	if(feof(fp))
	{
		break;
		}
	}

	if(sizectr != 1 || textctr != 1)
	{
		fprintf(stderr, "\x1b[3;31;31mERROR! Invalid number of .size or .text directives\x1b[0m\n");
		fclose(fp);
		exit(EXIT_FAILURE);
	}	

    rewind(fp);
	
	fgets(buff, sizeof(buff)/sizeof(char), fp);
	char * tokenized = strtok(buff, "\t\"");
	if(strcmp(tokenized, ".size") == 0)
	{
		tokenized = strtok(NULL, "\t\"");
		memSize = HEX_TO_DEC(tokenized);
		
		y86memory = (unsigned char *) calloc(1, memSize + 1);
	} 
	
	
	memset(tokenized, 0, sizeof(tokenized)/sizeof(char));
	
	//fetch and add it to memory
	while(fgets(buff, sizeof(buff)/sizeof(char), fp))
	{
		tokenized = strtok(buff, "\t\"");

		if(strcmp(tokenized, ".string") == 0)
		{
			tokenized = strtok(NULL, "\t\"");
			unsigned long address = HEX_TO_DEC(tokenized);
			tokenized = strtok(NULL, "\t\"");
			int i;
			for(i = 0; i < strlen(tokenized); i++)
			{
				y86memory[address + i] = tokenized[i];
			}
		}
		else if(strcmp(tokenized, ".long") == 0)
		{
			tokenized = strtok(NULL, "\t\"");
			unsigned long address = HEX_TO_DEC(tokenized);
			tokenized = strtok(NULL, "\t\"");
			*((int *) &y86memory[address]) = atoi(tokenized);
		}
		else if(strcmp(tokenized, ".bss") == 0)
		{
			tokenized = strtok(NULL, "\t\"");
			unsigned long address = HEX_TO_DEC(tokenized);
			tokenized = strtok(NULL, "\t\"");
			int size = atoi(tokenized);
			
			int i;
			for(i = 0; i <= size; i++)
			{
				y86memory[address + i] = (long) NULL;
			}
		}
		else if(strcmp(tokenized, ".byte") == 0)
		{
			tokenized = strtok(NULL, "\t\"");
			unsigned long address = HEX_TO_DEC(tokenized);
			tokenized = strtok(NULL, "\t\"");
			char byte[3];
			byte[0] = tokenized[0];
			byte[1] = tokenized[1];
			byte[2] = '\0';
			unsigned long byteValue = HEX_TO_DEC(byte);
			y86memory[address] = (unsigned char)byteValue;
		}
		else if(strcmp(tokenized, ".text") == 0)
		{
			tokenized = strtok(NULL, "\t\"");
			unsigned long address = HEX_TO_DEC(tokenized);
			pc = address;
			startInstructions = pc;
			tokenized = strtok(NULL, "\t\"");
			int i;
			char byte[3];
			for(i = 0; i < strlen(tokenized); i+=2)
			{
				byte[0] = tokenized[i];
				byte[1] = tokenized[i+1];
				byte[2] = '\0';
				y86memory[address] = hextobyte(byte);
				address++;
			}
			endInstructions = address;
		}
		memset(tokenized, 0, sizeof(tokenized)/sizeof(char));
	}
	
	Execute();
	free(y86memory);
	printf("\x1b[3;31;32mNo Errors encountered! ProgramStatus: AOK\x1b[0m\n");
	fclose(fp);
return 0;
}

//execute the instructions
void Execute()
{
	while(pc < endInstructions)
	{
		unsigned int instructionSet = (unsigned int) y86memory[pc];
		if(pc < startInstructions)
		{
			exit(EXIT_FAILURE);
		}
		
		switch (instructionSet)
		{
			case 0x00: //nop
				pc++; 
				break;
			case 0x10: //halt
				halt(); 
				pc++; 
				break;
			case 0x20: //rmovl
				rrmovl(); 
				pc += 2; 
				break;
			case 0x30: //irmovl
				irmovl(); 
				pc += 6; 
				break;
			case 0x40: //rmmovl
				rmmovl(); 
				pc += 6; 
				break;
			case 0x50: //mrmovl
				mrmovl(); 
				pc += 6; 
				break;
			case 0x60: //addl
				addl(); 
				pc += 2; 
				break;
			case 0x61: //subl
				subl(); 
				pc += 2; 
				break;
			case 0x62: //andl
				andl(); 
				pc += 2; 
				break;
			case 0x63: //xorl
				xorl(); 
				pc += 2; 
				break;
			case 0x64: //mull
				mull(); 
				pc += 2; 
				break;
			case 0x65: //cmpl
	            cmpl(); 
	            pc += 2; 
	            break;
			case 0x70: //jmp
				jmp(); 
				break;
			case 0x71: //jump if less than or equal
				jle(); 
				break;
			case 0x72: //jump if less than
				jl(); 
				break;
			case 0x73: //jump if equal
				je(); 
				break;
			case 0x74: //jump if not equal
				jne(); 
				break;
			case 0x75: //jump if greater or equal
				jge(); 
				break;
			case 0x76: //jump if greater
				jg(); 
				break;
			case 0x80: //call
				call(); 
				break;
			case 0x90: //return
				ret(); 
				break;
			case 0xa0: //pushl
				pushl(); 
				pc += 2; 
				break;
			case 0xb0: //popl
				popl(); 
				pc += 2; 
				break;
			case 0xc0: //readb
				readb(); 
				pc += 6; 
				break;
			case 0xc1: //readl
				readl(); 
				pc += 6; 
				break;
			case 0xd0: //writeb
				writeb(); 
				pc += 6; 
				break;
			case 0xd1: //writel
				writel(); 
				pc += 6; 
				break;
            case 0xe0: //movsbl
	            movsbl(); 
	            pc += 6; 
	            break;
			default: //if not a valid hex character or instruction
				fprintf(stderr, "\x1b[3;31;31mERROR! Execution stopped at 0x%x. ProgramStatus: INS\nNow exiting...\x1b[0m\n" , pc); 
				exit(EXIT_FAILURE); 
			break;
		}
	}
}

void halt()
{
	printf("\n\x1b[3;31;31mHalt Encountered.\nExecution stopped at 0x%x. ProgramStatus: HLT\x1b[0m\n", pc);
	exit(EXIT_FAILURE);
}
/*
 * Puts the value stored in one register into another.
 */
void rrmovl()
{
	unsigned int nextByte = y86memory[pc + 1];
	char * reg = Byte_to_Char(nextByte);
	if(reg[0] >= 'a' || reg[1] >= 'a')
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		registers[reg[1] - '0'] = registers[reg[0] - '0'];
	}
}

/*
 * Puts an immediate value into a register.
 */

void irmovl()
{
	unsigned int regBytes = y86memory[pc + 1];
	char * reg = Byte_to_Char(regBytes);
	if(reg[0] != 'f' || reg[1] >= 'a')
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		long immediate = getValue(pc + 2,y86memory);
		registers[reg[1] - '0'] = immediate;
	}
}

/*
 * Stores the value of a register in memory.
 */
void rmmovl()
{
	unsigned int regBytes = y86memory[pc + 1];
	char * reg = Byte_to_Char(regBytes);
	if(reg[0] >= 'a' || reg[1] >= 'a')
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{	
		long displacement = getValue(pc + 2,y86memory);
		*((int *) &y86memory[displacement + registers[reg[1] -'0']]) = registers[reg[0] - '0']; 
	}
}

/*
 * Stores a value in memory in a register.
 */

void mrmovl()
{
	unsigned int regBytes = y86memory[pc + 1];
	char * reg = Byte_to_Char(regBytes);
	if(reg[0] >= 'a' || reg[1] >= 'a')
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{	
		long displacement = getValue(pc + 2,y86memory);
		registers[reg[0] - '0'] = *((int *) &y86memory[displacement + registers[reg[1] - '0']]);
	}
}

//Performs an add operation

void addl()
{
	unsigned int regBytes = y86memory[pc + 1];
	char * reg = Byte_to_Char(regBytes);
	OF = 0;
	SF = 0;
	ZF = 0;
	
	if(reg[0] >= 'a' || reg[1] >= 'a')
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		long long sum = registers[reg[1] - '0'] + registers[reg[0] - '0'];
		if((registers[(int)reg[0]] > 0 && registers[(int)reg[1]] > 0 && sum < 0) || 
		   (registers[(int)reg[0]] < 0 && registers[(int)reg[1]] < 0 && sum > 0) ||
		   (sum > (pow(2, 31) - 1)) || (sum < (-1 * pow(2,31))))
		{
			OF = 1;
			printf("\x1b[3;31;31mERROR! STACK OVERFLOW at Instruction 0x%x\nNow exiting...\x1b[0m\n", pc); 
			exit(EXIT_FAILURE);
		}
		
		if(sum == 0)
		{
			ZF = 1;
		}
		else if(sum < 0)
		{
			SF = 1;
		}
		
		registers[reg[1] - '0'] = (long) sum;
	}
}

//performs a subtration operation

void subl()
{
	unsigned int regBytes = y86memory[pc + 1];
	const char * reg = Byte_to_Char(regBytes);
	ZF = 0;
	SF = 0;
	OF = 0;
	
	if(reg[0] >= 'a' || reg[1] >= 'a')
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		long long difference = registers[reg[1] - '0'] - registers[reg[0] - '0'];
		if((registers[(int)reg[0]] < 0 && registers[(int)reg[1]] > 0 && difference < 0) || 
		   (registers[(int)reg[0]] > 0 && registers[(int)reg[1]] < 0 && difference > 0) ||
		   (difference > pow(2, 31) - 1) || (difference < (-1 * pow(2,31))))
		{
			OF = 1;
			printf("\x1b[3;31;31mERROR! Overflow at 0x%x \nexiting...\x1b[0m\n",pc);
			exit(EXIT_FAILURE);
		}
		
		if(difference == 0)
		{
			ZF = 1;
		}
		else if(difference < 0)
		{
			SF = 1;
		}
		registers[reg[1] - '0'] = (long) difference;
	}
}

//Ands two registers together
void andl()
{
	unsigned int regBytes = y86memory[pc + 1];
	char * reg = Byte_to_Char(regBytes);
	ZF = 0;	SF = 0;
	
	if(reg[0] >= 'a' || reg[1] >= 'a')
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		long and = registers[reg[1] - '0'] & registers[reg[0] -'0'];
		if(and == 0)
		{
			ZF = 1; //set zero flag
		}
		else if (and < 0)
		{
			SF = 1; //negative, sets sign flag
		}
		
		registers[reg[1] - '0'] = and;
	}	
}

//Exclusive ORs two registers together

void xorl()
{
	unsigned int regBytes = y86memory[pc + 1];
	char * reg = Byte_to_Char(regBytes);
	ZF = 0;
	SF = 0;
	
	if(reg[0] >= 'a' || reg[1] >= 'a')
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		long xor = registers[reg[1] - '0'] ^ registers[reg[0] -'0'];
		if(xor == 0)
		{
			ZF = 1;
		}
		else if (xor < 0)
		{
			SF = 1;
		}
		
		registers[reg[1] - '0'] = xor;
	}
}

//Performs a multiplication operation 

void mull()
{
	unsigned int regBytes = y86memory[pc + 1];
	const char * reg = Byte_to_Char(regBytes);
	ZF = 0;
	SF = 0;
	OF = 0;
	
	if(reg[0] >= 'a' || reg[1] >= 'a')
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		long long product = registers[reg[0] - '0'] * registers[reg[1] - '0'];
		if((registers[(int)reg[0]] > 0 && registers[(int)reg[1]] > 0 && product < 0) ||
		   (registers[(int)reg[0]] < 0 && registers[(int)reg[1]] < 0 && product > 0) ||
		   (registers[(int)reg[0]] < 0 && registers[(int)reg[1]] > 0 && product > 0) ||
		   (registers[(int)reg[0]] > 0 && registers[(int)reg[1]] < 0 && product > 0) ||
		   product > pow(2, 32) - 1 || product < (-1 * pow(2, 31)))
		{
			OF = 1;
			printf("\x1b[3;31;31mERROR! STACK OVERFLOW at Instruction 0x%x\nProgramStatus: ADS\nNow exiting...\x1b[0m\n", pc);
			exit(EXIT_FAILURE);
		}
		
		if(product == 0)
		{
			ZF = 1;
		}
		else if(product < 0)
		{
			SF = 1;
		}
		
		registers[reg[1] - '0'] = (long) product;
	}
}

//Compares two registers and sets flags accordingly

void cmpl()
{
    unsigned int regBytes = y86memory[pc + 1];
    const char * reg = Byte_to_Char(regBytes);
    ZF = 0;
    SF = 0;
    OF = 0;
    
    if(reg[0] >= 'a' || reg[1] >= 'a')
    {
        printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
        exit(EXIT_FAILURE);
    }
    else
    {
        long long difference = registers[reg[1] - '0'] - registers[reg[0] - '0'];
        if((registers[(int)reg[0]] < 0 && registers[(int)reg[1]] > 0 && difference < 0) ||
           (registers[(int)reg[0]] > 0 && registers[(int)reg[1]] < 0 && difference > 0) ||
           (difference > pow(2, 31) - 1) || (difference < (-1 * pow(2,31))))
        {
            OF = 1;
            printf("\x1b[3;31;31mERROR! STACK OVERFLOW at Instruction 0x%xProgramStatus: ADS\nNow exiting...\x1b[0m\n", pc);
            exit(EXIT_FAILURE);
        }
        
        if(difference == 0)
        {
            ZF = 1;
        }
        else if(difference < 0)
        {
            SF = 1;
        }
        
    }
}

void jmp() //Jump
{
	unsigned long jump = getAddress(pc + 1,y86memory);
	if(jump < startInstructions || jump > endInstructions)
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		pc = jump;
	}
}

void jle() //Jump if less than or equal
{
	unsigned long jump = getAddress(pc + 1,y86memory);
	if(jump < startInstructions || jump > endInstructions)
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		if((SF ^ OF) | ZF)
			pc = jump;
		else
			pc += 5;
	}
}

void jl()//Jump is less than
{
	unsigned long jump = getAddress(pc + 1,y86memory);
	if(jump < startInstructions || jump > endInstructions)
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		if(SF ^ OF)
			pc = jump;
		else
			pc += 5;
	}
}

void je() //Jump if equal
{
	unsigned long jump = getAddress(pc + 1,y86memory);
	if(jump < startInstructions || jump > endInstructions)
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		if(ZF == 1)
			pc = jump;
		else
			pc += 5;
	}
}

void jne() //Jump is not equal
{
	unsigned long jump = getAddress(pc + 1,y86memory);
	
	if(jump < startInstructions || jump > endInstructions)
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		if(ZF == 0)
			pc = jump;
		else
			pc += 5;
	}
}

void jge()//Jump if greater than or equal
{
	unsigned long jump = getAddress(pc + 1,y86memory);
	if(jump < startInstructions || jump > endInstructions)
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		if((SF^OF) == 0)
			pc = jump;
		else
			pc += 5;
	}
}

void jg() //Jump if greater than
{
	unsigned long jump = getAddress(pc + 1,y86memory);

	if(jump < startInstructions || jump > endInstructions)
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		if(~(SF ^ OF) & ~ZF)
			pc = jump;
		else
			pc += 5;
	}
}

/*
 * Stores the value of the instruction following "call" 
 * The program counter is set to be the value following "call".
 */
void call()
{
	unsigned long address = getAddress(pc + 1,y86memory);
	pc += 5;
	if(address < startInstructions || address > endInstructions)
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		registers[4] -= 4;
		*((int *) &y86memory[registers[4]]) = pc;
		pc = address;
	}
}

/*
 * Pops the register and sets the program counter
 * to be equal to the value of it.
 */
void ret()
{
	pc = *((int*)&y86memory[registers[4]]);
	registers[4] += 4;
}

/*
 * Decrements %esp by four and stores the value of the
 * pushed register in the decremented %esp's place in memory.
 * Updates %esp and increments the program counter.
 */

void pushl()
{
	unsigned int regBytes = y86memory[pc + 1];
	char * reg = Byte_to_Char(regBytes);
	if(reg[0] >= 'a' || reg[1] != 'f')
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		registers[4] -= 4;
		*((int*) &y86memory[registers[4]]) = registers[reg[0] - '0'];
	}
}
/*
 * Increments %esp by four and stores the value of the
 * popped register in the incremented %esp's place in memory.
 * Updates %esp and increments the program counter.
 */
void popl()
{
	unsigned int regBytes = y86memory[pc + 1];
	char * reg = Byte_to_Char(regBytes);
	if(reg[0] >= 'a' || reg[1] != 'f')
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		registers[reg[0] - '0'] = *((int *) &y86memory[registers[4]]);
		registers[4] += 4;
	}
}

void readb()
{
	unsigned int regBytes = y86memory[pc + 1];
	char * reg = Byte_to_Char(regBytes);
	ZF = 0;
	
	if(reg[0] >= 'a' || reg[1] != 'f')
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		long immediate = getValue(pc + 2,y86memory);
		char byte[3];
		byte[2] = '\0';
		if(scanf("%hhx", byte) != EOF)
		{
			long address = registers[reg[0] - '0'] + immediate;
			if(address < endInstructions)
			{
				printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n",pc);
				exit(EXIT_FAILURE);
			}
			else
			{
				if(byte[0] > 'f' || byte[1] > 'f')
				{
					printf("\x1b[3;31;31mERROR! Invalid input. Input must be a hex value from 00 to ff\x1b[0m\n");
					exit(EXIT_FAILURE);
				}
				else
				{
					y86memory[address] = HEX_TO_DEC(byte);
					if(y86memory[address] == 0)
					{
						ZF = 1;
					}
				}
			}
		}
		else
		{
			ZF = 1;
		}
	}
}

void readl()
{
	unsigned int regBytes = y86memory[pc + 1];
	char * reg = Byte_to_Char(regBytes);
	ZF = 0;
	
	if(reg[0] >= 'a' || reg[1] != 'f')
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		long immediate = getValue(pc + 2,y86memory);
		unsigned long int word[32];
		word[31] = '\0';
		if(scanf("%ld", word) != EOF)
		{
			*((unsigned long int *) (&y86memory[registers[reg[0] - '0'] + immediate])) = *word;
			if(*((unsigned long int *) (&y86memory[registers[reg[0] - '0'] + immediate])) == 0 )
			{
				ZF = 1;
			}
		}
		else
		{
			ZF = 1;
		}
	}
}

void writeb()
{
	unsigned int regBytes = y86memory[pc + 1];
	char * reg = Byte_to_Char(regBytes);
	
	if(reg[0] >= 'a' || reg[1] != 'f')
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		long immediate = getValue(pc + 2,y86memory);
		unsigned char byte = (unsigned char) y86memory[registers[reg[0] - '0'] + immediate];
		printf("%c", byte);
	}
}

void writel()
{
	unsigned int regBytes = y86memory[pc + 1];
	char * reg = Byte_to_Char(regBytes);
	
	if(reg[0] >= 'a' || reg[1] != 'f')
	{
		printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
		exit(EXIT_FAILURE);
	}
	else
	{
		long immediate = getValue(pc + 2,y86memory);
		unsigned long int word = *((int *) &y86memory[registers[reg[0] - '0'] + immediate]);
		printf("%ld", word);
	}
}

void movsbl()
{
    unsigned int regBytes = y86memory[pc + 1];
    char * reg = Byte_to_Char(regBytes);
    if(reg[0] >= 'a' || reg[1] >= 'a')
    {
        printf("\x1b[3;31;31mERROR! Invalid Address encountered.\nExecution stopped at 0x%x. ProgramStatus: ADR\nNow exiting...\x1b[0m\n", pc);
        exit(EXIT_FAILURE);
    }
    else
    {
        long displacement = getValue(pc + 2,y86memory);
        registers[reg[0] - '0'] = *((int *) &y86memory[displacement + registers[reg[1] - '0']]);
    }
}

