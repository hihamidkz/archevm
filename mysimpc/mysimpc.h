#ifndef MYSIMPC_H
#define MYSIMPC_H

int sc_memoryInit();
int sc_memorySet(int address, int value);
int sc_memoryGet(int address, int *value);
int sc_memorySave(char *filename);
int sc_memoryLoad(char *filename);
int sc_regInit();
int sc_regSet(int regist, int value);
int sc_regGet(int regist, int *value);
int sc_commandEncode(int command, int operand, int *value);
int sc_commandDecode(int value, int *command, int *operand);

#define OVERFLOW 1
#define DIVISION_BY_ZERO 2
#define MEMORY_BORDER_OUT 3
#define CLOCK_PULSE_INGORE 4
#define INCORRECT_COMMAND 5

#endif
