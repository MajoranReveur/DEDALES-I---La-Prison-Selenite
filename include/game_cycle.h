#include "print.h"
#include "input.h"
#include "structures.h"
#include "game_menu.h"

int get_player();

void display_current_screen(struct position camera);
char is_in_map(int x, int y, int map, int zone);
void main_loop();

void launch_game();