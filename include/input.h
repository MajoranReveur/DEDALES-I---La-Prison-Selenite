#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include "print.h"
#include "options.h"
#include "file_write.h"
#include "game_menu.h"

extern char software_mode;
extern char file_loaded;
extern struct project project_data;

extern char inputs[25];

void default_inputs();
void clean_inputs();
void load_input();
void load_input_long();
void string_input(char* rep, size_t length, char *instruction);
int int_input(int n, int x, int y); //TODO
const char* get_key_name(int i);
void input_editor();

#endif