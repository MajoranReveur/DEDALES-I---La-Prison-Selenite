#include "print.h"

const SDL_Color text_colors[7] = {
	{0, 0, 0}, //0 - Black
	{255, 255, 255}, //1 - White
	{255, 0, 0}, //2 - Red
	{0, 255, 0}, //3 - Green
	{0, 0, 255}, //4 - Blue
	{255, 255, 0}, //5 - Yellow
	{100, 100, 100}, //6 - Gray
};

void copy_current_screen()
{
	SDL_SetRenderTarget(renderer, screen_save);
    SDL_RenderCopy(renderer, intermediate_window, NULL, NULL);
	SDL_SetRenderTarget(renderer, intermediate_window);
}

void paste_last_screen()
{
	SDL_RenderCopy(renderer, screen_save, NULL, NULL);
}

char print_init(char* police_path)
{
	frame = 0;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return 0;
	if (software_mode)
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
    		SDL_RenderPresent(renderer);
			intermediate_window = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1104 - software_mode * 400, 704);
			screen_save = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1104 - software_mode * 400, 704);
			if (intermediate_window && screen_save)
			{
				SDL_SetRenderTarget(renderer, intermediate_window);
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
			}
			if (intermediate_window)
				SDL_DestroyTexture(intermediate_window);
			if (screen_save)
				SDL_DestroyTexture(screen_save);
			SDL_DestroyRenderer(renderer);
		}
		SDL_DestroyWindow(window);
	}
	return 0;
}

void print_refresh()
{
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, intermediate_window, NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_SetRenderTarget(renderer, intermediate_window);
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

void print_text(int x, int y, const char* text, int size, int color)
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

void print_text_centered(int x, int y, const char* text, int size, int color, int length)
{
	SDL_Surface* surface = TTF_RenderText_Solid(police[size], text, text_colors[color]);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	int w = 0;
	int h = 0;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_Rect dest = { x,y,w,h };
	dest.x = x + (length - dest.w) / 2;
	SDL_RenderCopy(renderer, texture, NULL, &dest);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void print_int(int x, int y, long long value, int to_fill, int size, int color)
{
	char number[20];
	int_to_str(number, value, to_fill);
	print_text(x, y, number, size, color);
}

void print_int_centered(int x, int y, long long value, int to_fill, int size, int color, int length)
{
	char number[20];
	int_to_str(number, value, to_fill);
	print_text_centered(x, y, number, size, color, length);
}

void print_error(const char* text)
{
	int screen_size = 1104 - 400 * software_mode;
	SDL_Surface* surface = TTF_RenderText_Solid(police[1], text, text_colors[2]);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	int w = 0;
	int h = 0;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_Rect dest = { 0,300,w,h };
	dest.x = (screen_size - dest.w) / 2;
	rect(dest.x - 10, dest.y - 10, dest.w + 20, dest.h + 20, 255, 0, 0);
	rect(dest.x - 5, dest.y - 5, dest.w + 10, dest.h + 10, 0, 0, 0);
	SDL_RenderCopy(renderer, texture, NULL, &dest);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	clean_inputs();
	print_refresh();
	while (!inputs[0] && !inputs[5])
		load_input_error(); //Won't allow to save, open the menu, the inventory...
	clean_inputs();
}

void print_error_int(int value)
{
	char number[30] = { 0 };
	int_to_str(number, value, 0);
	print_error(number);
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

void print_notif(struct notification notif, int y)
{
	SDL_Surface* surface = TTF_RenderText_Solid(police[1], notif.message, text_colors[1]);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	int w = 0;
	int h = 0;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_Rect dest = { 5, y + 5, w, h };
	rect_alpha(dest.x - 5, dest.y - 5, dest.w + 10, dest.h + 10, 150, 150, 150, 150);
	SDL_RenderCopy(renderer, texture, NULL, &dest);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void display_sprite(int type, int x, int y, int size, int column, int line)
{
	SDL_Rect dest = { x, y, size, size };
	SDL_Rect source = { column * size, line * size, size, size };
	SDL_RenderCopy(renderer, sprites[type], &source, &dest);
}

int angle_type(char top, char right, char diagonal)
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

void display_minimap_knowledge(int x, int y, int map, int zone, int player)
{
	rect(0, 0, 704, 704, 0, 0, 0);
    if (zone < 1 || zone > project_data.parameters[11])
        return;
    if (map < 0 || map >= project_data.zones[zone - 1].map_number)
		return;
	struct map m = project_data.zones[zone - 1].maps[map];
	int i = 0;
	while (i < 88 && x + i < m.x)
	{
		int j = 0;
		while (j < 88 && y + j < m.y)
		{
			if (x + i >= 0 && y + j >= 0 && save_data.knowledge[player].zones[zone - 1].maps[map].cells[x + i][y + j])
				display_sprite(2, i * 8, j * 8, 8, m.cells[x+i][y+j], 0);
			j++;
		}
		i++;
	}
}

void display_minimap_full(int x, int y, struct map map)
{
	rect(0, 0, 704, 704, 0, 0, 0);
	int i = 0;
	while (i < 88 && x + i < map.x)
	{
		int j = 0;
		while (j < 88 && y + j < map.y)
		{
			if (x + i >= 0 && y + j >= 0)
				display_sprite(2, i * 8, j * 8, 8, map.cells[x+i][y+j], 0);
			j++;
		}
		i++;
	}
}

void cursor(int x, int y, int w, int h, int r, int g, int b)
{
	rect(x + 4, y, w - 8, 8, r, g, b);
	rect(x + 4, y + h - 8, w - 8, 8, r, g, b);
	rect(x, y + 4, 8, h - 8, r, g, b);
	rect(x + w - 8, y + 4, 8, h - 8, r, g, b);
}

void display_map_cells(int x, int y, struct map map)
{
	int dx = x % 8;
	int dy = y % 8;
	x = (x - dx) / 8;
	y = (y - dy) / 8;
	rect(0, 0, 704, 704, 0, 0, 0);
	int i = 0;
	while (i < 12 && x + i < map.x)
	{
		int j = 0;
		while (j < 12 && y + j < map.y)
		{
			if (x + i >= 0 && y + j >= 0)
			{
				int a = x + i;
				int b = y + j;
				char top = 0;
				char topleft = 0;
				char topright = 0;
				char left = 0;
				char right = 0;
				char downleft = 0;
				char downright = 0;
				char down = 0;
				if (b > 0)
				{
					top = map.cells[a][b-1] == map.cells[a][b];
					if (a > 0)
						topleft = map.cells[a-1][b-1] == map.cells[a][b];
					if (a < map.x - 1)
						topright = map.cells[a+1][b-1] == map.cells[a][b];
				}
				if (b < map.y - 1)
				{
					down = map.cells[a][b+1] == map.cells[a][b];
					if (a > 0)
						downleft = map.cells[a-1][b+1] == map.cells[a][b];
					if (a < map.x - 1)
						downright = map.cells[a+1][b+1] == map.cells[a][b];
				}
				if (a > 0)
					left = map.cells[a-1][b] == map.cells[a][b];
				if (a < map.x - 1)
					right = map.cells[a+1][b] == map.cells[a][b];
				int print_x = i * 64 - dx * 8;
				int print_y = j * 64 - dy * 8;
				display_sprite(0, print_x, print_y, 32, map.cells[a][b] * 2, angle_type(top, left, topleft) * 2);
				display_sprite(0, print_x + 32, print_y, 32, map.cells[a][b] * 2 + 1, angle_type(top, right, topright) * 2);
				display_sprite(0, print_x, print_y + 32, 32, map.cells[a][b] * 2, angle_type(down, left, downleft) * 2 + 1);
				display_sprite(0, print_x + 32, print_y + 32, 32, map.cells[a][b] * 2 + 1, angle_type(down, right, downright) * 2 + 1);
			}
			j++;
		}
		i++;
	}
}

void display_map_items(int x, int y, struct map map, int player)
{
	int dx = x % 8;
	int dy = y % 8;
	x = (x - dx) / 8;
	y = (y - dy) / 8;
	int i = 0;
	while (i < 12 && x + i < map.x)
	{
		int j = 0;
		while (j < 12 && y + j < map.y)
		{
			if (x + i >= 0 && y + j >= 0)
			{
				int a = x + i;
				int b = y + j;
				int print_x = i * 64 - dx * 8;
				int print_y = j * 64 - dy * 8;
				if (map.items[a][b].type)
				{
					if (map.items[a][b].type == 16)
					{
						if (perception[player][0])
							display_sprite(3, print_x, print_y, 64, map.items[a][b].type - 1, 0);
					}
					else
						display_sprite(3, print_x, print_y, 64, map.items[a][b].type - 1, 0);
				}
			}
			j++;
		}
		i++;
	}
}

void display_map_characters(int x, int y, int map, int zone)
{
	int i = 0;
	print_int(0, 0, project_data.character_positions[0].zone, 0, 1, 2);
	print_int(0, 50, zone, 0, 1, 2);
	while (i < 5)
	{
		if (project_data.character_positions[i].zone == zone)
		{
			if (project_data.character_positions[i].map == map)
				display_sprite(4, (project_data.character_positions[i].x - x) * 8,  (project_data.character_positions[i].y - y) * 8 - 16, 64,
				i * 4 + project_data.character_positions[i].orientation, project_data.character_positions[i].x % 8 + project_data.character_positions[i].y % 8);
			
		}
		i++;
	}
}

void display_map_shadow_character(int x, int y, int map, int zone, int player)
{
    if (zone < 1 || zone > project_data.parameters[11])
        return;
    if (map < 0 || map >= project_data.zones[zone - 1].map_number)
		return;
	int dx = x % 8;
	if (x < 0)
	{
		dx = x;
		while (dx < 0)
			dx += 8;
		dx %= 8;
	}
	int dy = y % 8;
	if (y < 0)
	{
		dy = y;
		while (dy < 0)
			dy += 8;
		dy %= 8;
	}
	x = (x - dx) / 8;
	y = (y - dy) / 8;
	struct position p = project_data.character_positions[player];
	struct map m = project_data.zones[zone - 1].maps[map];
	p.x = p.x / 8 + (p.x % 8 > 4);
	p.y = p.y / 8 + (p.y % 8 > 4);
	int i = 0;
	while (i < 12 && x + i < m.x)
	{
		int	j = 0;
		while (j < 12 && y + j < m.y)
		{
			if (x + i >= 0 && y + j >= 0)
			{
				int distancex = x + i - p.x;
				int distance = distancex;
				if (distancex < 0)
					distance = -distancex;
				int distancey = y + j - p.y;
				if (distancey < 0)
					distance -=distancey;
				else
					distance += distancey;
				int base = 0;
				if (distance == 3)
				{
					if (p.orientation < 2 && distancey <= 0 && dy < 5)
						base = dy * 32;
					if (p.orientation < 2 && distancey > 0 && dy > 4)
						base = 255 - dy * 32;
					if (p.orientation > 1 && distancex <= 0 && dx < 5)
						base = dx * 32;
					if (p.orientation > 1 && distancex > 0 && dx > 4)
						base = 255 - dx * 32;
				}
				if (distance > 3)
					base = 255;
				if (distance == 4 && distancex && distancey)
				{
					if (p.orientation < 2 && distancey <= 0 && dy > 4)
						base = dy * 32;
					if (p.orientation < 2 && distancey > 0 && dy < 5)
						base = 255 - dy * 32;
					if (p.orientation > 1 && distancex <= 0 && dx > 4)
						base = dx * 32;
					if (p.orientation > 1 && distancex > 0 && dx < 5)
						base = 255 - dx * 32;
				}
				if (is_in_map(x+i, y+j, map, zone))
				{
					char knowledge_of_cell = save_data.knowledge[player].zones[zone - 1].maps[map].cells[x + i][y + j];
					if (knowledge_of_cell == 0)
						base = 255;
					if (knowledge_of_cell == 1)
						base = 128 + base / 2;
				}
				if (base >= 256)
					base = 255;
				rect_alpha(i * 64 - dx * 8, j * 64 - dy * 8, 64, 64, 0, 0, 0, base);
				/*
				if (distance > 3)
					rect(i * 64 - dx * 8, j * 64 - dy * 8, 64, 64, 0, 0, 0);
				else
				{
				}*/
			}
			j++;
		}
		i++;
	}
}

void display_littlemap(int x, int y, struct map map)
{
	rect(0, 0, 704, 704, 0, 0, 0);
	int i = 0;
	while (i < 44 && x + i < map.x)
	{
		int j = 0;
		while (j < 44 && y + j < map.y)
		{
			if (x + i >= 0 && y + j >= 0)
				display_sprite(1, i * 16, j * 16, 16, map.cells[x + i][y + j], 0);
			j++;
		}
		i++;
	}
}

void display_littlemap_knowledge(int x, int y, int map, int zone, int player)
{
	rect(0, 0, 704, 704, 0, 0, 0);
    if (zone < 1 || zone > project_data.parameters[11])
        return;
    if (map < 0 || map >= project_data.zones[zone - 1].map_number)
		return;
	struct map m = project_data.zones[zone - 1].maps[map];
	int i = 0;
	while (i < 44 && x + i < m.x)
	{
		int j = 0;
		while (j < 44 && y + j < m.y)
		{
			if (x + i >= 0 && y + j >= 0 && save_data.knowledge[player].zones[zone - 1].maps[map].cells[x + i][y + j])
				display_sprite(1, i * 16, j * 16, 16, m.cells[x + i][y + j], 0);
			j++;
		}
		i++;
	}
}

void display_cardsprite(int x, int y, int id)
{
	SDL_Rect dest = { x, y, 32, 64 };
	SDL_Rect source = { id * 32, 0, 32, 64 };
	SDL_RenderCopy(renderer, sprites[6], &source, &dest);
}

char check_choice(char* title, char* author)
{
	int size_screen = 1104 - software_mode * 400;
	SDL_Surface* surfacetitle = TTF_RenderText_Solid(police[1], title, text_colors[1]);
	SDL_Surface* surfaceauthor = TTF_RenderText_Solid(police[1], author, text_colors[1]);
	SDL_Texture* texturetitle = SDL_CreateTextureFromSurface(renderer, surfacetitle);
	SDL_Texture* textureauthor = SDL_CreateTextureFromSurface(renderer, surfaceauthor);
	int wtitle = 0;
	int htitle = 0;
	SDL_QueryTexture(texturetitle, NULL, NULL, &wtitle, &htitle);
	int wauthor = 0;
	int hauthor = 0;
	SDL_QueryTexture(textureauthor, NULL, NULL, &wauthor, &hauthor);
	SDL_Rect desttitle = {70, 250, wtitle, htitle};
	SDL_Rect destauthor = {70, 350, wauthor, hauthor};
	SDL_Rect srctitle = {0, 0, wtitle, htitle};
	SDL_Rect srcauthor = {0, 0, wauthor, hauthor};
	if (wtitle > size_screen - 70 * 2)
	{
		desttitle.w = size_screen - 70 * 2;
		srctitle.w = size_screen - 70 * 2;
	}
	if (wauthor > size_screen - 70 * 2)
	{
		destauthor.w = size_screen - 70 * 2;
		srcauthor.w = size_screen - 70 * 2;
	}
	rect(50, 100, size_screen - 50 * 2, 400, 255, 255, 255);
	clean_inputs();
	int x = 0;
	while (!inputs[0] && !inputs[5] && !inputs[6])
	{
		rect(58, 108, size_screen - 58 * 2, 384, 0, 0, 0);
		print_text_centered(58, 120, "Choisir ce fichier ?", 1, 1, size_screen - 58 * 2);
		print_text(70, 220, "Nom :", 1, 1);
		print_text(70, 320, "De :", 1, 1);
		print_text_centered(70, 400, "Oui", 1, 1 + (x == 0), size_screen - 70 * 2);
		print_text_centered(70, 430, "Non", 1, 1 + (x == 1), size_screen - 70 * 2);
		SDL_RenderCopy(renderer, texturetitle, &srctitle, &desttitle);
		SDL_RenderCopy(renderer, textureauthor, &srcauthor, &destauthor);
		print_refresh();
		load_input_long();
		if (inputs[3] && srctitle.x)
		{
			if (srctitle.x > 50)
				srctitle.x -= 50;
			else
				srctitle.x = 0;
		}
		if (inputs[3] && srcauthor.x)
		{
			if (srcauthor.x > 50)
				srcauthor.x -= 50;
			else
				srcauthor.x = 0;
		}
		if (inputs[4] && srctitle.x + (size_screen - 8 - 70 * 2) - 50 < wtitle)
		{
			if (srctitle.x + (size_screen - 8 - 70 * 2) < wtitle)
				srctitle.x += 50;
			else
				srctitle.x = wtitle - (size_screen - 8 - 70 * 2);
		}
		if (inputs[4] && srcauthor.x + (size_screen - 8 - 70 * 2) - 50 < wauthor)
		{
			if (srcauthor.x + (size_screen - 8 - 70 * 2) < wauthor)
				srcauthor.x += 50;
			else
				srcauthor.x =  wauthor - (size_screen - 8 - 70 * 2);
		}
		if (inputs[1] || inputs[2])
			x = !x;
	}
	SDL_DestroyTexture(texturetitle);
	SDL_FreeSurface(surfacetitle);
	SDL_DestroyTexture(textureauthor);
	SDL_FreeSurface(surfaceauthor);
	inputs[6] = 0;
	return (inputs[5] && x == 0);
}

void display_message(char* message)
{
	char line[301];
	int i = 0;
	while (message[i])
	{
		rect(5, 604, 694, 90, 0, 0, 0);
		int number_line = 0;
		while (message[i] && number_line < 3)
		{
			int j = 0;
			while (message[i + j] != '\n' && message[i + j] && j < 300)
			{
				line[j] = message[i + j];
				j++;
			}
			line[j] = 0;
			print_text(10, 609 + number_line * 30, line, 1, 1);
			i+=j;
			if (message[i] == '\n')
			{
				number_line += 1;
				i++;
			}
		}
		clean_inputs();
		print_refresh();
		while (!inputs[0] && !inputs[5])
			load_input_error();
	}
	clean_inputs();
}

int ask_question(char* message, char** options, int number_options)
{
	char line[301];
	int i = 0;
	rect(5, 604, 694, 90, 0, 0, 0);
	int number_line = 0;
	while (message[i] && number_line < 3)
	{
		int j = 0;
		while (message[i + j] != '\n' && message[i + j] && j < 300)
		{
			line[j] = message[i + j];
			j++;
		}
		line[j] = 0;
		print_text(10, 609 + number_line * 30, line, 1, 1);
		i+=j;
		if (message[i] == '\n')
		{
			number_line += 1;
			i++;
		}
	}
	
	int max_len = 0;
	i = 0;
	while (i < number_options)
	{
		SDL_Surface* surface = TTF_RenderText_Solid(police[1], options[i], text_colors[2]);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		int w = 0;
		int h = 0;
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface);
		if (max_len < w)
			max_len = w;
		i++;
	}
	clean_inputs();
	int choice = 0;
	while (!inputs[5] && !inputs[0])
	{
		i = 0;
		rect(689 - max_len, 594 - number_options * 30, max_len + 10, number_options * 30 + 10, 0, 0, 0);
		while (i < number_options)
		{
			print_text(694 - max_len, 599 - number_options * 30 + i * 30, options[i], 1, 1 + (choice == i));
			i++;
		}
		print_refresh();
		load_input_error();
		if (inputs[1])
			choice = (choice + (number_options - 1)) % number_options;
		if (inputs[2])
			choice = (choice + 1) % number_options;
	}
	
	clean_inputs();
	return choice;
}