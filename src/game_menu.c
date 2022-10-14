#include "game_menu.h"

void main_menu_game()
{
    while (!inputs[9] && !inputs[0])
	{
		int i = 0;
		while (!inputs[0] && !inputs[9])
		{
			rect(0, 0, 704, 704, 0, 0, 0);
			rect(4, 4, 696, 100, 150, 150, 150);
			rect(4, 108, 696, 592, 150, 150, 150);
			print_text_centered(0, 8, main_menu_texts[0], 704, 0, 0);
			print_text_centered(0, 48, main_menu_texts[1], 704, 0, 0);
			print_text(300, 300, main_menu_texts[2], 0, 0);
			print_text(300, 380, main_menu_texts[3], 0, 0);
			display_sprite(3, 200, 290 + 80 * i, 64, 15, 0);
			print_refresh();
			load_input_long(); 
			if (inputs[2] || inputs[3])
				i = !i;
		}
		if (i)
			inputs[9] = 1;
	}
	/*
	if (!inputs[9])
		start_game("Game", "Majoran");*/
}