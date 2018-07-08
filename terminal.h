#ifndef TERMINAL_H
#define TERMINAL_H

int accumulator;
int instructionCounter;

void sighandler(int signo);
void key_press();
void print_keys();
void print_bigChars(int value);

#endif
