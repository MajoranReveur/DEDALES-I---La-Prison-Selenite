#pragma once

#include <stdlib.h>

char file_loaded;
char file_saved;
struct project project_data;

struct position
{
    int zone; //0 : In an inventory
    int map;
    int x;
    int y;
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
5 - Number of maps of the Entrepot
6 - Number of maps of the Keys
7 - Number of maps of the Nightmares
8 - Number of maps of the Dreams
9 - Number of maps of the Oubliettes
10 - Number of requests from Nihil
11 - Number of requests from Sarah
12 - Number of requests from Emma
13 - Number of requests from Mathias
14 - Number of requests from Saihtam
15 - Number of containers items
*/

struct map
{
    int x;
    int y;
    int** cells;
    struct item **items;
};

struct project
{
    char* project_name;
    char* author_name;
	int parameters[16];
    struct position character_positions[5]; //First positions of each character
	struct request *requests[5]; //Requests for each of the 5 characters
    struct item inventories[50][5]; //Inventories for each of the 5 characters
    struct container *containers; //Content of items which have other items inside
	struct map *maps[5]; //Maps of the 5 zones
	char modified; //True if the project has been modified since the last save
    char valid; //True if the project was valid at the last analysis
};

void free_container(struct container c);
void free_map(struct map r);
void free_project(struct project p);
void delete_container(struct project p, long ID);

void add_item(struct container container, int value, int type);
void remove_item(struct container container, int value);
