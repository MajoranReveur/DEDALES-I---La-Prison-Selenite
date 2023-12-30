#include "item_editor.h"

char is_requestable(int type)
{
    return (type == 0 || (type >= 6 && type != 22));
}

int item_choice(int type, char mode)
{
    inputs[5] = 0;
    inputs[6] = 0;
    inputs[10] = 0;
    int i = type % 6;
    int j = type / 6;
    while (!inputs[0] && !inputs[5] && !inputs[6])
    {
        int y = 0;
        rect(104, 92, 496, 436, 0, 0, 255);
        rect(108, 96, 488, 428, 0, 0, 0);
        while (y < 5)
        {
            int x = 0;
            while (x < 6)
            {
                if (x + y * 6 < 29)
                    rect(x * 80 + 112 + 8, y * 80 + 100 + 8, 64, 64, 100, 100, 100);
                display_sprite(3, x * 80 + 112 + 8, y * 80 + 100 + 8, 64, x + y * 6 - 1, 0);
                if (!is_requestable(x + y * 6) && mode > 0)
                    rect_alpha(x * 80 + 112 + 8, y * 80 + 100 + 8, 64, 64, 100, 100, 100, 200);
                x++;
            }
            y++;
        }
        cursor(i * 80 + 112, j * 80 + 100, 80, 80, 255, 0, 0);
        if (i + j * 6 < 29)
            print_text_centered(112, 500, items_texts[i + j * 6], 1, 1, 480);
        print_refresh();
        load_input_long();
        if (inputs[1] && j)
            j--;
        if (inputs[2] && j < 4)
            j++;
        if (inputs[3] && i)
            i--;
        if (inputs[4] && i < 5)
            i++;
        if (inputs[5] && mode > 0)
        {
            inputs[5] = is_requestable(i + j * 6);
        }
    }
    if (i + j * 6 < 29)
        return i + j * 6;
    return type;
}

void map_choice(int *value, int zone)
{
    inputs[5] = 0;
    rect(104, 92, 496, 436, 0, 0, 255);
    rect(108, 96, 488, 428, 0, 0, 0);
    print_text_centered(112, 150, zones_texts[zone], 1, 1, 480);
    print_text_centered(112, 180, "Cartes disponibles :", 1, 1, 480);
    print_int_centered(112, 205, project_data.zones[zone].map_number, 0, 1, 1, 480);
    print_refresh();
    if (project_data.zones[zone].map_number == 0)
    {
        print_text_centered(112, 300, "Choix impossible.", 1, 2, 480);
        print_refresh();
        while (!inputs[0] && !inputs[5])
            load_input_long();
        *value = 0;
        return;
    }
    int v = int_input(*value, 320, 300);
    if (v > project_data.zones[zone].map_number || v == 0)
    {
        print_text_centered(112, 350, "Valeur invalide.", 1, 2, 480);
        print_refresh();
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5])
            load_input_long();
        *value = 0;
        return;
    }
    *value = v;
}

void candy_choice(int *value)
{
    inputs[5] = 0;
    rect(104, 92, 496, 436, 0, 0, 255);
    rect(108, 96, 488, 428, 0, 0, 0);
    int max = project_data.parameters[5];
    if (max < project_data.parameters[9])
        max = project_data.parameters[9];
    print_text_centered(112, 180, "Valeur maximale possible :", 1, 1, 480);
    print_int_centered(112, 205, max, 0, 1, 1, 480);
    print_refresh();
    if (max == 0)
    {
        print_text_centered(112, 300, "Choix impossible.", 1, 2, 480);
        print_refresh();
        while (!inputs[0] && !inputs[5])
            load_input_long();
        *value = 0;
        return;
    }
    int v = int_input(*value, 320, 300);
    if (v > max || v == 0)
    {
        print_text_centered(112, 350, "Valeur invalide.", 1, 2, 480);
        print_refresh();
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5])
            load_input_long();
        *value = 0;
        return;
    }
    *value = v;
}

void place_choice(int *zone, int *map)
{
    inputs[5] = 0;
    inputs[6] = 0;
    rect(104, 92, 496, 436, 0, 0, 255);
    rect(108, 96, 488, 428, 0, 0, 0);
    int i = *zone;
    if (i == 0)
        i = 1; // Default value
    while (!inputs[0] && !inputs[5] && !inputs[6])
    {
        rect(104, 92, 496, 436, 0, 0, 255);
        rect(108, 96, 488, 428, 0, 0, 0);
        print_text_centered(112, 130, "Zone :", 1, 1, 480);
        int j = 0;
        while (j < 5)
        {
            print_text_centered(112, 200 + j * 50, zones_texts[j], 1, 1 + (i == j + 1), 480);
            j++;
        }
        print_refresh();
        load_input_long();
        if (inputs[1])
            i = (i - 1 + 4) % 5 + 1;
        if (inputs[2])
            i = (i - 1 + 1) % 5 + 1;
    }
    if (inputs[5])
    {
        int value = *map;
        map_choice(&value, i - 1);
        if (value)
        {
            *zone = i;
            *map = value;
        }
    }
}

void card_choice(int *value)
{
    inputs[5] = 0;
    inputs[6] = 0;
    inputs[10] = 0;
    int i = *value % 8;
    int j = *value / 8;
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
                display_cardsprite(x * 60 + 120 + 8, y * 100 + 130 + 8, x + y * 8);
                x++;
            }
            y++;
        }
        print_text_centered(112, 450, cards_texts[i + j * 8], 1, 1, 480);
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
    }
    *value = i + j * 8;
}

void change_type(struct item *item, struct position p, char mode)
{
    int old_type = item->type;
    item->type = item_choice(item->type, mode);
    char was_container = old_type >= 7 && old_type <= 13;
    char is_container = item->type >= 7 && item->type <= 13;
    if (old_type != item->type)
    {
        if (was_container && is_container)
        {
            struct item *new_list = NULL;
            if (item->type == 8)
                new_list = realloc(project_data.containers[item->ID].items, sizeof(struct item) * 23);
            if (new_list == NULL && item->type == 8)
            {
                item->type = old_type;
                print_error("Erreur : Memoire insuffisante.");
            }
            else
            {
                int j = 0;
                if (item->type == 8)
                {
                    project_data.containers[item->ID].items = new_list;
                    project_data.containers[item->ID].size = 23;
                }
                while (j < project_data.containers[item->ID].size)
                {
                    project_data.containers[item->ID].items[j].type = 0;
                    project_data.containers[item->ID].items[j].value = 0;
                    project_data.containers[item->ID].items[j].ID = 0;
                    project_data.containers[item->ID].items[j].activation = 0;
                    j++;
                }
            }
        }
        if (mode < 2)
        {
            if (was_container && !is_container)
            {
                delete_container(item->ID);
                item->ID = 0;
            }
            if (!was_container && is_container)
            {
                struct item *list_for_cards = NULL;
                if (item->type == 8)
                    list_for_cards = malloc(sizeof(struct item) * 23);
                if (list_for_cards == NULL && item->type == 8)
                {
                    item->type = old_type;
                    print_error("Erreur : Memoire insuffisante.");
                }
                else
                {
                    long new_ID = project_data.parameters[10];
                    struct container *new_list = realloc(project_data.containers, sizeof(struct container) * (new_ID + 1));
                    if (new_list == NULL)
                    {
                        item->type = old_type;
                        print_error("Erreur : Memoire insuffisante.");
                        print_error_int(new_ID);
                    }
                    else
                    {
                        project_data.containers = new_list;
                        project_data.parameters[10]++;
                        item->ID = new_ID;
                        item->value = 0;
                        project_data.containers[new_ID].items = NULL;
                        project_data.containers[new_ID].size = 0;
                        project_data.containers[new_ID].position = p;
                        if (item->type == 8)
                        {
                            project_data.containers[new_ID].items = list_for_cards;
                            project_data.containers[new_ID].size = 23;
                            int j = 0;
                            while (j < project_data.containers[item->ID].size)
                            {
                                project_data.containers[item->ID].items[j].type = 0;
                                project_data.containers[item->ID].items[j].value = 0;
                                project_data.containers[item->ID].items[j].ID = 0;
                                project_data.containers[item->ID].items[j].activation = 0;
                                j++;
                            }
                        }
                    }
                }
            }
            if (!is_container)
            {
                item->value = 0;
                item->ID = 0;
            }
        }
        if (mode == 2)
            item->ID = 0;
        item->activation = 0;
        item->value = 0;
    }
}

void edit_content_card(long ID)
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
                    rect(x * 60 + 120 + 8, y * 100 + 130 + 8, 32, 64, 100, 100, 100);
                    if (project_data.containers[ID].items[x + y * 8 - 1].type == 6)
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
                if (project_data.containers[ID].items[i + j * 8 - 1].type == 15)
                {
                    project_data.containers[ID].items[i + j * 8 - 1].type = 0;
                    project_data.containers[ID].items[i + j * 8 - 1].value = 0;
                }
                else
                {
                    project_data.containers[ID].items[i + j * 8 - 1].type = 15;
                    project_data.containers[ID].items[i + j * 8 - 1].value = i + j * 8;
                }
                inputs[5] = 0;
            }
        }
    }
}

void edit_content_battery(long ID)
{
    inputs[5] = 0;
    rect(104, 92, 496, 436, 0, 0, 255);
    rect(108, 96, 488, 428, 0, 0, 0);
    print_text_centered(112, 180, "Capacite :", 1, 1, 480);
    print_int_centered(112, 205, project_data.containers[ID].size, 0, 1, 1, 480);
    print_refresh();
    if (project_data.containers[ID].size == 0)
    {
        print_text_centered(112, 300, "Choix impossible.", 1, 2, 480);
        print_refresh();
        while (!inputs[0] && !inputs[5])
            load_input_long();
        return;
    }
    int count = 0;
    int i = 0;
    while (i < project_data.containers[ID].size)
    {
        if (project_data.containers[ID].items[i].type == 21)
            count++;
        i++;
    }
    count = int_input(count, 320, 300);
    if (count > project_data.containers[ID].size)
    {
        print_text_centered(112, 350, "Valeur invalide.", 1, 2, 480);
        print_refresh();
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5])
            load_input_long();
        return;
    }
    i = 0;
    while (i < project_data.containers[ID].size)
    {
        if (i < count)
            project_data.containers[ID].items[i].type = 21;
        else
            project_data.containers[ID].items[i].type = 0;
        i++;
    }
}

void edit_content_level(long ID, int type)
{
    int item_type = type + 7 - (type == 11) + (type == 10) * 2;
    inputs[5] = 0;
    inputs[6] = 0;
    inputs[10] = 0;
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
    char changes = 1;
    // project_data.containers[ID].items[size - 1].type = item_type;
    // project_data.containers[ID].items[size - 1].value = 99;
    int_to_str(string_capacity, size, 1);
    printf("test");
    i = 0;
    while (!inputs[0] && !inputs[5] && !inputs[6])
    {
        int j = 0;
        if (changes)
        {
            count = 0;
            category_count = 0;
            int old_value = 0;
            while (j < size)
            {
                if (project_data.containers[ID].items[j].value)
                {
                    count++;
                    if (project_data.containers[ID].items[j].value != old_value)
                    {
                        category_count++;
                        old_value = project_data.containers[ID].items[j].value;
                    }
                }
                j++;
            }
            int_to_str(string_count, count, 1);
            const char *fields[3] = {string_count, "/", string_capacity};
            concat_str(string_total_capacity, fields, 8, 3);
            changes = 0;
            j = 0;
        }
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
        while (y < 11 && y + camera < category_count + 2)
        {
            if (y + camera < category_count)
            {
                int value = project_data.containers[ID].items[j].value;
                print_int(180, y * 30 + 190, value, 1, 1, 1);
                int count_of_value = 0;
                while (j < size && project_data.containers[ID].items[j].value == value)
                {
                    j++;
                    count_of_value++;
                }
                int_to_str(count_by_category + 1, count_of_value, 1);
                print_text(400, y * 30 + 190, count_by_category, 1, 1 + (i == y + camera));
            }
            if (y + camera == category_count)
                print_text(180, y * 30 + 190, "Ajouter", 1, 1 + (i == y + camera));
            if (y + camera == category_count + 1)
                print_text(180, y * 30 + 190, "Retour", 1, 1 + (i == y + camera));
            y++;
        }
        print_refresh();
        load_input_long();
        if (inputs[1])
            i = (i + category_count + 1) % (category_count + 2);
        if (inputs[2])
            i = (i + 1) % (category_count + 2);
        if (((inputs[3] && count) || (inputs[4] && count < size)) && i < category_count)
        {
            j = 0;
            int k = 0;
            int value = project_data.containers[ID].items[0].value;
            while (k < i)
            {
                while (project_data.containers[ID].items[j].value == value)
                {
                    j++;
                }
                value = project_data.containers[ID].items[j].value;
                k++;
            }
            if (inputs[4])
            {
                struct item o; o.type = item_type; o.value = value; o.activation = 0; o.ID = 0;
                add_item(project_data.containers[ID], o);
            }
            if (inputs[3])
                remove_item(project_data.containers[ID], value);
            changes = 1;
        }
        if (inputs[5])
        {
            if (i == category_count && count < size)
            {
                int value = 0;
                if (type == 7)
                    candy_choice(&value);
                if (type == 9)
                    map_choice(&value, 1);
                if (type == 10)
                    map_choice(&value, 2);
                if (type == 11)
                    map_choice(&value, 3);
                if (value)
                {
                    struct item o; o.type = item_type; o.value = value; o.activation = 0; o.ID = 0;
                    add_item(project_data.containers[ID], o);
                    changes = 1;
                }
            }
            inputs[5] = (i == category_count + 1);
        }
    }
}

void change_capacity(long ID)
{
    rect(104, 92, 496, 436, 0, 0, 255);
    rect(108, 96, 488, 428, 0, 0, 0);
    print_text_centered(112, 180, "Nouvelle capacite :", 1, 1, 480);
    int value = int_input(project_data.containers[ID].size, 330, 300);
    inputs[5] = 0;
    if (value == 0)
    {
        free(project_data.containers[ID].items);
        project_data.containers[ID].items = NULL;
        project_data.containers[ID].size = 0;
    }
    else
    {
        struct item *new_item_list = realloc(project_data.containers[ID].items, sizeof(struct item) * value);
        if (new_item_list != NULL)
        {
            int fill_null = project_data.containers[ID].size;
            while (fill_null < value)
            {
                new_item_list[fill_null].activation = 0;
                new_item_list[fill_null].type = 0;
                new_item_list[fill_null].value = 0;
                new_item_list[fill_null].ID = 0;
                fill_null++;
            }
            project_data.containers[ID].items = new_item_list;
            project_data.containers[ID].size = value;
        }
        else
        {
            print_text_centered(112, 400, "Erreur de memoire.", 1, 2, 480);
            print_refresh();
            while (!inputs[0] && !inputs[5])
                load_input_long();
        }
    }
}

void modify_item(struct item *item, struct position p, char mode) // Mode 0 : creation in map, Mode 1 : creation in inventory, Mode 2 : Merchandise in a request, so no creation
{
    clean_inputs();
    char end = 0;
    int i = 0;
    while (!inputs[0] && !inputs[6] && !inputs[10] && !end)
    {
        rect(104, 92, 496, 436, 0, 0, 255);
        rect(108, 96, 488, 428, 0, 0, 0);
        rect(316, 100, 72, 72, 255, 255, 255);
        rect(320, 104, 64, 64, 100, 100, 100);
        char options[10] = {0}; // 10 is way larger than needed, the maximum is theorically 6.
        options[0] = 1;
        display_sprite(3, 320, 104, 64, item->type - 1, 0);
        print_text_centered(112, 180, "Type :", 1, 1 + (i == 0), 480);
        print_text_centered(112, 205, items_texts[item->type], 1, 6, 480);
        int j = 1; // Change type
        if (item->type == 5 || (item->type >= 15 && item->type <= 20) || item->type == 22)
        {
            print_text_centered(112, 180 + j * 70, "Valeur :", 1, 1 + (i == j), 480);
            if (item->value)
            {
                if (item->type == 15)
                    print_text_centered(112, 205 + j * 70, cards_texts[item->value], 1, 6, 480);
                else
                    print_int_centered(112, 205 + j * 70, item->value, 0, 1, 6, 480);
            }
            else
                print_text_centered(112, 205 + j * 70, "Non defini", 1, 6, 480);
            options[j] = 2 + (item->type == 15); // 2 : Choice of map, 3 : Choice of card (value)
            j++;
        }
        if (item->type == 6)
        {
            print_text_centered(112, 180 + j * 70, "Lieu :", 1, 1 + (i == j), 480);
            if (item->value && item->ID)
            {
                print_text_centered(112, 200 + j * 70, zones_texts[item->value - 1], 1, 6, 480);
                print_int_centered(112, 220 + j * 70, item->ID, 0, 1, 6, 480);
            }
            else
            {
                print_text_centered(112, 205 + j * 70, "Non defini", 1, 6, 480);
            }
            options[j] = 4; // Choice of location (zone + map)
            j++;
        }
        if (item->type == 14 && mode > 0)
        {
            print_text_centered(112, 180 + j * 70, "Valeur :", 1, 1 + (i == j), 480);
            if (item->value)
                print_int_centered(112, 205 + j * 70, item->value, 0, 1, 6, 480);
            else
                print_text_centered(112, 205 + j * 70, "Non defini", 1, 6, 480);
            options[j] = 2; // Choice of map
            j++;
        }
        if (item->type == 13 && mode < 2)
        {
            print_text_centered(112, 180 + j * 70, "Puissance :", 1, 1 + (i == j), 480);
            print_int_centered(112, 205 + j * 70, item->value, 0, 1, 6, 480);
            options[j] = 5; // Numerical value
            j++;
        }
        if (item->type >= 7 && item->type <= 13 && item->type != 8 && mode < 2)
        {
            print_text_centered(112, 180 + j * 70, "Capacite :", 1, 1 + (i == j), 480);
            print_int_centered(112, 205 + j * 70, project_data.containers[item->ID].size, 0, 1, 6, 480);
            options[j] = 6; // Numerical value
            j++;
        }
        if (item->type >= 7 && item->type <= 13 && mode == 2)
        {
            print_text_centered(112, 180 + j * 70, "Capacite :", 1, 1 + (i == j), 480);
            if (item->ID == 0)
                print_text_centered(112, 205 + j * 70, "Vide", 1, 6, 480);
            if (item->ID == 1)
                print_text_centered(112, 205 + j * 70, "Plein", 1, 6, 480);
            if (item->ID > 1)
                print_text_centered(112, 205 + j * 70, "Peu importe", 1, 6, 480);
            options[j] = 6; // Numerical value
            j++;
        }
        if (item->type >= 16 && item->type <= 20 && mode == 0)
        {
            print_text_centered(112, 180 + j * 70, "Sceau :", 1, 1 + (i == j), 480);
            print_text_centered(112, 205 + j * 70, cards_texts[item->ID], 1, 6, 480);
            options[j] = 7; // Choice of card
            j++;
        }
        if (item->type == 3 || item->type == 5 || item->type == 7 || item->type == 12 || item->type == 13 || item->type == 16)
        {
            print_text_centered(112, 180 + j * 70, "Active ?", 1, 1 + (i == j), 480);
            if (item->activation)
                print_text_centered(112, 205 + j * 70, "Oui", 1, 6, 480);
            else
                print_text_centered(112, 205 + j * 70, "Non", 1, 6, 480);
            options[j] = 8; // Switch the boolean
            j++;
        }
        if (item->type >= 7 && item->type <= 13 && mode < 2)
        {
            print_text_centered(112, 180 + j * 70, "Modifier le contenu", 1, 1 + (i == j), 480);
            options[j] = 9;
            print_text_centered(112, 220 + j * 70, "Retour", 1, 1 + (i == j + 1), 480);
            j++;
        }
        else
            print_text_centered(112, 180 + j * 70, "Retour", 1, 1 + (i == j), 480);
        j++;
        print_refresh();
        load_input_long();
        if (inputs[1])
            i = (i + j - 1) % j;
        if (inputs[2])
            i = (i + 1) % j;
        if (inputs[5])
        {
            int value = item->value;
            if (options[i] == 1)
                change_type(item, p, mode);
            if (options[i] == 2)
            {
                if (item->type == 14)
                    map_choice(&value, 0);
                if (item->type == 16 || item->type == 22)
                    map_choice(&value, 1);
                if (item->type == 5 || item->type == 17)
                    map_choice(&value, 2);
                if (item->type == 18 || item->type == 19)
                    map_choice(&value, 3);
            }
            if (options[i] == 3)
                card_choice(&value);
            if (options[i] == 4)
            {
                int map = item->ID;
                place_choice(&value, &map);
                item->ID = map;
            }
            if (options[i] == 6)
            {
                if (mode == 2)
                    item->ID = (item->ID + 1) % 3;
                else
                    change_capacity(item->ID);
            }
            if (options[i] == 5)
            {
                rect(104, 92, 496, 436, 0, 0, 255);
                rect(108, 96, 488, 428, 0, 0, 0);
                print_text_centered(112, 180, "Puissance entre 0 et 13 :", 1, 1, 480);
                value = int_input(value, 330, 300);
                inputs[5] = 0;
                if (value > 13)
                {
                    print_text_centered(112, 400, "Valeur non valide.", 1, 2, 480);
                    print_refresh();
                    while (!inputs[0] && !inputs[5])
                        load_input_long();
                    value = item->value;
                }
            }
            if (options[i] == 7)
            {
                int v = item->ID;
                card_choice(&v);
                item->ID = v;
            }
            if (options[i] == 8)
                item->activation = !(item->activation);
            if (options[i] == 9)
            {
                if (project_data.containers[item->ID].size == 0)
                    print_error("Capacite nulle !");
                else
                {
                if (item->type == 8)
                    edit_content_card(item->ID);
                if (item->type == 12 || item->type == 13)
                    edit_content_battery(item->ID);
                if (item->type == 7 || (item->type >= 9 && item->type <= 11))
                    edit_content_level(item->ID, item->type);
                }
            }
            if (i == j - 1)
                end = 1;
            item->value = value;
        }
    }
    clean_inputs();
}