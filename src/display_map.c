#include "display_map.h"

int x = 0;
int y = 0;

void display_map(int zone, int *map)
{
    if (zone && zone <= project_data.parameters[11])
    {
        if (*map < project_data.zones[zone - 1].map_number)
        {
            if (!save_data.knowledge[get_player()].zones[zone - 1].maps[*map].has_map)
            {
                print_error("Carte non disponible");
                return;
            }
            clean_inputs();
            while (!inputs[0] && !inputs[5])
            {
                if (zone == 1 || zone == 5)
                    display_littlemap_knowledge(x, y, *map, zone, get_player());
                else
                    display_littlemap(x, y, project_data.zones[zone - 1].maps[*map]);
                print_refresh();
                load_input();
                if (inputs[1])
                    y--;
                if (inputs[2])
                    y++;
                if (inputs[3])
                    x--;
                if (inputs[4])
                    x++;
                if (inputs[16] && *map)
                    (*map)--;
                if (inputs[17] && *map + 1 < project_data.zones[zone - 1].map_number)
                    (*map)++;
                if (inputs[16] || inputs[17])
                    clean_inputs();
            }
        }
    }
}

void map_menu(int zone)
{
    char quit = 0;
    int map_numbers[5] = {0};
    int i = 0;
    int player = get_player();
    char found = 0;
    while (i < 5)
    {
        int j = 0;
        while (j < project_data.zones[i].map_number)
        {
            if (save_data.knowledge[player].zones[i].maps[j].has_map)
                map_numbers[i] = j + 1;
            j++;
        }
        found = (found || map_numbers[i]);
        i++;
    }
    if (!found)
    {
        print_error("Aucune carte");
        return;
    }
    while (!map_numbers[zone - 1])
    {
        zone++;
        if (zone > 5)
            zone = 1;
    }
    i = 0;
    while(!inputs[0] && !quit && zone)
    {
        int map_number = map_numbers[zone - 1];
        int camera = 0;
        rect(0, 0, 704, 704, 0, 0, 0);
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5] && !inputs[6])
        {
            int j = 0;
            if (i < map_number && save_data.knowledge[player].zones[zone - 1].maps[i].has_map)
            {
                if (zone == 1 || zone == 5)
                    display_minimap_knowledge(0, 0, i, zone, get_player());
                else
                    display_minimap_full(x, y, project_data.zones[zone - 1].maps[i]);
            }
            else
                rect(0, 0, 704, 704, 0, 0, 0);
            rect_alpha(404, 0, 300, 704, 100, 100, 100, 150);
            print_text_centered(404, 50,  zones_texts[zone - 1], 0, 1, 300);
            while (j < 11)
            {
                if (camera + j < map_number)
                {
                    if (save_data.knowledge[player].zones[zone - 1].maps[camera + j].has_map)
                        print_int_centered(404, 200 + j * 50, camera + j + 1, 2, 1, 1 + (camera + j == i), 300);
                    else
                        print_text_centered(404, 200 + j * 50, "???", 1, 1 + (camera + j == i), 300);
                }
                if (camera + j == map_number)
                    print_text_centered(404, 200 + j * 50, "Retour", 1, 1 + (camera + j == i), 300);
                j++;
            }
            print_refresh();
            load_input_long();
            if (inputs[1])
            {
                if (i)
                    i--;
                else
                    i = map_number;
            }
            if (inputs[2])
            {
                if (i < map_number)
                    i++;
                else
                    i = 0;
            }
            if (inputs[4])
            {
                zone = (zone) % 5 + 1;
                while (!map_numbers[zone - 1])
                    zone = (zone) % 5 + 1;
                map_number = map_numbers[zone - 1];
            }
            if (inputs[3])
            {
                zone = (zone + 3) % 5 + 1;
                while (!map_numbers[zone - 1])
                    zone = (zone + 3) % 5 + 1;
                map_number = map_numbers[zone - 1];
            }
            if (i > map_number)
                i = map_number;
            if (camera >= i)
            {
                if (i)
                    camera = i - 1;
                else
                    camera = 0;
            }
            if (camera + 11 < i)
            {
                if (i < map_number)
                    camera = i - 10;
                else
                    camera = i - 11;
            }
        }
        if (inputs[5])
        {
            if (i < map_number)
                display_map(zone, &i);
            else
                quit = 1;
        }
        if (inputs[6])
            quit = 1;
    }
    clean_inputs();
}