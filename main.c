#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "mybigchars/mybigchars.h"
#include "mysimpc/mysimpc.h"
#include "myreadkey/myreadkey.h"
#include "terminal.h"

int main()
{
	int value;
	mt_clrscr();
	bc_box(1, 1, 10, 62);
	mt_gotoXY(1, 25);
	write(1, "Memory", 7);
	sc_memoryInit();
	mt_gotoXY(2, 3);

	bc_box(1, 64, 1, 21);
	mt_gotoXY(1, 69);
	write(1, "accumulator", 12);
	bc_box(4, 64, 1, 21);
	mt_gotoXY(4, 66);
	write(1, "instructionCounter", 19);
	bc_box(7, 64, 1, 21);
	mt_gotoXY(7, 71);
	write(1, "Operation", 9);
	bc_box(10, 64, 1, 21);
	mt_gotoXY(10, 73);
	write(1, "Flags", 6);
	bc_box(13, 1, 8, 52);
	bc_box(13, 54, 8, 31);
	mt_gotoXY(13, 55);
	write(1, "Keys:", 6);
	print_keys();
	mt_gotoXY(23, 1);
	write(1, "Input\\Output\n", 13);
	signal(SIGUSR1, sighandler);
	raise(SIGUSR1);
	sc_memoryGet(0, &value);
	print_bigChars(value);
	int j = 0;
	mt_gotoXY(2, 2);
	
	key_press();
	rk_mytermregime(0, 0, 1, 1, 1);
	return 0;
}




