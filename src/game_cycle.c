#include "game_cycle.h"

int player = 0;
char savable = 1;
char in_motion = 0;
char map_changed = 0;
char endgame = 0;

int get_player()
{
    return player;
}

char is_savable()
{
    return savable;
}

void end_game()
{
    endgame = 1;
}

void reset_cinematic_triggers()
{
    int z = 0;
    while (z < project_data.parameters[11])
    {
        int m = 0;
        while (m < project_data.zones[z].map_number)
        {
            int x = 0;
            while (x < project_data.zones[z].maps[m].x)
            {
                int y = 0;
                while (y < project_data.zones[z].maps[m].y)
                {
                    project_data.zones[z].maps[m].cells[x][y].cinematic_trigger = 0;
                    y++;
                }
                x++;
            }
            m++;
        }
        z++;
    }
    int i = 0;
    while (i < project_data.parameters[12])
    {
        if (project_data.cinematics[i].trigger.active)
        {
            int type = project_data.cinematics[i].trigger.type;
            //Case for location
            if (type == 1 || type == 2)
            {
                struct position p = project_data.cinematics[i].trigger.p;
                if (project_data.zones[p.zone - 1].maps[p.map].cells[p.x][p.y].cinematic_trigger == 0)
                {
                    project_data.zones[p.zone - 1].maps[p.map].cells[p.x][p.y].cinematic_trigger = i + 1;
                }
            }
        }
        i++;
    }
}

void run_event(struct cinematic_event c)
{
    if (c.type == 0)
    {
        display_message(c.dialog.string);
    }
    if (c.type == 18)
    {
        display_message("FIN DE LA PARTIE !!!");
        endgame = 1;
    }
}

void run_cinematic(long id)
{
    int i = 0;
    while (i < project_data.cinematics[id].length)
    {
        run_event(project_data.cinematics[id].events[i]);
        i++;
    }
}

long find_another_cinematic_position(struct position p, long start)
{
    long i = start;
    while (i < project_data.parameters[12])
    {
        if (project_data.cinematics[i].trigger.active)
        {
            int type = project_data.cinematics[i].trigger.type;
            //Case for location
            if (type == 1 || type == 2)
            {
                struct position p2 = project_data.cinematics[i].trigger.p;
                if (p.zone == p2.zone && p.map == p2.map && p.x == p2.x && p.y == p2.y)
                    return i + 1;
            }
        }
        i++;
    }
    return 0;
}

void handle_cinematic(long id)
{
    savable = 0;
    project_data.cinematics[id].trigger.active = 0;
    run_cinematic(id);
    int type = project_data.cinematics[id].trigger.type;
    if (type == 1 || type == 2)
    {
        struct position p = project_data.cinematics[id].trigger.p;
        project_data.zones[p.zone - 1].maps[p.map].cells[p.x][p.y].cinematic_trigger = find_another_cinematic_position(p, 0);
    }
    map_changed = 1;
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

void get_item_description_item(struct item i, int quantity, char* str)
{
    char number[21] = {0};
    int_to_str(number, quantity, 0);
    const char* str_fields[3] = {items_texts[i.type], " x ", number};
    concat_str(str, str_fields, 200, 3);
}

void update_active_requests()
{
    int i = 0;
    while (i < 4 + project_data.zones[0].map_number + project_data.zones[4].map_number)
    {
        if (!save_data.request_states[i].active)
        {
            //print_error("Found free request slot:");
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
                    //print_error("activated !");
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
    if (project_data.parameters[11] >= camera.zone && camera.zone)
    {
        if (project_data.zones[camera.zone - 1].map_number > camera.map)
        {
            display_map_cells(camera.x - 40, camera.y - 40, project_data.zones[camera.zone - 1].maps[camera.map]);
            display_map_items(camera.x - 40, camera.y - 40, project_data.zones[camera.zone - 1].maps[camera.map], player);
        }
        display_map_characters(camera.x - 40, camera.y - 40, camera.map, camera.zone);
        if (camera.zone == 1 || camera.zone == 5)
            display_map_shadow_character(camera.x - 40, camera.y - 40, camera.map, camera.zone, player);
        display_sprite(4, (project_data.character_positions[player].x - camera.x + 40) * 8,  (project_data.character_positions[player].y - camera.y + 40) * 8 - 16, 
        64, player * 4 + project_data.character_positions[player].orientation, project_data.character_positions[player].x % 8 + project_data.character_positions[player].y % 8);
    }
    if (!camera.zone)
    {
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

struct cell get_current_cell(struct position p_player)
{
    if (!is_in_map(p_player.x / 8, p_player.y / 8, p_player.map, p_player.zone))
    {
        struct cell c;
        c.type = 2; c.thought = 0; c.item.type = 0; c.sprite = 0; c.cinematic_trigger = 0; c.first_sprite = 0;
        return c;
    }
    return project_data.zones[p_player.zone - 1].maps[p_player.map].cells[p_player.x / 8][p_player.y / 8];
}

struct cell get_front_cell(struct position p_player)
{
    struct cell c;
    c.type = 2; c.thought = 0; c.item.type = 0; c.sprite = 0; c.cinematic_trigger = 0; c.first_sprite = 0;
    if (!is_in_map(p_player.x / 8, p_player.y / 8, p_player.map, p_player.zone))
        return c;
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
        return c;
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
    return project_data.zones[p_player.zone - 1].maps[p_player.map].cells[x][y].item;
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
    project_data.zones[p_player.zone - 1].maps[p_player.map].cells[x][y].item.type = 0;
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

void unmark_all_items()
{
    int i = 0;
    while (i < 40)
    {
        project_data.inventories[player][i].marked = 0;
        if (project_data.inventories[player][i].type >= 7 && project_data.inventories[player][i].type <= 13)
        {
            int j = 0;
            long container = project_data.inventories[player][i].ID;
            while (j < project_data.containers[container].size)
            {
                project_data.containers[container].items[j].marked = 0;
                j++;
            }
        }
        i++;
    }
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
            project_data.inventories[character][i] = objet;
            return;
        }
        i++;
    }
}


void exchange_marked_items(int character)
{
    int i = 0;
    while (i < 40)
    {
        if (project_data.inventories[player][i].type >= 7 && project_data.inventories[player][i].type <= 13)
        {
            int j = 0;
            long container = project_data.inventories[player][i].ID;
            while (j < project_data.containers[container].size)
            {
                if (project_data.containers[container].items[j].marked)
                {
                    project_data.containers[container].items[j].marked = 0;
                    take_item(character, project_data.containers[container].items[j]);
                    project_data.containers[container].items[j].type = 0;
                }
                j++;
            }
        }
        if (project_data.inventories[player][i].marked)
        {
            //print_error("exchange :");
            //print_error_int(i);
            //print_error_int(character);
            project_data.inventories[player][i].marked = 0;
            take_item(character, project_data.inventories[player][i]);
            project_data.inventories[player][i].type = 0;
            //inventory();
        }
        i++;
    }
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

int item_choice_card_request(long ID, struct item objet_request, int quantity, int total)
{
    inputs[5] = 0;
    inputs[6] = 0;
    inputs[10] = 0;
    int i = 0;
    int j = 0;
    while (!inputs[0] && !inputs[5] && !inputs[6])
    {
        int y = 0;
        rect(104, 92, 496, 436, 0, 0, 255);
        rect(108, 96, 488, 428, 0, 0, 0);
        rect(i * 60 + 120, j * 100 + 130, 48, 80, 0, 0, 255);
        while (y < 3)
        {
            int x = 0;
            while (x < 8)
            {
                if (x || y)
                {
                    if (project_data.containers[ID].items[x + y * 8 - 1].marked)
                        rect(x * 60 + 120 + 4, y * 100 + 130 + 4, 40, 72, 100, 250, 100);
                    //rect(x * 60 + 120 + 8, y * 100 + 130 + 8, 32, 64, 100, 100, 100);
                    if (project_data.containers[ID].items[x + y * 8 - 1].type == 15)
                        display_cardsprite(x * 60 + 120 + 8, y * 100 + 130 + 8, x + y * 8);
                }
                x++;
            }
            y++;
        }
        if (i || j)
            print_text_centered(112, 450, cards_texts[i + j * 8], 1, 1, 480);
        else
            print_text_centered(112, 450, "Retour", 1, 1, 480);
        print_refresh();
        load_input_long();
        if (inputs[1] && j)
            j--;
        if (inputs[2] && j < 2)
            j++;
        if (inputs[3] && i)
            i--;
        if (inputs[4] && i < 7)
            i++;
        if (inputs[5])
        {
            if (i || j)
            {
                if (is_item_matching(objet_request, project_data.containers[ID].items[i + j * 8 - 1]))
                {
                    if (project_data.containers[ID].items[i + j * 8 - 1].marked)
                    {
                        project_data.containers[ID].items[i + j * 8 - 1].marked = 0;
                        total--;
                    }
                    else
                    {
                        project_data.containers[ID].items[i + j * 8 - 1].marked = 1;
                        total++;
                    }
                }
                inputs[5] = 0;
            }
        }
    }
    clean_inputs();
    return total;
}

int item_choice_container_request(long ID, int type, struct item objet_request, int quantity, int total)
{
    clean_inputs();
    char count_by_category[21] = {0};
    count_by_category[0] = 'x';
    char string_capacity[20] = {0};
    char string_count[20] = {0};
    char string_total_capacity[8] = {0};
    int i = 0;
    int camera = 0;
    int size = project_data.containers[ID].size;
    int count = 0;
    int category_count = 0;
    int_to_str(string_capacity, size, 1);
    int old_value = 0;
    while (i < size)
    {
        if (project_data.containers[ID].items[i].value)
        {
            count++;
            if (project_data.containers[ID].items[i].value != old_value)
            {
                category_count++;
                old_value = project_data.containers[ID].items[i].value;
            }
        }
        i++;
    }
    int_to_str(string_count, count, 1);
    const char *fields[3] = {string_count, "/", string_capacity};
    concat_str(string_total_capacity, fields, 8, 3);
    i = 0;
    while (!inputs[0] && !inputs[6])
    {
        int j = 0;
        int y = 0;
        int k = 0;
        while (k < camera)
        {
            int value = project_data.containers[ID].items[j].value;
            while (project_data.containers[ID].items[j].value == value)
                j++;
            k++;
        }
        rect(104, 92, 496, 436, 0, 0, 255);
        rect(108, 96, 488, 428, 0, 0, 0);
        print_text_centered(112, 100, items_texts[type], 0, 1, 480);
        print_text_centered(112, 140, string_total_capacity, 1, 1, 480);
        while (y < 11 && y + camera < category_count + 1)
        {
            if (y + camera < category_count)
            {
                int value = project_data.containers[ID].items[j].value;
                print_int(180, y * 30 + 190, value, 1, 1, 1);
                int count_of_value = 0;
                int count_of_marked = 0;
                while (j < size && project_data.containers[ID].items[j].value == value)
                {
                    if (project_data.containers[ID].items[j].marked)
                        count_of_marked++;
                    j++;
                    count_of_value++;
                }
                int_to_str(count_by_category + 1, count_of_value, 1);
                print_text(400, y * 30 + 190, count_by_category, 1, 1 + (i == y + camera));
                if (count_of_marked)
                {
                    int_to_str(count_by_category + 1, count_of_marked, 1);
                    print_text(450, y * 30 + 190, count_by_category, 1, 3);
                }

            }
            if (y + camera == category_count)
                print_text(180, y * 30 + 190, "Retour", 1, 1 + (i == y + camera));
            y++;
        }
        print_refresh();
        load_input_long();
        if (inputs[1])
            i = (i + category_count) % (category_count + 1);
        if (inputs[2])
            i = (i + 1) % (category_count + 1);
        if (inputs[5])
        {
            inputs[6] = (i == category_count);
            inputs[4] = (i < category_count);
        }
        if (inputs[4] || inputs[3])
        {
            int k = 0;
            int value = project_data.containers[ID].items[0].value;
            j = 0;
            while (k < i)
            {
                while (project_data.containers[ID].items[j].value == value)
                    j++;
                value = project_data.containers[ID].items[j].value;
                k++;
            }
            if (inputs[4])
            {
                while (project_data.containers[ID].items[j].value == value && project_data.containers[ID].items[j].marked)
                    j++;
                if (is_item_matching(objet_request, project_data.containers[ID].items[j]))
                {
                    project_data.containers[ID].items[j].marked = 1;
                    total++;
                }
            }
            if (inputs[3])
            {
                int last = j;
                while (project_data.containers[ID].items[j].value == value && project_data.containers[ID].items[j].marked)
                {
                    last = j;
                    j++;
                }
                if (project_data.containers[ID].items[last].value == value && project_data.containers[ID].items[last].marked)
                {
                    project_data.containers[ID].items[last].marked = 0;
                    total--;
                }
            }
        }
    }
    clean_inputs();
    return total;
}

char check_card_capacities(char* card_capacities)
{
    char current_cards[24] = {0};
    int i = 0;
    while (i < 40)
    {
        if (project_data.inventories[player][i].type == 8)
        {
            int j = 0;
            long container = project_data.inventories[player][i].ID;
            while (j < project_data.containers[container].size)
            {
                if (project_data.containers[container].items[j].type == 15 && project_data.containers[container].items[j].marked)
                    current_cards[j]++;
                j++;
            }
        }
        i++;
    }
    i = 0;
    while (i < 24)
    {
        if (card_capacities[i] < current_cards[i])
            return 0;
        i++;
    }
    return 1;
}

char item_choice_request(struct item objet_request, int quantity, char* cards_capacities)
{
    int total = 0;
    unmark_all_items();
    clean_inputs();
    int x = 0;
    int y = 0;
    int valid = 0;
    while (!inputs[0] && !valid)
    {
        rect(56, 56, 592, 592, 255, 255, 255);
        rect(60, 60, 584, 584, 0, 0, 0);
        int i = 0;
        while (i < 8)
        {
            int j = 0;
            while (j < 5)
            {
                rect(68 + 72 * i, 68 + 72 * j, 64, 64, 50, 50 + project_data.inventories[player][i + j * 8].marked * 150, 50);
                if (project_data.inventories[player][i + j * 8].type)
                    display_sprite(3, 68 + 72 * i, 68 + 72 * j, 64, project_data.inventories[player][i + j * 8].type - 1, 0);
                j++;
            }
            i++;
        }
        rect(60, 428, 584, 4, 255, 255, 255);
        rect(64 + 72 * x, 64 + 72 * y, 72, 4, 0, 0, 255);
        rect(64 + 72 * x, 64 + 72 * y, 4, 72, 0, 0, 255);
        rect(132 + 72 * x, 64 + 72 * y, 4, 72, 0, 0, 255);
        rect(64 + 72 * x, 132 + 72 * y, 72, 4, 0, 0, 255);
        
        print_text_centered(112, 436, items_texts[project_data.inventories[player][x + y * 8].type], 1, 1, 480);
        print_text_centered(112, 466, "A selectionner :", 1, 1, 480);
        char item_desc[201] = {0};
        get_item_description_item(objet_request, quantity, item_desc);
        print_text_centered(112, 496, item_desc, 1, 1, 480);
        print_text_centered(112, 526, "Selection :", 1, 1, 480);
        print_int_centered(112, 556, total, 0, 1, 1, 480);
        print_refresh();
        load_input_long();
        if (inputs[3] && x)
            x--;
        if (inputs[4] && x < 7)
            x++;
        if (inputs[1] && y)
            y--;
        if (inputs[2] && y < 4)
            y++;
        if (inputs[5])
        {
            struct item objet = project_data.inventories[player][x + y * 8];
            if (objet.type)
            {
                if (is_item_matching(objet, objet_request))
                {
                    if (project_data.inventories[player][x + y * 8].marked)
                    {
                        project_data.inventories[player][x + y * 8].marked = 0;
                        total -= 1;
                    }
                    else
                    {
                        project_data.inventories[player][x + y * 8].marked = 1;
                        total += 1;
                    }
                }
                if (objet.type == 9 ||objet.type == 7)
                    total = item_choice_container_request(objet.ID, objet.type, objet_request, quantity, total);
                if (objet.type == 8)
                    total = item_choice_card_request(objet.ID, objet_request, quantity, total);
            }
        }
        if (inputs[6])
        {
            if (total == quantity)
            {
                if (objet_request.type == 15)
                    return check_card_capacities(cards_capacities);
                valid = 1;
            }
            if (total > quantity)
                print_error("Too many items selected.");
            if (total < quantity)
                print_error("Not enough items selected.");
            if (!valid)
            {
                clean_inputs();
                return 0;
            }
        }
    }
    clean_inputs();
    return valid;
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
            //print_error("drop :");
            //print_error_int(i);
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

void take_item_position(struct position p, int character)
{
    if (is_in_map(p.x / 8, p.y / 8, p.map, p.zone))
    {
        struct item objet = project_data.zones[p.zone - 1].maps[p.map].cells[p.x / 8][p.y / 8].item;
        if (objet.type == 14)
        {
            objet.value = p.map + 1;
            project_data.zones[p.zone - 1].maps[p.map].cells[p.x / 8][p.y / 8].item = objet;
        }
        if (can_take_item(character, objet, 1))
        {
            take_item(character, objet);
            project_data.zones[p.zone - 1].maps[p.map].cells[p.x / 8][p.y / 8].item.type = 0;
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
            project_data.zones[zone - 1].maps[map].cells[i][j].type = backup_data.zones[zone - 1].maps[map].cells[i][j].type;
            project_data.zones[zone - 1].maps[map].cells[i][j].item = backup_data.zones[zone - 1].maps[map].cells[i][j].item;
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
            if (i == player)
                map_changed = 1;
        }
        i++;
    }
}

void quit_portal(int zone, int map, int player)
{
    if (zone < 2 || zone == 5)
        return;
    //print_error_int(zone);
    //print_error_int(map);
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
    reset_cinematic_triggers();
    in_motion = 0;
    map_changed = 1;
}

void get_item_description(struct request r, char* str)
{
    char number[21] = {0};
    int_to_str(number, r.objective.value4, 0);
    const char* str_fields[3] = {items_texts[r.objective.value1 / 2], " x ", number};
    concat_str(str, str_fields, 200, 3);
}

void get_card_capacities(int character, char* card_capacities)
{
    int i = 0;
    while (i < 40)
    {
        if (project_data.inventories[character][i].type == 8)
        {
            int j = 0;
            long container = project_data.inventories[character][i].ID;
            while (j < project_data.containers[container].size)
            {
                if (project_data.containers[container].items[j].type == 0)
                    card_capacities[j]++;
                j++;
            }
        }
        i++;
    }
}

void dealing_with_request(int character, int value)
{
    struct request r = project_data.requests[character][value];
    char success = (r.objective.activated == 2);
    if (r.objective.type == 0 && !success)
    {
        display_message("Petite discussion.");
        success = 1;
    }
    if (r.objective.type == 1 && !success)
    {
        display_message("Montre moi...");
        char item_desc[201] = {0};
        get_item_description(r, item_desc);
        char item_str[201] = {0};
        const char* str[2] = {"Objet : ", item_desc};
        concat_str(item_str, str, 200, 2);
        display_message(item_str);
        if (has_item_request(player, r))
            success = 1;
    }
    if (r.objective.type == 2 && !success)
    {
        display_message("Donne moi...");
        char item_desc[201] = {0};
        get_item_description(r, item_desc);
        char item_str[201] = {0};
        const char* str[2] = {"Objet : ", item_desc};
        concat_str(item_str, str, 200, 2);
        display_message(item_str);
        struct item objet = get_item_from_request(r);
        if (can_take_item(character, objet, 1))
        {
            //print_error("Try to get it !");
            char cards_capacities[24] = {0};
            get_card_capacities(character, cards_capacities);
            if (item_choice_request(objet, r.objective.value4, cards_capacities))
            {
                exchange_marked_items(character);
		        //inventory();
                success = 1;
            }
        }
        else
            display_message("Ah non, je ne peux pas prendre ça en fait.");
    }
    if (success)
    {
        //print_error("success");
        project_data.requests[character][value].objective.activated = 2;
        //print_error_int(character);
        //print_error_int(project_data.character_positions[player].zone);
        int request_character = character;
        if (character == 4)
        {
            if (project_data.character_positions[player].zone == 1)
                request_character = 4 + project_data.character_positions[player].map;
            if (project_data.character_positions[player].zone == 5)
                request_character = 4 + project_data.character_positions[player].map + project_data.zones[0].map_number;
        }
        //print_error_int(request_character);
        
        if (r.reward.type && has_item(character, r.reward, 1))
        {
            //print_error("reward");
            if (can_take_item(player, r.reward, 1))
            {
                struct item objet = drop_item(character, r.reward);
                take_item(player, objet);
                add_item_notification(objet);
                project_data.requests[character][value].objective.activated = 0;
                save_data.request_states[request_character].active = 0;
                update_active_requests();
            }
            else
            {
                display_message("Oh, tu ne peux pas prendre de recompense...");
                display_message("Reviens plus tard !");
            }
        }
        if (!r.reward.type)
        {
            //print_error("no reward");
            project_data.requests[character][value].objective.activated = 0;
            save_data.request_states[request_character].active = 0;
            update_active_requests();
        }
    }
    clean_inputs();
}

void main_loop()
{
    clean_inputs();
    char first_move = 0;
    in_motion = 0;
    struct position p_player;
    while (!endgame && !inputs[0])
    {
        update_knowledge(player);
        p_player = project_data.character_positions[player];
        display_current_screen(p_player);
        display_notifications();
        print_refresh();
        load_input();
        if (inputs[18])
            quit_portal(p_player.zone, p_player.map, player);
        if (map_changed)
            in_motion = 0;
        p_player = project_data.character_positions[player];
        
        if (!in_motion && !map_changed && inputs[5])
        {
            print_error("crash ?");
            if (is_in_map(p_player.x / 8, p_player.y / 8, p_player.map, p_player.zone))
            {
                struct item objet = project_data.zones[p_player.zone - 1].maps[p_player.map].cells[p_player.x / 8][p_player.y / 8].item;
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
            print_error("Nope.");
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
            project_data.character_positions[player] = p_player;
            p_player = project_data.character_positions[player];
            display_current_screen(p_player);
            display_notifications();
            print_refresh();
            if (inputs[1] || inputs[2] || inputs[3] || inputs[4])
            {
                first_move = 1;
                in_motion = 1;
            }
            if (!is_in_map(p_player.x / 8, p_player.y / 8, p_player.map, p_player.zone))
            {
                first_move = 0;
                in_motion = 0;
            }
        }
        //Cinematics when passing by a cell
        if (in_motion && !(p_player.x % 8 || p_player.y % 8))
        {
            if (is_in_map(p_player.x / 8, p_player.y / 8, p_player.map, p_player.zone))
            {
                long cinematic_id = project_data.zones[p_player.zone - 1].maps[p_player.map].cells[p_player.x / 8][p_player.y / 8].cinematic_trigger;
                if (cinematic_id)
                {
                    long id = cinematic_id;
                    while (id && project_data.cinematics[id - 1].trigger.type != 1)
                    {
                        id = find_another_cinematic_position(project_data.cinematics[id - 1].trigger.p, id);
                    }
                    if (id)
                        handle_cinematic(id - 1);
                }
            }
        }
        if (in_motion && !(p_player.x % 8 || p_player.y % 8) && is_in_map(p_player.x / 8, p_player.y / 8, p_player.map, p_player.zone))
        {
            struct cell cell = get_front_cell(p_player);
            if (cell.type == 2)
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
        else
            in_motion = 0;
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
            if (is_in_map(p_player.x / 8, p_player.y / 8, p_player.map, p_player.zone))
            {
                project_data.zones[p_player.zone - 1].maps[p_player.map].remaining_green_cells = 0;
                while (i < project_data.zones[p_player.zone - 1].maps[p_player.map].x)
                {
                    int j = 0;
                    while (j < project_data.zones[p_player.zone - 1].maps[p_player.map].y)
                    {
                        if (project_data.zones[p_player.zone - 1].maps[p_player.map].cells[i][j].type == 7)
                            project_data.zones[p_player.zone - 1].maps[p_player.map].remaining_green_cells++;
                        j++;
                    }
                    i++;
                }
            }
            map_changed = 0;
            in_motion = 0;
            update_request_positions();
            struct cell cell = get_current_cell(p_player);
            if (cell.type >= 3 && cell.type <= 6)
                in_motion = 1;
            if (cell.type == 3)
                p_player.orientation = 2;
            if (cell.type == 4)
                p_player.orientation = 0;
            if (cell.type == 5)
                p_player.orientation = 1;
            if (cell.type == 6)
                p_player.orientation = 3;
            if (cell.type == 8)
            {
                project_data.character_positions[player] = p_player;
                quit_portal(p_player.zone, p_player.map, player);
                p_player = project_data.character_positions[player];
                map_changed = 1;
            }
            if (cell.type == 7)
            {
                project_data.zones[p_player.zone - 1].maps[p_player.map].cells[p_player.x / 8][p_player.y / 8].type = 8;
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
                struct cell cell = get_current_cell(p_player);
                if (cell.type == 1 || (cell.type >= 3 && cell.type <= 6))
                    in_motion = 1;
                if (cell.type == 3)
                    p_player.orientation = 2;
                if (cell.type == 4)
                    p_player.orientation = 0;
                if (cell.type == 5)
                    p_player.orientation = 1;
                if (cell.type == 6)
                    p_player.orientation = 3;
                if (cell.type == 8)
                {
                    project_data.character_positions[player] = p_player;
                    reboot_portal(p_player.zone, p_player.map, player);
                    p_player = project_data.character_positions[player];
                    map_changed = 1;
                }
                if (cell.type == 7)
                {
                    project_data.zones[p_player.zone - 1].maps[p_player.map].cells[p_player.x / 8][p_player.y / 8].type = 8;
                    project_data.zones[p_player.zone - 1].maps[p_player.map].remaining_green_cells--;
                }
            }
            if (!in_motion && p_player.zone == 2 && project_data.zones[p_player.zone - 1].maps[p_player.map].remaining_green_cells == 0)
            {
                //Portal is activated
                struct position p = save_data.portals[player].portal_position;
                project_data.zones[p.zone - 1].maps[p.map].cells[p.x / 8][p.y / 8].item.activation = 1;
                //make the player leave the portal
                project_data.character_positions[player] = p_player;
                quit_portal(p_player.zone, p_player.map, player);
                p_player = project_data.character_positions[player];
            }
        }
        project_data.character_positions[player] = p_player;
        //Cinematics when stop at a cell
        if (!in_motion && !map_changed && !(p_player.x % 8) && !(p_player.y % 8))
        {
            if (is_in_map(p_player.x / 8, p_player.y / 8, p_player.map, p_player.zone))
            {
                long cinematic_id = project_data.zones[p_player.zone - 1].maps[p_player.map].cells[p_player.x / 8][p_player.y / 8].cinematic_trigger;
                if (cinematic_id)
                {
                    handle_cinematic(cinematic_id - 1);
                }
            }
        }
        savable = !in_motion && !map_changed;
    }
}

void launch_game(char with_save, int save_spot)
{
    endgame = 0;
    savable = 1;
    in_motion = 0;
    map_changed = 0;
    reset_cinematic_triggers();
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
            backup_data.zones[i].maps[j].cells = malloc(sizeof(struct cell*) * project_data.zones[i].maps[j].x);
            valid = (backup_data.zones[i].maps[j].cells != NULL);
            int x = 0;
            while (x < project_data.zones[i].maps[j].x && valid)
            {
                backup_data.zones[i].maps[j].cells[x] = malloc(sizeof(struct cell) * project_data.zones[i].maps[j].y);
                valid = (backup_data.zones[i].maps[j].cells[x] != NULL);
                int y = 0;
                while (y < project_data.zones[i].maps[j].y && valid)
                {
                    backup_data.zones[i].maps[j].cells[x][y] = project_data.zones[i].maps[j].cells[x][y];
                    y++;
                }
                x++;
            }
            while (x && !valid)
            {
                x--;
                free(backup_data.zones[i].maps[j].cells[x]);
            }
            if (!valid)
            {
                free(backup_data.zones[i].maps[j].cells);
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
                    x++;
                }
                free(backup_data.zones[i].maps[j].cells);
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
        free_save_data(&save_data);
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
    //print_error("Start !");
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
        i = 0;
        while (i < project_data.parameters[12])
        {
            if (project_data.cinematics[i].trigger.type == 0 && project_data.cinematics[i].trigger.active)
                handle_cinematic(i);
            i++;
        }
        update_active_requests();
        main_loop();
    }
    free_backup();
    free_save_data(&save_data);
}