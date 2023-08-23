#include "print.h"
#include "input.h"
#include "structures.h"
#include "game_menu.h"

int get_player();
char is_savable();

void display_current_screen(struct position camera);
char is_in_map(int x, int y, int map, int zone);
void reload_with_character(int character);
void main_loop();

void launch_game(char with_save, int save_spot);