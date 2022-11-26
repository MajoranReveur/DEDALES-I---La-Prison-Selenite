#pragma once
#include "structures.h"
#include "print.h"
#include "input.h"
#include "languages.h"
#include "editor_menu.h"

int item_choice(int type);
void modify_item(struct item *item, struct position p, char mode);