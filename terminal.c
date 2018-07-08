#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>

#include "mybigchars/mybigchars.h"
#include "myterm/myterm.h"
#include "mysimpc/mysimpc.h"
#include "myreadkey/myreadkey.h"
#include "terminal.h"

void print_bigChars(int value)
{
	int c, o;
	sc_commandDecode(value, &c, &o);
	
	int v[4];
	int ch[2];
	ch[0] = 4279769088;
	ch[1] = 1579263;
	
	bc_printbigchar(ch, 14, 4, 0, 0);

	v[0] = c / 16;
	v[1] = c & 0x0F;
	v[2] = o / 16;
	v[3] = o & 0x0F;
	
	for (int i = 0; i < 4; i++) {
		switch (v[i]) {
			case 0x0:
			ch[1] = 1019462595;
			ch[0] = 3284386620;
			break;
			case 0x1:
			ch[0] = 1732189959;
			ch[1] = 117901063;
			break;
			case 0x2:
			ch[0] = 50578431;
			ch[1] = 4290822399;
			break;
			case 0x3:
			ch[0] = 4228056060;
			ch[1] = 4228055811;
			break;
			case 0x4:
			ch[0] = 1714822670;
			ch[1] = 101121990;
			break;
			case 0x5:
			ch[0] = 4290822399;
			ch[1] = 4278387459;
			break;
			case 0x6:
			ch[0] = 3228572679;
			ch[1] = 4291019775;
			break;
			case 0x7:
			ch[0] = 100860927;
			ch[1] = 1613764620;
			break;
			case 0x8:
			ch[0] = 3284386620;
			ch[1] = 1019462460;
			break;
			case 0x9:
			ch[0] = 4291019775;
			ch[1] = 3228572679;
			break;
			case 0xA:
			ch[0] = 3284368920;
			ch[1] = 3284386815;
			break;
			case 0xB:
			ch[0] = 3284386812;
			ch[1] = 4240688124;
			break;
			case 0xC:
			ch[0] = 3233833756;
			ch[1] = 476299456;
			break;
			case 0xD:
			ch[0] = 3284389104;
			ch[1] = 4039951299;
			break;
			case 0xE:
			ch[0] = 4290822399;
			ch[1] = 4290822336;
			break;
			case 0xF:
			ch[0] = 4290822399;
			ch[1] = 3233857728;
			break;
			default:
			break;
		}
		bc_printbigchar(ch, 14, 13 + 10 * i, 0, 0);
	}
	mt_gotoXY(2, 2);
}

void print_memory(int value)
{
	int c, o;
	char buf[10] = {0};
	
	sc_commandDecode(value, &c, &o);
	sprintf(buf, "+%.2x", c);
	write(1, buf, sizeof(buf) / sizeof(char));
	sprintf(buf, "%.2x ", o);
	write(1, buf, sizeof(buf) / sizeof(char));
}

void print_accum()
{
	int c, o;
	char buf[10] = {0};
	
	sc_commandDecode(accumulator, &c, &o);
	mt_gotoXY(2, 72);
	sprintf(buf, "+%.2x", c);
	write(1, buf, sizeof(buf) / sizeof(char));
	sprintf(buf, "%.2x\n", o);
	write(1, buf, sizeof(buf) / sizeof(char));
}

void print_instCounter()
{
	char buf[10] = {0};
	
	mt_gotoXY(5, 72);
	sprintf(buf, "+%.4x\n", instructionCounter);
	write(1, buf, sizeof(buf) / sizeof(char));
}

void print_operation()
{
	int value; 
	int c, o;
	char buf[10] = {0};
	
	sc_memoryGet(instructionCounter, &value);
	sc_commandDecode(value, &c, &o);
	mt_gotoXY(8, 71);
	sprintf(buf, "+%.2x : %.2x\n", c, o);
	write(1, buf, sizeof(buf) / sizeof(char));
}

void print_regFlags()
{
	int reg;
	char buf[10] = {0};
	
	for (int i = 1; i < 6; i++) {
		sc_regGet(i, &reg);
		mt_gotoXY(11, 71 + (i - 1) * 2);
		sprintf(buf, "%d", reg);
		write(1, buf, sizeof(buf) / sizeof(char));
	}
}

void signalhandler(int signo)
{
	int value;
	
	if (instructionCounter == 99)
		instructionCounter = 0;
	
	instructionCounter++;
	print_instCounter();
	print_operation();
	sc_memoryGet(instructionCounter, &value);
	
	print_bigChars(value);
}

void sighandler(int signo)
{
	accumulator = 0;
	instructionCounter = 0;
	sc_regInit();
	sc_memoryInit();
	sc_regSet(4, 1);
	
	rk_mytermregime(0, 0, 1, 1, 1);
	mt_gotoXY(2, 3);
	//srand(time(NULL));
	int value;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			//sc_memorySet(i * 10 + j, rand() % 255);
			sc_memoryInit();
			sc_memoryGet(i * 10 + j, &value);
			print_memory(value);
		}
		mt_gotoXY(i + 3, 3);
	}
	print_accum();
	print_instCounter();
	print_operation();
	print_regFlags();
	mt_gotoXY(2, 2);
	rk_mytermregime(0, 0, 1, 0, 1);
}

void set_memory(int *x, int *y)
{	
	if (*x > 11 || *y < 4 || *x == 1 || *y > 63)
		return;
	
	if ((*y - 4) % 6 != 0)
		return;
	
	char buf[4] = {0};
	int c = 0, o = 0, val;

	for (int i = 0; i < 4; i++) {
		read(0, &buf[i], 1);
		buf[i] = (buf[i] > 64 && buf[i] < 71) ? buf[i] + 32 : buf[i];
		buf[i] = (buf[i] < 48 || (buf[i] > 57 && buf[i] < 97) || buf[i] > 102) ? 48 : buf[i];
		write(0, &buf[i], 1);
		buf[i] = (buf[i] < 58) ? buf[i] - 48 : buf[i] - 87;
	}
	c |= buf[0];
	c = (c << 4) | buf[1];
	o |= buf[2];
	o = (o << 4) | buf[3];
	c = (c > 127) ? 0 : c;
	o = (o > 127) ? 0 : o;
	int i = (*x - 2) * 10 + (*y - 4) / 6;
	
	*y += 4;
	sc_commandEncode(c, o, &val);
	sc_memorySet(i, val);
	
	if (i == instructionCounter) {
		print_bigChars(val);
		mt_gotoXY(*x, *y);
	}
}

void set_accumulator()
{
	char buf[4] = {0};
	int c = 0; int o = 0; int value;
	
	write(0, "\E[2;73H", 7);
	for (int i = 0; i < 4; i++) {
		read(0, &buf[i], 1);
		buf[i] = (buf[i] > 64 && buf[i] < 71) ? buf[i] + 32 : buf[i];
		buf[i] = (buf[i] < 48 || (buf[i] > 57 && buf[i] < 97) || buf[i] > 102) ? 48 : buf[i];
		write(0, &buf[i], 1);
		buf[i] = (buf[i] < 58) ? buf[i] - 48 : buf[i] - 87;
	}
	c |= buf[0];
	c = (c << 4) | buf[1];
	o |= buf[2];
	o = (o << 4) | buf[3];
	sc_commandEncode(c, o, &value);
	accumulator = value;
	
	write(0, "\E[2;2H", 7);
}

void set_instCounter()
{
	char buf[2] = {0};
	int c = 0;
	
	write(0, "\E[5;75H", 7);
	
	read(0, &buf[0], 1);
	buf[0] = (buf[0] < 48 || buf[0] > 55) ? 48 : buf[0];
	write(0, &buf[0], 1);
	buf[0] -= 48;
	read(0, &buf[1], 1);
	buf[1] = (buf[1] > 64 && buf[1] < 71) ? buf[1] + 32 : buf[1];
	buf[1] = (buf[1] < 48 || (buf[1] > 57 && buf[1] < 97) || buf[1] > 102) ? 48 : buf[1];
	write(0, &buf[1], 1);
	buf[1] = (buf[1] < 58) ? buf[1] - 48 : buf[1] - 87;
	
	c |= buf[0];
	c = (c << 4) | buf[1];
	instructionCounter = c;
	
	int value;
	sc_memoryGet(c, &value);
	print_bigChars(value);
	print_operation();
	write(0, "\E[2;2H", 7);
}

void save_memory(int *x, int *y)
{
	char filename[100];
	memset(filename, 0, 100);
	
	rk_mytermregime(0, 0, 1, 1, 1);
	mt_gotoXY(*x, *y);
	write(1, "Input filename: ", 17);
	
	for (int i = 0; ; i++) {
		read(0, &filename[i], 1);
		if (filename[i] == '\n') {
			filename[i] = 0;
			break;
		}
	}
	sc_memorySave(filename);
	*x += 1;
	rk_mytermregime(0, 0, 1, 0, 1);
}

void load_memory(int *x, int *y)
{
	char filename[100];
	memset(filename, 0, 100);
	
	rk_mytermregime(0, 0, 1, 1, 1);
	mt_gotoXY(*x, *y);
	write(1, "Input filename: ", 17);
	
	for (int i = 0; ; i++) {
		read(0, &filename[i], 1);
		if (filename[i] == '\n') {
			filename[i] = 0;
			break;
		}
	}
	sc_memoryLoad(filename);
	int val;
	mt_gotoXY(2, 3);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			sc_memoryGet(i * 10 + j, &val);
			print_memory(val);
		}
		mt_gotoXY(i + 3, 3);
	}
	*x += 1;
	mt_gotoXY(2, 2);
	rk_mytermregime(0, 0, 1, 0, 1);
}

void key_press()
{
	enum keys k = 0;
	int x = 2, y = 2;
	int tx = 24, ty = 1;
	int val;
	sc_regSet(4, 1);
	while (1) {
		int reg;
		sc_regGet(4, &reg);
		if (reg == 0) {
			//print_regFlags();
			signal (SIGALRM, signalhandler);
			alarm(1);
			x = 2;
			y = 2;
		}
		rk_readkey(&k);
		if (k > 0) {
			alarm(0);
			sc_regSet(4, 1);
			//print_regFlags();
		}
		switch (k) {
			case KEYUP:
			x = (x > 1) ? x - 1 : x;
			write(0, "\E[A", 3);
			break;
			case KEYDOWN:
			x = x + 1;
			write(0, "\E[B", 3);
			break;
			case KEYRIGHT:
			y = y + 1;
			write(0, "\E[C", 3);
			break;
			case KEYLEFT:
			y = (y > 1) ? y - 1 : y;
			write(0, "\E[D", 3);
			break;
			case KEYS:
			save_memory(&tx, &ty);
			x = tx; y = ty;
			break;
			case KEYL:
			load_memory(&tx, &ty);
			x = 2; y = 2;
			break;
			//case F2:
			//sc_memoryGet(0, &val);
			//print_bigChars(val);
			break;
			case F5:
			set_accumulator();
			break;
			case F6:
			set_instCounter();
			break;
			case ESC:
			mt_gotoXY(tx, ty);
			return;
			break;
			case ENTER:
			set_memory(&x, &y);
			break;
			case KEYI:
			signal(SIGUSR1, sighandler);
			raise(SIGUSR1);
			x = 2; y = 2;
			break;
			case KEYR:
			sc_regSet(4, 0);
			break;
			case KEYT:
			break;
			default:
			break;
		}
	}
}

void print_keys()
{
	mt_gotoXY(14, 55);
	write(1, "l  - load", 10);
	mt_gotoXY(15, 55);
	write(1, "s  - save", 10);
	mt_gotoXY(16, 55);
	write(1, "r  - run", 9);
	mt_gotoXY(17, 55);
	write(1, "t  - step", 10);
	mt_gotoXY(18, 55);
	write(1, "i  - reset", 11);
	mt_gotoXY(19, 55);
	write(1, "F5 - accumulator", 17);
	mt_gotoXY(20, 55);
	write(1, "F6 - instructionCounter", 24);
}




