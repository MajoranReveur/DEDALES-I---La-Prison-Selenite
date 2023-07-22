#include "map_modify.h"

int x_camera = 0;
int y_camera = 0;
int x_cursor = 0;
int y_cursor = 0;
char x_printer[20] = { 0 };
char y_printer[20] = { 0 };

void update_camera(int zone, int map)
{
    if (x_cursor < 0)
        x_cursor = 0;
    if (y_cursor < 0)
        y_cursor = 0;
    if (x_cursor >= project_data.zones[zone].maps[map].x)
        x_cursor = project_data.zones[zone].maps[map].x - 1;
    if (y_cursor >= project_data.zones[zone].maps[map].y)
        y_cursor = project_data.zones[zone].maps[map].y - 1;
    if (x_camera + 1 > x_cursor)
        x_camera = x_cursor - 1;
    if (x_camera < x_cursor - 9)
        x_camera = x_cursor - 9;
    if (y_camera + 1 > y_cursor)
        y_camera = y_cursor - 1;
    if (y_camera < y_cursor - 9)
        y_camera = y_cursor - 9;
    int i = 1;
    int a = project_data.zones[zone].maps[map].x;
    while (a > 9)
    {
        a /= 10;
        i++;
    }
    int j = 1;
    int b = project_data.zones[zone].maps[map].y;
    while (b > 9)
    {
        b /= 10;
        j++;
    }
    if (i > 3)
        i = 3;
    if (j > 3)
        j = 3;
    x_printer[0] = 'X';
    x_printer[1] = ':';
    y_printer[0] = 'Y';
    y_printer[1] = ':';
    int_to_str(x_printer + 2, x_cursor + 1, i);
    int_to_str(x_printer + i + 3, project_data.zones[zone].maps[map].x, i);
    x_printer[i + 2] = '/';
    int_to_str(y_printer + 2, y_cursor + 1, j);
    int_to_str(y_printer + j + 3, project_data.zones[zone].maps[map].y, j);
    y_printer[j + 2] = '/';
}

int cell_choice(int type)
{
    inputs[5] = 0;
    inputs[6] = 0;
    inputs[10] = 0;
    int i = type % 6;
    int j = type / 6;
    while (!inputs[0] && !inputs[5] && !inputs[6])
    {
        int y = 0;
        rect(104, 92, 496, 276, 0, 0, 255);
        rect(108, 96, 488, 268, 0, 0, 0);
        while (y < 2)
        {
            int x = 0;
            while (x < 6)
            {
                display_sprite(0, x * 80 + 112 + 8, y * 80 + 100 + 8, 64, x + y * 6, 0);
                x++;
            }
            y++;
        }
        cursor(i * 80 + 112, j * 80 + 100, 80, 80, 255, 0, 0);
        if (i + j * 6 < 9)
            print_text_centered(112, 280, cells_texts[i + j * 6], 1, 1, 480);
        print_refresh();
        load_input_long();
        if (inputs[1] && j)
            j--;
        if (inputs[2] && j < 1)
            j++;
        if (inputs[3] && i)
            i--;
        if (inputs[4] && i < 5)
            i++;
    }
    if (i + j * 6 < 9)
        return i + j * 6;
    return type;
}

void modify_cell_map(int zone, int *map)
{
    int type = 0;
    inputs[5] = 0;
    inputs[6] = 0;
    inputs[10] = 0;
    while (!inputs[0] && !inputs[6])
    {
        display_map_cells(x_camera * 8, y_camera * 8, project_data.zones[zone].maps[*map]);
        rect(704, 0, 400, 704, 0, 0, 0);
        print_text_centered(704, 10, zones_texts[zone], 0, 1, 400);
        print_int_centered(704, 50, *map + 1, 3, 1, 1, 400);
        print_text_centered(704, 150, "Changer de Type :", 1, 1, 400);
        print_text_centered(704, 180, get_key_name(9), 1, 6, 400);
        print_text_centered(704, 210, "Affecter le Type :", 1, 1, 400);
        print_text_centered(704, 240, get_key_name(4), 1, 6, 400);
        print_text_centered(704, 270, "Retour :", 1, 1, 400);
        print_text_centered(704, 300, get_key_name(5), 1, 6, 400);
        print_text_centered(704, 450, "Type actuel :", 1, 1, 400);
        cursor((x_cursor - x_camera) * 64, (y_cursor - y_camera) * 64, 64, 64, 255, 0, 0);
        display_sprite(0, 874, 500, 64, type, 0);
        print_text_centered(704, 600, x_printer, 1, 1, 400);
        print_text_centered(704, 630, y_printer, 1, 1, 400);
        print_refresh();
        load_input_long();
        if (inputs[5])
        {
            if (x_cursor >= 0 && x_cursor < project_data.zones[zone].maps[*map].x && y_cursor >= 0 && y_cursor < project_data.zones[zone].maps[*map].y)
                project_data.zones[zone].maps[*map].cells[x_cursor][y_cursor] = type;
        }
        if (inputs[10])
            type = cell_choice(type);
        if (inputs[1])
            y_cursor--;
        if (inputs[2])
            y_cursor++;
        if (inputs[3])
            x_cursor--;
        if (inputs[4])
            x_cursor++;
        if (inputs[16] && *map)
            (*map)--;
        if (inputs[17] && *map + 1 < project_data.zones[zone].map_number)
            (*map)++;
        update_camera(zone, *map);
    }
}

void modify_item_map(int zone, int *map)
{
    clean_inputs();
    while (!inputs[0] && !inputs[6])
    {
        display_map_cells(x_camera * 8, y_camera * 8, project_data.zones[zone].maps[*map]);
        display_map_items(x_camera * 8, y_camera * 8, project_data.zones[zone].maps[*map], 0);
        rect(704, 0, 400, 704, 0, 0, 0);
        print_text_centered(704, 10, zones_texts[zone], 0, 1, 400);
        print_int_centered(704, 50, *map + 1, 3, 1, 1, 400);
        struct item item = project_data.zones[zone].maps[*map].items[x_cursor][y_cursor];
        print_text_centered(704, 200, "Type :", 1, 1, 400);
        print_text_centered(704, 220, items_texts[item.type], 1, 1, 400);
        int type = item.type;
        int j = 0;
        if (type == 5 || (type >= 15 && type <= 20) || type == 22)
        {
            print_text_centered(704, 280 + j * 70, "Valeur :", 1, 1, 400);
            if (item.value)
            {
                if (type == 15)
                    print_text_centered(704, 305 + j * 70, cards_texts[item.value], 1, 6, 400);
                else
                    print_int_centered(704, 305 + j * 70, item.value, 0, 1, 6, 400);
            }
            else
                print_text_centered(704, 305 + j * 70, "Non defini", 1, 6, 400);
            j++;
        }
        if (type == 6)
        {
            print_text_centered(704, 280 + j * 70, "Lieu :", 1, 1, 400);
            if (item.value && item.ID)
            {
                print_text_centered(704, 300 + j * 70, zones_texts[item.value], 1, 6, 400);
                print_int_centered(704, 320 + j * 70, item.ID, 0, 1, 6, 400);
            }
            else
                print_text_centered(704, 305 + j * 70, "Non defini", 1, 6, 400);
            j++;
        }
        if (type == 13)
        {
            print_text_centered(704, 280 + j * 70, "Puissance :", 1, 1, 400);
            print_int_centered(704, 305 + j * 70, item.value, 0, 1, 6, 400);
            j++;
        }
        if (type >= 7 && type <= 13)
        {
            print_text_centered(704, 280 + j * 70, "Capacite :", 1, 1, 400);
            char capacity[20] = {0};
            char content[20] = {0};
            int size = project_data.containers[item.ID].size;
            int count = 0;
            int i = 0;
            while (i < size)
            {
                if (project_data.containers[item.ID].items[i].type)
                    count++;
                i++;
            }
            int_to_str(capacity, size, 1);
            int_to_str(content, count, 1);
            const char* fields[3] = {content, "/", capacity};
            char capacity_output[8] = {0};
            concat_str(capacity_output, fields, 8, 3);
            print_text_centered(704, 305 + j * 70, capacity_output, 1, 6, 400);
            j++;
        }
        if (type >= 16 && type <= 20)
        {
            print_text_centered(704, 280 + j * 70, "Sceau :", 1, 1, 400);
            print_text_centered(704, 305 + j * 70, cards_texts[item.ID], 1, 6, 400);
            j++;
        }
        if (type == 3 || type == 5 || type == 7 || type == 12 || type == 13 || type == 16)
        {
            print_text_centered(704, 280 + j * 70, "Active ?", 1, 1, 400);
            if (item.activation)
                print_text_centered(704, 305 + j * 70, "Oui", 1, 6, 400);
            else
                print_text_centered(704, 305 + j * 70, "Non", 1, 6, 400);
            j++;
        }
        cursor((x_cursor - x_camera) * 64, (y_cursor - y_camera) * 64, 64, 64, 255, 0, 0);
        cursor(866, 492, 80, 80, 0, 0, 255);
        if (type)
        {
            rect(874, 500, 64, 64, 150, 150, 150);
            display_sprite(3, 874, 500, 64, item.type - 1, 0);
        }
        print_text_centered(704, 600, x_printer, 1, 1, 400);
        print_text_centered(704, 630, y_printer, 1, 1, 400);
        print_refresh();
        load_input_long();
        if (inputs[5])
        {
            if (x_cursor >= 0 && x_cursor < project_data.zones[zone].maps[*map].x && y_cursor >= 0 && y_cursor < project_data.zones[zone].maps[*map].y)
            {
                struct position p = {zone + 1, *map, x_cursor, y_cursor};
                modify_item(&item, p, 0);
                project_data.zones[zone].maps[*map].items[x_cursor][y_cursor] = item;
            }
        }
        if (inputs[1])
            y_cursor--;
        if (inputs[2])
            y_cursor++;
        if (inputs[3])
            x_cursor--;
        if (inputs[4])
            x_cursor++;
        if (inputs[16] && *map)
            (*map)--;
        if (inputs[17] && *map + 1 < project_data.zones[zone].map_number)
            (*map)++;
        update_camera(zone, *map);
    }
    clean_inputs();
}

void move_cursor(int zone, int *map)
{
    inputs[5] = 0;
    inputs[6] = 0;
    inputs[10] = 0;
    while (!inputs[0] && !inputs[5] && !inputs[6])
    {
        display_map_cells(x_camera * 8, y_camera * 8, project_data.zones[zone].maps[*map]);
        rect(704, 0, 400, 704, 0, 0, 0);
        print_text_centered(704, 10, zones_texts[zone], 0, 1, 400);
        print_int_centered(704, 50, *map + 1, 3, 1, 1, 400);
        print_text_centered(704, 600, x_printer, 1, 1, 400);
        print_text_centered(704, 630, y_printer, 1, 1, 400);
        cursor((x_cursor - x_camera) * 64, (y_cursor - y_camera) * 64, 64, 64, 255, 0, 0);
        print_refresh();
        load_input_long();
        if (inputs[1])
            y_cursor--;
        if (inputs[2])
            y_cursor++;
        if (inputs[3])
            x_cursor--;
        if (inputs[4])
            x_cursor++;
        if (inputs[16] && *map)
            (*map)--;
        if (inputs[17] && *map + 1 < project_data.zones[zone].map_number)
            (*map)++;
        update_camera(zone, *map);
    }
    clean_inputs();
}

void change_start(int zone, int* map)
{
    x_cursor = project_data.zones[zone].maps[*map].x_start;
    y_cursor = project_data.zones[zone].maps[*map].y_start;
    update_camera(zone, *map);
    inputs[5] = 0;
    inputs[6] = 0;
    inputs[10] = 0;
    while (!inputs[0] && !inputs[5] && !inputs[6])
    {
        display_map_cells(x_camera * 8, y_camera * 8, project_data.zones[zone].maps[*map]);
        rect(704, 0, 400, 704, 0, 0, 0);
        print_text_centered(704, 10, zones_texts[zone], 0, 1, 400);
        print_int_centered(704, 50, *map + 1, 3, 1, 1, 400);
        print_text_centered(704, 600, x_printer, 1, 1, 400);
        print_text_centered(704, 630, y_printer, 1, 1, 400);
        cursor((x_cursor - x_camera) * 64, (y_cursor - y_camera) * 64, 64, 64, 255, 0, 0);
        print_refresh();
        load_input_long();
        if (inputs[1])
            y_cursor--;
        if (inputs[2])
            y_cursor++;
        if (inputs[3])
            x_cursor--;
        if (inputs[4])
            x_cursor++;
        if (inputs[16] && *map)
        {
            (*map)--;
            x_cursor = project_data.zones[zone].maps[*map].x_start;
            y_cursor = project_data.zones[zone].maps[*map].y_start;
        }
        if (inputs[17] && *map + 1 < project_data.zones[zone].map_number)
        {
            (*map)++;
            x_cursor = project_data.zones[zone].maps[*map].x_start;
            y_cursor = project_data.zones[zone].maps[*map].y_start;
        }
        update_camera(zone, *map);
    }
    if (inputs[5])
    {
        project_data.zones[zone].maps[*map].x_start = x_cursor;
        project_data.zones[zone].maps[*map].y_start = y_cursor;
    }
    clean_inputs();
}

char position_choice_cell(int zone, int *map, struct position *p)
{
    inputs[5] = 0;
    inputs[6] = 0;
    inputs[10] = 0;
    if (p->zone)
    {
        x_cursor = p->x;
        y_cursor = p->y;
    }
    update_camera(zone, *map);
    clean_inputs();
    while (!inputs[0] && !inputs[5] && !inputs[6])
    {
        display_map_cells(x_camera * 8, y_camera * 8, project_data.zones[zone].maps[*map]);
        display_map_items(x_camera * 8, y_camera * 8, project_data.zones[zone].maps[*map], 0);
        rect(704, 0, 400, 704, 0, 0, 0);
        print_text_centered(704, 10, zones_texts[zone], 0, 1, 400);
        print_int_centered(704, 50, *map + 1, 3, 1, 1, 400);
        print_text_centered(704, 600, x_printer, 1, 1, 400);
        print_text_centered(704, 630, y_printer, 1, 1, 400);
        cursor((x_cursor - x_camera) * 64, (y_cursor - y_camera) * 64, 64, 64, 255, 0, 0);
        print_refresh();
        load_input_long();
        if (inputs[1])
            y_cursor--;
        if (inputs[2])
            y_cursor++;
        if (inputs[3])
            x_cursor--;
        if (inputs[4])
            x_cursor++;
        if (inputs[16] && *map)
            (*map)--;
        if (inputs[17] && *map + 1 < project_data.zones[zone].map_number)
            (*map)++;
        update_camera(zone, *map);
    }
    if (inputs[5])
    {
        p->x = x_cursor;
        p->y = y_cursor;
        p->map = *map;
        p->zone = zone + 1;
        clean_inputs();
        return 1;
    }
    clean_inputs();
    return 0;
}

void delete_map(int zone, int map)
{
    int old_x = project_data.zones[zone].maps[map].x;
    int old_y = project_data.zones[zone].maps[map].y;
    int i = 0;
    while (i < old_x)
    {
        int j = 0;
        while (j < old_y)
        {
            int type = project_data.zones[zone].maps[map].items[i][j].type;
            if (type >= 7 && type <= 13)
                delete_container(project_data.zones[zone].maps[map].items[i][j].ID);
            j++;
        }
        i++;
    }
    free_map(project_data.zones[zone].maps[map]);
    //Shift the maps of the same zone
    i = map;
    while (i + 1 < project_data.zones[zone].map_number)
    {
        project_data.zones[zone].maps[i].cells = project_data.zones[zone].maps[i + 1].cells;
        project_data.zones[zone].maps[i].items = project_data.zones[zone].maps[i + 1].items;
        project_data.zones[zone].maps[i].thoughts = project_data.zones[zone].maps[i + 1].thoughts;
        project_data.zones[zone].maps[i].color_length = project_data.zones[zone].maps[i + 1].color_length;
        project_data.zones[zone].maps[i].color_sequency = project_data.zones[zone].maps[i + 1].color_sequency;
        project_data.zones[zone].maps[i].initial_delay = project_data.zones[zone].maps[i + 1].initial_delay;
        project_data.zones[zone].maps[i].x = project_data.zones[zone].maps[i + 1].x;
        project_data.zones[zone].maps[i].y = project_data.zones[zone].maps[i + 1].y;
        i++;
    }
    project_data.zones[zone].map_number--;
    //Try to realloc the total zone maps
    if (project_data.zones[zone].map_number)
    {
        struct map* new_map_list = realloc(project_data.zones[zone].maps, sizeof(struct map) * project_data.zones[zone].map_number);
        if (new_map_list)
            project_data.zones[zone].maps = new_map_list;
    }
    else
        project_data.zones[zone].maps = NULL;
    //Delete all items relative to the map
    i = 0;
    while (i < project_data.parameters[11])
    {
        int j = 0;
        while (j < project_data.zones[i].map_number)
        {
            int x = 0;
            while (x < project_data.zones[i].maps[j].x)
            {
                int y = 0;
                while (y < project_data.zones[i].maps[j].x)
                {
                    int type = project_data.zones[i].maps[j].items[x][y].type;
                    int value = project_data.zones[i].maps[j].items[x][y].value;
                    long ID = project_data.zones[i].maps[j].items[x][y].ID;
                    if ((type == 5 && zone == 3) ||
                    (type == 16 && zone == 1) ||
                    (type == 17 && zone == 3) ||
                    (type == 18 && zone == 2) ||
                    (type == 19 && zone == 2)
                    )
                    {
                        if (value == map + 1)
                            project_data.zones[i].maps[j].items[x][y].value = 0;
                        if (value > map + 1)
                            project_data.zones[i].maps[j].items[x][y].value--;
                    }
                    if (type == 6 && value == zone + 1)
                    {
                        if (ID == map + 1)
                        {
                            project_data.zones[i].maps[j].items[x][y].ID = 0;
                            project_data.zones[i].maps[j].items[x][y].value = 0;
                        }
                        if (ID > map + 1)
                            project_data.zones[i].maps[j].items[x][y].ID--;
                    }
                    if ((type == 9 && zone == 1) ||
                    (type == 10 && zone == 2) ||
                    (type == 11 && zone == 3))
                    {
                        int index = 0;
                        int count = 0;
                        while (index < project_data.containers[ID].size)
                        {
                            if (project_data.containers[ID].items[index].value == map + 1)
                                count++;
                            index++;
                        }
                        while (count)
                        {
                            remove_item(project_data.containers[ID], map + 1);
                            count--;
                        }
                        index = 0;
                        while (index < project_data.containers[ID].size)
                        {
                            if (project_data.containers[ID].items[index].value > map + 1)
                                project_data.containers[ID].items[index].value--;
                            index++;
                        }
                    }
                    if (type == 7 && (zone == 0 || zone == 4) && project_data.zones[zone].map_number >= project_data.parameters[5] && project_data.zones[zone].map_number >= project_data.parameters[9])
                    {
                        int max = project_data.parameters[5];
                        if (max < project_data.parameters[9])
                            max = project_data.parameters[9];
                        int index = 0;
                        int count = 0;
                        while (index < project_data.containers[ID].size)
                        {
                            if (project_data.containers[ID].items[index].value == map + 1)
                                count++;
                            index++;
                        }
                        while (count)
                        {
                            remove_item(project_data.containers[ID], map + 1);
                            count--;
                        }
                    }
                    y++;
                }
                x++;
            }
            j++;
        }
        i++;
    }
    print_error("Carte suprimee !");
}

void change_map_dimensions(int zone, int map)
{
    int old_x = project_data.zones[zone].maps[map].x;
    int old_y = project_data.zones[zone].maps[map].y;
    rect(0, 0, 1104, 704, 0, 0, 0);
    char controls_string[100] = {0};
    const char* controls_strings[4] = {"Changer le chiffre : ", get_key_name(0), ", ", get_key_name(1)};
    concat_str(controls_string, controls_strings, 100, 4);
    print_text_centered(0, 610, controls_string, 1, 6, 1104);
    controls_strings[0] = "Changer de chiffre : ";
    controls_strings[1] = get_key_name(2);
    controls_strings[3] = get_key_name(3);
    concat_str(controls_string, controls_strings, 100, 4);
    print_text_centered(0, 640, controls_string, 1, 6, 1104);
    controls_strings[0] = "Confirmer : ";
    controls_strings[1] = get_key_name(4);
    controls_strings[2] = "";
    controls_strings[3] = "";
    concat_str(controls_string, controls_strings, 100, 4);
    print_text_centered(0, 670, controls_string, 1, 6, 1104);
    print_text_centered(0, 10, project_data.project_name, 0, 1, 1104);
    print_text_centered(0, 50, project_data.author_name, 1, 1, 1104);
    print_text_centered(0, 100, "- Nouvelles Dimensions -", 1, 1, 1104);
    char x_string[20] = {0};
    char y_string[20] = {0};
    int_to_str(x_string, old_x, 0);
    int_to_str(y_string, old_y, 0);
    const char* dimensions_strings[4] = {"Dimensions : ", x_string, "x", y_string};
    char dimensions_string[50] = {0};
    concat_str(dimensions_string, dimensions_strings, 50, 4);
    print_text_centered(0, 150, dimensions_string, 1, 1, 1104);
    print_text_centered(0, 220, "Longueur :", 1, 1, 1104);
    print_text_centered(0, 280, "Largeur :", 1, 1, 1104);
    int x = int_input(old_x, 530, 250);
    int_to_str(x_string, x, 0);
    dimensions_strings[1] = x_string;
    print_text_centered(0, 150, dimensions_string, 1, 0, 1104);
    concat_str(dimensions_string, dimensions_strings, 50, 4);
    print_text_centered(0, 150, dimensions_string, 1, 1, 1104);
    // print_int_centered(0, 200, x, 3, 1, 1, 1104);
    int y = int_input(old_y, 530, 310);
    int_to_str(y_string, y, 0);
    dimensions_strings[3] = y_string;
    print_text_centered(0, 150, dimensions_string, 1, 0, 1104);
    concat_str(dimensions_string, dimensions_strings, 50, 4);
    print_text_centered(0, 150, dimensions_string, 1, 1, 1104);
    // print_int_centered(0, 260, y, 3, 1, 1, 1104);
    if (!x || !y)
    {
        print_error("Une carte ne peut pas avoir 0 cases !");
        return;
    }
    //Allocation of the new map
    int **x_cells = malloc(sizeof(int *) * x);
    char **x_thoughts = NULL;
    struct item **x_items = NULL;
    if (x_cells != NULL)
        x_items = malloc(sizeof(struct item *) * x);
    if (x_items != NULL)
        x_thoughts = malloc(sizeof(char *) * x);
    char valid = x_thoughts != NULL;
    int i = 0;
    while (i < x && valid)
    {
        x_cells[i] = NULL;
        x_items[i] = NULL;
        x_thoughts[i] = NULL;
        i++;
    }
    i = 0;
    while (i < x && valid)
    {
        x_cells[i] = malloc(sizeof(int) * y);
        x_items[i] = malloc(sizeof(struct item) * y);
        x_thoughts[i] = malloc(sizeof(char) * y);
        valid = (x_cells[i] != NULL && x_items[i] != NULL && x_thoughts[i] != NULL);
        i++;
    }
    while (i && !valid)
    {
        i--;
        free(x_cells[i]);
        free(x_items[i]);
        free(x_thoughts[i]);
    }
    if (!valid)
    {
        free(x_cells);
        free(x_items);
        free(x_thoughts);
        print_error("Pas assez de memoire !");
        return;
    }
    i = 0;
    print_error("Initialisation...");
    while (i < x)
    {
        int j = 0;
        while (j < y)
        {
            x_cells[i][j] = 0;
            x_items[i][j].activation = 0;
            x_items[i][j].ID = 0;
            x_items[i][j].type = 0;
            x_items[i][j].value = 0;
            x_thoughts[i][j] = 0;
            j++;
        }
        i++;
    }
    i = 0;
    //Fill the cells already knew
    while (i < x && i < old_x)
    {
        int j = 0;
        while (j < y && j < old_y)
        {
            x_cells[i][j] = project_data.zones[zone].maps[map].cells[i][j];
            x_items[i][j].activation = project_data.zones[zone].maps[map].items[i][j].activation;
            x_items[i][j].ID = project_data.zones[zone].maps[map].items[i][j].ID;
            x_items[i][j].type = project_data.zones[zone].maps[map].items[i][j].type;
            x_items[i][j].value = project_data.zones[zone].maps[map].items[i][j].value;
            x_thoughts[i][j] = project_data.zones[zone].maps[map].thoughts[i][j];
            j++;
        }
        i++;
    }
    //Delete old containers
    i = x;
    while (i < old_x)
    {
        int j = y;
        while (j < old_y)
        {
            int type = project_data.zones[zone].maps[map].items[i][j].type;
            if (type >= 7 && type <= 13)
                delete_container(project_data.zones[zone].maps[map].items[i][j].ID);
            j++;
        }
        i++;
    }
    //Delete the old map
    free_map(project_data.zones[zone].maps[map]);
    project_data.zones[zone].maps[map].cells = x_cells;
    project_data.zones[zone].maps[map].items = x_items;
    project_data.zones[zone].maps[map].thoughts = x_thoughts;
    project_data.zones[zone].maps[map].x = x;
    project_data.zones[zone].maps[map].y = y;
    print_error("Carte modifiee !");
}

void modify_map(int zone, int map)
{
    char quit = 0;
    clean_inputs();
    update_camera(zone, map);
    int i = 0;
    while (!inputs[0] && !quit)
    {
        while (!inputs[0] && !inputs[5] && !inputs[6] && !inputs[16] && !inputs[17])
        {
            display_map_cells(x_camera * 8, y_camera * 8, project_data.zones[zone].maps[map]);
            display_map_items(x_camera * 8, y_camera * 8, project_data.zones[zone].maps[map], 0);
            rect(704, 0, 400, 704, 0, 0, 0);
            print_text_centered(704, 10, zones_texts[zone], 0, 1, 400);
            print_int_centered(704, 50, map + 1, 3, 1, 1, 400);
            print_text_centered(704, 250, "Cases", 1, 1 + (i == 0), 400);
            print_text_centered(704, 300, "Objets", 1, 1 + (i == 1), 400);
            print_text_centered(704, 350, "Redimensionner", 1, 1 + (i == 2), 400);
            print_text_centered(704, 400, "Supprimer la Carte", 1, 1 + (i == 3), 400);
            print_text_centered(704, 450, "Deplacer le Curseur", 1, 1 + (i == 4), 400);
            if (zone > 0 && zone != 4)
            {
                print_text_centered(704, 500, "Changer le Depart", 1, 1 + (i == 5), 400);
                print_text_centered(704, 550, "Retour", 1, 1 + (i == 6), 400);
            }
            else
                print_text_centered(704, 500, "Retour", 1, 1 + (i == 5), 400);
            print_text_centered(704, 600, x_printer, 1, 1, 400);
            print_text_centered(704, 630, y_printer, 1, 1, 400);
            print_refresh();
            load_input_long();
            if (inputs[1])
                i = (i + 5 + (zone > 0 && zone != 4)) % (6 + (zone > 0 && zone != 4));
            if (inputs[2])
                i = (i + 1) % (6 + (zone > 0 && zone != 4));
        }
        if (inputs[5])
        {
            if (i == 5)
            {
                if (zone > 0 && zone != 4)
                    change_start(zone, &map);
                else
                    quit = 1;
            }
            if (i == 6)
                quit = 1;
            if (i == 0)
                modify_cell_map(zone, &map);
            if (i == 1)
                modify_item_map(zone, &map);
            if (i == 4)
                move_cursor(zone, &map);
            if (i == 2)
            {
                change_map_dimensions(zone, map);
                update_camera(zone, map);
            }
            if (i == 3)
            {
                delete_map(zone, map);
                quit = 1;
            }
            clean_inputs();
        }
        if (inputs[6])
            quit = 1;
        if (inputs[16] && map)
            map--;
        if (inputs[17] && map + 1 < project_data.zones[zone].map_number)
            map++;
        if (inputs[16] || inputs[17])
            update_camera(zone, map);
        clean_inputs();
    }
}

char create_map(int zone)
{
    if (project_data.zones[zone].map_number >= 99)
    {
        print_error("99 Cartes ont deja ete creees !");
        return 0;
    }
    rect(0, 0, 1104, 704, 0, 0, 0);
    char controls_string[100] = {0};
    const char* controls_strings[4] = {"Changer le chiffre : ", get_key_name(0), ", ", get_key_name(1)};
    concat_str(controls_string, controls_strings, 100, 4);
    print_text_centered(0, 610, controls_string, 1, 6, 1104);
    controls_strings[0] = "Changer de chiffre : ";
    controls_strings[1] = get_key_name(2);
    controls_strings[3] = get_key_name(3);
    concat_str(controls_string, controls_strings, 100, 4);
    print_text_centered(0, 640, controls_string, 1, 6, 1104);
    controls_strings[0] = "Confirmer : ";
    controls_strings[1] = get_key_name(4);
    controls_strings[2] = "";
    controls_strings[3] = "";
    concat_str(controls_string, controls_strings, 100, 4);
    print_text_centered(0, 670, controls_string, 1, 6, 1104);
    char x_string[20] = {0};
    char y_string[20] = {0};
    const char* dimensions_strings[4] = {"Dimensions : ", "...", "x", "..."};
    char dimensions_string[50] = {0};
    concat_str(dimensions_string, dimensions_strings, 50, 4);
    print_text_centered(0, 10, project_data.project_name, 0, 1, 1104);
    print_text_centered(0, 50, project_data.author_name, 1, 1, 1104);
    print_text_centered(0, 100, "- Nouvelle Carte -", 1, 1, 1104);
    print_text_centered(0, 150, dimensions_string, 1, 1, 1104);
    print_text_centered(0, 220, "Longueur :", 1, 1, 1104);
    print_text_centered(0, 280, "Largeur :", 1, 1, 1104);
    int x = int_input(0, 530, 250);
    int_to_str(x_string, x, 0);
    dimensions_strings[1] = x_string;
    print_text_centered(0, 150, dimensions_string, 1, 0, 1104);
    concat_str(dimensions_string, dimensions_strings, 50, 4);
    print_text_centered(0, 150, dimensions_string, 1, 1, 1104);
    // print_int_centered(0, 200, x, 3, 1, 1, 1104);
    int y = int_input(0, 530, 310);
    int_to_str(y_string, y, 0);
    dimensions_strings[3] = y_string;
    print_text_centered(0, 150, dimensions_string, 1, 0, 1104);
    concat_str(dimensions_string, dimensions_strings, 50, 4);
    print_text_centered(0, 150, dimensions_string, 1, 1, 1104);
    // print_int_centered(0, 260, y, 3, 1, 1, 1104);
    if (!x || !y)
    {
        print_error("Une carte ne peut pas avoir 0 cases !");
        return 0;
    }
    int **x_cells = malloc(sizeof(int *) * x);
    struct item **x_items = NULL;
    char **x_thoughts = NULL;
    if (x_cells != NULL)
        x_items = malloc(sizeof(struct item *) * x);
    if (x_items != NULL)
        x_thoughts = malloc(sizeof(char *) * x);
    char valid = x_thoughts != NULL;
    int i = 0;
    while (i < x && valid)
    {
        x_cells[i] = NULL;
        x_items[i] = NULL;
        x_thoughts[i] = NULL;
        i++;
    }
    i = 0;
    while (i < x && valid)
    {
        x_cells[i] = malloc(sizeof(int) * y);
        x_items[i] = malloc(sizeof(struct item) * y);
        x_thoughts[i] = malloc(sizeof(char) * y);
        valid = (x_cells[i] != NULL && x_items[i] != NULL && x_thoughts[i] != NULL);
        i++;
    }
    struct map *new_map_list = NULL;
    if (valid)
        new_map_list = realloc(project_data.zones[zone].maps, sizeof(struct map) * (project_data.zones[zone].map_number + 1));
    valid = new_map_list != NULL;
    while (i && !valid)
    {
        i--;
        free(x_cells[i]);
        free(x_items[i]);
        free(x_thoughts[i]);
    }
    if (!valid)
    {
        free(x_cells);
        free(x_items);
        free(x_thoughts);
        print_error("Pas assez de memoire !");
        return 0;
    }
    i = 0;
    while (i < x)
    {
        int j = 0;
        while (j < y)
        {
            x_cells[i][j] = 0;
            x_items[i][j].activation = 0;
            x_items[i][j].ID = 0;
            x_items[i][j].type = 0;
            x_items[i][j].value = 0;
            x_thoughts[i][j] = 0;
            j++;
        }
        i++;
    }
    new_map_list[project_data.zones[zone].map_number].thoughts = x_thoughts;
    new_map_list[project_data.zones[zone].map_number].cells = x_cells;
    new_map_list[project_data.zones[zone].map_number].items = x_items;
    new_map_list[project_data.zones[zone].map_number].x = x;
    new_map_list[project_data.zones[zone].map_number].y = y;
    new_map_list[project_data.zones[zone].map_number].initial_delay = 0;
    new_map_list[project_data.zones[zone].map_number].color_length = 0;
    new_map_list[project_data.zones[zone].map_number].color_sequency = NULL;
    project_data.zones[zone].map_number++;
    project_data.zones[zone].maps = new_map_list;
    print_error("Carte creee !");
    return 1;
}