#pragma once

#include <stdlib.h>

char file_loaded;
char file_saved;
struct project project_data;
struct savedatas save_data;
struct backup backup_data;

struct position
{
    int zone; //0 : In an inventory/Undefined
    int map;
    int x;
    int y;
    int orientation;
};

struct item
{
    int type;
    int value;
    long ID; //Maximum : (999*999*5 + 250) containers, *100 with items inside. So basically around 500 million. Rather use long than size_t, which can be maxed at 65 335 only.
    char activation;
};

struct container
{
    struct position position; //For editor : only useful for suppression
    struct item* items;
    int size;
};

struct request
{
    struct position place; //For spawn : it can be change via cinematics (for Emma it is reinitialized at each map change)
    int orientation;
    int type; //0 - Talking, 1 - Show an item, 2 - Give an item, 3 - Surprise
    struct position destination; //For surprise requests
    struct item item_wanted; //For item requests
    int item_quantity; //For item requests
    char vision_field; //0 - Straight forward, 1 - On lines and rows, 2 - Diagonals
    struct item reward_item; //For containers : those who contains Candies, Dreams and Keys can only be empty
    int wanted_character; //0 : Everybody can do it
    char active; //True if it is efficient. If a playable character has an active request, it'll take effect as soon as the player choose another character (the character is no more playable then)
};

/*
Project parameters :
0 - Needed cinematics ?
1 - Basic Luminosity
2 - Oubliettes Luminosity
3 - Maximal Luminosity
4 - Initial Character
5 - Number of requests from Nihil
6 - Number of requests from Sarah
7 - Number of requests from Emma
8 - Number of requests from Mathias
9 - Number of requests from Saihtam
10 - Number of containers items
11 - Number of zones (cannot be less than 5)
*/

struct portal
{
    int type;
    int value;
    struct position last_position;
};

struct map_backup
{
    int** cells;
    struct item **items;
};

struct zone_backup
{
    struct map_backup *maps;
};

struct backup
{
    struct zone_backup *zones;
};

struct request_state
{
    char active;
    int value;
};

struct savedatas //The save will also contain the current map estates, the character positions (and their inventories) and the containers
{
    struct portal portals[5];
    struct position sleep_target;
    struct request_state *request_states;
};

struct map
{
    int x;
    int y;
    int x_start;
    int y_start;
    int remaining_green_cells;
    int initial_delay;
    int color_length; //Length of color_sequency
    int* color_sequency;
    int** cells;
    char** thoughts;
    struct item **items;
};

struct zone
{
    int map_number;
    struct map *maps;
};

struct project
{
    char* project_name;
    char* author_name;
	int parameters[12];
    struct position character_positions[5]; //First positions of each character
	struct request *requests[5]; //Requests for each of the 5 characters
    struct item inventories[50][5]; //Inventories for each of the 5 characters
    struct container *containers; //Content of items which have other items inside
	struct zone *zones; //Different zones
	char modified; //True if the project has been modified since the last save
    char valid; //True if the project was valid at the last analysis
};

void free_container(struct container c);
void free_map(struct map r);
void free_project(struct project p);
void delete_container(long ID);

void add_item(struct container container, struct item o);
void remove_item(struct container container, int value);
