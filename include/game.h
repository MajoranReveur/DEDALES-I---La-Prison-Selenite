#pragma once
#include "print.h"
#include "input.h"
#include "files.h"
#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

extern char** main_menu_texts;
extern char** menu_texts;
extern char** inventory_texts;
extern char** open_texts;
extern char** drop_texts;
extern char** move_texts;
extern char** zones_texts;
extern char** maps_menu_texts;
extern char** no_map_texts;
extern char** no_take_texts;
extern char** items_texts;
extern char** cards_texts;
extern char** small_card_texts;
extern char** block_texts;
extern char** free_key_texts;
extern char** computer_texts;
extern char** mobile_texts;
extern char** electrical_texts;
extern char** request_characters_texts;

struct notif
{
	char message[50];
	int len;
	int delay;
};

struct character_location
{
	int x;
	int y;
	int zone;
	int level;
};

//void start_game(char* title, char* author);