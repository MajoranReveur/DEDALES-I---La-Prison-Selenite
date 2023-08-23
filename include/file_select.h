#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include "print.h"
#include "input.h"

size_t filelist(char* type, char*** list, size_t count);
int file_choice(char* type, char** finaltitle, char** finalauthor);
int save_choice();

void save_choice_for_load();
void save_choice_for_save();
void save_choice_for_load_with_game();

void open_latest_game();
void get_younger_spot();

void get_older_spot();