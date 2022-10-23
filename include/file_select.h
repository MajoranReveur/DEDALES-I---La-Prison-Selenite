#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include "print.h"
#include "input.h"

size_t filelist(char* type, char*** list, size_t count);
int file_choice(char* type, char** finaltitle, char** finalauthor);
