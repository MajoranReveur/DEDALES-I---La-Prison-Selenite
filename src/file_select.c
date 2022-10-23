#include "file_select.h"

size_t filelist(char* type, char*** list, size_t count) // Fill list with the count first elements with type format, or return the number of files with this format if count is null
{
	struct _finddata_t c_file;
	intptr_t hFile;
	size_t size = count;
	count = size;
	size = 0;
	(*list) = malloc(sizeof(char*) * count * 2);
	if ((hFile = _findfirst(type, &c_file)) == -1L || !(*list))
		return 0; // No file found
	do
	{
		if (size < count) // File copy in list if the list is big enough
		{
			char* stringtitle = malloc(sizeof(char) * 301);
			char* stringauthor = malloc(sizeof(char) * 101);
			if (!stringtitle || !stringauthor)
			{
				free(stringtitle);
				free(stringauthor);
				return size;
			}
			size_t i = 0;
			while (c_file.name[i] != '[')
			{
				stringtitle[i] = c_file.name[i];
				i++;
			}
			stringtitle[i] = 0;
			size_t j = 0;
			i++;
			while (c_file.name[i + j] != ']')
			{
				stringauthor[j] = c_file.name[i + j];
				j++;
			}
			stringauthor[j] = 0;
			(*list)[size * 2] = stringtitle;
			(*list)[size * 2 + 1] = stringauthor;
		}
		size++;
	} while (_findnext(hFile, &c_file) == 0);
	_findclose(hFile);
	return size; // can be less than count if some files are deleted during the operation. If files are added, they will be ignored
}

int file_choice(char* type, char** finaltitle, char** finalauthor)
{
	int x = 0;
	int t = 0;
	char** list = NULL;
	size_t count = filelist(type, &list, 0);
	filelist(type, &list, count);
	inputs[0] = 0;
	while (!inputs[0] && !inputs[9])
	{
		rect(0, 0, 1104, 704, 0, 0, 0);
		int y = 0;
		while (y < 15 && x + y < count)
		{
			print_text(0, 110 + y * 30, list[(x + y) * 2], 1, 1 + (t == x + y));
			y++;
		}
		if (count == 0)
			print_text_centered(0, 308, "Aucun fichier detecte.", 1, 1, 1104);
		print_text_centered(0, 20, "Selection de fichier", 1, 1, 1104);
		print_refresh();
		load_input_long();
		if (inputs[2])
			t--;
		if (inputs[3])
			t++;
		if (t < 0)
			t = count - 1;
		if (t >= count)
			t = 0;
		if (t < x + 1)
			x = t - 1;
		if (t > x + 10)
			x = t - 10;
		if (x < 0)
			x = 0;
		if (x > count - 10)
			x = count - 10;
		if (inputs[1])
		{
			rect(12, 80, 1000, 484, 0, 0, 0);
			size_t i = 0;
			while (i < count)
			{
				free(list[i * 2]);
				free(list[i * 2 + 1]);
				i++;
			}
			free(list);
			count = filelist(type, &list, 0);
			count = filelist(type, &list, count);
			t = 0;
		}
	}
	*finaltitle = NULL;
	*finalauthor = NULL;
	if (count)
	{
		if (!inputs[9])
		{
			*finaltitle = list[t * 2];
			*finalauthor = list[t * 2 + 1];
		}
		size_t i = 0;
		while (i < count)
		{
			if (i != t || inputs[9])
			{
				free(list[i * 2]);
				free(list[i * 2 + 1]);
			}
			i++;
		}
	}
	free(list);
	return (count && inputs[0]);
}
