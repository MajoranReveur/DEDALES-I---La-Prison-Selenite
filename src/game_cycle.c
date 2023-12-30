#include "game_cycle.h"

int player = 0;
char savable = 1;
char in_motion = 0;
char map_changed = 0;

int get_player()
{
    return player;
}

char is_savable()
{
    return savable;
}

void update_perception(int player)
{
    int i = 0;
    while (i < 10)
    {
        perception[player][i] = 0;
        i++;
    }
    i = 0;
    while (i < 40)
    {
        struct item objet = project_data.inventories[player][i];
        if (objet.type == 9)
            perception[player][0] = 1;
        i++;
    }
}

void update_request_positions()
{    
    int i = 0;
    while (i < 5)
    {
        int character = i;
        if (character == 4)
        {
            if (project_data.character_positions[player].zone == 1)
                character = 4 + project_data.character_positions[player].map;
            if (project_data.character_positions[player].zone == 5)
                character = 4 + project_data.character_positions[player].map + project_data.zones[0].map_number;
        }
        if (save_data.request_states[character].active && character != player)
        {
            struct position p = project_data.requests[character][save_data.request_states[character].value].objective.p;
            project_data.character_positions[character].map = p.map;
            project_data.character_positions[character].zone = p.zone;
            project_data.character_positions[character].x = p.x * 8;
            project_data.character_positions[character].y = p.y * 8;
            project_data.character_positions[character].orientation = p.orientation;
        }
        i++;
    }
}

void update_active_requests()
{
    int i = 0;
    while (i < 4 + project_data.zones[0].map_number + project_data.zones[4].map_number)
    {
        if (!save_data.request_states[i].active)
        {
            //print_error("Found free request :");
            //print_error_int(i);
            int j = 0;
            int character = i;
            if (i >= 4)
                character = 4;
            int zone = 1;
            int map = i - 4;
            if (i >= 4 + project_data.zones[0].map_number)
            {
                zone = 5;
                map = i - 4 - project_data.zones[0].map_number;
            }
            char found = 0;
            while (j < project_data.parameters[5 + character] && !found)
            {
                if (project_data.requests[character][j].objective.activated)
                {
                    if (character != 4 || (zone == project_data.requests[character][j].objective.p.zone && map == project_data.requests[character][j].objective.p.map))
                    {
                        save_data.request_states[i].active = 1;
                        save_data.request_states[i].value = j;
                        found = 1;
                    }
                }
                j++;
            }
        }
        i++;
    }
    update_request_positions();
}

void display_current_screen(struct position camera)
{
    rect(0, 0, 704, 704, 0, 0, 0);
    if (project_data.parameters[11] >= camera.zone)
    {
        if (project_data.zones[camera.zone - 1].map_number > camera.map)
        {
            display_map_cells(camera.x - 40, camera.y - 40, project_data.zones[camera.zone - 1].maps[camera.map]);
            display_map_items(camera.x - 40, camera.y - 40, project_data.zones[camera.zone - 1].maps[camera.map], player);
        }
        display_map_characters(camera.x - 40, camera.y - 40, camera.map, camera.zone);
        if (camera.zone != 1 || camera.zone == 5)
            display_map_shadow_character(camera.x - 40, camera.y - 40, camera.map, camera.zone, player);
        display_sprite(4, (project_data.character_positions[player].x - camera.x + 40) * 8,  (project_data.character_positions[player].y - camera.y + 40) * 8 - 16, 
        64, player * 4 + project_data.character_positions[player].orientation, project_data.character_positions[player].x % 8 + project_data.character_positions[player].y % 8);
    }
}

void order_notifications()
{
    int i = 0;
    while (i < 10)
    {
        if (notifs[i].delay > 0)
        {
            int j = 0;
            char found = 0;
            while (!found && j < i)
            {
                found = notifs[j].delay == 0;
                if (!found)
                    j++;
            }
            if (found)
            {
                notifs[j].delay = notifs[i].delay;
                copy_str(notifs[j].message, notifs[i].message, 300);
                notifs[i].delay = 0;
            }
        }
        i++;
    }
}

void add_notification(char* message)
{
    int i = 0;
    int min_delay = 0;
    while (i < 10 && notifs[i].delay > 0)
    {
        if (notifs[min_delay].delay > notifs[i].delay)
            min_delay = i;
        i++;
    }
    if (i == 10)
    {
        notifs[min_delay].delay = 0;
        order_notifications();
        i = 9;
    }
    notifs[i].delay = 100;
    copy_str(notifs[i].message, message, 300);
}

void add_item_notification(struct item objet)
{
    char message[301] = {0};
    if (objet.type >= 14 && objet.type <= 20)
    {
        const char* str[5] = {"Obtenu : ", items_texts[objet.type], " (", "", ")"};
        if (objet.type == 15)
            str[3] = cards_texts[objet.value];
        else
        {
            char number[21];
            int_to_str(number, objet.value, 1);
            str[3] = number;
        }
        concat_str(message, str, 300, 5);
    }
    else
    {
        const char* str[2] = {"Obtenu : ", items_texts[objet.type]};
        concat_str(message, str, 300, 2);
    }
    add_notification(message);
}

void display_notifications()
{
    int i = 0;
    int y = 0;
    while (i < 10)
    {
        if (notifs[i].delay > 0)
        {
            print_notif(notifs[i], y * 30);
            notifs[i].delay--;
            y++;
        }
        i++;
    }
    order_notifications();
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

void delete_front_item(struct position p_player)
{
    if (!is_in_map(p_player.x / 8, p_player.y / 8, p_player.map, p_player.zone))
        return;
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
        return;
    project_data.zones[p_player.zone - 1].maps[p_player.map].items[x][y].type = 0;
}

struct item get_item_from_request(struct request r)
{
    struct item objet;
    objet.activation = r.objective.value1 % 2;
    objet.type = r.objective.value1 / 2;
    objet.ID = r.objective.value2;
    objet.value = r.objective.value3;
    return objet;
}

char can_take_card(int character, int card_type, int quantity)
{
    int i = 0;
    int found = 0;
    while (i < 40)
    {
        if (project_data.inventories[character][i].type == 8)
        {
            if (project_data.containers[project_data.inventories[character][i].ID].items[card_type - 1].type == 0)
                found++;
        }
        i++;
    }
    return found >= quantity;
}

char can_take_item_in_container(int character, int container_type, int quantity)
{
    int i = 0;
    int found = 0;
    while (i < 40 && found < quantity)
    {
        if (project_data.inventories[character][i].type == container_type)
        {
            int j = 0;
            while (j < project_data.containers[project_data.inventories[character][i].ID].size && found < quantity)
            {
                if (project_data.containers[project_data.inventories[character][i].ID].items[j].type == 0)
                    found++;
                j++;
            }
        }
        i++;
    }
    return found >= quantity;
}

char can_take_item(int character, struct item objet, int quantity)
{
    if (objet.type == 15)
        return (can_take_card(character, objet.value, quantity));
    if (objet.type == 14)
        return (can_take_item_in_container(character, 7, quantity));
    if (objet.type == 16)
        return (can_take_item_in_container(character, 9, quantity));
    int i = 0;
    int found = 0;
    while (i < 40)
    {
        if (!project_data.inventories[character][i].type)
            found++;
        i++;
    }
    return found >= quantity;
}

char is_item_matching(struct item i1, struct item i2)
{
    //print_error("matching?");
    //print_error_int(i1.type);
    //print_error_int(i2.type);
    if (i1.type != i2.type)
        return 0;
    //print_error("yep !");
    return 1;
}

char has_item_in_container(int character, int container_type, struct item objet, int quantity)
{
    int i = 0;
    int found = 0;
    //print_error("searching for...");
    //print_error_int(container_type);
    //print_error_int(quantity);
    while (i < 40 && found < quantity)
    {
        if (project_data.inventories[character][i].type == container_type)
        {
            //print_error("found one !");
            int j = 0;
            while (j < project_data.containers[project_data.inventories[character][i].ID].size && found < quantity)
            {
                if (is_item_matching(project_data.containers[project_data.inventories[character][i].ID].items[j], objet))
                    found++;
                j++;
            }
        }
        i++;
    }
    //if (found >= quantity)
    //    print_error("All good there !");
    return (found >= quantity);
}

char has_item(int character, struct item objet, int quantity)
{
    if (objet.type == 15 && has_item_in_container(character, 8, objet, quantity))
        return 1;
    if (objet.type == 14 && has_item_in_container(character, 7, objet, quantity))
        return 1;
    if (objet.type == 16 && has_item_in_container(character, 9, objet, quantity))
        return 1;
    int i = 0;
    int found = 0;
    while (i < 40)
    {
        if (is_item_matching(project_data.inventories[character][i], objet))
            found++;
        i++;
    }
    return (found >= quantity);
}

struct item drop_item_in_container(int character, int container_type, struct item objet)
{
    int i = 0;
    int found = 0;
    while (i < 40)
    {
        if (project_data.inventories[character][i].type == container_type)
        {
            int j = 0;
            while (j < project_data.containers[project_data.inventories[character][i].ID].size)
            {
                if (is_item_matching(project_data.containers[project_data.inventories[character][i].ID].items[j], objet))
                {
                    struct item result = project_data.containers[project_data.inventories[character][i].ID].items[j];
                    if (container_type == 8)
                        project_data.containers[project_data.inventories[character][i].ID].items[j].type = 0;
                    else
                        remove_item(project_data.containers[project_data.inventories[character][i].ID], result.value);
                    return result;
                }
                j++;
            }
        }
        i++;
    }
    return objet; //SHOULD NEVER HAPPEN
}

struct item drop_item(int character, struct item objet)
{
    if (objet.type == 15 && has_item_in_container(character, 8, objet, 1))
        return (drop_item_in_container(character, 8, objet));
    if (objet.type == 14 && has_item_in_container(character, 7, objet, 1))
        return (drop_item_in_container(character, 7, objet));
    if (objet.type == 16 && has_item_in_container(character, 9, objet, 1))
        return (drop_item_in_container(character, 9, objet));
    int i = 0;
    while (i < 40)
    {
        if (is_item_matching(project_data.inventories[character][i], objet))
        {
            struct item result = project_data.inventories[character][i];
            project_data.inventories[character][i].type = 0;
            print_error("drop :");
            print_error_int(i);
            return result;
        }
        i++;
    }
    return objet; // SHOULD NEVER HAPPEN
}

char has_item_request(int character, struct request r)
{
    struct item objet = get_item_from_request(r);
    return has_item(player, objet, r.objective.value4);
}

char has_key(int value)
{
    struct item key;
    key.type = 16;
    key.value = value;
    key.activation = 0;
    key.ID = 0;
    return has_item(player, key, 1);
}

char has_item_type(int type)
{
    struct item objet;
    objet.type = type;
    objet.value = 0;
    objet.activation = 0;
    objet.ID = 0;
    return has_item(player, objet, 1);
}

void take_card(int character, struct item objet)
{
    int i = 0;
    while (i < 40)
    {
        if (project_data.inventories[character][i].type == 8)
        {
            if (project_data.containers[project_data.inventories[character][i].ID].items[objet.value - 1].type == 0)
            {
                project_data.containers[project_data.inventories[character][i].ID].items[objet.value - 1] = objet;
                return;
            }
        }
        i++;
    }
}

void take_item_in_container(int character, int container_type, struct item objet)
{
    int i = 0;
    while (i < 40)
    {
        if (project_data.inventories[character][i].type == container_type)
        {
            int j = 0;
            while (j < project_data.containers[project_data.inventories[character][i].ID].size)
            {
                if (project_data.containers[project_data.inventories[character][i].ID].items[j].type == 0)
                {
                    add_item(project_data.containers[project_data.inventories[character][i].ID], objet);
                    return;
                }
                j++;
            }
        }
        i++;
    }
}

void take_item(int character, struct item objet)
{
    if (objet.type == 15)
    {
        take_card(character, objet);
        return;
    }
    if (objet.type == 14)
    {
        take_item_in_container(character, 7, objet);
        return;
    }
    if (objet.type == 16)
    {
        take_item_in_container(character, 9, objet);
        return;
    }
    int i = 0;
    while (i < 40)
    {
        if (!project_data.inventories[character][i].type)
        {
            project_data.inventories[player][i] = objet;
            return;
        }
        i++;
    }
}

void take_item_position(struct position p, int character)
{
    if (is_in_map(p.x / 8, p.y / 8, p.map, p.zone))
    {
        struct item objet = project_data.zones[p.zone - 1].maps[p.map].items[p.x / 8][p.y / 8];
        if (objet.type == 14)
        {
            objet.value = p.map + 1;
            project_data.zones[p.zone - 1].maps[p.map].items[p.x / 8][p.y / 8] = objet;
        }
        if (can_take_item(character, objet, 1))
        {
            take_item(character, objet);
            project_data.zones[p.zone - 1].maps[p.map].items[p.x / 8][p.y / 8].type = 0;
            add_item_notification(objet);
        }
        else
            add_notification("Pas de place...");
    }
    update_perception(player);
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
    print_error_int(zone);
    print_error_int(map);
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

void update_knowledge(int player)
{
    struct position p = project_data.character_positions[player];
    if (p.x % 8 == 0 && p.y % 8 == 0)
    {
        p.x /= 8;
        p.y /= 8;
        if (is_in_map(p.x, p.y, p.map, p.zone))
        {
            if (save_data.knowledge[player].zones[p.zone - 1].maps[p.map].cells[p.x][p.y] < 2)
                save_data.knowledge[player].zones[p.zone - 1].maps[p.map].cells[p.x][p.y] = 2;
        }
        if (is_in_map(p.x - 1, p.y, p.map, p.zone))
        {
            if (save_data.knowledge[player].zones[p.zone - 1].maps[p.map].cells[p.x-1][p.y] < 1)
                save_data.knowledge[player].zones[p.zone - 1].maps[p.map].cells[p.x-1][p.y] = 1;
        }
        if (is_in_map(p.x + 1, p.y, p.map, p.zone))
        {
            if (save_data.knowledge[player].zones[p.zone - 1].maps[p.map].cells[p.x+1][p.y] < 1)
                save_data.knowledge[player].zones[p.zone - 1].maps[p.map].cells[p.x+1][p.y] = 1;
        }
        if (is_in_map(p.x, p.y-1, p.map, p.zone))
        {
            if (save_data.knowledge[player].zones[p.zone - 1].maps[p.map].cells[p.x][p.y-1] < 1)
                save_data.knowledge[player].zones[p.zone - 1].maps[p.map].cells[p.x][p.y-1] = 1;
        }
        if (is_in_map(p.x, p.y+1, p.map, p.zone))
        {
            if (save_data.knowledge[player].zones[p.zone - 1].maps[p.map].cells[p.x][p.y+1] < 1)
                save_data.knowledge[player].zones[p.zone - 1].maps[p.map].cells[p.x][p.y+1] = 1;
        }
    }
}

void reload_with_character(int character)
{
    player = character;
    update_perception(player);
    update_request_positions();
    in_motion = 0;
    map_changed = 1;
}

void get_item_description(struct request r, char* str)
{
    char number[21] = {0};
    int_to_str(number, r.objective.value4, 0);
    char* str_fields[3] = {items_texts[r.objective.value1 / 2], " x ", number};
    concat_str(str, str_fields, 200, 3);
}

void dealing_with_request(int character, int value)
{
    struct request r = project_data.requests[character][value];
    char success = 0;
    if (r.objective.type == 0)
    {
        display_message("Petite discussion.");
        success = 1;
    }
    if (r.objective.type == 1)
        display_message("Montre moi...");
    if (r.objective.type == 2)
        display_message("Donne moi...");
    if (r.objective.type == 1 || r.objective.type == 2)
    {
        char item_desc[201] = {0};
        get_item_description(r, item_desc);
        char item_str[201] = {0};
        char* str[2] = {"Objet : ", item_desc};
        concat_str(item_str, str, 200, 2);
        display_message(item_str);
        if (has_item_request(player, r))
        {
            if (r.objective.type == 1)
                success = 1;
            if (r.objective.type == 2)
            {
                print_error("Try to get it !");
                struct item objet = get_item_from_request(r);
                if (can_take_item(character, objet, 1))
                {
                    objet = drop_item(player, objet);
                    take_item(character, objet);
                    success = 1;
                }
            }
            if (success)
                display_message("Requete faisable.");
        }
    }
    if (success)
    {
        print_error_int(character);
        print_error_int(project_data.character_positions[player].zone);
        project_data.requests[character][value].objective.activated = 0;
        if (character == 4)
        {
            if (project_data.character_positions[player].zone == 1)
                character = 4 + project_data.character_positions[player].map;
            if (project_data.character_positions[player].zone == 5)
                character = 4 + project_data.character_positions[player].map + project_data.zones[0].map_number;
        }
        print_error_int(character);
        save_data.request_states[character].active = 0;
        update_active_requests();
        
        if (r.reward.type && has_item(character, r.reward, 1))
        {
            print_error("reward");
            if (can_take_item(player, r.reward, 1))
            {
                struct item objet = drop_item(character, r.reward);
                take_item(player, objet);
                add_item_notification(objet);
            }
        }
    }
}

void main_loop()
{
    char done = 0;
    char first_move = 0;
    in_motion = 0;
    struct position p_player;
    while (!done)
    {
        update_knowledge(player);
        p_player = project_data.character_positions[player];
        display_current_screen(p_player);
        display_notifications();
        print_refresh();
        load_input();
        p_player = project_data.character_positions[player];
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
                if (objet.type == 16 && has_item_type(9))
                {
                    if (!objet.activation)
                    {
                        if (project_data.zones[1].map_number >= objet.value && objet.value)
                        {
                            save_data.portals[player].last_position = p_player;
                            save_data.portals[player].portal_position = p_player;
                            save_data.portals[player].type = 2;
                            save_data.portals[player].value = objet.value;
                            p_player.zone = 2;
                            p_player.map = objet.value - 1;
                            p_player.x = project_data.zones[1].maps[objet.value - 1].x_start * 8;
                            p_player.y = project_data.zones[1].maps[objet.value - 1].y_start * 8;
                            map_changed = 1;
                            save_data.knowledge[player].zones[p_player.zone - 1].maps[p_player.map].has_map = 1;
                        }
                    }
                    else
                        take_item_position(p_player, player);
                }
                if ((objet.type >= 7 && objet.type <= 15) || objet.type == 21)
                    take_item_position(p_player, player);
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
            {
                first_move = 1;
                in_motion = 1;
            }
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
                {
                    in_motion = 0;
                    if (first_move)
                    {
                        if (has_item_type(9))
                        {
                            char number[21] = {0};
                            int_to_str(number, objet.value, 1);
                            const char* strs[4] = {"Un Sceau de categorie ", number, " bloque le passage.\n", "Mais vous n'avez pas la Cle necessaire pour\nle supprimer..."};
                            char str[301] = {0};
                            if (has_key(objet.value))
                            {
                                strs[3] = "Lever le Sceau ?";
                                concat_str(str, strs, 300, 4);
                                char* options[2] = {"Non", "Oui"};
                                if (ask_question(str, options, 2) == 1)
                                    delete_front_item(p_player);
                            }
                            else
                            {
                                concat_str(str, strs, 300, 4);
                                display_message(str);
                            }
                            
                        }
                        else
                            display_message("Un Sceau bloque le passage.");
                    }
                }
                if (objet.type == 4 || objet.type == 5)
                    in_motion = 0;
            }
        }
        if (in_motion && first_move)
        {
            int i = 0;
            while (i < 5)
            {
                int character = i;
                if (character == 4)
                {
                    if (project_data.character_positions[player].zone == 1)
                        character = 4 + project_data.character_positions[player].map;
                    if (project_data.character_positions[player].zone == 5)
                        character = 4 + project_data.character_positions[player].map + project_data.zones[0].map_number;
                }
                if (save_data.request_states[character].active)
                {
                    struct request r = project_data.requests[i][save_data.request_states[character].value];
                    if (r.objective.p.zone == project_data.character_positions[player].zone &&
                        r.objective.p.map == project_data.character_positions[player].map)
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
                        int x = p_player.x / 8 + dx;
                        int y = p_player.y / 8 + dy;
                        if (r.objective.p.x == x && r.objective.p.y == y)
                        {
                            dealing_with_request(i, save_data.request_states[character].value);
                            in_motion = 0;
                        }
                    }
                }
                i++;
            }
        }
        first_move = 0;
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
            update_request_positions();
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
                //print_error("Wait");
                struct position p = save_data.portals[player].portal_position;
                //print_error_int(p.zone);
                //print_error_int(p.map);
                //print_error_int(p.x);
                //print_error_int(p.y);
                project_data.zones[p.zone - 1].maps[p.map].items[p.x / 8][p.y / 8].activation = 1;
                //print_error_int(4);
                project_data.character_positions[player] = p_player;
                //print_error_int(5);
                quit_portal(p_player.zone, p_player.map, player);
                p_player = project_data.character_positions[player];
                //print_error_int(6);
                map_changed = 1;
            }
        }
        project_data.character_positions[player] = p_player;
        savable = !in_motion && !map_changed;
    }
}

void launch_game(char with_save, int save_spot)
{
    save_data.request_states = NULL;
    if (!allocate_knowledge(&save_data))
    {
        print_error("Pas assez de memoire");
        return;
    }
    int i = 0;
    while (i < 10)
    {
        notifs[i].delay = 0;
        i++;
    }
    i=0;
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
    if (!valid)
    {
        free_backup();
        free_save_data(save_data);
        print_error("Pas assez de memoire");
        return;
    }
    i = 0;
    while (i < 5)
    {
        update_perception(i);
        project_data.character_positions[i].x *= 8;
        project_data.character_positions[i].y *= 8;
        project_data.character_positions[i].orientation = 0;
        save_data.portals[i].value = 0;
        save_data.portals[i].type = 0;
        i++;
    }
    print_error("Start !");
    player = project_data.parameters[4];
    map_changed = 1;
    if (with_save)
    {
        if (open_save(save_spot))
        {
            main_loop();
        }
    }
    else
    {
        update_active_requests();
        main_loop();
    }
    free_backup();
    free_save_data(save_data);
}