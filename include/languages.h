#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include "tools.h"
#include "print.h"

char** main_menu_texts;
char** menu_texts;
char** inventory_texts;
char** open_texts;
char** drop_texts;
char** move_texts;
char** zones_texts;
char** maps_menu_texts;
char** no_map_texts;
char** no_take_texts;
char** cells_texts;
char** items_texts;
char** cards_texts;
char** small_card_texts;
char** block_texts;
char** free_key_texts;
char** computer_texts;
char** mobile_texts;
char** electrical_texts;
char** request_characters_texts;

char* texts[200];

void update_texts();

char language_load(char* language, char first_one);