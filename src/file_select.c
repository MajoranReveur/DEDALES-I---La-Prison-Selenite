#include "file_select.h"
#include <sys/stat.h>
#include <time.h>

extern char software_mode; 

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
	int size_screen = 1104 - software_mode * 400;
	int x = 0;
	int t = 0;
	char** list = NULL;
	size_t count = filelist(type, &list, 0);
	filelist(type, &list, count);
	inputs[5] = 0;
	while (!inputs[5] && !inputs[0] && !inputs[6])
	{
		rect(0, 0, size_screen, 704, 0, 0, 0);
		int y = 0;
		while (y < 17 && x + y < count)
		{
			print_text(0, 110 + y * 30, list[(x + y) * 2], 1, 1 + (t == x + y));
			y++;
		}
		if (count == 0)
			print_text_centered(0, 308, "Aucun fichier detecte.", 1, 1, size_screen);
		print_text_centered(0, 20, "Selection de fichier", 1, 1, size_screen);
		rect(0, 620, 1104, 8, 255, 255, 255);
		print_text_centered(0, 640, "Selectionner :", 1, 1, size_screen/3);
		print_text_centered(1104/3, 640, "Actualiser :", 1, 1, size_screen/3);
		print_text_centered(2*1104/3, 640, "Retour :", 1, 1, size_screen/3);
		print_text_centered(0, 670, get_key_name(4), 1, 1, size_screen/3);
		print_text_centered(1104/3, 670, get_key_name(9), 1, 1, size_screen/3);
		print_text_centered(2*1104/3, 670, get_key_name(5), 1, 1, size_screen/3);
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
			rect(12, 80, size_screen - 12 * 2, 484, 0, 0, 0);
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

char is_same_game(int spot)
{
    char number[2] = {spot + '0', 0};
    const char* file_fields[3] = {
        "levels/saves/save",
        number,
        ".txt"
    };
    char filename[451] = { 0 };
    concat_str(filename, file_fields, 450, 3);
    FILE* file = fopen(filename, "r");
    if (!file)
		return 0; // No save
	char c = fgetc(file);
	int i = 0;
	while (project_data.project_name[i] == c)
	{
		i++;
		c = fgetc(file);
	}
	if (project_data.project_name[i] != 0 || c != '\n')
	{
    	fclose(file);
		return 1;
	}
	c = fgetc(file);
	i = 0;
	while (project_data.author_name[i] == c)
	{
		i++;
		c = fgetc(file);
	}
    fclose(file);
	if (project_data.author_name[i] != 0 || c != '\n')
		return 1;
    return 2;
}

const time_t get_date(int spot)
{
    char number[2] = {spot + '0', 0};
    const char* file_fields[3] = {
        "levels/saves/save",
        number,
        ".txt"
    };
    char filename[451] = { 0 };
    concat_str(filename, file_fields, 450, 3);
	struct stat stats;
	if (stat(filename, &stats) == 0)
	{
		//print_error(ctime(&stats.st_mtime));
		return stats.st_mtime;
	}
    return 0;
}

char projecttitles[10][301];
char authornames[10][101];

void get_game_info(int spot, int* players)
{
    char number[2] = {spot + '0', 0};
    const char* file_fields[3] = {
        "levels/saves/save",
        number,
        ".txt"
    };
    char filename[451] = { 0 };
    concat_str(filename, file_fields, 450, 3);
    FILE* file = fopen(filename, "r");
	projecttitles[spot][0] = 0;
	authornames[spot][0] = 0;
	players[spot] = 0;
    if (!file)
		return;
	char c = fgetc(file);
	int i = 0;
	while (i < 300 && c != '\n' && c != EOF)
	{
		projecttitles[spot][i] = c;
		i++;
		c = fgetc(file);
	}
	projecttitles[spot][i] = 0;
	print_error(projecttitles[spot]);
	if (c != '\n')
	{
    	fclose(file);
		return;
	}
	c = fgetc(file);
	i = 0;
	while (i < 300 && c != '\n' && c != EOF)
	{
		authornames[spot][i] = c;
		i++;
		c = fgetc(file);
	}
	authornames[spot][i] = 0;
	c = fgetc(file);
	if (c >= '0' && c < '5')
		players[spot] = c - '0';
    fclose(file);
}

void update_save_states(char *save_states, time_t* dates, int* players)
{
	int i = 0;
	projecttitles[0][0] = 0;
	while (i < 10)
	{
		save_states[i] = is_same_game(i);
		dates[i] = get_date(i);
		get_game_info(i, players);
		i++;
	}
}

int save_choice()
{
	int size_screen = 1104 - software_mode * 400;
	int x = 0;
	int t = 0;
	char number[2] = {0};
	char save_states[10] = {0};
	projecttitles[0][0] = 0;
	int players[10] = {0};
	time_t save_dates[10];
	update_save_states(save_states, save_dates, players);
	clean_inputs();
	while (!inputs[5] && !inputs[0] && !inputs[6])
	{
		rect(0, 0, size_screen, 704, 0, 0, 0);
		int y = 0;
		while (y < 5 && x + y < 10)
		{
			number[0] = '0' + x + y;
			print_text(50, 110 + y * 100, number, 1, 1 + (t == x + y));
			if (save_states[x + y] == 0)
				print_text(80, 130 + y * 100, "AUCUNE DONNEE", 0, 5);
			else
			{
				print_text(50, 170 + y * 100, ctime(save_dates + x + y), 1, 6);
				print_text(150, 110 + y * 100, projecttitles[x + y], 1, 1);
				print_text(150, 140 + y * 100, authornames[x + y], 1, 1);
				display_sprite(4, 80, 105 + y * 100, 64, players[x + y], 0);
				//rect(150, 110 + y * 70, 50, 25, (save_states[x + y] == 1) * 255, (save_states[x + y] == 2) * 255, 0);
			}
			if (x + y != 9)
				rect(50, 200 + y * 100, 604, 4, 150, 150, 150);
			y++;
		}
		print_text_centered(0, 20, "Selection de sauvegarde", 1, 1, size_screen);
		rect(0, 620, 1104, 8, 255, 255, 255);
		print_text_centered(0, 640, "Selectionner :", 1, 1, size_screen/3);
		print_text_centered(1104/3, 640, "Actualiser :", 1, 1, size_screen/3);
		print_text_centered(2*1104/3, 640, "Retour :", 1, 1, size_screen/3);
		print_text_centered(0, 670, get_key_name(4), 1, 1, size_screen/3);
		print_text_centered(1104/3, 670, get_key_name(9), 1, 1, size_screen/3);
		print_text_centered(2*1104/3, 670, get_key_name(5), 1, 1, size_screen/3);
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
				t = 9;
				x = 0;
				if (x + 4 < t)
					x = t - 4;
			}
		}
		if (inputs[2])
		{
			if (t + 1 < 10)
			{
				t++;
				if (x + 3 < t && t + 1 < 10)
					x = t - 3;
			}
			else
			{
				t = 0;
				x = 0;
			}
		}
		if (inputs[10])
			update_save_states(save_states, save_dates, players);
	}
	return t;
}

void save_choice_for_load()
{
	clean_inputs();
	while (!inputs[6] && !inputs[0])
	{
		int choice = save_choice();
		if (inputs[5])
		{
			char state = is_same_game(choice);
			if (state == 0)
			{
				print_error("Save unavailable.");
				clean_inputs();
			}
			if (state == 1)
			{
				print_error("Save taken for another game.");
				clean_inputs();
			}
			if (state == 2)
			{
				open_save(choice);
				inputs[6] = 1;
			}
			inputs[5] = 1;
		}
	}
	inputs[6] = inputs[5];
}

void save_choice_for_save()
{
	clean_inputs();
	while (!inputs[6] && !inputs[0])
	{
		int choice = save_choice();
		if (inputs[5])
		{
			save_game(choice);
			inputs[6] = 1;
			inputs[5] = 1;
		}
	}
	inputs[6] = inputs[5];
} 

void save_choice_for_load_with_game()
{
	clean_inputs();
	while (!inputs[6] && !inputs[0])
	{
		int choice = save_choice();
		if (inputs[5])
		{
			char state = is_same_game(choice);
			if (state == 0)
			{
				print_error("Save unavailable.");
				clean_inputs();
			}
			else
			{
				char author[101] = {0};
				char title[301] = {0};
				int i = 0;
				while (projecttitles[choice][i])
				{
					title[i] = projecttitles[choice][i];
					i++;
				}
				i = 0;
				while (authornames[choice][i])
				{
					author[i] = authornames[choice][i];
					i++;
				}
				i = 0;
				project_data.author_name = author;
				project_data.project_name = title;
				if (open_project(&project_data))
				{
					launch_game(1, choice);
					free_project(project_data);
				}
				inputs[6] = 1;
			}
			inputs[5] = 1;
		}
	}
	inputs[6] = inputs[5];
}

void open_latest_game()
{
	int i = 1;
	int spot = 0;
	char is_valid = is_same_game(0);
	time_t date = get_date(0);
	while (i < 10)
	{
		time_t new_date = get_date(i);
		if (date < new_date && is_same_game(i))
		{
			date = new_date;
			spot = i;
		}
		i++;
	}
	if (!is_valid)
	{
		print_error("No saves yet !");
		return;
	}
	clean_inputs();
	while (!inputs[6] && !inputs[0])
	{
		int choice = spot;
		char state = is_same_game(choice);
		if (state == 0)
		{
			print_error("Save unavailable.");
			clean_inputs();
		}
		else
		{
			int players[10];
			get_game_info(choice, players);
			char author[101] = {0};
			char title[301] = {0};
			int i = 0;
			while (projecttitles[choice][i])
			{
				title[i] = projecttitles[choice][i];
				i++;
			}
			i = 0;
			while (authornames[choice][i])
			{
				author[i] = authornames[choice][i];
				i++;
			}
			i = 0;
			print_error(author);
			print_error(title);
			project_data.author_name = author;
			project_data.project_name = title;
			if (open_project(&project_data))
			{
				launch_game(1, choice);
				free_project(project_data);
			}
			inputs[6] = 1;
			inputs[5] = 1;
		}
	}
	inputs[6] = inputs[5];
}

void get_older_spot()
{
	int i = 1;
	int spot = 0;
	time_t date = get_date(0);
	while (i < 10)
	{
		time_t new_date = get_date(i);
		if (date > new_date)
		{
			date = new_date;
			spot = i;
		}
		i++;
	}
	save_game(spot);
}

void get_younger_spot()
{
	int i = 1;
	int spot = 0;
	char is_valid = (is_same_game(0) == 2);
	time_t date = get_date(0);
	while (i < 10)
	{
		time_t new_date = get_date(i);
		if (date < new_date && is_same_game(i) == 2)
		{
			date = new_date;
			spot = i;
		}
		i++;
	}
	if (is_valid)
		open_save(spot);
	else
		print_error("No saves yet !");
}