#include "files.h"

const int LANGUAGE_SIZE = 100;
const int PSEUDO_SIZE = 100;
const int NAME_SIZE = 300;

char int_load(FILE* file, int* result, char end)
{
	char b = fgetc(file);
	*result = 0;
	while (b != end && b != EOF)
	{
		if (b < '0' || b>'9')
		{
			b = EOF;
		}
		else
		{
			*result = *result * 10 + b - 48;
			b = fgetc(file);
		}
	}
	return (b == end);
}

char string_load(FILE* file, char** result)
{
	fpos_t position;
	size_t len = 1;
	fgetpos(file, &position);
	char b = fgetc(file);
	while (b == '\'')
	{
		while (b != '\n' && b != EOF)
			b = fgetc(file);
		fgetpos(file, &position);
		b = fgetc(file);
	}
	while (b != '\n' && b != EOF)
	{
		b = fgetc(file);
		len++;
	}
	fsetpos(file, &position);
	char* str = calloc(len, sizeof(char));
	if (!str)
		return 0;
	b = fgetc(file);
	size_t a = 0;
	while (b != '\n' && b != EOF)
	{
		str[a] = b;
		a++;
		b = fgetc(file);
	}
	*result = str;
	return 1;
}

char list_load(FILE* file, int* list, size_t n) // *result must have been allocated with a size n
{
	size_t a = 0;
	char valid = 1;
	while (a + 1 < n && valid)
	{
		valid = int_load(file, list + a, ' ');
		a++;
	}
	if (valid && a < n)
		valid = int_load(file, list + a, '\n');
	return valid;
}

char load_mapss(int** map, FILE* filemap, int zone)
{
	int sizes[] = { 0,0 };
	if (list_load(filemap, sizes, 2))
	{
		size_t length = sizes[0] * sizes[1];
		int* newmap = malloc(sizeof(int) * (2 + length + 2 * (zone >= 1 && zone <= 3) + (zone == 2)));
		if (newmap == NULL)
			return 0;
		size_t i = 0;
		char valid = 1;
		while (i < length && valid)
		{
			valid = list_load(filemap, newmap + 2 + i, 1);
			i++;
		}
		if (valid && zone >= 1 && zone <= 3)
			valid = list_load(filemap, newmap + 2 + length, 2 + (zone == 2));
		if (valid)
		{
			newmap[0] = sizes[0];
			newmap[1] = sizes[1];
			*map = newmap;
			return 1;
		}
	}
	return 0;
}

void fill(int* list, size_t len)
{
	size_t i = 0;
	while (i < len)
	{
		list[i] = 0;
		i++;
	}
}

char load_game(struct game* game, char* title, char* author)
{
	game->modify = 0;
	game->portal_goal = 0;
	fill(game->item_count, 18);
	fill(game->parameters, 35);
	fill(game->portal_entry, 4);
	fill(game->lights, 99);
	game->item_values = NULL;
	game->containers = NULL;
	game->container_list = NULL;
	char path[450] = { 0 };
	size_t a = copy_str(path, "levels/games/", 450);
	a += copy_str(path + a, title, 450 - a);
	a += copy_str(path + a, "[", 450 - a);
	a += copy_str(path + a, author, 450 - a);
	a += copy_str(path + a, "].txt", 450 - a);
	FILE* file = fopen(path, "r");
	if (file == NULL)
	{
		print_error("Impossible d'ouvrir le\nfichier.\nVerifiez le dossier\nlevels/games/ et le\nnom du projet.");
		return 0;
	}
	char valid = list_load(file, game->parameters, 35);
	//Load maps
	int i = 0;
	while (i < 5)
	{
		game->maps[i] = NULL;
		game->items[i] = NULL;
		game->requests[i] = NULL;
		fill(game->inventories[i], 50);
		if (valid)
		{
			game->maps[i] = malloc(sizeof(int*) * (game->parameters[25 + i]));
			game->items[i] = malloc(sizeof(int*) * (game->parameters[25 + i]));
			valid = game->maps[i] != NULL && game->items[i] != NULL;
			if (valid)
			{
				int j = 0;
				while (j < game->parameters[25 + i])
				{
					game->maps[i][j] = NULL;
					game->items[i][j] = NULL;
					if (valid)
						valid = load_mapss(game->maps[i] + j, file, i);
					if (valid)
					{
						game->items[i][j] = calloc(game->maps[i][j][0] * game->maps[i][j][1], sizeof(int));
						valid = game->items[i][j] != NULL;
					}
					j++;
				}
			}
		}
		i++;
	}
	//Load items
	if (valid)
		valid = list_load(file, game->item_count, 20);
	int sum = 0;
	if (valid)
	{
		i = 0;
		while (i < 20)
		{
			sum += game->item_count[i];
			i++;
		}
		game->item_values = malloc(sizeof(int) * sum * 3);
		valid = game->item_values != NULL;
	}
	int coord[3] = { 0 };
	i = sum;
	while (i && valid)
	{
		i--;
		valid = list_load(file, coord, 3);
		if (valid)
			valid = list_load(file, game->item_values + i * 3, 3);
		if (valid && coord[0] >= 0 && coord[0] < 5 &&
			coord[1] >= 0 && coord[1] < game->parameters[coord[0] + 25] &&
			coord[2] >= 0 && coord[2] < game->maps[coord[0]][coord[1]][0] * game->maps[coord[0]][coord[1]][1])
		{
			game->items[coord[0]][coord[1]][coord[2]] = i + 1;
		}
		else
			valid = 0;
	}
	//Load requests
	i = 0;
	while (i < 5 && valid)
	{
		game->requests[i] = malloc(game->parameters[30 + i] * 10 * sizeof(int));
		valid = game->requests[i] != NULL;
		if (valid)
			valid = list_load(file, game->requests[i], game->parameters[30 + i] * 10);
		i++;
	}
	//containers allocations
	if (valid)
	{
		int container_count = game->item_count[3] + game->item_count[4] +
			game->item_count[5] + game->item_count[6] +
			game->item_count[8] + game->item_count[9] + game->item_count[17];
		game->containers = malloc(sizeof(int*) * container_count);
		valid = game->containers != NULL;
	}
	if (valid)
	{
		i = 0;
		int j = 0;
		while (i < sum && valid)
		{
			int value = game->item_values[i * 3];
			int length = 0;
			if (value == 4)
				length = 23;
			if (value == 5 || value == 6 || value == 7 || value == 18)
				length = 99;
			if (value == 9 || value == 10)
				length = game->item_values[i * 3 + 1];
			if (length)
			{
				game->containers[j] = calloc(length, sizeof(int));
				game->item_values[i * 3 + 2] = j;
				valid = game->containers[j] != NULL;
				j++;
			}
			i++;
		}
	}
	//Map for characters allocation
	i = 0;
	while (i < 5)
	{
		int j = 0;
		while (j < 5)
		{
			game->character_maps[i][j] = NULL;
			if (valid)
			{
				game->character_maps[i][j] = malloc(sizeof(char*) * game->parameters[25 + j]);
				valid = game->character_maps[i][j] != NULL;
				if (valid)
				{
					int k = 0;
					while (k < game->parameters[25 + j])
					{
						game->character_maps[i][j][k] = NULL;
						if (valid)
						{
							size_t len = game->maps[j][k][0] * game->maps[j][k][1] + 1;
							game->character_maps[i][j][k] = malloc(sizeof(char) * len);
							valid = game->character_maps[i][j][k] != NULL;
							if (valid)
							{
								while (len)
								{
									len--;
									game->character_maps[i][j][k][len] = 0;
								}
							}
						}
						k++;
					}
				}
			}
			j++;
		}
		i++;
	}
	if (!valid)
		free_game(game);
	fclose(file);

	return valid;
}

char free_game(struct game* proj)
{
	int i = 0;
	while (i < 5)
	{
		if (proj->maps[i])
		{
			int j = 0;
			while (j < proj->parameters[25 + i])
			{
				free(proj->maps[i][j]);
				j++;
			}
		}
		if (proj->items[i])
		{
			int j = 0;
			while (j < proj->parameters[25 + i])
			{
				free(proj->items[i][j]);
				j++;
			}
		}
		int j = 0;
		while (j < 5)
		{
			if (proj->character_maps[i][j])
			{
				int k = 0;
				while (k < proj->parameters[25 + j])
				{
					free(proj->character_maps[i][j][k]);
					k++;
				}
			}
			free(proj->character_maps[i][j]);
			j++;
		}
		free(proj->maps[i]);
		free(proj->requests[i]);
		i++;
	}
	return 1;
}

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
	return (count && !inputs[9]);
}

char language_load(char* language, char first_one)
{
	char path[150] = { 0 };
	size_t a = copy_str(path, "res/languages/", 150);
	a += copy_str(path + a, language, 150 - a);
	a += copy_str(path + a, ".txt", 150 - a);
	FILE* file = fopen(path, "r");
	char* new_texts[200];
	if (file)
	{
		int a = 0;
		char valid = 1;
		while (a < 200 && valid)
		{
			valid = string_load(file, new_texts + a);
			a++;
		}
		if (valid)
		{
			while (a)
			{
				a--;
				if (!first_one)
					free(texts[a]);
				texts[a] = new_texts[a];
			}
			return 1;
		}
		while (a)
		{
			a--;
			free(new_texts[a]);
		}
	}
	return 0;
}