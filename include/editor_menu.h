#pragma once

#include "print.h"
#include "input.h"
#include "languages.h"
#include "editor_load.h"
#include "editor_create.h"
#include "structures.h"
#include "file_write.h"
#include "map_modify.h"
#include "request_editor.h"
#include "cinematic_editor.h"

extern struct project project_data;
extern char** main_menu_texts;

void project_menu();
void main_menu_editor();
char position_choice_zone(struct position *p);
char position_choice_all_zone(struct position *p);