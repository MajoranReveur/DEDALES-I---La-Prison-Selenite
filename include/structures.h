#pragma once

#include <stdlib.h>

char file_loaded;
char file_saved;
struct project project_data;
struct savedatas save_data;
struct backup backup_data;
char perception[10][5];

struct notification
{
    char message[301];
    int delay;
};

struct notification notifs[10];

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
    char marked; //for requests only, isn't saved
};

struct container
{
    struct position position; //For editor : only useful for suppression
    struct item* items;
    int size;
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
12 - Number of cinematics
*/

struct portal
{
    int type;
    int value;
    struct position last_position;
    struct position portal_position;
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

struct map_knowledge
{
    char** cells; //0 : unknown, 1 : seen, 2 : visited, 3 : Marked (for electrical armories)
    char has_map;
};

struct zone_knowledge
{
    struct map_knowledge *maps;
};

struct character_knowledge
{
    struct zone_knowledge *zones;
};

struct text
{
    int length;
    char* string;
};

struct mission
{
    int type;
    struct position p;
    struct position tp;
    int value1;
    int value2;
    int value3;
    int value4;
    char activated;
};

struct request
{
    struct mission objective;
    struct item reward;
};

struct cinematic_event
{
    int type;
    struct text dialog;
    int value1;
    int value2;
    int value3;
    int value4;
    struct position p;
    struct position target;
};

struct cinematic_trigger
{
    int type;
    struct position p;
    int value;
    long ID;
    char active;
};

struct cinematic
{
    int length;
    struct cinematic_event* events;
    struct cinematic_trigger trigger;
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
    struct character_knowledge knowledge[5];
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
    long** cinematic_triggers;
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
	int parameters[13];
    struct position character_positions[5]; //First positions of each character
	struct request *requests[5]; //Requests for each of the 5 characters
    struct item inventories[5][40]; //Inventories for each of the 5 characters
    struct container *containers; //Content of items which have other items inside
	struct zone *zones; //Different zones
    struct cinematic *cinematics;
	char modified; //True if the project has been modified since the last save
    char valid; //True if the project was valid at the last analysis
};

void free_cinematic_event(struct cinematic_event *c);
void free_cinematic(struct cinematic *c);
void free_container(struct container c);
void free_map(struct map r);
void free_project(struct project p);
void free_save_data(struct savedatas s);
void free_backup();
void delete_container(long ID);

void add_item(struct container container, struct item o);
void remove_item(struct container container, int value);


char allocate_knowledge(struct savedatas *s);