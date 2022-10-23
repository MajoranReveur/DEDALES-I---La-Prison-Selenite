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

char string_load(FILE* file, char** result)
{
	fpos_t position;
	size_t len = 1;
	fgetpos(file, &position);
	char b = fgetc(file);
	while (b == '\'')
	{
		while (b != '\n' && b != EOF)
			b = fgetc(file);
		fgetpos(file, &position);
		b = fgetc(file);
	}
	while (b != '\n' && b != EOF)
	{
		b = fgetc(file);
		len++;
	}
	fsetpos(file, &position);
	char* str = calloc(len, sizeof(char));
	if (!str)
		return 0;
	b = fgetc(file);
	size_t a = 0;
	while (b != '\n' && b != EOF)
	{
		str[a] = b;
		a++;
		b = fgetc(file);
	}
	*result = str;
	return 1;
}

char language_load(char* language, char first_one)
{
    char* file_fields[3] = {
        "res/languages/",
        language,
        ".txt"
    };
    char filename[151] = { 0 };
    concat_str(filename, file_fields, 150, 3);
	FILE* file = fopen(filename, "r");
	char* new_texts[200];
	if (file)
	{
		int a = 0;
		char valid = 1;
		while (a < 200 && valid)
		{
			valid = string_load(file, new_texts + a);
			a++;
		}
		if (valid)
		{
			while (a)
			{
				a--;
				if (!first_one)
					free(texts[a]);
				texts[a] = new_texts[a];
			}
			return 1;
		}
		while (a)
		{
			a--;
			free(new_texts[a]);
		}
	}
	return 0;
}