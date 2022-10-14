#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <io.h>
#include "print.h"
#include "input.h"
#include "tools.h"

extern char* texts[200]; // from languages

struct game
{
	int parameters[35];
	int* requests[5];
	int** maps[5];
	int** items[5];
	int* item_values;
	int** containers;
	int* container_list;
	int item_count[18];
	int portal_entry[4];
	size_t portal_goal;
	int inventories[5][50];
	char modify;
	char** character_maps[5][5];
	int lights[99];
};

void int_writing(FILE* file, int* list, int n);

char map_save(int* map, FILE* filemap);

char int_load(FILE* file, int* result, char end);

char string_load(FILE* file, char** result);

char load_mapss(int** map, FILE* filemap, int zone);

char load_game(struct game* game, char* title, char* author);

char save_game(struct game* game, char* title, char* author);

char free_game(struct game* game);

int file_choice(char* type, char** finaltitle, char** finalauthor);

char language_load(char* language, char first_one);