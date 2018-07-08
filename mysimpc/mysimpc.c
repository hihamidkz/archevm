#include <stdio.h>

#include "mysimpc.h"

static int memory[100];
static int reg_flags;

int sc_memoryInit()
{
	int i;
	
	for (i = 0; i < 100; i++)
		memory[i] = 0;
		
	return 0;
}

int sc_memorySet(int address, int value)
{
	if (address > 99) {
		sc_regSet(MEMORY_BORDER_OUT, 1);
		return -1;
	}
	
	memory[address] = value;
	return 0;
}

int sc_memoryGet(int address, int *value)
{
	if (address > 99) {
		sc_regSet(MEMORY_BORDER_OUT, 1);
		return -1;
	}
	
	*value = memory[address];
	return 0;
}

int sc_memorySave(char *filename)
{
	FILE *stream = fopen(filename, "wb");
	
	if (!stream)
		return -1;
	
	fwrite(memory, sizeof(int), sizeof(memory) / sizeof(int), stream);
	fclose(stream);
	return 0;
}

int sc_memoryLoad(char *filename)
{
	FILE *stream = fopen(filename, "rb");
	
	if (!stream)
		return -1;
	
	fread(memory, sizeof(int), sizeof(memory) / sizeof(int), stream);
	fclose(stream);
	return 0;
}
		
int sc_regInit()
{
	reg_flags = 0;
	return 0;
}

int sc_regSet(int regist, int value)
{
	if (value < 0 || value > 1 || regist > 5 || regist < 1)
		return -1; 

	reg_flags = (value == 0) ? reg_flags & (~(1 << (regist - 1))) : reg_flags | (1 << (regist - 1));
	return 0;
}

int sc_regGet(int regist, int *value)
{
	if (regist > 5 || regist < 1)
		return -1;
	
	*value = (reg_flags >> (regist - 1)) & 0x1;
	return 0;
}

int sc_commandEncode(int command, int operand, int *value)
{
	*value = 0;
	
	if (operand < 0 || operand > 127)
		return -1;
	if (command < 0 || command > 127)
		return -1;
	
	*value = command;
	*value = *value << 7;
	*value = *value | operand;
	return 0;
}

int sc_commandDecode(int value, int *command, int *operand)
{
	if (value > 16383 || value < 0) {
		sc_regSet(INCORRECT_COMMAND, 1);
		return -1;
	}
	
	*command = value >> 7;
	*operand = value & 127;
	return 0;
}







