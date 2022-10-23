#include "tools.h"

size_t copy_str(char* src, char* str, int length)
{
	int a = 0;
	while (str[a] && a < length)
	{
		src[a] = str[a];
		a++;
	}
	src[a] = 0;
	return a;
}

void int_to_str(char* str, int value, int to_fill)
{
	int length = 1;
	long long a = value;
	while (a > 9)
	{
		a /= 10;
		length++;
	}
	if (length < to_fill)
		length = to_fill;
	if (length < 20)
	{
		str[length] = 0;
		while (length)
		{
			length--;
			str[length] = value % 10 + 48;
			value /= 10;
		}
	}
	else
		str[0] = 0;
}

void concat_str(char* dest, char** str, size_t dest_length, int str_size)
{
	size_t a = 0;
	int i = 0;
	while (i < str_size)
	{
		a += copy_str(dest + a, str[i], dest_length - a);
		i++;
	}
}