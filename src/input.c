#include "input.h"

const Uint32 DELAY = 20;
SDL_Keycode CONTROLS[24];
char inputs[25] = {0};
char in_options = 0;
char in_inventory = 0;
Uint32 start_time;

char* input_names[] = {
		"Haut",
		"Bas",
		"Gauche",
		"Droite",
		"Valider/Continuer",
		"Retour",
		"Ramasser",
		"Ouvrir le Menu",
		"Options",
		"Changer de Type",
		"Changer l'Affichage",
		"Eveil",
		"Sommeil",
		"Inventaire",
		"Ouvrir la Carte",
		"Carte Precedente",
		"Carte Suivante",
		"Quitter un Portail",
		"Liste des Requetes",
		"Sauvegarde Rapide",
		"Chargement Rapide",
		"Raccourci Objet 1",
		"Raccourci Objet 2",
		"Raccourci Objet 3"
	};

void default_inputs()
{
	CONTROLS[0] = SDLK_UP;
	CONTROLS[1] = SDLK_DOWN;
	CONTROLS[2] = SDLK_LEFT;
	CONTROLS[3] = SDLK_RIGHT;
	CONTROLS[4] = SDLK_SPACE;
	CONTROLS[5] = SDLK_RETURN;
	CONTROLS[6] = SDLK_SPACE;
	CONTROLS[7] = SDLK_RETURN;
	CONTROLS[8] = SDLK_ESCAPE;
	CONTROLS[9] = SDLK_BACKSPACE;
	CONTROLS[10] = SDLK_BACKSPACE;
	CONTROLS[11] = SDLK_LCTRL;
	CONTROLS[12] = SDLK_LSHIFT;
	CONTROLS[13] = SDLK_i;
	CONTROLS[14] = SDLK_BACKSPACE;
	CONTROLS[15] = SDLK_KP_MINUS;
	CONTROLS[16] = SDLK_KP_PLUS;
	CONTROLS[17] = SDLK_q;
	CONTROLS[18] = SDLK_r;
	CONTROLS[19] = SDLK_s;
	CONTROLS[20] = SDLK_c;
	CONTROLS[21] = SDLK_1;
	CONTROLS[22] = SDLK_2;
	CONTROLS[23] = SDLK_3;
	start_time = SDL_GetTicks();
}

void clean_inputs()
{
	int i = 1;
	while (i < 24)
	{
		inputs[i] = 0;
		i++;
	}
}

void load_input()
{
	SDL_Event ev;
	do
	{
		SDL_WaitEventTimeout(&ev, 10);
		if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_CLOSE)
			ev.type = SDL_QUIT;
		if (ev.type == SDL_QUIT)
		{
			start_time -= DELAY;
			inputs[0] = 1;
		}
		else if (ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP)
		{
			int i = 0;
			while (i < 23)
			{
				if (ev.key.keysym.sym == CONTROLS[i])
					inputs[i + 1] = ev.type == SDL_KEYDOWN;
				i++;
			}
		}
	} while (SDL_GetTicks() < start_time + DELAY);
	if (inputs[9] && !in_options)
	{
		in_options = 1;
		options_menu();
		in_options = 0;
	}
	if (inputs[20] && file_loaded && !file_saved)
	{
		if (software_mode == 0)
			save_project(project_data);
	}
	if (inputs[14] && software_mode == 1 && in_inventory == 0)
	{
		in_inventory = 1;
		inventory();
		in_inventory = 0;
	}
	start_time = SDL_GetTicks();
}

void load_input_long()
{
	SDL_Event ev;
	start_time = SDL_GetTicks();
	do
	{
		SDL_WaitEvent(&ev);
		if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_CLOSE)
			ev.type = SDL_QUIT;
		if (ev.type == SDL_QUIT)
		{
			start_time -= DELAY;
			inputs[0] = 1;
		}
		else if (ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP)
		{
			int i = 0;
			while (i < 24)
			{
				if (ev.key.keysym.sym == CONTROLS[i])
					inputs[i + 1] = ev.type == SDL_KEYDOWN;
				i++;
			}
		}
	} while (SDL_GetTicks() < start_time + DELAY);
	if (inputs[9] && !in_options)
	{
		in_options = 1;
		options_menu();
		in_options = 0;
	}
	if (inputs[20] && file_loaded)
	{
		if (software_mode == 0)
			save_project(project_data);
	}
	if (inputs[14] && software_mode == 1 && in_inventory == 0)
	{
		in_inventory = 1;
		inventory();
		in_inventory = 0;
	}
}

void string_input(char *rep, size_t length, char *instruction)
{
	int screen_size = 1104 - software_mode * 400;
	size_t len = 0;
	size_t position = 0;
	size_t camera = 0;
	SDL_Event event;
	char ending = (inputs[0] != 0);
	char single_letter[2];
	single_letter[1] = 0;
	rect(0, 0, 1104, 704, 0, 0, 0);
	print_text(20, 600, "Actuel :", 1, 1);
	print_text(20, 650, "Maximum :", 1, 1);
	print_text(20, 20, instruction, 1, 1);
	print_text(screen_size / 2, 600, "Valider :", 1, 1);
	print_text(screen_size / 2, 650, "Return", 1, 1);
	while (!ending)
	{
		rect(0, 100, 1104, 50, 0, 0, 0);
		size_t i = 0;
		while (i + camera < len && i < 61)
		{
			single_letter[0] = rep[i + camera];
			print_text(18 * i + 4, 100, single_letter, 1, 1);
			i++;
		}
		rect((position - camera) * 18, 100, 4, 40, 255, 255, 255);
		rect(270, 600, 434, 104, 0, 0, 0);
		print_int(270, 600, len, 0, 1, 1);
		print_int(270, 650, length, 0, 1, 1);
		print_refresh();
		SDL_WaitEvent(&event);
		if (event.type == SDL_QUIT)
		{
			ending = 1;
			inputs[0] = 1;
		}
		else if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_BACKSPACE && position)
			{
				position--;
				if (camera)
					camera--;
				len--;
				size_t i = position;
				while (i < len)
				{
					rep[i] = rep[i + 1];
					i++;
				}
				rep[len] = 0;
			}
			if (event.key.keysym.sym == SDLK_RETURN && len)
				ending = 2;
			if (event.key.keysym.sym == SDLK_LEFT && position)
				position--;
			if (event.key.keysym.sym == SDLK_RIGHT && position < len)
				position++;
			if (position <= camera && position)
				camera = position - 1;
			if (position <= camera && !position)
				camera = 0;
			if (position - camera > 60)
				camera = position - 60;
		}
		else if (event.type == SDL_TEXTINPUT)
		{
			char *letter = event.text.text;
			size_t l = 0;
			while (letter[l])
				l++;
			size_t l_copy = len + l < length ? l : length - len;
			l = len;
			while (l > position)
			{
				l--;
				rep[l + l_copy] = rep[l];
			}
			l = 0;
			while (l < l_copy)
			{
				rep[position + l] = event.text.text[l];
				l++;
			}
			len += l_copy;
			position += l_copy;
			rep[len] = 0;
		}
	}
	if (ending == 1)
		inputs[0] = 1;
}

int int_input(int n, int x, int y)
{
	int dx = 100;
	int i = 0;
	char number[2] = {0, 0};
	inputs[5] = 0;
	inputs[6] = 0;
	while (!inputs[5] && !inputs[6] && !inputs[0])
	{
		int j = 3;
		int delta = 1;
		while (j)
		{
			j--;
			number[0] = (n / delta) % 10 + 48;
			print_text(x + 15 * j, y, number, 1, 1 + (i == j));
			delta *= 10;
		}
		print_refresh();
		load_input_long();
		j = 3;
		delta = 1;
		while (j)
		{
			j--;
			number[0] = (n / delta) % 10 + 48;
			print_text(x + 15 * j, y, number, 1, (inputs[5] || inputs[6]));
			delta *= 10;
		}
		if (inputs[3] && dx != 100)
		{
			dx *= 10;
			i--;
		}
		if (inputs[4] && dx != 1)
		{
			dx /= 10;
			i++;
		}
		if (inputs[1])
		{
			if ((n / dx) % 10 == 9)
				n -= dx * 9;
			else
				n += dx;
		}
		if (inputs[2])
		{
			if ((n / dx) % 10 == 0)
				n += dx * 9;
			else
				n -= dx;
		}
	}
	return n;
}

char check_compatibility(int x, SDL_KeyCode key)
{
	char input_types[23][3] = 
	{
		{1, 1, 1},
		{1, 1, 1},
		{1, 1, 1},
		{1, 1, 1},
		{1, 0, 0},
		{1, 0, 0},
		{0, 0, 1},
		{0, 0, 1},
		{1, 1, 1},
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 1, 0},
		{0, 1, 0},
		{0, 0, 1},
		{0, 0, 1},
		{1, 1, 1},
		{1, 1, 1},
		{1, 0, 1},
		{1, 0, 1},
		{1, 0, 1}
	};
	int i = 0;
	while (i < 23)
	{
		if (key == CONTROLS[i])
		{
			if (i != x && ((input_types[i][0] && input_types[x][0]) 
			|| (input_types[i][1] && input_types[x][1]) 
			|| (input_types[i][2] && input_types[x][2])))
			{
				print_error("Conflit de touche !");
				print_error(input_names[i]);
				return 0;
			}
		}
		i++;
	}
	return 1;
}

const char* get_key_name(int i)
{
	if (i >= 0 && i < 23)
		return(SDL_GetKeyName(CONTROLS[i]));
	return("");
}

void input_editor()
{
	int screen_size = 1104 - software_mode * 400;
	int x = 0;
	int camera = 0;
	while (!inputs[0] && !inputs[9] && !inputs[6])
	{
		rect(0, 0, screen_size, 704, 0, 0, 0);
		print_text_centered(0, 20, "CONTROLES", 0, 1, screen_size);
		int i = 0;
		while (i < 12)
		{
			print_text_centered(0, 120 + 50 * i, input_names[camera + i], 1, 1 + (camera + i == x), screen_size / 2);
			print_text_centered(screen_size / 2, 120 + 50 * i, get_key_name(camera + i), 1, 1, screen_size / 2);
			i++;
		}
		print_refresh();
		load_input_long();
		if (inputs[1] && x)
		{
			x--;
			if (camera && camera == x)
				camera--;
		}
		if (inputs[2] && x < 23)
		{
			x++;
			if (camera < 11 && camera + 11 == x)
				camera++;
		}
		if (inputs[5])
		{
			i = 0;
			rect(0, 0, screen_size, 704, 0, 0, 0);
			print_text_centered(0, 20, "CONTROLES", 0, 1, screen_size);
			while (i < 12)
			{
				print_text_centered(0, 120 + 50 * i, input_names[camera + i], 1, 1 + (camera + i == x), screen_size / 2);
				if (x == camera + i)
					print_text_centered(screen_size / 2, 120 + 50 * i, "En attente...", 1, 1, screen_size / 2);
				else
					print_text_centered(screen_size / 2, 120 + 50 * i, SDL_GetKeyName(CONTROLS[camera + i]), 1, 1, screen_size / 2);
				i++;
			}
			print_refresh();
			SDL_Event ev;
			char new_input_done = 0;
			while (!new_input_done)
			{
				SDL_WaitEvent(&ev);
				if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_CLOSE) 
					ev.type = SDL_QUIT;
				if (ev.type == SDL_QUIT)
				{
					inputs[0] = 1;
					new_input_done = 1;
				}
				else if (ev.type == SDL_KEYDOWN)
				{
					if (check_compatibility(x, ev.key.keysym.sym))
						CONTROLS[x] = ev.key.keysym.sym;
					new_input_done = 1;
				}
			}
			inputs[5] = 0;
		}
	}
}