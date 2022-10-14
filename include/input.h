#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include "print.h"

extern char inputs[10];

void load_input();
void load_input_long();
void string_input(char* rep, size_t length, char *instruction);
int int_input(int n, int x, int y); //TODO

#endif