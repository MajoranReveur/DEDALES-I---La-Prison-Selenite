#pragma once

#include "print.h"
#include "input.h"
#include "languages.h"
#include "editor_load.h"
#include "editor_create.h"
#include "structures.h"
#include "file_write.h"
#include "map_modify.h"

struct project project_data;
extern char** main_menu_texts;

void project_menu();
void main_menu_editor();