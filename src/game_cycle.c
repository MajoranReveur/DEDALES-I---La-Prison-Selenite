#include "game_cycle.h"

int player = 0;

void display_current_screen(struct position camera)
{
    rect(0, 0, 704, 704, 0, 0, 0);
    print_int(0, 0, project_data.parameters[11], 0, 1, 2);
    if (project_data.parameters[11] >= camera.zone)
    {
        if (project_data.zones[camera.zone - 1].map_number > camera.map)
        {
            display_map_cells(camera.x - 40, camera.y - 40, project_data.zones[camera.zone - 1].maps[camera.map]);
            display_map_items(camera.x - 40, camera.y - 40, project_data.zones[camera.zone - 1].maps[camera.map]);
        }
        display_map_characters(camera.x - 40, camera.y - 40, camera.map, camera.zone);
        display_map_shadow_character(camera.x - 40, camera.y - 40, project_data.zones[camera.zone - 1].maps[camera.map], player);
    }
    print_refresh();
}

char is_in_map(int x, int y, int map, int zone)
{
    if (zone < 1 || zone > project_data.parameters[11])
        return 0;
    if (map < 0 || map >= project_data.zones[zone - 1].map_number)
        return 0;
    return (x >= 0 && x < project_data.zones[zone - 1].maps[map].x && y >= 0 && y < project_data.zones[zone - 1].maps[map].y);
}

int get_current_cell(struct position p_player)
{
    if (!is_in_map(p_player.x / 8, p_player.y / 8, p_player.map, p_player.zone))
        return 2;
    return project_data.zones[p_player.zone - 1].maps[p_player.map].cells[p_player.x / 8][p_player.y / 8];
}

int get_front_cell(struct position p_player)
{
    if (!is_in_map(p_player.x / 8, p_player.y / 8, p_player.map, p_player.zone))
        return 2;
    int dx = 0;
    int dy = 0;
    if (p_player.orientation == 1)
        dy = -1;
    if (p_player.orientation == 0)
        dy = 1;
    if (p_player.orientation == 2)
        dx = -1;
    if (p_player.orientation == 3)
        dx = 1;
    int x = p_player.x / 8 + dx;
    int y = p_player.y / 8 + dy;
    if (!is_in_map(x, y, p_player.map, p_player.zone))
        return 2;
    return project_data.zones[p_player.zone - 1].maps[p_player.map].cells[x][y];
}

struct item get_front_item(struct position p_player)
{
    if (!is_in_map(p_player.x / 8, p_player.y / 8, p_player.map, p_player.zone))
    {
        struct item i;
        i.type = 0;
        return i;
    }
    int dx = 0;
    int dy = 0;
    if (p_player.orientation == 1)
        dy = -1;
    if (p_player.orientation == 0)
        dy = 1;
    if (p_player.orientation == 2)
        dx = -1;
    if (p_player.orientation == 3)
        dx = 1;
    int x = p_player.x / 8 + dx;
    int y = p_player.y / 8 + dy;
    if (!is_in_map(x, y, p_player.map, p_player.zone))
    {
        struct item i;
        i.type = 0;
        return i;
    }
    return project_data.zones[p_player.zone - 1].maps[p_player.map].items[x][y];
}

void reboot_portal(int zone, int map, int player)
{
    int i = 0;
    while (i < project_data.zones[zone - 1].maps[map].x)
    {
        int j = 0;
        while (j < project_data.zones[zone - 1].maps[map].y)
        {
            project_data.zones[zone - 1].maps[map].cells[i][j] = backup_data.zones[zone - 1].maps[map].cells[i][j];
            project_data.zones[zone - 1].maps[map].items[i][j] = backup_data.zones[zone - 1].maps[map].items[i][j];
            j++;
        }
        i++;
    }
    i = 0;
    int count = 0;
    char nihil_in = 0;
    while (i < 5)
    {
        if (save_data.portals[i].type == zone && save_data.portals[i].value - 1 == map)
        {
            count++;
            if (!i)
                nihil_in = 1;
        }
        i++;
    }
    i = 0;
    while (i < 5)
    {
        if (save_data.portals[i].type == zone && save_data.portals[i].value - 1 == map)
        {
            if (count == 1 || (zone >= 2 && zone <= 4 && nihil_in && i == 0) || (i == player && (zone > 4 || !nihil_in)))
            {
                project_data.character_positions[i].x = project_data.zones[zone - 1].maps[map].x_start * 8;
                project_data.character_positions[i].y = project_data.zones[zone - 1].maps[map].y_start * 8;
            }
            else
            {
                project_data.character_positions[i] = save_data.portals[i].last_position;
                save_data.portals[i].type = 0;
                save_data.portals[i].value = 0;
            }
        }
        i++;
    }
}

void quit_portal(int zone, int map, int player)
{
    reboot_portal(zone, map, player);
    int i = 0;
    i = 0;
    while (i < 5)
    {
        if (save_data.portals[i].type == zone && save_data.portals[i].value - 1 == map)
        {
            project_data.character_positions[i] = save_data.portals[i].last_position;
            save_data.portals[i].type = 0;
            save_data.portals[i].value = 0;
        }
        i++;
    }
}

void main_loop()
{
    char done = 0;
    char in_motion = 0;
    char map_changed = 0;
    struct position p_player;
    while (!done)
    {
        p_player = project_data.character_positions[player];
        display_current_screen(p_player);
        load_input();
        if (inputs[0])
            done = 1;
        if (!in_motion && !map_changed && inputs[5])
        {
            if (is_in_map(p_player.x / 8, p_player.y / 8, p_player.map, p_player.zone))
            {
                struct item objet = project_data.zones[p_player.zone - 1].maps[p_player.map].items[p_player.x / 8][p_player.y / 8];
                if (objet.type == 2 && p_player.zone == 1 && p_player.map > 0)
                {
                    p_player.map--;
                    map_changed = 1;
                }
                if (objet.type == 1 && p_player.zone == 1 && p_player.map + 1 < project_data.zones[p_player.zone - 1].map_number)
                {
                    p_player.map++;
                    map_changed = 1;
                }
                if (objet.type == 16)
                {
                    if (!objet.activation)
                    {
                        if (project_data.zones[1].map_number >= objet.value && objet.value)
                        {
                            save_data.portals[player].last_position = p_player;
                            save_data.portals[player].type = 2;
                            save_data.portals[player].value = objet.value;
                            p_player.zone = 2;
                            p_player.map = objet.value - 1;
                            p_player.x = project_data.zones[1].maps[objet.value - 1].x_start * 8;
                            p_player.y = project_data.zones[1].maps[objet.value - 1].y_start * 8;
                            map_changed = 1;
                        }
                    }
                }
            }
        }
        if (!in_motion && !map_changed)
        {
            if (inputs[1])
                p_player.orientation = 1;
            if (inputs[2])
                p_player.orientation = 0;
            if (inputs[3])
                p_player.orientation = 2;
            if (inputs[4])
                p_player.orientation = 3;
            if (inputs[1] || inputs[2] || inputs[3] || inputs[4])
                in_motion = 1;
        }
        if (in_motion && !(p_player.x % 8 || p_player.y % 8))
        {
            int cell = get_front_cell(p_player);
            if (cell == 2)
                in_motion = 0;
            else
            {
                struct item objet = get_front_item(p_player);
                if (objet.type == 22)
                    in_motion = 0;
            }
        }
        if (map_changed)
        {
            clean_inputs();
            int i = 0;
            project_data.zones[p_player.zone - 1].maps[p_player.map].remaining_green_cells = 0;
            while (i < project_data.zones[p_player.zone - 1].maps[p_player.map].x)
            {
                int j = 0;
                while (j < project_data.zones[p_player.zone - 1].maps[p_player.map].y)
                {
                    if (project_data.zones[p_player.zone - 1].maps[p_player.map].cells[i][j] == 7)
                        project_data.zones[p_player.zone - 1].maps[p_player.map].remaining_green_cells++;
                    j++;
                }
                i++;
            }
            map_changed = 0;
            int cell = get_current_cell(p_player);
            if (cell >= 3 && cell <= 6)
                in_motion = 1;
            if (cell == 3)
                p_player.orientation = 2;
            if (cell == 4)
                p_player.orientation = 0;
            if (cell == 5)
                p_player.orientation = 1;
            if (cell == 6)
                p_player.orientation = 3;
            if (cell == 8)
            {
                project_data.character_positions[player] = p_player;
                quit_portal(p_player.zone, p_player.map, player);
                p_player = project_data.character_positions[player];
                map_changed = 1;
            }
            if (cell == 7)
            {
                project_data.zones[p_player.zone - 1].maps[p_player.map].cells[p_player.x / 8][p_player.y / 8] = 8;
                project_data.zones[p_player.zone - 1].maps[p_player.map].remaining_green_cells--;
            }
        }
        if (in_motion)
        {
            int dx = 0;
            int dy = 0;
            if (p_player.orientation == 1)
                dy = -1;
            if (p_player.orientation == 0)
                dy = 1;
            if (p_player.orientation == 2)
                dx = -1;
            if (p_player.orientation == 3)
                dx = 1;
            p_player.x += dx;
            p_player.y += dy;
            in_motion = (p_player.x % 8 || p_player.y % 8);
            if (!in_motion)
            {
                int cell = get_current_cell(p_player);
                if (cell == 1 || (cell >= 3 && cell <= 6))
                    in_motion = 1;
                if (cell == 3)
                    p_player.orientation = 2;
                if (cell == 4)
                    p_player.orientation = 0;
                if (cell == 5)
                    p_player.orientation = 1;
                if (cell == 6)
                    p_player.orientation = 3;
                if (cell == 8)
                {
                    project_data.character_positions[player] = p_player;
                    reboot_portal(p_player.zone, p_player.map, player);
                    p_player = project_data.character_positions[player];
                    map_changed = 1;
                }
                if (cell == 7)
                {
                    project_data.zones[p_player.zone - 1].maps[p_player.map].cells[p_player.x / 8][p_player.y / 8] = 8;
                    project_data.zones[p_player.zone - 1].maps[p_player.map].remaining_green_cells--;
                }
            }
            if (!in_motion && p_player.zone == 2 && project_data.zones[p_player.zone - 1].maps[p_player.map].remaining_green_cells == 0)
            {
                struct position p = save_data.portals[player].last_position;
                project_data.zones[p.zone - 1].maps[p.map].items[p.x / 8][p.y / 8].activation = 1;
                project_data.character_positions[player] = p_player;
                quit_portal(p_player.zone, p_player.map, player);
                p_player = project_data.character_positions[player];
                map_changed = 1;
            }
        }
        project_data.character_positions[player] = p_player;
    }
}

void launch_game()
{
    int i = 0;
    backup_data.zones = malloc(sizeof(struct zone_backup) * project_data.parameters[11]);
    char valid = backup_data.zones != NULL;
    while (i < project_data.parameters[11] && valid)
    {
        backup_data.zones[i].maps = malloc(sizeof(struct map_backup) * project_data.zones[i].map_number);
        valid = backup_data.zones[i].maps != NULL;
        int j = 0;
        while (j < project_data.zones[i].map_number && valid)
        {
            backup_data.zones[i].maps[j].cells = malloc(sizeof(int*) * project_data.zones[i].maps[j].x);
            backup_data.zones[i].maps[j].items = malloc(sizeof(struct item*) * project_data.zones[i].maps[j].x);
            valid = (backup_data.zones[i].maps[j].cells != NULL && backup_data.zones[i].maps[j].items != NULL);
            int x = 0;
            while (x < project_data.zones[i].maps[j].x && valid)
            {
                backup_data.zones[i].maps[j].cells[x] = malloc(sizeof(int) * project_data.zones[i].maps[j].y);
                backup_data.zones[i].maps[j].items[x] = malloc(sizeof(struct item) * project_data.zones[i].maps[j].y);
                valid = (backup_data.zones[i].maps[j].cells[x] != NULL && backup_data.zones[i].maps[j].items[x] != NULL);
                int y = 0;
                while (y < project_data.zones[i].maps[j].y && valid)
                {
                    backup_data.zones[i].maps[j].cells[x][y] = project_data.zones[i].maps[j].cells[x][y];
                    backup_data.zones[i].maps[j].items[x][y] = project_data.zones[i].maps[j].items[x][y];
                    y++;
                }
                x++;
            }
            while (x && !valid)
            {
                x--;
                free(backup_data.zones[i].maps[j].cells[x]);
                free(backup_data.zones[i].maps[j].items[x]);
            }
            if (!valid)
            {
                free(backup_data.zones[i].maps[j].cells);
                free(backup_data.zones[i].maps[j].items);
                backup_data.zones[i].maps[j].cells = NULL;
            }
            j++;
        }
        while (j && !valid)
        {
            j--;
            if (backup_data.zones[i].maps[j].cells)
            {
                int x = 0;
                while (x < project_data.zones[i].maps[j].x)
                {
                    free(backup_data.zones[i].maps[j].cells[x]);
                    free(backup_data.zones[i].maps[j].items[x]);
                    x++;
                }
                free(backup_data.zones[i].maps[j].cells);
                free(backup_data.zones[i].maps[j].items);
            }
        }
        if (!valid)
        {
            free(backup_data.zones[i].maps);
            backup_data.zones[i].maps = NULL;
        }
        i++;
    }
    while (i && !valid)
    {
        i--;
        if (backup_data.zones[i].maps)
        {
            int j = 0;
            while (j < project_data.zones[i].map_number)
            {
                int x = 0;
                while (x < project_data.zones[i].maps[j].x)
                {
                    free(backup_data.zones[i].maps[j].cells[x]);
                    free(backup_data.zones[i].maps[j].items[x]);
                    x++;
                }
                free(backup_data.zones[i].maps[j].cells);
                free(backup_data.zones[i].maps[j].items);
                j++;
            }
            free(backup_data.zones[i].maps);
        }
    }
    if (!valid)
    {
        free(backup_data.zones);
        print_error("Pas assez de memoire");
        return;
    }
    i = 0;
    while (i < 5)
    {
        project_data.character_positions[i].x *= 8;
        project_data.character_positions[i].y *= 8;
        project_data.character_positions[i].orientation = 0;
        save_data.portals[i].value = 0;
        save_data.portals[i].type = 0;
        i++;
    }
    player = project_data.parameters[4];
    main_loop();
}