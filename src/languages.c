#include "languages.h"

const int MAIN_MENU_T = 4;
const int MENU_T = 8;
const int INVENTORY_T = 5;
const int OPEN_T = 1;
const int DROP_T = 1;
const int MOVE_T = 2;
const int ZONES_T = 5;
const int MAP_MENU_T = 5;
const int NO_MAP_T = 1;
const int NO_TAKE_T = 1;
const int ITEMS_T = 21;
const int CARDS_T = 24;
const int SMALL_CARDS_T = 24;
const int BLOCK_T = 5;
const int FREE_KEY_T = 1;
const int COMPUTER_T = 1;
const int MOBILE_T = 1;
const int ELECTRICAL_T = 3;
const int REQUEST_CHARACTERS_T = 5;

void update_texts()
{
	main_menu_texts = texts;
	menu_texts = main_menu_texts + MAIN_MENU_T;
	inventory_texts = menu_texts + MENU_T;
	open_texts = inventory_texts + INVENTORY_T;
	drop_texts = open_texts + OPEN_T;
	move_texts = drop_texts + DROP_T;
	zones_texts = move_texts + MOVE_T;
	maps_menu_texts = zones_texts + ZONES_T;
	no_map_texts = maps_menu_texts + MAP_MENU_T;
	no_take_texts = no_map_texts + NO_MAP_T;
	items_texts = no_take_texts + NO_TAKE_T;
	cards_texts = items_texts + ITEMS_T;
	small_card_texts = cards_texts + CARDS_T;
	block_texts = small_card_texts + SMALL_CARDS_T;
	free_key_texts = block_texts + BLOCK_T;
	computer_texts = free_key_texts + FREE_KEY_T;
	mobile_texts = computer_texts + COMPUTER_T;
	electrical_texts = mobile_texts + MOBILE_T;
	request_characters_texts = electrical_texts + ELECTRICAL_T;
}