#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc & free
#include <stdint.h> // use guaranteed 64-bit integers
#include "tokenizer.h" // Create header file and reference that
#include "memory.h" // built-in functions to read and write to a specific file

int32_t* reg; // Array of 32 32-bit registers

void init_regs();
bool interpret(char* instr);
void write_read_demo();

/**
 * Initialize register array for usage.
 * Malloc space for each 32-bit register then initializes each register to 0.
 * Do not alter this function!
 */
void init_regs(){
	int reg_amount = 32;
	reg = malloc(reg_amount * sizeof(int32_t)); // 32 * 4 bytes
	for(int i = 0; i < 32; i++)
		reg[i] = i;
}
/**
 * Compares two strings and returns 0 if they're equal, 1 otherwise.
 */
int compare(char* in, char* og) {
    char* tmin = in;
    char* tmog = og;
	while(*tmin != '\0' || *tmog != '\0') {
      if((*tmin != *tmog) || (*tmin == '\0' && *tmog != '\0') || (*tmin != '\0' && *tmog == '\0')) {
        return 1;
      }
      else if(*tmin == *tmog) {
         tmin++;
         tmog++;
      }
   }
	return 0;
}
/**
 * Removes the character 'X' from the parameter string then returns it.
 */
char* remove_x(char* str) {
	char* temp = str;
	while(*temp == 'X') {
		temp++;
	}
	return temp;
}
/**
 * Uses the compare function to determine if the parameter instruction is the same as the following if statements.
 * If it finds them to be equal then int a becomes one of the options and is returned otherwise a is returned as 0.
 */
int pick(char* str) {
	char* choice = str;
	int a = 0;
	if(compare(choice,"SW") == 0) {
		a = 1;
	}
	if(compare(choice,"LW") == 0) {
		a = 2;
	}
	if(compare(choice,"ADD") == 0) {
		a = 3;
	}
	if(compare(choice,"ADDI") == 0) {
		a = 4;
	}
	return a;
}
/**
 * Performs the assembly RISC-V ADD instruction.
 * Adds the values of two registers and stores them at the location of the leftmost register.
 */
void in_add(char* s1, char* s2, char* s3) {
	int a = atoi(remove_x(s1));
	int b = atoi(remove_x(s2));
	int c = atoi(remove_x(s3));
	reg[a] = reg[b] + reg[c];
}
/**
 * Performs the assembly RISC-V ADDI instruction.
 * Adds the value of a register and an immediate, then stores them at the location of the leftmost register.
 */
void in_addi(char* s1, char* s2, char* n) {
	int a = atoi(remove_x(s1));
	int b = atoi(remove_x(s2));
	int c = atoi(n);
	reg[a] = reg[b] + c;
}
/**
 * Performs the assembly RISC-V SW instruction.
 * Takes the value stored in a register and finds an address.
 * Then replaces the content of that adress in memory by the contents of the previously mentioned register.
 */
void in_sw(char* s1, char* n, char* s2) {
	int32_t rd = atoi(remove_x(s1));
	int32_t imm = atoi(n);
	int32_t rs = atoi(remove_x(s2));
	int32_t data = reg[rd];
	int32_t address = imm + reg[rs];
	write_address(data,address,"mem.txt");
}
/**
 * Performs the assembly RISC-V LW instruction.
 * Takes the value stored in a memory address.
 * Then replaces the content of a register by the value taken from the memory address.
 */
void in_lw(char* s1, char* n, char* s2) {
	int32_t rd = atoi(remove_x(s1));
	int32_t imm = atoi(n);
	int32_t rs = atoi(remove_x(s2));
	int32_t address = (imm*2) + reg[rs];
	printf("%ld,%ld\n\n",address,rd);
	int32_t store = read_address(address, "mem.txt");
	reg[rd] = store;
}
/**
 * Fill out this function and use it to read interpret user input to execute RV64 instructions.
 * You may expect that a single, properly formatted RISC-V instruction string will be passed
 * as a parameter to this function.
 */
/**
 * Tokenizes RISC-V assembly instructions then parses them.
 * After parsing executes the instruction making changes in either memory or registers then returns true.
 */
bool interpret(char* instr){
	char** tokens = tokenize(instr);
	int c = pick(tokens[0]);
	if(c != 0) {
		switch(c) {
			case 1:
				in_sw(tokens[1],tokens[2],tokens[3]);
			break;
			case 2:
				in_lw(tokens[1],tokens[2],tokens[3]);
			break;
			case 3:
				in_add(tokens[1],tokens[2],tokens[3]);
			break;
			case 4:
				in_addi(tokens[1],tokens[2],tokens[3]);
			break;
		}
	}
	else {
		printf("Invalid instruction please enter another one: \n");
	}
	free(tokens);
	return true;
}


/**
 * Simple demo program to show the usage of read_address() and write_address() found in memory.c
 * Before and after running this program, look at mem.txt to see how the values change.
 * Feel free to change "data_to_write" and "address" variables to see how these affect mem.txt
 * Use 0x before an int in C to hardcode it as text, but you may enter base 10 as you see fit.
 */
void write_read_demo(){
	int32_t data_to_write = 0xFFF; // equal to 4095
	int32_t address = 0x98; // equal to 152
	char* mem_file = "mem.txt";

	// Write 4095 (or "0000000 00000FFF") in the 20th address (address 152 == 0x98)
	int32_t write = write_address(data_to_write, address, mem_file);
	if(write == (int32_t) NULL)
		printf("ERROR: Unsucessful write to address %0X\n", 0x40);
	int32_t read = read_address(address, mem_file);

	printf("Read address %lu (0x%lX): %lu (0x%lX)\n", address, address, read, read); // %lu -> format as an long-unsigned
}
//Prints all the registers from the regs array.
void print_regs(){
	int col_size = 10;
	for(int i = 0; i < 8; i++){
		printf("X%02i:%.*lld", i, col_size, (long long int) reg[i]);
		printf(" X%02i:%.*lld", i+8, col_size, (long long int) reg[i+8]);
		printf(" X%02i:%.*lld", i+16, col_size, (long long int) reg[i+16]);
		printf(" X%02i:%.*lld\n", i+24, col_size, (long long int) reg[i+24]);
	}
}

/**
 * Your code goes in the main
 *
 */
int main(){
	// Do not write any code between init_regs
	init_regs(); // DO NOT REMOVE THIS LINE

	print_regs();

	// Below is a sample program to a write-read. Overwrite this with your own code.
	//write_read_demo();

	printf(" RV32 Interpreter.\nType RV32 instructions. Use upper-case letters and space as a delimiter.\nEnter 'EOF' character to end program\n");

	char* instruction = malloc(1000 * sizeof(char));
	bool is_null = false;
	// fgets() returns null if EOF is reached.
	is_null = fgets(instruction, 1000, stdin) == NULL;
	while(!is_null){
		interpret(instruction);
		printf("\n");
		print_regs();
		printf("\n");

		is_null = fgets(instruction, 1000, stdin) == NULL;
	}

	printf("Good bye!\n");

	return 0;
}