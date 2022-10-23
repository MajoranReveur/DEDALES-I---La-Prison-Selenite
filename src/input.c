#include "input.h"

const Uint32 DELAY = 20;
const SDL_Keycode CONTROLS[9] = { SDLK_SPACE, SDLK_RETURN, SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_BACKSPACE, SDLK_w, SDLK_f };
char inputs[10] = { 0,0,0,0,0,0,0,0,0,0 };

void load_input()
{
	SDL_Event ev;
	Uint32 start_time = SDL_GetTicks();
	do
	{
		SDL_WaitEventTimeout(&ev, 10);
		if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_CLOSE)
			ev.type = SDL_QUIT;
		if (ev.type == SDL_QUIT)
		{
			start_time -= DELAY;
			inputs[9] = 1;
		}
		else if (ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP)
		{
			int i = 0;
			while (i < 9)
			{
				if (ev.key.keysym.sym == CONTROLS[i])
					inputs[i] = ev.type == SDL_KEYDOWN;
				i++;
			}
		}
	} while (SDL_GetTicks() < start_time + DELAY);
}

void load_input_long()
{
	SDL_Event ev;
	Uint32 start_time = SDL_GetTicks();
	do
	{
		SDL_WaitEvent(&ev);
		if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_CLOSE)
			ev.type = SDL_QUIT;
		if (ev.type == SDL_QUIT)
		{
			start_time -= DELAY;
			inputs[9] = 1;
		}
		else if (ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP)
		{
			int i = 0;
			while (i < 9)
			{
				if (ev.key.keysym.sym == CONTROLS[i])
					inputs[i] = ev.type == SDL_KEYDOWN;
				i++;
			}
		}
	} while (SDL_GetTicks() < start_time + DELAY);
}

void string_input(char* rep, size_t length, char *instruction)
{
	size_t len = 0;
	size_t position = 0;
	size_t camera = 0;
	SDL_Event event;
	char ending = (inputs[9] != 0);
	char single_letter[2];
	single_letter[1] = 0;
	rect(0, 0, 1104, 704, 0, 0, 0);
	print_text(20, 600, "Actuel :", 1, 1);
	print_text(20, 650, "Maximum :", 1, 1);
	print_text(20, 20, instruction, 1, 1);
	while (!ending)
	{
		rect(0, 100, 1104, 50, 0, 0, 0);
		size_t i = 0;
		while (i + camera < len && i < 61)
		{
			single_letter[0] = rep[i + camera];
			print_text(18*i + 4, 100, single_letter, 1, 1);
			i++;
		}
		rect((position-camera) * 18, 100, 4, 40, 255, 255, 255);
		rect(270, 600, 434, 104, 0, 0, 0);
		print_int(270, 600, len, 0, 1, 1);
		print_int(270, 650, length, 0, 1, 1);
		print_refresh();
		SDL_WaitEvent(&event);
		if (event.type == SDL_QUIT)
		{
			ending = 1;
			inputs[9] = 1;
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
			char* letter = event.text.text;
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
		inputs[9] = 1;
}

int int_input(int n, int x, int y)
{
	int dx = 100;
	int i = 0;
	char number[2] = { 0, 0 };
	inputs[0] = 0;
	inputs[1] = 0;
	while (!inputs[0] && !inputs[1] && !inputs[9])
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
			print_text(x + 15 * j, y, number, 1, (inputs[0] || inputs[1]));
			delta *= 10;
		}
		if (inputs[4] && dx != 100)
		{
			dx *= 10;
			i--;
		}
		if (inputs[5] && dx != 1)
		{
			dx /= 10;
			i++;
		}
		if (inputs[2])
		{
			if ((n / dx) % 10 == 9)
				n -= dx * 9;
			else
				n += dx;
		}
		if (inputs[3])
		{
			if ((n / dx) % 10 == 0)
				n += dx * 9;
			else
				n -= dx;
		}
	}
	return n;
}