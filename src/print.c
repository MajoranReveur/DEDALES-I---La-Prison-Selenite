#include "print.h"
#include "input.h"

const SDL_Color text_colors[6] = {
	{0, 0, 0},
	{255, 255, 255},
	{255, 0, 0},
	{0, 255, 0},
	{0, 0, 255},
	{255, 255, 0}
};

char print_init(char* police_path, char mode)
{
	frame = 0;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return 0;
	if (mode)
		window = SDL_CreateWindow("LPS - Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 704, 704, 0);
	else
		window = SDL_CreateWindow("LPS - Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1104, 704, 0); 
	if (window)
	{
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			TTF_Init();
			police[0] = TTF_OpenFont(police_path, 40);
			police[1] = TTF_OpenFont(police_path, 20);
			int i = 0;
			while (i < 7)
			{
				sprites[i] = NULL;
				i++;
			}
			if (police[0] != NULL && police[1] != NULL)
				return 1;
			TTF_CloseFont(police[0]);
			TTF_CloseFont(police[1]);
			TTF_Quit();
			SDL_DestroyRenderer(renderer);
		}
		SDL_DestroyWindow(window);
	}
	return 0;
}

void print_refresh()
{
	SDL_RenderPresent(renderer);
	frame = (frame + 1) % 16;
}

void print_close()
{
	TTF_CloseFont(police[0]);
	TTF_CloseFont(police[1]);
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

char load_sprites(char* paths[])
{
	int i = 0;
	char valid = 1;
	while (i < 7 && valid)
	{
		SDL_Surface* picture = SDL_LoadBMP(paths[i]);
		if (picture == NULL)
			valid = 0;
		else
		{
			SDL_DestroyTexture(sprites[i]);
			sprites[i] = SDL_CreateTextureFromSurface(renderer, picture);
			valid = sprites[i] != NULL;
		}
		SDL_FreeSurface(picture);
		i++;
	}
	if (!valid)
		free_sprites();
	return valid;
}

void free_sprites()
{
	int i = 0;
	while (i < 7)
	{
		SDL_DestroyTexture(sprites[i]);
		sprites[i] = NULL;
		i++;
	}
}

void print_text(int x, int y, char* text, int size, int color)
{
	SDL_Surface* surface = TTF_RenderText_Solid(police[size], text, text_colors[color]);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	int w = 0;
	int h = 0;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_Rect dest = { x,y,w,h };
	SDL_RenderCopy(renderer, texture, NULL, &dest);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void print_text_centered(int x, int y, char* text, int size, int color, int length)
{
	SDL_Surface* surface = TTF_RenderText_Solid(police[size], text, text_colors[color]);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	int w = 0;
	int h = 0;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_Rect dest = { x,y,w,h };
	dest.x = (length - dest.w) / 2;
	SDL_RenderCopy(renderer, texture, NULL, &dest);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void print_int(int x, int y, long long value, int printable, int size, int color)
{
	char number[20];
	int_to_str(number, value, printable);
	print_text(x, y, number, size, color);
}

void print_error(char* text)
{
	SDL_Surface* surface = TTF_RenderText_Solid(police[1], text, text_colors[2]);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	int w = 0;
	int h = 0;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_Rect dest = { 0,300,w,h };
	dest.x = (1104 - dest.w) / 2;
	rect(dest.x - 10, dest.y - 10, dest.w + 20, dest.h + 20, 255, 0, 0);
	rect(dest.x - 5, dest.y - 5, dest.w + 10, dest.h + 10, 0, 0, 0);
	SDL_RenderCopy(renderer, texture, NULL, &dest);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	inputs[0] = 0;
	print_refresh();
	while (!inputs[9] && !inputs[0])
		load_input();
	inputs[0] = 0;
}

void rect(int x, int y, int w, int h, int r, int g, int b)
{
	SDL_Rect rect;
	rect.x = x; rect.y = y; rect.w = w; rect.h = h;
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderFillRect(renderer, &rect);
}

void rect_alpha(int x, int y, int w, int h, int r, int g, int b, int alpha)
{
	SDL_Rect rect;
	rect.x = x; rect.y = y; rect.w = w; rect.h = h;
	SDL_SetRenderDrawColor(renderer, r, g, b, alpha);
	SDL_RenderFillRect(renderer, &rect);
}




void cursor(int x, int y, int w, int h, int r, int g, int b)
{
	rect(x + 5, y, w - 10, 10, r, g, b);
	rect(x + 5, y + h - 10, w - 10, 10, r, g, b);
	rect(x, y + 5, 10, h - 10, r, g, b);
	rect(x + w - 10, y + 5, 10, h - 10, r, g, b);
}

void display_sprite(int type, int x, int y, int size, int column, int line)
{
	SDL_Rect dest = { x, y, size, size };
	SDL_Rect source = { column * size, line * size, size, size };
	SDL_RenderCopy(renderer, sprites[type], &source, &dest);
}

int angle_from_neighbor(char top, char right, char diagonal)
{
	if (!top)
	{
		if (!right)
			return 0;
		return 2;
	}
	if (!right)
		return 1;
	if (!diagonal)
		return 3;
	return 4;
}

void display_map(int x, int y, int* map, int visibility, char* savemap, int* items, int* item_values, int* visible, int* requests)
{
	//visibility = 50;
	rect(0, 0, 704, 704, 0, 0, 0);
	int a = x % 8;
	int b = y % 8;
	x /= 8;
	y /= 8;
	int i = 0;
	while (i <= 11)
	{
		int j = 0;
		while (j <= 11)
		{
			int dx = i - (a > 4) - 5;
			int dy = j - (b > 4) - 5;
			if (dx < 0)
				dx = -dx;
			if (dy < 0)
				dy = -dy;
			char insums = (x - 5 + i >= 0 && x - 5 + i < *map);
			char incolumns = (y - 5 + j >= 0 && y - 5 + j < map[1]);
			size_t targetsave = *map * (y - 5 + j) + x - 5 + i;
			size_t target = 2 + targetsave;
			if (insums && incolumns && savemap[1 + targetsave] && dx + dy < visibility) //  && && savemap[targetsave]
			{
				char top = 0;
				char topleft = 0;
				char topright = 0;
				char left = 0;
				char right = 0;
				char downleft = 0;
				char downright = 0;
				char down = 0;
				if (targetsave >= map[0])
				{
					top = map[target - map[0]] == map[target];
					if (targetsave % map[0] != 0)
						topleft = map[target - 1 - map[0]] == map[target];
					if (targetsave % map[0] != map[0] - 1)
						topright = map[target + 1 - map[0]] == map[target];
				}
				if (targetsave < map[0] * (map[1] - 1))
				{
					down = map[target + map[0]] == map[target];
					if (targetsave % map[0] != 0)
						downleft = map[target - 1 + map[0]] == map[target];
					if (targetsave % map[0] != map[0] - 1)
						downright = map[target + 1 + map[0]] == map[target];
				}
				if (targetsave % map[0] != 0)
					left = map[target - 1] == map[target];
				if (targetsave % map[0] != map[0] - 1)
					right = map[target + 1] == map[target];
				int printx = i * 64 - a * 8;
				int printy = j * 64 - b * 8;
				display_sprite(0, printx, printy, 32, map[target] * 2, angle_from_neighbor(top, left, topleft) * 2);
				display_sprite(0, printx + 32, printy, 32, map[target] * 2 + 1, angle_from_neighbor(top, right, topright) * 2);
				display_sprite(0, printx, printy + 32, 32, map[target] * 2, angle_from_neighbor(down, left, downleft) * 2 + 1);
				display_sprite(0, printx + 32, printy + 32, 32, map[target] * 2 + 1, angle_from_neighbor(down, right, downright) * 2 + 1);
				/*if (y - 5 + j == 0 || map[target - *map] == 2)
					display_tilesprite(printx, printy - 64, 2);*/
				if (items[targetsave])
				{
					char printable = 1;
					if (item_values[3 * (items[targetsave] - 1)] == 3 && !visible[0])
						printable = 0;
					if (item_values[3 * (items[targetsave] - 1)] == 16 && !visible[1])
						printable = 0;
					if (printable)
						display_sprite(3, printx, printy, 64, item_values[3 * (items[targetsave] - 1)] - 1, frame / 2);
				}
				if (savemap[1 + targetsave] == 1)
					rect_alpha(printx, printy, 64, 64, 0, 0, 0, 150);
			}
			else
				rect(i * 64 - a * 8, j * 64 - b * 8, 64, 64, 0, 0, 0);
			j++;
		}
		i++;
	}
}

void display_minimap(int x, int y, int* map, char* character_map, int a, int b)
{
	rect(a, b, 704, 704, 0, 0, 0);
	int i = 0;
	while (i < 88 && x + i < *map)
	{
		int j = 0;
		while (j < 88 && y + j < map[1])
		{
			char insums = (x + i >= 0);
			char incolumns = (y + j >= 0);
			int target = 2 + (*map * (y + j) + x + i);
			if (insums && incolumns && character_map[target - 1])
			{
				display_sprite(2, i * 8 + a, j * 8 + b, 8, map[target], 0);
			}
			j++;
		}
		i++;
	}
}

void display_littlemap(int x, int y, int* map, char* character_map, int* items, int* item_values, int mode)
{
	rect(0, 0, 704, 704, 0, 0, 0);
	int i = 0;
	while (i < 44 && x + i < *map)
	{
		int j = 0;
		while (j < 44 && y + j < map[1])
		{
			char insums = (x + i >= 0);
			char incolumns = (y + j >= 0);
			int target = *map * (y + j) + x + i;
			if (insums && incolumns && character_map[target + 1])
			{
				display_sprite(1, i * 16, j * 16, 16, map[target + 2], 0);
				if (items[target] && item_values[items[target] * 3 - 3] == mode)
					rect(i * 16, j * 16, 16, 16, 255, 0, 255);
			}
			j++;
		}
		i++;
	}
}

void print_characters(int x, int y, int* requests, int delayX, int delayY, int visibility, char* savemap, int length)
{
	int i = 0;
	while (i < 5)
	{
		if (requests[i * 3])
		{
			int dx = requests[i * 3] - 1 - x + (delayX > 4) - (delayX < -4);
			int dy = requests[i * 3 + 1] - 1 - y + (delayY > 4) - (delayY < -4);
			if (dx < 0)
				dx = -dx;
			if (dy < 0)
				dy = -dy;
			if (dx + dy < visibility && savemap[requests[i * 3] + (requests[i * 3 + 1] - 1) * length])
				display_sprite(4, 5 * 64 + (requests[i * 3] - 1 - x) * 64 + delayX * 8,
					5 * 64 + (requests[i * 3 + 1] - 1 - y) * 64 + delayY * 8,
					64, i * 4, 0);
		}
		i++;
	}
}

void display_cardsprite(int x, int y, int id, int frame)
{
	SDL_Rect dest = { x, y, 32, 64 };
	SDL_Rect source = { id * 32, 0, 32, 64 };
	SDL_RenderCopy(renderer, sprites[6], &source, &dest);
}