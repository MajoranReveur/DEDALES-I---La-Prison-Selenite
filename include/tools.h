#pragma once

#include <stdio.h>
#include <stdlib.h>

size_t copy_str(char* src, const char* str, int length);

void int_to_str(char* str, int value, int to_fill);

void concat_str(char* dest, const char** str, size_t dest_length, int str_size);