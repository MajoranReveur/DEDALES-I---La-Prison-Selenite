#include "game.h"

struct game game;
int directions[] = { 0, 1, 0, -1, -1, 0, 1, 0 };
struct notif notifs[10];
char scenes[8] = { 0 };


char map_options[26] = { 0 }; //20 Objets, 5 Persos, 1 pour les Personnages Suppl�mentaires
char map_printable[26] = { 0 };

int zone;
int level;
int* map;
int* map_items;
char* map_shadows;
int current_player;
int current_requests[204];
char seen_requests[204];
struct character_location characters_positions[5]; //Emma's position is updated at each level change
//TODO : positions for added characters : the list should be dynamic.
char playable[4];
int all_characters[999][999];

void dialog(char* message, char* value)
{
	int talker = 0;
	int face = 0;
	rect(0, 604, 704, 100, 0, 0, 0);
	rect(4, 608, 696, 92, 150, 150, 150);
	char text[201] = { 0 };
	int i = 0;
	int j = 0;
	int lines = 0;
	char end = 0;
	while (!end && !inputs[9])
	{
		char special = 0;
		if (message[i] == '*')
		{
			print_text(8, 608 + lines * 40, text, 0, 0);
			lines++;
			while (j)
			{
				j--;
				text[j] = 0;
			}
			special = 1;
		}
		if (message[i] == '&' || !message[i] || lines == 2)
		{
 			if (talker)
			{
				rect(0, 556, 308, 48, 0, 0, 0);
				rect(4, 560, 300, 44, 150, 150, 150);
				if (talker < 6)
					print_text(8, 562, request_characters_texts[talker - 1], 0, 0);
				display_sprite(5, 572, 478, 128, talker - 1, face);
			}
			print_text(8, 608 + lines * 40, text, 0, 0);
			print_refresh();
			inputs[0] = 0;
			inputs[1] = 0;
			while (!inputs[0] && !inputs[1] && !inputs[9])
				load_input();
			inputs[0] = 0;
			inputs[1] = 0;
			while (j)
			{
				j--;
				text[j] = 0;
			}
			rect(0, 604, 704, 100, 0, 0, 0);
			rect(4, 608, 696, 92, 150, 150, 150);
			lines = 0;
			special = 1;
		}
		if (message[i] == '#')
		{
			int k = 0;
			while (value[k] && j < 200)
			{
				text[j] = value[k];
				k++;
				j++;
			}
			special = 1;
		}
		if (message[i] == '{')
		{
			i++;
			int parameter = 0;
			int length = 0;
			char valid = 1;
			while (message[i] && message[i] != '}')
			{
				if (message[i] >= '0' && message[i] <= '9' && length < 4)
				{
					parameter = parameter * 10 + message[i] - '0';
					length++;
				}
				else
					valid = 0;
				i++;
			}
			if (!length || !valid)
				parameter = 0;
			if (parameter > 5)
				parameter = 0;
			talker = parameter;
			special = 1;
		}
		if (message[i] == '[')
		{
			i++;
			int parameter = 0;
			int length = 0;
			char valid = 1;
			while (message[i] && message[i] != ']')
			{
				if (message[i] >= '0' && message[i] <= '9' && length < 4)
				{
					parameter = parameter * 10 + message[i] - '0';
					length++;
				}
				else
					valid = 0;
				i++;
			}
			if (!length || !valid)
				parameter = 0;
			if (parameter > 5)
				parameter = 0;
			face = parameter;
			special = 1;
		}
		if (!special && j < 200)
		{
			text[j] = message[i];
			j++;
		}
		end = !message[i];
		i++;
	}
}

char dialog_choice(char* option1, char* option2)
{
	rect(0, 604, 704, 100, 0, 0, 0);
	rect(4, 608, 696, 92, 150, 150, 150);
	print_text(48, 608, option1, 0, 0);
	print_text(48, 648, option2, 0, 0);
	inputs[0] = 0;
	inputs[1] = 0;
	char i = 0;
	while (!inputs[0] && !inputs[1] && !inputs[9])
	{
		rect(8, 620 + i * 40, 30, 10, 0, 0, 0);
		print_refresh();
		rect(8, 620 + i * 40, 30, 10, 150, 150, 150);
		load_input_long();
		if (inputs[2] || inputs[3])
			i = !i;
	}
	inputs[0] = 0;
	inputs[1] = 0;
	return i;
}

char has_item(int* invent, int type, int value, int quantity)
{
	int i = 0;
	int toFound = type;
	if (type == 16)
		toFound = 7;
	if (type == 14)
		toFound = 4;
	if (type == 15)
		toFound = 18;
	int found = 0;
	while (i < 50)
	{
		if (game.item_values[invent[i] * 3 - 3] == toFound)
		{
			if (game.item_values[invent[i] * 3 - 3] == type)
			{
				if (toFound == 4)
				{
					int* box = game.containers[game.item_values[invent[i] * 3 - 1]];
					int j = 0;
					char valid = 1;
					while (j < 23)
					{
						valid = valid && (box[j] != 0) == value;
						j++;
					}
					if (valid)
						found++;
				}
				else
					found++;
			}
			else
			{
				int* box = game.containers[game.item_values[invent[i] * 3 - 1]];
				if (value == 0)
				{
					int max = 99 * (type != 14) + 23 * (type == 14);
					int j = 0;
					while (j < max)
					{
						if (box[j])
							found++;
						j++;
					}
				}
				else
				{
					if (type == 14)
						found += (box[value - 1] != 0);
					else
					{
						int j = 0;
						while (j < 99)
						{
							if (box[j])
							{
								if (value == game.item_values[box[j] * 3 - 2])
									found++;
							}
							j++;
						}
					}
				}
			}
		}
		i++;
	}
	return found >= quantity;
}

int drop_item(int* invent, int type, int value)
{
	int i = 0;
	int toFound = type;
	if (type == 16)
		toFound = 7;
	if (type == 14)
		toFound = 4;
	if (type == 15)
		toFound = 18;
	while (i < 50)
	{
		if (game.item_values[invent[i] * 3 - 3] == toFound)
		{
			if (game.item_values[invent[i] * 3 - 3] == type)
			{
				if (toFound == 14)
				{
					int* box = game.containers[game.item_values[invent[i] * 3 - 1]];
					int j = 0;
					char valid = 1;
					while (j < 23)
					{
						valid = valid && (box[j] != 0) == value;
						j++;
					}
					if (valid)
					{
						int r = invent[i];
						invent[i] = 0;
						return r;
					}
				}
				else
				{
					int r = invent[i];
					invent[i] = 0;
					return r;
				}
			}
			else
			{
				int* box = game.containers[game.item_values[invent[i] * 3 - 1]];
				if (value == 0)
				{
					int max = 99 * (type != 14) + 23 * (type == 14);
					int j = 0;
					while (j < max)
					{
						if (box[j])
						{
							int r = box[j];
							box[j] = 0;
							return r;
						}
						j++;
					}
				}
				else
				{
					if (type == 14)
					{
						if (box[value - 1])
						{
							int r = box[value - 1];
							box[value - 1] = 0;
							return r;
						}
					}
					else
					{
						int j = 0;
						while (j < 99)
						{
							if (box[j])
							{
								if (value == game.item_values[box[j] * 3 - 2])
								{
									box[j] = 0;
								}
							}
							j++;
						}
					}
				}
			}
		}
		i++;
	}
	return 0;
}

void card_menu(int* box)
{
	rect(46, 46, 640, 358, 0, 0, 0);
	rect(50, 50, 632, 232, 100, 100, 100);
	int i = 0;
	while (i < 8)
	{
		int j = 0;
		while (j < 3)
		{
			if (i + j * 8 < 23 && box[i + j * 8])
				display_cardsprite(54 + i * 76, 54 + j * 76, i + 8 * j + 1, 0);
			j++;
		}
		i++;
	}
	inputs[0] = 0;
	int x = 0;
	int y = 0;
	while (!inputs[0] && !inputs[9])
	{
		rect(50, 286, 632, 114, 100, 100, 100);
		if (x + 8 * y < 23 && box[x + 8 * y])
			print_text(54, 290, cards_texts[x + 8 * y + 1], 0, 0);
		else
			print_text(54, 290, cards_texts[0], 0, 0);
		cursor(54 + x * 76, 54 + y * 76, 32, 64, 255, 0, 0);
		print_refresh();
		load_input_long();
		cursor(54 + x * 76, 54 + y * 76, 32, 64, 100, 100, 100);
		if (x + 8 * y < 23 && box[x + 8 * y])
			display_cardsprite(54 + x * 76, 54 + y * 76, x + 8 * y + 1, 0);
		if (inputs[3] && y + 1 < 3)
			y++;
		if (inputs[2] && y > 0)
			y--;
		if (inputs[5] && x + 1 < 8)
			x++;
		if (inputs[4] && x > 0)
			x--;
	}
}

void map_view(int zone, int level, int character, int px, int py, int* visible, int pzone, int plevel)
{
	size_t visible_items[20] = { 0 };
	size_t len = game.maps[zone][level][0] * game.maps[zone][level][1];
	while (len)
	{
		len--;
		if (game.items[zone][level][len] && game.character_maps[character][zone][level][len + 1])
			visible_items[game.item_values[game.items[zone][level][len] * 3 - 3] - 1]++;
	}
	if (!visible[0])
		visible_items[2] = 0;
	if (!visible[1])
		visible_items[15] = 0;
	int modes[22] = { 0 };
	if (pzone == zone && plevel == level)
		modes[1] = 21;
	int i = 0;
	int j = 1 + (pzone == zone && plevel == level);
	while (i < 20)
	{
		if (visible_items[i])
		{
			modes[j] = i + 1;
			j++;
		}
		i++;
	}
	int mode = 0;
	inputs[0] = 0;
	inputs[6] = 0;
	int x = 0;
	int y = -3;
	int min = 0;
	while (!inputs[9] && !inputs[1] && !inputs[6])
	{
		display_littlemap(x, y, game.maps[zone][level], game.character_maps[character][zone][level], game.items[zone][level], game.item_values, modes[mode]);
		rect(0, 0, 704, 48, 150, 150, 150);
		print_text(4, 4, maps_menu_texts[0], 0, 0);
		if (modes[mode] < 21)
			print_text(300, 4, items_texts[modes[mode]], 0, 0);
		else
		{
			print_text(300, 4, maps_menu_texts[2], 0, 0);
			rect((px-x) * 16, (py-y) * 16, 16, 16, 255, 0, 255);
		}
		print_refresh();
		load_input_long();
		if (inputs[2])
			y--;
		if (inputs[3])
			y++;
		if (inputs[4])
			x--;
		if (inputs[5])
			x++;
		if (inputs[0])
		{
			inputs[0] = 0;
			rect_alpha(0, 0, 704, 704, 0, 0, 0, 150);
			while (!inputs[0] && !inputs[9] && !inputs[1] && !inputs[6])
			{
				rect(0, 0, 400, 704, 100, 100, 100);
				int a = 0;
				while (a < 13 && min + a < j)
				{
					if (modes[min + a] == 21)
						print_text(44, 100 + 40 * a, request_characters_texts[0], 0, 0);
					else
						print_text(44, 100 + 40 * a, items_texts[modes[min + a]], 0, 0);
					a++;
				}
				rect(4, 118 + 40 * (mode - min), 30, 10, 0, 0, 0);
				print_refresh();
				rect(4, 118 + 40 * (mode - min), 30, 10, 100, 100, 100);
				load_input_long();
				if (inputs[2])
				{
					if (mode)
					{
						mode--;
						if (min)
							min--;
					}
				}
				if (inputs[3])
				{
					if (mode + 1 < j)
					{
						mode++;
						if (min + 11 < mode)
							min = mode - 11;
					}
				}
			}

		}
	}
}

void map_menu(int character, int px, int py, int* visible, int pzone, int plevel)
{
	int zones[10] = { 0 };
	int i = 0;
	while (i < 5)
	{
		int k = 0;
		while (k < game.parameters[25 + i])
		{
			if (game.character_maps[character][i][k][0])
			{
				zones[i + 5] = k + 1;
				if (!zones[i])
					zones[i] = k + 1;
			}
			k++;
		}
		i++;
	}
	i = 0;
	int zone = 0;
	while (zone < 5 && !zones[zone])
		zone++;
	if (zone == 5)
	{
		dialog(no_map_texts[0], "");
		return;
	}
	int level = 0;
	int min = 0;
	while (!inputs[9] && !inputs[1] && !inputs[6])
	{
		rect(0, 0, 704, 704, 0, 0, 0);
		if (game.character_maps[character][zone][zones[zone] + level - 1][0])
			display_minimap(0, 0, game.maps[zone][zones[zone] + level - 1], game.character_maps[character][zone][zones[zone] + level - 1], 0, 104);
		rect(4, 4, 696, 96, 150, 150, 150);
		//rect(0, 104, 600, 600, 250, 0, 0);
		rect(604, 108, 96, 596, 150, 150, 150);
		print_text_centered(0, 8, zones_texts[zone], 704, 0, 0);
		int i = 0;
		while (i < 13 && zones[zone] + i + min <= zones[zone + 5])
		{
			if (game.character_maps[character][zone][zones[zone] + i + min - 1][0])
				print_int(642, 116 + 40 * i, zones[zone] + i + min, 2, 0, 0);
			else
				print_text(642, 116 + 40 * i, maps_menu_texts[1], 0, 0);
			i++;
		}
		rect(608, 130 + 40 * (level - min), 30, 10, 0, 0, 0);
		print_refresh();
		rect(608, 130 + 40 * (level - min), 30, 10, 150, 150, 150);
		load_input_long();
		if (inputs[2])
		{
			if (level)
			{
				level--;
				if (min)
					min--;
			}
			else
			{
				level = zones[zone + 5] - zones[zone];
				min = level - 11;
				if (min < 0)
					min = 0;
			}
		}
		if (inputs[3])
		{
			if (zones[zone] + level < zones[zone + 5])
			{
				level++;
				if (min + 11 < level)
					min = level - 11;
			}
			else
			{
				level = 0;
				min = 0;
			}
		}
		if (inputs[4])
		{
			zone = (zone + 4) % 5;
			while (!zones[zone])
				zone = (zone + 4) % 5;
		}
		if (inputs[5])
		{
			zone = (zone + 1) % 5;
			while (!zones[zone])
				zone = (zone + 1) % 5;
		}
		if (inputs[0])
		{
			map_view(zone, zones[zone] + level - 1, character, px, py, visible, pzone, plevel);
			inputs[6] = 0;
		}
	}
}

void key_menu(int* container)
{
	int x = 0;
	int j = 0;
	char title[200] = { 0 };
	char number[20] = { 0 };
	char* str[] = { items_texts[7], " (", number, "/99)" };
	while (!inputs[9] && !inputs[6] && !inputs[1])
	{
		inputs[0] = 0;
		rect(0, 0, 704, 704, 0, 0, 0);
		rect(4, 100, 296, 600, 100, 100, 100);
		rect(300, 100, 400, 600, 100, 100, 100);
		rect(4, 4, 696, 92, 100, 100, 100);
		rect(284, 100, 12, 594, 150, 150, 150);
		rect(284, 100 + 6 * x, 12, 78, 50, 50, 50);
		int i = 0;
		int total = 0;
		while (i < 99)
		{
			if (container[i])
				total++;
			i++;
		}
		int_to_str(number, total, 2);
		concat_str(title, str, 200, 4);
		print_text_centered(0, 20, title, 704, 0, 0);
		i = 0;
		while (i < 13)
		{
			if (container[x + i])
				print_int(100, 104 + i * 40, game.item_values[3 * container[x + i] - 2], 2, 0, 0);
			else
				print_text(100, 104 + i * 40, "--", 0, 0);
			i++;
		}
		rect(8, 112 + j * 40, 30, 10, 0, 0, 0);
		print_refresh();
		load_input_long();
		if (inputs[3])
		{
			if (j < 12)
				j++;
			else
			{
				if (x + j < 98)
					x++;
				else
				{
					x = 0;
					j = 0;
				}
			}
		}
		if (inputs[2])
		{
			if (j > 0)
				j--;
			else
			{
				if (x > 0)
					x--;
				else
				{
					x = 86;
					j = 12;
				}
			}
		}
	}
}

void pumpkin_menu(int* container)
{
	int x = 0;
	int j = 0;
	char title[200] = { 0 };
	char number[20] = { 0 };
	char* str[] = { items_texts[18], " (", number, "/99)" };
	while (!inputs[9] && !inputs[6] && !inputs[1])
	{
		inputs[0] = 0;
		rect(0, 0, 704, 704, 0, 0, 0);
		rect(4, 100, 296, 600, 100, 100, 100);
		rect(300, 100, 400, 600, 100, 100, 100);
		rect(4, 4, 696, 92, 100, 100, 100);
		rect(284, 100, 12, 594, 150, 150, 150);
		rect(284, 100 + 6 * x, 12, 78, 50, 50, 50);
		int i = 0;
		int total = 0;
		while (i < 99)
		{
			if (container[i])
				total++;
			i++;
		}
		int_to_str(number, total, 2);
		concat_str(title, str, 200, 4);
		print_text_centered(0, 20, title, 704, 0, 0);
		while (i < 13)
		{
			if (container[x + i])
				print_int(100, 104 + i * 40, game.item_values[3 * container[x + i] - 2], 2, 0, 0);
			else
				print_text(100, 104 + i * 40, "--", 0, 0);
			i++;
		}
		rect(8, 112 + j * 40, 30, 10, 0, 0, 0);
		print_refresh();
		load_input_long();
		if (inputs[3])
		{
			if (j < 12)
				j++;
			else
			{
				if (x + j < 98)
					x++;
				else
				{
					x = 0;
					j = 0;
				}
			}
		}
		if (inputs[2])
		{
			if (j > 0)
				j--;
			else
			{
				if (x > 0)
					x--;
				else
				{
					x = 86;
					j = 12;
				}
			}
		}
	}
}

void inventory(int* invent)
{
	char title[200] = { 0 };
	char number[20] = { 0 };
	char* str[] = { inventory_texts[0], " (", number, "/50)" };
	while (!inputs[9] && !inputs[6] && !inputs[1])
	{
		rect(0, 0, 704, 704, 0, 0, 0);
		rect(4, 100, 696, 348, 100, 100, 100);
		rect(4, 4, 696, 92, 100, 100, 100);
		int i = 0;
		int total = 0;
		while (i < 10)
		{
			int j = 0;
			while (j < 5)
			{
				if (invent[j * 10 + i])
				{
					display_sprite(3, 4 + i * 70, 104 + j * 70, 64, game.item_values[3 * invent[j * 10 + i] - 3] - 1, 0);
					total++;
				}
				j++;
			}
			i++;
		}
		int_to_str(number, total, 2);
		concat_str(title, str, 200, 4);
		print_text_centered(0, 20, title, 704, 0, 0);
		inputs[0] = 0;
		int x = 0;
		int y = 0;
		while (!inputs[1] && !inputs[9] && !inputs[6] && !inputs[0])
		{
			rect(4, 452, 696, 248, 100, 100, 100);
			if (invent[y * 10 + x])
				print_text(8, 456, items_texts[game.item_values[3 * invent[y * 10 + x] - 3]], 0, 0);
			else
				print_text(8, 456, items_texts[0], 0, 0);
			cursor(4 + x * 70, 104 + y * 70, 64, 64, 255, 0, 0);
			print_refresh();
			load_input_long();
			cursor(4 + x * 70, 104 + y * 70, 64, 64, 100, 100, 100);
			rect(4 + x * 70, 104 + y * 70, 64, 64, 100, 100, 100);
			if (invent[y * 10 + x])
				display_sprite(3, 4 + x * 70, 104 + y * 70, 64, game.item_values[3 * invent[y * 10 + x] - 3] - 1, 0);
			if (inputs[3] && y + 1 < 5)
				y++;
			if (inputs[2] && y > 0)
				y--;
			if (inputs[5] && x + 1 < 10)
				x++;
			if (inputs[4] && x > 0)
				x--;
			if (inputs[0] && invent[y * 10 + x])
			{
				rect(4, 496, 696, 204, 100, 100, 100);
				print_text(48, 496, inventory_texts[1], 0, 0);
				print_text(48, 536, inventory_texts[2], 0, 0);
				print_text(48, 576, inventory_texts[3], 0, 0);
				print_text(48, 616, inventory_texts[4], 0, 0);
				print_refresh();
				inputs[0] = 0;
				i = 0;
				while (!inputs[0] && !inputs[9] && !inputs[1] && !inputs[6])
				{
					rect(8, 512 + i * 40, 30, 10, 0, 0, 0);
					print_refresh();
					load_input_long();
					rect(8, 512 + i * 40, 30, 10, 100, 100, 100);
					if (inputs[2])
						i--;
					if (inputs[3])
						i++;
					if (i < 0)
						i = 3;
					if (i >= 4)
						i = 0;
				}
				if (inputs[0])
				{
					int item_type = game.item_values[3 * invent[y * 10 + x] - 3];
					if (i == 0)
					{
						char found = 0;
						if (item_type == 7)
						{
							key_menu(game.containers[game.item_values[invent[y * 10 + x] * 3 - 1]]);
							found = 1;
						}
						if (item_type == 18)
						{
							pumpkin_menu(game.containers[game.item_values[invent[y * 10 + x] * 3 - 1]]);
							found = 1;
						}
						if (item_type == 4)
						{
							card_menu(game.containers[game.item_values[invent[y * 10 + x] * 3 - 1]]);
							found = 1;
						}
						if (!found)
							dialog(*open_texts, "");
						inputs[6] = 0;
					}
					inputs[0] = 1;
				}
			}
		}
	}
}

int menu(int zone, int level, int x, int y, int dir, int* map, int delay, int* visible_items, int character, int* requests, int luminosity)
{
	int i = 0;
	inputs[1] = 0;
	char* options[] = { menu_texts[1], menu_texts[2], menu_texts[3], menu_texts[4], menu_texts[5], menu_texts[6] };
	int len = 0;
	while (i < 8)
	{
		int l = 2;
		while (menu_texts[i][l])
			l++;
		if (len < l)
			len = l;
		i++;
	}
	len = len * 30 + 10;
	while (i != 1 && !inputs[9] && !inputs[1])
	{
		display_map(x * 8 - delay * directions[dir * 2], y * 8 - delay * directions[dir * 2 + 1], map, luminosity, game.character_maps[character][zone][level], game.items[zone][level], game.item_values, visible_items, requests);
		print_characters(x, y, requests, delay * directions[dir * 2], delay * directions[dir * 2 + 1], 5, game.character_maps[character][zone][level], map[0]);
		display_sprite(4, 5 * 64, 5 * 64, 64, dir, 0);
		rect_alpha(0, 0, 704, 704, 0, 0, 0, 150);
		rect(0, 0, len, 704, 100, 100, 100);
		print_text_centered(0, 20, menu_texts[0], len, 0, 0);
		rect(0, 100, len, 704, 150, 150, 150);
		i = 0;
		while (i < 6)
		{
			print_text(20, 150 + i * 40, options[i], 0, 0);
			i++;
		}
		i = 0;
		inputs[0] = 0;
		while (!inputs[0] && !inputs[9] && !inputs[1])
		{
			rect(5, 155 + i * 40, 10, 30, 0, 0, 0);
			print_refresh();
			load_input_long();
			rect(5, 155 + i * 40, 10, 30, 150, 150, 150);
			if (inputs[2])
				i--;
			if (inputs[3])
				i++;
			if (i < 0)
				i = 5;
			if (i >= 6)
				i = 0;
		}
		i++;
		if (i == 6)
			inputs[9] = 1;
		if (i == 2)
			inventory(game.inventories[character]);
		if (i == 3)
			map_menu(character, x, y, visible_items, zone, level);
		if (i == 4 && zone == 1)
			return 1;
	}
	inputs[1] = 0;
	return 0;
}

void add_notif(char* m)
{
	int j = 0;
	int min = 100;
	int j_min = 0;
	while (j < 10)
	{
		if (notifs[j].delay < min)
		{
			j_min = j;
			min = notifs[j].delay;
		}
		j++;
	}
	int i = 0;
	notifs[j_min].len = 0;
	notifs[j_min].delay = 100;
	while (i < 49 && m[i])
	{
		notifs[j_min].message[i] = m[i];
		notifs[j_min].len++;
		i++;
	}
	notifs[j_min].message[i] = 0;
	return;
}

void print_notif()
{ 
	int i = 0;
	int j = 0;
	while (i < 10)
	{
		if (notifs[i].delay)
		{
			rect(0, 40 * j, notifs[i].len * 30, 40, 150 + 100 * (j % 2), 150 + 100 * (j % 2), 150 + 100 * ((j + 1) % 2));
			print_text(0, 40 * j, notifs[i].message, 0, 0);
			notifs[i].delay--;
			j++;                                                                                                                                                                                                                                                            		}
		i++;
	}
}

void load_key(int* map, size_t len)
{
	game.portal_goal = 0;
	size_t i = 2;
	while (i < len)
	{
		if (map[i] == 8 || map[i] == 7)
		{
			game.portal_goal++;
			map[i] = 7;
		}
		i++;
	}
}

char change_map(int** map, int* z, int* l, int* x, int* y, int zone, int level, int newx, int newy, int* requests, int character)
{
	*map = game.maps[zone][level];
	if (zone == 1)
	{
		size_t len = (*map)[0] * (*map)[1] + 2;
		load_key(*map, len);
		game.portal_entry[0] = *z;
		game.portal_entry[1] = *l;
		game.portal_entry[2] = *x;
		game.portal_entry[3] = *y;
		*x = (*map)[len] - 1;
		*y = (*map)[len + 1] - 1;
	}
	else
	{
		*x = newx;
		*y = newy;
	}
	*z = zone;
	*l = level;
	game.modify = 1;
	int i = 0;
	while (i < 5)
	{
		int j = 0;
		char found = i == character;
		requests[i * 3] = 0;
		requests[i * 3 + 1] = 0;
		requests[i * 3 + 2] = 0;
		while (j < game.parameters[30 + i] && !found)
		{
			int* coord = game.requests[i] + j * 10;
			if (coord[0])
				found = (i != 2);
			if (coord[0] == zone + 1 && coord[1] == level + 1)
			{
				found = 1;
				requests[i * 3] = coord[2];
				requests[i * 3 + 1] = coord[3];
				requests[i * 3 + 2] = j + 1;
			}
			j++;
		}
		if (!found && game.parameters[4 + 4 * i] == zone + 1 && game.parameters[5 + 4 * i] == level + 1)
		{
			requests[i * 3] = game.parameters[6 + 4 * i];
			requests[i * 3 + 1] = game.parameters[7 + 4 * i];
			requests[i * 3 + 2] = 0;
		}
		i++;
	}
	return 1;
}

char take(int* invent, int item, int* visible_items, int level)
{
	char message[50] = { 0 };
	int* coord = game.item_values + item * 3 - 3;
	int item_type = coord[0];
	if (item_type == 4 || item_type == 5 || item_type == 6 ||
		item_type == 7 || item_type == 9 || item_type == 10 ||
		item_type == 11 || item_type == 12 || item_type == 13 ||
		item_type == 18)
	{
		int i = 0;
		while (i < 50)
		{
			if (!invent[i])
			{
				invent[i] = item;
				if (item_type == 7)
					visible_items[1]++;
				char* strs[] = { "+1 ", items_texts[item_type], " !" };
				concat_str(message, strs, 50, 3);
				add_notif(message);
				return 1;
			}
			i++;
		}
	}
	int container_type = 0;
	if (item_type == 15)
	{
		container_type = 18;
		if (coord[1] == 0)
			coord[1] = level + 1;
	}
	if (item_type == 16)
	{
		if (visible_items[1])
			container_type = 7;
		else
			return 0;
	}
	if (container_type)
	{
		int i = 0;
		while (i < 50)
		{
			if (invent[i] && game.item_values[invent[i] * 3 - 3] == container_type)
			{
				int j = 0;
				int* box = game.containers[game.item_values[invent[i] * 3 - 1]];
				while (j < 99)
				{
					if (!box[j])
					{
						box[j] = item;
						char number[20];
						int_to_str(number, coord[1], 2);
						char* strs[] = { "+1 ", items_texts[item_type], " (", number, ") !" };
						concat_str(message, strs, 50, 5);
						add_notif(message);
						return 1;
					}
					j++;
				}
			}
			i++;
		}
	}
	if (item_type == 14)
	{
		int i = 0;
		while (i < 50)
		{
			if (invent[i] && game.item_values[invent[i] * 3 - 3] == 4)
			{
				int* cards = game.containers[game.item_values[invent[i] * 3 - 1]];
				if (!cards[coord[1] - 1])
				{
					cards[coord[1] - 1] = item;
					char message[50] = { 0 };
					char* strs[] = { "+1 ", small_card_texts[coord[1]], " !" };
					concat_str(message, strs, 50, 3);
					add_notif(message);
					return 1;
				}
			}
			i++;
		}
	}
	add_notif(*no_take_texts);
	return 0;
}

int is_there_character(int* requests, int x, int y)
{
	int i = 0;
	while (i < 5)
	{
		if (requests[i * 3] - 1 == x && requests[i * 3 + 1] - 1 == y)
			return i + 1;
		i++;
	}
	return 0;
}

char handle_request(int player, int npc, int* request, int* requests)
{
	char achieved = 0;
	if (request[4] < 2)
	{/*
		dialog_name("Ah, je te cherchais !", "Saihtam");
		dialog_name("En principe, tu devrais\npouvoir passer...", "Saihtam");
		dialog_name("Sauf que c'est la fin\nde la demo ici.", "Saihtam");
		dialog_name("Donc sois tu retournes\nen bas...", "Saihtam");
		dialog_name("Sois tu quittes le jeu.", "Saihtam");*/
		achieved = 0;
	}
	if (request[4] > 2)
	{
		if (request[4] == 3)
			dialog("Est-ce que tu peux me\nmontrer un truc ?", "");
		else
			dialog("J'aimerais bien mettre\nla main sur un truc...", "");
		char str[200] = { 0 };
		size_t a = copy_str(str, "Tu as deja vu un ", 200);
		a+= copy_str(str + a, items_texts[request[7]], 200 - a);
		a += copy_str(str + a, " ?", 200 - a);
		dialog(str, "");
		a = copy_str(str, "Il m'en faudrait ", 200);
		if (!request[9])
			request[9] = 1;
		if (request[9] > 10)
		{
			str[a] = (request[9] / 10) % 10 + 48;
			a++;
		}
		str[a] = request[9] % 10 + 48;
		a++;
		if (request[7] == 7 || request[7] == 18 || request[7] == 4)
		{
			if (!request[8])
				a += copy_str(str + a, "\nvide", 200 - a);
			else
				a += copy_str(str + a, "\nplein", 200 - a);
		}
		else
		{
			if (request[7] == 9)
			{
				if (!request[8])
					a += copy_str(str + a, "\neteint", 200 - a);
				else
					a += copy_str(str + a, "\nallume", 200 - a);
			}
			if (request[7] == 10)
			{
				if (!request[8])
					a += copy_str(str + a, "\neteinte", 200 - a);
				else
					a += copy_str(str + a, "\nallumee", 200 - a);
			}
		}
		if ((request[7] == 7 || request[7] == 9 || request[7] == 18 || request[7] == 4 || request[7] == 10) && request[9] > 1)
			a += copy_str(str + a, "s", 200 - a);
		if (request[8] && (request[7] == 15 || request[7] == 16 || request[7] == 14))
		{
			a += copy_str(str + a, "\nde type ", 200 - a);
			if (request[7] == 14)
				a += copy_str(str + a, small_card_texts[request[8]], 200 - a);
			else
			{
				if (request[8] > 10)
				{
					str[a] = (request[8] / 10) % 10 + 48;
					a++;
				}
				str[a] = request[8] % 10 + 48;
				a++;
			}
		}
		a += copy_str(str + a, ".", 200 - a);
		str[a] = 0;
		dialog(str, "");
		if (has_item(game.inventories[player], request[7], request[8], request[9]))
		{
			if (request[4] == 3)
				achieved = dialog_choice("Montrer", "Ne rien faire") == 0;
			else
			{
				achieved = dialog_choice("Donner", "Ne rien faire") == 0;
				if (achieved)
				{
					int i = 0;
					while (i < request[9])
					{
						drop_item(game.inventories[player], request[7], request[8]);
						i++;
					}
				}
			}
		}
	}
	if (achieved)
	{
		achieved = 1;
		if (request[5])
		{
			dialog("Tiens, voici pour toi.", "");
			if (request[5] == 8)
			{
				dialog("Une carte de l'etage !", "");
				game.character_maps[player][request[0] - 1][request[1] - 1][0] = 1;
			}
			if (request[5] == 13)
			{
				int i = 0;
				while (game.item_values[i * 3] != 13)
					i++;
				take(game.inventories[player], i + 1, NULL, 1);
				dialog("Une pile !", "");
				achieved = 2;
			}
		}
		if (npc == 4)
			achieved = 3;
		if (achieved == 1)
			dialog("Allez, a plus tard !", "");
		request[0] = 0;
	}
	return achieved;
}

void update_map(int x, int y, int* map, char* character_map)
{
	size_t cell = 1 + x + y * map[0];
	character_map[cell] = 2;
	if (x && !character_map[cell - 1])
		character_map[cell - 1] = (map[cell - 1 + 1] == 2) + 1;
	if (y && !character_map[cell - map[0]])
		character_map[cell - map[0]] = (map[cell - map[0] + 1] == 2) + 1;
	if (x < map[0] && !character_map[cell + 1])
		character_map[cell + 1] = (map[cell + 1 + 1] == 2) + 1;
	if (y < map[1] && !character_map[cell + map[0]])
		character_map[cell + map[0]] = (map[cell + map[0] + 1] == 2) + 1;
}

void move_character(int* cx, int* cy, int zone, int level, int dirc, int character, int* requests, int player, int luminosity, int dir, int* visible_items)
{
	int x = *cx;
	int y = *cy;
	int i = 0;
	int xp = 0;
	int yp = 0;
	if (player != character)
	{
		xp = requests[character * 3] - 1;
		yp = requests[character * 3 + 1] - 1;
		requests[character * 3] = 0;
		requests[character * 3 + 1] = 0;
	}
	while (i < 8)
	{
		if (player == character)
		{
			display_map(x * 8 + i * directions[dirc * 2], y * 8 + i * directions[dirc * 2 + 1], game.maps[zone][level], luminosity, game.character_maps[player][zone][level], game.items[zone][level], game.item_values, visible_items, requests);
			print_characters(x, y, requests, -i * directions[dirc * 2], -i * directions[dirc * 2 + 1], luminosity, game.character_maps[player][zone][level], game.maps[zone][level][0]);
			display_sprite(4, 5 * 64, 5 * 64, 64, dirc, i);
		}
		else
		{
			display_map(x * 8, y * 8, game.maps[zone][level], luminosity, game.character_maps[player][zone][level], game.items[zone][level], game.item_values, visible_items, requests);
			print_characters(x, y, requests, 0, 0, luminosity, game.character_maps[player][zone][level], game.maps[zone][level][0]);
			display_sprite(4, 5 * 64, 5 * 64, 64, dir, 0);
			display_sprite(4, 5 * 64 + (xp - x) * 64 + i * directions[dirc * 2] * 8,
				5 * 64 + (yp - y) * 64 + i * directions[dirc * 2 + 1] * 8,
				64, character * 4 + dirc, i);
		}
		i++;
		print_refresh();
		load_input();
	}
	if (player != character)
	{
		requests[character * 3] = xp + directions[dirc * 2] + 1;
		requests[character * 3 + 1] = yp + directions[dirc * 2 + 1] + 1;
	}
	else
	{
		*cx = x + directions[dirc * 2];
		*cy = y + directions[dirc * 2 + 1];
	}
}
/*
void scene_1(int* x, int* y, int* requests, int luminosity, int dir, int* visible_items)
{
	update_map(*x, *y, game.maps[0][0], game.character_maps[0][0][0]);
	display_map(*x * 8, *y * 8, game.maps[0][0], luminosity, game.character_maps[0][0][0], game.items[0][0], game.item_values, visible_items, requests);
	display_sprite(4, 5 * 64, 5 * 64, 64, 0, 0);
	display_sprite(4, 5 * 64, 6 * 64, 64, 5, 0);
	requests[3] = 11;
	requests[4] = 14;
	dialog_name("AAAAAH !", "???");
	dialog_name("AAAAAH !", "Nihil");
	dialog_name("Mais ca va pas de\nsurgir comme ca ?!", "???");
	dialog_name("C'est toi qui m'a\nfait peur !", "Nihil");
	dialog_name("...", "???");
	dialog_name("...", "Nihil");
	dialog_name("Je m'appelle Sarah.", "Sarah");
	dialog_name("OK. Cool.", "Nihil");
	dialog_name("...", "Sarah");
	dialog_name("En principe, tu dois\ndire qui t'es, la.", "Sarah");
	dialog_name("Euh... Nihil.\nJe crois.", "Nihil");
	dialog_name("Toi aussi, tu es \ncoince ici ?", "Sarah");
	dialog_name("Oui, tu sais ou\non est ?", "Nihil");
	dialog_name("Dans un labyrinthe.", "Sarah");
	dialog_name("Oui, mais comment on\nen sort ?", "Nihil");
	dialog_name("...", "Sarah");
	dialog_name("Viens voir.", "Sarah");
	move_character(x, y, 0, 0, 0, 1, requests, 0, luminosity, dir, visible_items);
	requests[3] = 12;
	requests[4] = 17;
	move_character(x, y, 0, 0, 0, 0, requests, 0, luminosity, dir, visible_items);
	update_map(*x, *y, game.maps[0][0], game.character_maps[0][0][0]);
	move_character(x, y, 0, 0, 0, 0, requests, 0, luminosity, dir, visible_items);
	update_map(*x, *y, game.maps[0][0], game.character_maps[0][0][0]);
	move_character(x, y, 0, 0, 3, 0, requests, 0, luminosity, dir, visible_items);
	update_map(*x, *y, game.maps[0][0], game.character_maps[0][0][0]);
	move_character(x, y, 0, 0, 3, 0, requests, 0, luminosity, dir, visible_items);
	update_map(*x, *y, game.maps[0][0], game.character_maps[0][0][0]);
	move_character(x, y, 0, 0, 0, 0, requests, 0, luminosity, dir, visible_items);
	update_map(*x, *y, game.maps[0][0], game.character_maps[0][0][0]);
	move_character(x, y, 0, 0, 0, 0, requests, 0, luminosity, dir, visible_items);
	int i = 0;
	while (i < 6)
	{
		int j = 0;
		while (j < 3)
		{
			update_map(*x - i, *y + j, game.maps[0][0], game.character_maps[0][0][0]);
			j++;
		}
		i++;
	}
	display_map(*x * 8, *y * 8, game.maps[0][0], luminosity, game.character_maps[0][0][0], game.items[0][0], game.item_values, visible_items, requests);
	display_sprite(4, 5 * 64, 5 * 64, 64, 2, 0);
	display_sprite(4, 4 * 64, 5 * 64, 64, 7, 0);
	dialog_name("Voici la Chambre !", "Sarah");
	dialog_name("Y a pas de lit ?", "Nihil");
	dialog_name("C'est pas moi qui ait\nchoisi le nom !", "Sarah");
	dialog_name("Qui, alors ?", "Nihil");
	dialog_name("Le type en noir.", "Sarah");
	dialog_name("Donc on n'est pas\ntous seuls ?", "Nihil");
	dialog_name("On est\ncombien ?", "Nihil");
	dialog_name("Aucune idee.", "Sarah");
	dialog_name("Mais ce type ne\ncompte pas.", "Sarah");
	dialog_name("Euh ? Pourquoi ?", "Nihil");
	dialog_name("Tu comprendras quand\ntu le verras.", "Sarah");
	dialog_name("OK ...?", "Nihil");
	dialog_name("Il m'a aussi dit que\npour sortir d'ici,", "Sarah");
	dialog_name("je devais recuperer\ndes cartes de tarot.", "Sarah");
	dialog_name("...", "Nihil");
	dialog_name("Des cartes de tarot ?", "Nihil");
	dialog_name("Me regarde pas comme\nca !", "Sarah");
	dialog_name("Et tu en as combien ?", "Nihil");
	dialog_name("Zero.", "Sarah");
	dialog_name("Ah.", "Nihil");
	dialog_name("J'ai pas pu aller\nbien loin...", "Sarah");
	dialog_name("Il y a des sceaux\npartout, ici.", "Sarah");
	dialog_name("Je peux me charger\nd'eux !", "Nihil");
	dialog_name("C'est vrai ?!", "Sarah");
	dialog_name("OK, dans ce cas...", "Sarah");
	dialog_name("Je t'attend ici, alors!", "Sarah");
	dialog_name("Hein ?", "Nihil");
	dialog_name("Bah quoi ?", "Sarah");
	dialog_name("Si je pars, ce sera\nplus dur de se", "Sarah");
	dialog_name("retrouver apres...", "Sarah");
	dialog_name("Mais on peut utiliser\nla Chambre comme", "Sarah");
	dialog_name("point de repere.", "Sarah");
	dialog_name("Ah, je vois...", "Nihil");
	dialog_name("Tiens, tu en auras\nbesoin.", "Sarah");
	dialog_name("Hmm ?", "Nihil");
	dialog_name("Un paquet de cartes.", "Sarah");
	dialog_name("Pour les cartes de\ntarot ?", "Nihil");
	dialog_name("Oui.", "Sarah");
	dialog_name("...", "Sarah");
	dialog_name("Non, attend.", "Sarah");
	dialog_name("Le type en noir m'a\ndit que l'une des", "Sarah");
	dialog_name("cartes a recuperer en\npriorite, c'est", "Sarah");
	dialog_name("l'Empereur.", "Sarah");
	dialog_name("Pourquoi ?", "Nihil");
	dialog_name("J'en sais rien, moi !", "Sarah");
	dialog_name("Bon, je vais essayer\nde trouver cette", "Nihil");
	dialog_name("carte au plus vite,\nalors...", "Nihil");
	dialog_name("Super, a tout a\nl'heure !", "Sarah");
	i = 0;
	while (game.item_values[i * 3] != 4)
		i++;
	take(game.inventories[0], i + 1, visible_items, 1);

	scenes[1] = 1;
}

void scene_2(int* x, int* y, int* requests, int luminosity, int dir, int* visible_items)
{
	update_map(*x, *y, game.maps[0][1], game.character_maps[0][0][1]);
	display_map(*x * 8, *y * 8, game.maps[0][1], luminosity, game.character_maps[0][0][1], game.items[0][1], game.item_values, visible_items, requests);
	display_sprite(4, 5 * 64, 5 * 64, 64, 3, 0);
	display_sprite(4, 6 * 64, 5 * 64, 64, 10, 0);
	dialog_name("Salut.", "???");
	dialog_name("Ah !", "Nihil");
	dialog_name("...", "???");
	dialog_name("Je t'ai fait peur ?", "???");
	dialog_name("Desole, tu m'as\nsurpris...", "Nihil");
	dialog_name("Mouais.", "???");
	dialog_name("...", "Nihil");
	dialog_name("...", "???");
	dialog_name("Un probleme ?", "???");
	dialog_name("Pourquoi une gamine\nserait coincee ici ?", "Nihil");
	dialog_name("Je m'appelle Emma.", "Emma");
	dialog_name("Et je ne suis pas\ncoincee !", "Emma");
	dialog_name("Je dessine la carte\ndu labyrinthe.", "Emma");
	dialog_name("Serieux ?!", "Nihil");
	dialog_name("Oui. D'ailleurs...", "Emma");
	dialog_name("J'ai remarque un truc\nbizarre.", "Emma");
	dialog_name("Mes cartes s'adaptent\na la memoire de", "Emma");
	dialog_name("ceux qui les\nregardent.", "Emma");
	dialog_name("C'est-a-dire ?", "Nihil");
	dialog_name("Quand je decouvre un\nnouveau couloir,", "Emma");
	dialog_name("elles se mettent a\njour tout seul...", "Emma");
	dialog_name("Trop bizarre.", "Emma");
	dialog_name("...", "Nihil");
	dialog_name("Au fait !", "Emma");
	dialog_name("Si t'es pas malin,\nau moins, t'es grand.", "Emma");
	dialog_name("Tu pourrais m'aider ?", "Emma");
	dialog_name("A quoi ?", "Nihil");
	dialog_name("J'ai trouve des\narmoires electriques.", "Emma");
	dialog_name("Si on les active, on\npourrait surement", "Emma");
	dialog_name("allumer la lumiere !", "Emma");
	dialog_name("Serieux ?!", "Nihil");
	dialog_name("Mais chuis trop petite\npour les activer.", "Emma");
	dialog_name("Du coup, voici mon\noffre.", "Emma");
	dialog_name("Si tu me passes les\nbonbons d'un etage,", "Emma");
	dialog_name("je te file la carte\navec la position des", "Emma");
	dialog_name("armoires dessus.", "Emma");
	dialog_name("Deal ?", "Emma");
	dialog_name("Des... bonbons ?", "Nihil");
	dialog_name("Ouiiii !!!", "Emma");
	dialog_name("Ici, chaque bonbon est\ndifferent en fonction", "Emma");
	dialog_name("de l'etage ou il se\ntrouve.", "Emma");
	dialog_name("Et je veux tous les\ngouter.", "Emma");
	dialog_name("Bon... OK.", "Nihil");
	dialog_name("Tiens !", "Emma");
	dialog_name("Une citrouille ?", "Nihil");
	dialog_name("C'est fait pour les\nbonbons !", "Emma");
	dialog_name("On peut meme en faire\nun carrosse !", "Emma");
	dialog_name("Euh... Merci.", "Nihil");
	dialog_name("Au fait, j'ai vu...", "Nihil");
	dialog_name("J'ai pas envie de\nm'ennuyer dans la", "Emma");
	dialog_name("Chambre avec l'autre\nvieille.", "Emma");
	dialog_name("Mais ?!", "Nihil");
	dialog_name("Ce serait plus simple !", "Nihil");
	dialog_name("Mais moins fun.", "Emma");
	dialog_name("Allez, bonne chance\navec les bonbons !", "Emma");
	dialog_name("...", "Nihil");
	dialog_name("Sale gosse.", "Nihil");
	dialog_name("J'ai entendu !", "Emma");
	int i = 0;
	while (game.item_values[i * 3] != 18)
		i++;
	take(game.inventories[0], i + 1, visible_items, 1);
	game.requests[2][0] = 1;
	scenes[2] = 1;
}

void scene_3(int* x, int* y, int* requests, int luminosity, int dir, int* visible_items)
{
	update_map(*x, *y, game.maps[0][2], game.character_maps[0][0][2]);
	display_map(*x * 8, *y * 8, game.maps[0][2], luminosity, game.character_maps[0][0][2], game.items[0][2], game.item_values, visible_items, requests);
	display_sprite(4, 5 * 64, 5 * 64, 64, 1, 0);
	display_sprite(4, 5 * 64, 4 * 64, 64, 12, 0);
	dialog_name("Salut.", "???");
	dialog_name("Euh ?!", "Nihil");
	dialog_name("Excuse-moi, tu peux\nm'aider ?", "???");
	dialog_name("Euh... Oui ?", "Nihil");
	dialog_name("Je suis Mathias.", "Mathias");
	dialog_name("Et moi, Nihil.", "Nihil");
	dialog_name("J'ai trouve un\nmoyen de sortir de", "Mathias");
	dialog_name("ce dedale !", "Mathias");
	dialog_name("Ah ?", "Nihil");
	dialog_name("Mais pour ca, il me\nfaut une pile...", "Mathias");
	dialog_name("Quelqu'un m'a dit que\npour sortir, il fallait", "Nihil");
	dialog_name("recuperer des cartes\nde tarot...", "Nihil");
	dialog_name("Tu parles de Sarah ?", "Mathias");
	dialog_name("Laisse tomber !", "Mathias");
	dialog_name("Les cartes, c'est sa\nsortie a elle.", "Mathias");
	dialog_name("Pas la notre.", "Mathias");
	dialog_name("Et donc toi, il te\nfaut une pile ?", "Nihil");
	dialog_name("Yep.", "Mathias");
	dialog_name("Mais je ne peux pas\ndescendre en bas.", "Mathias");
	dialog_name("Bah pourquoi ?", "Nihil");
	dialog_name("Je t'expliquerai apres.", "Mathias");
	dialog_name("Bon, dans ce cas, je\nreviens des que je", "Nihil");
	dialog_name("trouve une pile !", "Nihil");
	dialog_name("D'accord, a plus !", "Mathias");
	scenes[3] = 1;
}

void scene_4(int* x, int* y, int* requests, int luminosity, int dir, int* visible_items)
{
	int b = requests[12];
	int c = requests[13];
	//requests[3] = 0;
	requests[12] = 0;
	update_map(*x, *y, game.maps[0][0], game.character_maps[0][0][0]);
	display_map(*x * 8, *y * 8, game.maps[0][0], luminosity, game.character_maps[0][0][0], game.items[0][0], game.item_values, visible_items, requests);
	print_characters(*x, *y, requests, 0, 0, luminosity, game.character_maps[0][0][0], game.maps[0][0][0]);
	if (*x == 12)
	{
		move_character(x, y, 0, 0, 0, 0, requests, 0, luminosity, 0, visible_items);
		move_character(x, y, 0, 0, 2, 0, requests, 0, luminosity, 2, visible_items);
	}
	display_map(*x * 8, *y * 8, game.maps[0][0], luminosity, game.character_maps[0][0][0], game.items[0][0], game.item_values, visible_items, requests);
	print_characters(*x, *y, requests, 0, 0, luminosity, game.character_maps[0][0][0], game.maps[0][0][0]);
	display_sprite(4, 5 * 64, 5 * 64, 64, 1, 0);
	dialog_name("Une pile ?", "Nihil");
	dialog_name("Vu que tu vas trainer\nun peu partout,", "Sarah");
	dialog_name("je me suis dit que ca\npourrait t'aider.", "Sarah");
	dialog_name("Mouais...", "Nihil");
	requests[12] = 15;
	requests[13] = 14;
	display_map(*x * 8, *y * 8, game.maps[0][0], luminosity + 5, game.character_maps[0][0][0], game.items[0][0], game.item_values, visible_items, requests);
	print_characters(*x, *y, requests, 0, 0, luminosity + 5, game.character_maps[0][0][0], game.maps[0][0][0]);
	display_sprite(4, 5 * 64, 5 * 64, 64, 1, 0);
	move_character(x, y, 0, 0, 1, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 1, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 2, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 0, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 2, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 2, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 2, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 0, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 0, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 3, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 3, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 0, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 0, 4, requests, 0, luminosity, 1, visible_items);
	display_map(*x * 8, *y * 8, game.maps[0][0], luminosity, game.character_maps[0][0][0], game.items[0][0], game.item_values, visible_items, requests);
	print_characters(*x, *y, requests, 0, 0, luminosity, game.character_maps[0][0][0], game.maps[0][0][0]);
	display_sprite(4, 5 * 64, 5 * 64, 64, 1, 0);
	dialog_name("Scusez-moi...", "???");
	dialog_name("Toi !", "Sarah");
	dialog_name("Hein ?", "Nihil");
	move_character(x, y, 0, 0, 2, 0, requests, 0, luminosity, 2, visible_items);
	move_character(x, y, 0, 0, 0, 0, requests, 0, luminosity, 0, visible_items);
	move_character(x, y, 0, 0, 0, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 2, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 2, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 2, 4, requests, 0, luminosity, 1, visible_items);
	int a = requests[12];
	requests[12] = 0;
	display_map(*x * 8, *y * 8, game.maps[0][0], luminosity, game.character_maps[0][0][0], game.items[0][0], game.item_values, visible_items, requests);
	print_characters(*x, *y, requests, 0, 0, luminosity, game.character_maps[0][0][0], game.maps[0][0][0]);
	display_sprite(4, 5 * 64, 5 * 64, 64, 1, 0);
	display_sprite(4, 4 * 64, 4 * 64, 64, 17, 0);
	dialog_name("Tss...", "???");
	dialog_name("Le type en noir ?", "Nihil");
	dialog_name("Evidemment, il a encore\nfile.", "???");
	dialog_name("Et aucune nouvelle d'la\nSalle de Surveillance.", "???");
	dialog_name("Eh oh !", "Nihil");
	dialog_name("Quoi ?", "???");
	dialog_name("Vous etes qui ?", "Nihil");
	dialog_name("Saihtam.", "Saihtam");
	dialog_name("Le Gerant de\nl'Entrepot.", "Saihtam");
	dialog_name("L'Entrepot ?", "Nihil");
	dialog_name("Oui, l'Entrepot des\nReves.", "Saihtam");
	dialog_name("La ou sont gardes les\nSonges et les", "Saihtam");
	dialog_name("Cauchemars.", "Saihtam");
	dialog_name("Et pourquoi c'est un\nlabyrinthe ?", "Nihil");
	dialog_name("Mesure de securite\nsuite a un probleme.", "Saihtam");
	dialog_name("Maintenant, si vous me\npermettez, j'aimerais", "Saihtam");
	dialog_name("resoudre ce probleme\navant qu'une autre", "Saihtam");
	dialog_name("mesure de defense ne\ns'active.", "Saihtam");
	requests[12] = a;
	move_character(x, y, 0, 0, 3, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 3, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 3, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 1, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 1, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 1, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 2, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 2, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 1, 4, requests, 0, luminosity, 1, visible_items);
	move_character(x, y, 0, 0, 1, 4, requests, 0, luminosity, 1, visible_items);
	requests[12] = 0;
	move_character(x, y, 0, 0, 1, 0, requests, 0, luminosity, 2, visible_items);
	move_character(x, y, 0, 0, 3, 0, requests, 0, luminosity, 0, visible_items);
	display_map(*x * 8, *y * 8, game.maps[0][0], luminosity, game.character_maps[0][0][0], game.items[0][0], game.item_values, visible_items, requests);
	print_characters(*x, *y, requests, 0, 0, luminosity, game.character_maps[0][0][0], game.maps[0][0][0]);
	display_sprite(4, 5 * 64, 5 * 64, 64, 1, 0);
	dialog_name("Il etait... rude,\nnon ?", "Nihil");
	dialog_name("Presse, ouais.", "Sarah");
	dialog_name("...", "Nihil");
	dialog_name("...", "Sarah");
	dialog_name("Je crois que je\nl'aime pas.", "Nihil");
	dialog_name("En tout cas, il a\nete plus locace", "Sarah");
	dialog_name("avec toi que moi !", "Sarah");
	dialog_name("Serieux ?!", "Nihil");
	dialog_name("Il ne m'a pas dit qu'on\netait dans le monde", "Sarah");
	dialog_name("des Reves...", "Sarah");
	dialog_name("Dans ce cas, il suffit\njuste de se reveiller ?", "Nihil");
	dialog_name("Possible...", "Sarah");
	dialog_name("Bon, par contre, il n'a\npas reparle de la", "Sarah");
	dialog_name("carte de l'Empereur...", "Sarah");
	dialog_name("On fait quoi, alors ?", "Nihil");
	dialog_name("Tu es toujours partant\npour recuperer toutes", "Sarah");
	dialog_name("les cartes ?", "Sarah");
	dialog_name("Je suppose que oui...", "Nihil");
	dialog_name("De toute facon, je vois\npas ce qu'on pourrait", "Nihil");
	dialog_name("faire d'autre...", "Nihil");
	dialog_name("Bon, a tout a l'heure,\nalors!", "Sarah");
	dialog_name("Salut !", "Nihil");
	requests[12] = b;
	requests[13] = c;
	scenes[4] = 1;
}

void scene_5(int* x, int* y, int* requests, int luminosity, int dir, int* visible_items)
{
	update_map(*x, *y, game.maps[0][2], game.character_maps[0][0][2]);
	display_map(*x * 8, *y * 8, game.maps[0][2], luminosity, game.character_maps[0][0][2], game.items[0][2], game.item_values, visible_items, requests);
	display_sprite(4, 5 * 64, 5 * 64, 64, 1, 0);
	display_sprite(4, 5 * 64, 4 * 64, 64, 12, 0);
	dialog_name("Merci beaucoup !", "Mathias");
	dialog_name("Alors ?", "Nihil");
	dialog_name("C'est quoi , ta\nsortie ?", "Nihil");
	dialog_name("Chut !", "Mathias");
	dialog_name("Les murs ont des\noreilles...", "Mathias");
	dialog_name("Suis-moi !", "Mathias");
	move_character(x, y, 0, 2, 2, 3, requests, 0, luminosity, 1, visible_items);
	requests[9]--;
	requests[9]--;
	display_map(*x * 8, *y * 8, game.maps[0][2], luminosity, game.character_maps[0][0][2], game.items[0][2], game.item_values, visible_items, requests);
	display_sprite(4, 5 * 64, 5 * 64, 64, 1, 0);
	dialog_name("Attend-moi !", "Nihil");
	move_character(x, y, 0, 2, 1, 0, requests, 0, luminosity, 1, visible_items);
	update_map(*x, *y, game.maps[0][2], game.character_maps[0][0][2]);
	display_map(*x * 8, *y * 8, game.maps[0][2], luminosity, game.character_maps[0][0][2], game.items[0][2], game.item_values, visible_items, requests);
	display_sprite(4, 5 * 64, 5 * 64, 64, 2, 0);
	dialog_name("Que ?!", "Nihil");
	dialog_name("IL... IL EST PASSE\nDANS LE SCEAU ?!", "Nihil");
	dialog_name("...", "Nihil");
	dialog_name("C'est quoi, ce type ?", "Nihil");
	scenes[5] = 1;
}
*/
int play(int character)
{
	int* coord = game.parameters + 4 + 4 * character;
	game.modify = 1;
	int zone = coord[0] - 1;
	int level = coord[1] - 1;
	int x = coord[2] - 1;
	int y = coord[3] - 1;
	int* map = NULL;
	int requests[15];
	int visible_items[6] = { 0, 0, 0, 0, 0, 0 };
	game.requests[2][0] = 0;
	if (!change_map(&map, &zone, &level, &x, &y, zone, level, x, y, requests, character))
		return 0;
	char exit = 0;
	int dir = 0;
	int delay = 0;
	char walking = 0;
	int luminosity = 0;
	update_map(x, y, map, game.character_maps[character][zone][level]);
	while (!inputs[9] && !exit)
	{
		if ((zone > 0 && zone < 4))
		{
			game.character_maps[character][zone][level][0] = 1;
			luminosity = 13;
		}
		if (zone == 0)
			luminosity = game.parameters[2 + game.lights[level]];
		visible_items[0] = game.character_maps[character][zone][level][0];
		/*
		if (x == 10 && y == 12 && level == 0 && !scenes[1] && !delay)
			scene_1(&x, &y, requests, luminosity, dir, visible_items);
		if (x == 10 && y == 5 && level == 1 && !scenes[2] && !delay)
			scene_2(&x, &y, requests, luminosity, dir, visible_items);
		if (x == 3 && y == 17 && level == 2 && !scenes[3] && !delay)
			scene_3(&x, &y, requests, luminosity, dir, visible_items);*/
		display_map(x * 8 - delay * directions[dir * 2], y * 8 - delay * directions[dir * 2 + 1], map, luminosity, game.character_maps[character][zone][level], game.items[zone][level], game.item_values, visible_items, requests);
		print_characters(x, y, requests, delay * directions[dir * 2], delay * directions[dir * 2 + 1], luminosity, game.character_maps[character][zone][level], map[0]);
		if (delay && walking)
			display_sprite(4, 5 * 64, 5 * 64, 64, dir, delay - 1);
		else
			display_sprite(4, 5 * 64, 5 * 64, 64, dir, 0);
		print_notif();
		print_refresh();
		load_input();
		if (delay || game.modify)
		{
			if (delay)
				delay--;
			//Redirections
			if (!delay || game.modify)
			{
				update_map(x, y, map, game.character_maps[character][zone][level]);
				int current_case = map[2 + x + y * *map];
				if ((current_case == 1 && !game.modify) || (current_case > 2 && current_case < 7))
				{
					if (current_case == 3)
						dir = 2;
					if (current_case == 4)
						dir = 0;
					if (current_case == 5)
						dir = 1;
					if (current_case == 6)
						dir = 3;
					int i = x + directions[dir * 2];
					int j = y + directions[dir * 2 + 1];
					if (i >= 0 && j >= 0 && i < *map && j < map[1] && map[2 + i + j * *map] != 2)
					{
						int item = game.items[zone][level][i + j * map[0]];
						if (item)
							item = game.item_values[3 * item - 3];
						if (item != 17)
						{
							walking = map[2 + x + y * *map] != 1;
							delay = 8;
							x = i;
							y = j;
						}
					}
				}
				game.modify = 0;
				if (current_case == 8 && zone == 1)
				{
					size_t len = map[0] * map[1] + 2;
					x = map[len] - 1;
					y = map[len + 1] - 1;
					load_key(map, len);
					game.modify = 1;
					delay = 0;
				}
				if (current_case == 7 && zone == 1)
				{
					map[2 + x + y * *map] = 8;
					game.portal_goal--;
					if (!game.portal_goal)
					{
						change_map(&map, &zone, &level, &x, &y, game.portal_entry[0], game.portal_entry[1], game.portal_entry[2], game.portal_entry[3], requests, character);
						int item = game.items[zone][level][x + y * *map];
						game.item_values[item * 3 - 1] = 25;
						add_notif(*free_key_texts);
						if (take(game.inventories[character], item, visible_items, level))
							game.items[zone][level][x + y * *map] = 0;
					}
				}
			}
		}
		else
		{
			//New direction
			if (!delay)
			{
				int action = 0;
				if (inputs[2])
					action = 2;
				if (inputs[3])
					action = 1;
				if (inputs[4])
					action = 3;
				if (inputs[5])
					action = 4;
				if (action)
				{
					dir = action - 1;
					int i = x + directions[dir * 2];
					int j = y + directions[dir * 2 + 1];
					if (i >= 0 && j >= 0 && i < *map && j < map[1] && map[2 + i + j * *map] != 2)
					{
						int item = game.items[zone][level][i + j * map[0]];
						if (item)
							item = game.item_values[3 * item - 3];
						if (item == 17)
						{
							display_map(x * 8 - delay * directions[dir * 2], y * 8 - delay * directions[dir * 2 + 1], map, luminosity, game.character_maps[character][zone][level], game.items[zone][level], game.item_values, visible_items, requests);
							print_characters(x, y, requests, delay * directions[dir * 2], delay * directions[dir * 2 + 1], luminosity, game.character_maps[character][zone][level], map[0]);
							display_sprite(4, 5 * 64, 5 * 64, 64, dir, 0);
							print_notif();
							if (visible_items[1])
							{
								char m[200] = { 0 };
								int value = game.item_values[3 * game.items[zone][level][i + j * map[0]] - 2];
								size_t a = copy_str(m, "Un sceau de type ", 200);
								m[a] = (value / 10) % 10 + 48;
								m[a + 1] = value % 10 + 48;
								a += 2;
								a += copy_str(m + a, "\nvous barre la route.", 200 - a);
								dialog(m, "");
								if (has_item(game.inventories[character], 16, value, 1))
								{
									if (dialog_choice("Ouvrir", "Ne rien faire") == 0)
									{
										game.items[zone][level][i + j * map[0]] = 0;
									}
								}
								else
									dialog("Vous ne pouvez pas\nl'ouvrir.", "");
							}
							else
							{
								dialog("Un sceau vous barre la\nroute.", "");
							}
						}
						else
						{
							int c = is_there_character(requests, i, j);
							if (c == 0 || requests[c * 3 - 1] == 0)
							{
								if (item != 19 && item != 20)
								{
									walking = map[2 + x + y * *map] != 1;
									delay = 8;
									x = i;
									y = j;
								}
							}
							else
							{
								display_map(x * 8 - delay * directions[dir * 2], y * 8 - delay * directions[dir * 2 + 1], map, luminosity, game.character_maps[character][zone][level], game.items[zone][level], game.item_values, visible_items, requests);
								print_characters(x, y, requests, delay * directions[dir * 2], delay * directions[dir * 2 + 1], luminosity, game.character_maps[character][zone][level], map[0]);
								display_sprite(4, 5 * 64, 5 * 64, 64, dir, 0);
								int* request = game.requests[c - 1] + (requests[c * 3 - 1] - 1) * 10;
								int result = handle_request(character, c, request, requests);
								if (result == 1)
								{
									display_map(x * 8 - delay * directions[dir * 2], y * 8 - delay * directions[dir * 2 + 1], map, luminosity, game.character_maps[character][zone][level], game.items[zone][level], game.item_values, visible_items, requests);
									print_characters(x, y, requests, delay * directions[dir * 2], delay * directions[dir * 2 + 1], luminosity, game.character_maps[character][zone][level], map[0]);
									display_sprite(4, 5 * 64, 5 * 64, 64, dir, 0);
									int i = 0;
									while (i < 255)
									{
										display_map(x * 8 - delay * directions[dir * 2], y * 8 - delay * directions[dir * 2 + 1], map, luminosity, game.character_maps[character][zone][level], game.items[zone][level], game.item_values, visible_items, requests);
										print_characters(x, y, requests, delay * directions[dir * 2], delay * directions[dir * 2 + 1], luminosity, game.character_maps[character][zone][level], map[0]);
										display_sprite(4, 5 * 64, 5 * 64, 64, dir, 0);
										rect_alpha(0, 0, 704, 704, 0, 0, 0, i);
										print_refresh();
										load_input();
										i += 5;
									}
									change_map(&map, &zone, &level, &x, &y, zone, level, x, y, requests, character);
									while (i)
									{
										i -= 5;
										display_map(x * 8 - delay * directions[dir * 2], y * 8 - delay * directions[dir * 2 + 1], map, luminosity, game.character_maps[character][zone][level], game.items[zone][level], game.item_values, visible_items, requests);
										print_characters(x, y, requests, delay * directions[dir * 2], delay * directions[dir * 2 + 1], luminosity, game.character_maps[character][zone][level], map[0]);
										display_sprite(4, 5 * 64, 5 * 64, 64, dir, 0);
										rect_alpha(0, 0, 704, 704, 0, 0, 0, i);
										print_refresh();
										load_input();
									}
								}
								/*
								if (result == 2 && !scenes[4])
								{
									scene_4(&x, &y, requests, luminosity, dir, visible_items);
									dir = 1;
								}
								if (result == 3 && !scenes[5])
								{
									scene_5(&x, &y, requests, luminosity, dir, visible_items);
									dir = 2;
								}*/
								if (result)
									change_map(&map, &zone, &level, &x, &y, zone, level, x, y, requests, character);
							}
						}
					}
					if (map[2 + i + j * *map] == 2)
					{
						int item = game.items[zone][level][i + j * map[0]];
						if (item)
							item = game.item_values[3 * item - 3];
						if (item == 3 && visible_items[0])
						{
							display_map(x * 8 - delay * directions[dir * 2], y * 8 - delay * directions[dir * 2 + 1], map, luminosity, game.character_maps[character][zone][level], game.items[zone][level], game.item_values, visible_items, requests);
							print_characters(x, y, requests, delay * directions[dir * 2], delay * directions[dir * 2 + 1], luminosity, game.character_maps[character][zone][level], map[0]);
							display_sprite(4, 5 * 64, 5 * 64, 64, dir, 0);
							print_notif();
							dialog(electrical_texts[0], "");
							game.lights[level] = dialog_choice(electrical_texts[1], electrical_texts[2]) == 0;
						}
					}
				}
			}
			//Other inputs
			if (!delay)
			{
				if (inputs[0])
				{
					size_t current_item = game.items[zone][level][x + y * *map];
					int item_type = 0;
					if (current_item)
						item_type = game.item_values[current_item * 3 - 3];
					if (item_type == 16 && !visible_items[1])
						item_type = 0;
					if (item_type < 4 || item_type == 17 || (item_type == 16 && game.item_values[current_item * 3 - 1] < 25))
					{
						if (item_type == 1)
							change_map(&map, &zone, &level, &x, &y, zone, level + 1, x, y, requests, character);
						if (item_type == 2)
							change_map(&map, &zone, &level, &x, &y, zone, level - 1, x, y, requests, character);
						if (item_type == 16)
						{
							if (game.item_values[current_item * 3 - 1] == 0)
							{
								change_map(&map, &zone, &level, &x, &y, 1, game.item_values[current_item * 3 - 2] - 1, x, y, requests, character);
								size_t j = 0;
								while (j < map[0] * map[1])
								{
									game.character_maps[character][1][level][j + 1] = 2;
									j++;
								}
								game.character_maps[character][1][level][0] = 1;
							}
						}
						update_map(x, y, map, game.character_maps[character][zone][level]);
					}
					else
					{
						if (take(game.inventories[character], current_item, visible_items, level))
							game.items[zone][level][x + y * *map] = 0;
					}
					inputs[0] = 0;
				}
				else
				{
					if (inputs[1])
					{
						if (menu(zone, level, x, y, dir, map, delay, visible_items, character, requests, luminosity))
							change_map(&map, &zone, &level, &x, &y, game.portal_entry[0], game.portal_entry[1], game.portal_entry[2], game.portal_entry[3], requests, character);
					}
					else
					{
						if (inputs[6])
						{
							if (game.character_maps[character][zone][level][0])
								map_view(zone, level, character, x, y, visible_items, zone, level);
							else
								dialog(no_map_texts[0], "");
						}
						inputs[6] = 0;
					}
					inputs[0] = 0;
				}
			}
		}
	}
	return exit - 1;
}

int find_request(int character, int zone, int level)
{
	char has_request = 0;
	int i = 0;
	while (i < game.parameters[30 + character] && !has_request)
	{
		int* coord = game.requests[character] + i * 10;
		if (coord[0])
			has_request = (character != 2);
		if (coord[0] == zone && coord[1] == level)
		{
			has_request = 1;
		}
		i++;
	}
	return i;
}

void start_game(char* title, char* author)
{
	//update_texts();
	int i = 0;
	while (i < 10)
	{
		notifs[i].delay = 0;
		i++;
	}
	i = 0;
	//Default locations
	while (i < 5)
	{
		int* coord = game.parameters + 4 + 4 * i;
		characters_positions[i].zone = coord[0] - 1;
		characters_positions[i].level = coord[1] - 1;
		characters_positions[i].x = coord[2] - 1;
		characters_positions[i].y = coord[3] - 1;
		i++;
	}
	//Define requests
	i = 0;

	int first_character = 0;
	zone = characters_positions[first_character].zone;
	level = characters_positions[first_character].level;
	if (!load_game(&game, title, author))
		return;
	rect(0, 0, 704, 704, 0, 0, 0);
	int* coord = game.parameters + 4 + 4 * first_character;
	game.modify = 1;
	zone = coord[0] - 1;
	level = coord[1] - 1;
	map = NULL;
	//dialog("Ceci est un # de test.&{1}Salut !&{2}[3]Euh...*Quoi ?*{3}[1]Degage, on t'a pas*sonne.", "test");
	while (!inputs[9] && first_character)
		first_character = play(first_character - 1);
	free_game(&game);
}

