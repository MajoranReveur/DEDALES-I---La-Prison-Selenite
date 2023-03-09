#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "editor_menu.h"
#include "languages.h"
#include "item_editor.h"

void modify_map(int zone, int map);
char create_map(int zone);
char position_choice_cell(int zone, int *map, struct position *p);