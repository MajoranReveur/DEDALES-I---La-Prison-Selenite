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
	inputs[5] = 0;
	while (!inputs[5] && !inputs[0] && !inputs[6])
	{
		rect(0, 0, 1104, 704, 0, 0, 0);
		int y = 0;
		while (y < 17 && x + y < count)
		{
			print_text(0, 110 + y * 30, list[(x + y) * 2], 1, 1 + (t == x + y));
			y++;
		}
		if (count == 0)
			print_text_centered(0, 308, "Aucun fichier detecte.", 1, 1, 1104);
		print_text_centered(0, 20, "Selection de fichier", 1, 1, 1104);
		rect(0, 620, 1104, 8, 255, 255, 255);
		print_text_centered(0, 640, "Selectionner :", 1, 1, 1104/3);
		print_text_centered(1104/3, 640, "Actualiser :", 1, 1, 1104/3);
		print_text_centered(2*1104/3, 640, "Retour :", 1, 1, 1104/3);
		print_text_centered(0, 670, get_key_name(4), 1, 1, 1104/3);
		print_text_centered(1104/3, 670, get_key_name(9), 1, 1, 1104/3);
		print_text_centered(2*1104/3, 670, get_key_name(5), 1, 1, 1104/3);
		print_refresh();
		load_input_long();
		if (inputs[1])
		{
			if (t > 0)
			{
				t--;
				if (x >= t && t > 0)
					x = t - 1;
			}
			else
			{
				t = count - 1;
				x = 0;
				if (x + 15 < t)
					x = t - 16;
			}
		}
		if (inputs[2])
		{
			if (t + 1 < count)
			{
				t++;
				if (x + 15 < t && t + 1 < count)
					x = t - 15;
			}
			else
			{
				t = 0;
				x = 0;
			}
		}
		if (inputs[10])
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
			x = 0;
		}
		if (inputs[5] && count)
			inputs[5] = check_choice(list[t * 2], list[t * 2 + 1]);
	}
	*finaltitle = NULL;
	*finalauthor = NULL;
	if (count)
	{
		if (!inputs[0])
		{
			*finaltitle = list[t * 2];
			*finalauthor = list[t * 2 + 1];
		}
		size_t i = 0;
		while (i < count)
		{
			if (i != t || inputs[0])
			{
				free(list[i * 2]);
				free(list[i * 2 + 1]);
			}
			i++;
		}
	}
	free(list);
	return (count && inputs[5]);
}
