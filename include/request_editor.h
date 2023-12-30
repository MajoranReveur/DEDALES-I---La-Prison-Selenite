#pragma once
#include "structures.h"
#include "print.h"
#include "input.h"
#include "languages.h"
#include "editor_menu.h"

void mission_editor(struct mission *m);
void request_editor(struct request *r, int character);
void request_character_editor(int character);