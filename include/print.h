#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "tools.h"
#include "input.h"
#include "structures.h"

char software_mode;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* sprites[7];
SDL_Texture* screen_save;
SDL_Texture* intermediate_window;
TTF_Font* police[2];
const SDL_Color text_colors[7];
char frame;

//Basic functions
char print_init(char* police_path); //Adapt depending of game or editor
void print_refresh();
void print_close();
char load_sprites(char* paths[]);
void free_sprites();

void copy_current_screen();
void paste_last_screen();

//Tool functions
void rect(int x, int y, int w, int h, int r, int g, int b);
void rect_alpha(int x, int y, int w, int h, int r, int g, int b, int alpha);
void cursor(int x, int y, int w, int h, int r, int g, int b); //TODO
void print_error(const char* text); //TODO
void print_error_int(int value);

//Text functions
void print_text(int x, int y, const char* text, int size, int color);
void print_int(int x, int y, long long value, int to_fill, int size, int color);
void print_text_centered(int x, int y, const char* text, int size, int color, int length);
void print_int_centered(int x, int y, long long value, int to_fill, int size, int color, int length);

//Sprite functions
void display_sprite(int type, int x, int y, int size, int column, int line);
void display_cardsprite(int x, int y, int id);
void print_characters(int x, int y, int* requests, int delayX, int delayY, int visibility, char* savemap, int length);

//Map functions
int angle_type(char top, char right, char diagonal);
void display_map(int x, int y, int* map, int visibility, char* savemap, int* items, int* item_values, int* visible, int* requests);
void display_map_full(int x, int y, struct map map, char print_item);
void display_minimap(int x, int y, struct map map, char **visible_map);
void display_minimap_full(int x, int y, struct map map);
void display_littlemap(int x, int y, int* map, char* character_map, int* items, int* item_values, int mode);

char check_choice(char* title, char* author);