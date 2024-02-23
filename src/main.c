#include <stdio.h>
#include <stdlib.h>
#include "print.h"
#include "input.h"
#include "game_menu.h"
#include "editor_menu.h"
//#include "game.h"
//#include "files.h"

extern char software_mode; 
extern char file_loaded; 
extern char file_saved; 

extern char** main_menu_texts;
int main(int argc, char** argv)
{
	software_mode = 0; //0 for editor, 1 for game
	file_loaded = 0;
	file_saved = 1;
	default_inputs();
	if (!language_load("francais", 1))
		return 0;
	update_texts();
	if (print_init("res/fonts/pixelmix.ttf"))
	{
		//input_editor();
		char* spritesheets[] = {
			"res/sprites/bigtiles.bmp",
			"res/sprites/smalltiles.bmp",
			"res/sprites/minitiles.bmp",
			"res/sprites/items.bmp",
			"res/sprites/characters.bmp",
			"res/sprites/dialogs.bmp",
			"res/sprites/cardsprites.bmp"
		};
		if (load_sprites(spritesheets))
		{
			if (software_mode)
				main_menu_game();
			else
				main_menu_editor();
			free_sprites();
		}
	}
	print_close();
	return 0;
}