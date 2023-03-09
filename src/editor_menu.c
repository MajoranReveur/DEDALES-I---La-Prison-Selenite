#include "editor_menu.h"

void map_choice_menu(int zone)
{
    char quit = 0;
    while(!inputs[0] && !quit)
    {
        int map_numbers = project_data.zones[zone].map_number;
        int i = 0;
        int camera = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5] && !inputs[6])
        {
            int j = 0;
            while (j < 11)
            {
                if (camera + j < map_numbers)
                    print_int_centered(704, 200 + j * 50, camera + j + 1, 2, 1, 1 + (camera + j == i), 400);
                if (camera + j == map_numbers)
                    print_text_centered(704, 200 + j * 50, "Nouvelle Carte", 1, 1 + (camera + j == i), 400);
                if (camera + j == map_numbers + 1)
                    print_text_centered(704, 200 + j * 50, "Retour", 1, 1 + (camera + j == i), 400);
                j++;
            }
            if (i < map_numbers)
                display_minimap_full(0, 0, project_data.zones[zone].maps[i]);
            else
                rect(0, 0, 704, 704, 0, 0, 0);
            print_refresh();
            load_input_long();
            if (inputs[1])
            {
                if (i)
                    i--;
                else
                    i = map_numbers + 1;
            }
            if (inputs[2])
            {
                if (i < map_numbers + 1)
                    i++;
                else
                    i = 0;
            }
            if (camera >= i)
            {
                if (i)
                    camera = i - 1;
                else
                    camera = 0;
            }
            if (camera + 11 < i)
            {
                if (i < map_numbers + 1)
                    camera = i - 10;
                else
                    camera = i - 11;
            }
        }
        if (inputs[5])
        {
            if (i < map_numbers)
                modify_map(zone, i);
            if (i == map_numbers)
            {
                if (create_map(zone))
                    modify_map(zone, map_numbers);
            }
            if (i == map_numbers + 1)
                quit = 1;
        }
        if (inputs[6])
            quit = 1;
    }
    clean_inputs();
}

void zone_choice_menu()
{
    char quit = 0;
    while(!inputs[0] && !quit)
    {
        int i = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        print_text_centered(0, 10, project_data.project_name, 0, 1, 1104);
        print_text_centered(0, 50, project_data.author_name, 1, 1, 1104);
        print_text_centered(0, 100, "- Cartes -", 1, 1, 1104);
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5] && !inputs[6])
        {
            print_text_centered(0, 200, "Entrepot", 1, 1 + (i == 0), 1104);
            print_text_centered(0, 250, "Cles", 1, 1 + (i == 1), 1104);
            print_text_centered(0, 300, "Cauchemars", 1, 1 + (i == 2), 1104);
            print_text_centered(0, 350, "Songes", 1, 1 + (i == 3), 1104);
            print_text_centered(0, 400, "Oubliettes", 1, 1 + (i == 4), 1104);
            print_text_centered(0, 450, "Retour", 1, 1 + (i == 5), 1104);
            print_refresh();
            load_input_long();
            if (inputs[1])
                i = (i + 5) % 6;
            if (inputs[2])
                i = (i + 1) % 6;
        }
        if (inputs[5])
        {
            if (i < 5)
                map_choice_menu(i);
            if (i == 5)
                quit = 1;
        }
        if (inputs[6])
            quit = 1;
    }
    clean_inputs();
}

char position_choice_map(int zone, struct position *p)
{
    char quit = 0;
    if (p->zone)
    {
        if (position_choice_cell(p->zone - 1, &(p->map), p))
        {
            clean_inputs();
            return 1;
        }
    }
    clean_inputs();
    while(!inputs[0] && !quit)
    {
        int map_numbers = project_data.zones[zone].map_number;
        int i = 0;
        int camera = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5] && !inputs[6])
        {
            int j = 0;
            while (j < 11)
            {
                if (camera + j < map_numbers)
                    print_int_centered(704, 200 + j * 50, camera + j + 1, 2, 1, 1 + (camera + j == i), 400);
                if (camera + j == map_numbers)
                    print_text_centered(704, 200 + j * 50, "Retour", 1, 1 + (camera + j == i), 400);
                j++;
            }
            if (i < map_numbers)
                display_minimap_full(0, 0, project_data.zones[zone].maps[i]);
            else
                rect(0, 0, 704, 704, 0, 0, 0);
            print_refresh();
            load_input_long();
            if (inputs[1])
            {
                if (i)
                    i--;
                else
                    i = map_numbers;
            }
            if (inputs[2])
            {
                if (i < map_numbers)
                    i++;
                else
                    i = 0;
            }
            if (camera >= i)
            {
                if (i)
                    camera = i - 1;
                else
                    camera = 0;
            }
            if (camera + 11 < i)
            {
                if (i < map_numbers)
                    camera = i - 10;
                else
                    camera = i - 11;
            }
        }
        if (inputs[5])
        {
            if (i < map_numbers)
            {
                if (position_choice_cell(zone, &i, p))
                {
                    clean_inputs();
                    return 1;
                }
            }
            if (i == map_numbers)
                quit = 1;
        }
        if (inputs[6])
            quit = 1;
    }
    clean_inputs();
    return 0;
}

char position_choice_zone(struct position *p)
{
    char quit = 0;
    if (p->zone)
    {
        if (position_choice_map(p->zone - 1, p))
        {
            clean_inputs();
            return 1;
        }
    }
    while(!inputs[0] && !quit)
    {
        int i = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        print_text_centered(0, 10, project_data.project_name, 0, 1, 1104);
        print_text_centered(0, 50, project_data.author_name, 1, 1, 1104);
        print_text_centered(0, 100, "- Cartes -", 1, 1, 1104);
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5] && !inputs[6])
        {
            print_text_centered(0, 300, "Entrepot", 1, 1 + (i == 0), 1104);
            print_text_centered(0, 350, "Oubliettes", 1, 1 + (i == 1), 1104);
            print_text_centered(0, 400, "Retour", 1, 1 + (i == 2), 1104);
            print_refresh();
            load_input_long();
            if (inputs[1])
                i = (i + 2) % 3;
            if (inputs[2])
                i = (i + 1) % 3;
        }
        if (inputs[5])
        {
            if (i < 2)
            {
                if (position_choice_map(i, p))
                {
                    clean_inputs();
                    return 1;
                }
            }
            if (i == 2)
                quit = 1;
        }
        if (inputs[6])
            quit = 1;
    }
    clean_inputs();
    return 0;
}

void inventory_editor(int character)
{
    int x = 0;
    int y = 0;
    clean_inputs();
    while (!inputs[0] && !inputs[6])
    {
        int i = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        while (i < 10)
        {
            int j = 0;
            while (j < 5)
            {
                rect( 5 + i * 70, 170 + j * 70, 64, 64, 100, 100, 100);
                if (project_data.inventories[character][j * 10 + i].type)
                    display_sprite(3, 5 + i * 70, 170 + j * 70, 64, project_data.inventories[character][j * 10 + i].type - 1, 0);
                j++;
            }
            i++;
        }
        struct item item = project_data.inventories[character][y * 10 + x];
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
            char* fields[3] = {content, "/", capacity};
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
        cursor(5 + x * 70, 170 + y * 70, 64, 64, 255, 0, 0);
        cursor(866, 492, 80, 80, 0, 0, 255);
        if (type)
        {
            rect(874, 500, 64, 64, 150, 150, 150);
            display_sprite(3, 874, 500, 64, type - 1, 0);
        }
        print_refresh();
        load_input_long();
        if (inputs[5])
        {
            if (x >= 0 && x < 10 && y >= 0 && y < 5)
            {
                struct position p = {0, character, x, y};
                modify_item(&item, p, 0);
                project_data.inventories[character][y * 10 + x] = item;
            }
        }
        if (inputs[1] && y > 0)
            y--;
        if (inputs[2] && y < 4)
            y++;
        if (inputs[3] && x > 0)
            x--;
        if (inputs[4] && x < 9)
            x++;
    }
    clean_inputs();
}

void character_editor(int character)
{
    char quit = 0;
    char* character_names[] = {"Nihil", "Sarah", "Emma", "Mathias", "Saihtam"};
    while(!inputs[0] && !quit)
    {
        int i = 0;
        clean_inputs();
        while (!inputs[0] && !inputs[5] && !inputs[6])
        {
            rect(0, 0, 1104, 704, 0, 0, 0);
            print_text_centered(704, 100, "- Personnages -", 1, 1, 400);
            print_text_centered(704, 150, character_names[character], 1, 1, 400);
            print_text_centered(704, 300, "Position de Depart", 1, 1 + (i == 0), 400);
            print_text_centered(704, 350, "Inventaire", 1, 1 + (i == 1), 400);
            print_text_centered(704, 400, "Requetes", 1, 1 + (i == 2), 400);
            print_text_centered(704, 450, "Retour", 1, 1 + (i == 3), 400);
            struct position p = project_data.character_positions[character];
            if (p.zone)
            {
                display_map_full(p.x - 5, p.y - 5, project_data.zones[p.zone - 1].maps[p.map], 1);
                display_sprite(4, 320, 320, 64, character * 4, 0);
            }
            print_refresh();
            load_input_long();
            if (inputs[1])
                i = (i + 3) % 4;
            if (inputs[2])
                i = (i + 1) % 4;
        }
        if (inputs[5])
        {
            if (i == 0)
                position_choice_zone(&(project_data.character_positions[character]));
            if (i == 1)
                inventory_editor(character);
            if (i == 3)
                quit = 1;
        }
        if (inputs[6])
            quit = 1;
    }
    clean_inputs();
}

void character_menu()
{
    char quit = 0;
    while(!inputs[0] && !quit)
    {
        int i = 0;
        clean_inputs();
        while (!inputs[0] && !inputs[5] && !inputs[6])
        {
            rect(0, 0, 1104, 704, 0, 0, 0);
            print_text_centered(704, 100, "- Personnages -", 1, 1, 400);
            print_text_centered(704, 200, "Nihil", 1, 1 + (i == 0), 400);
            print_text_centered(704, 250, "Sarah", 1, 1 + (i == 1), 400);
            print_text_centered(704, 300, "Emma", 1, 1 + (i == 2), 400);
            print_text_centered(704, 350, "Mathias", 1, 1 + (i == 3), 400);
            print_text_centered(704, 400, "Saihtam", 1, 1 + (i == 4), 400);
            print_text_centered(704, 450, "Retour", 1, 1 + (i == 5), 400);
            if (i < 5)
            {
                struct position p = project_data.character_positions[i];
                if (p.zone)
                {
                    display_map_full(p.x - 5, p.y - 5, project_data.zones[p.zone - 1].maps[p.map], 1);
                    display_sprite(4, 320, 320, 64, i * 4, 0);
                }
            }
            print_refresh();
            load_input_long();
            if (inputs[1])
                i = (i + 5) % 6;
            if (inputs[2])
                i = (i + 1) % 6;
        }
        if (inputs[5])
        {
            if (i < 5)
                character_editor(i);
            if (i == 5)
                quit = 1;
        }
        if (inputs[6])
            quit = 1;
    }
    clean_inputs();
}

void modify_project_menu()
{
    char quit = 0;
    while(!inputs[0] && !quit)
    {
        int i = 0;
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5] && !inputs[6])
        {
            rect(0, 0, 1104, 704, 0, 0, 0);
            print_text_centered(0, 10, project_data.project_name, 0, 1, 1104);
            print_text_centered(0, 50, project_data.author_name, 1, 1, 1104);
            print_text_centered(0, 100, "- Modification de Projet -", 1, 1, 1104);
            print_text_centered(0, 200, "Cartes", 1, 1 + (i == 0), 1104);
            print_text_centered(0, 250, "Personnages", 1, 1 + (i == 1), 1104);
            print_text_centered(0, 300, "Parametres", 1, 1 + (i == 2), 1104);
            print_text_centered(0, 350, "Retour", 1, 1 + (i == 3), 1104);
            print_refresh();
            load_input_long();
            if (inputs[1])
                i = (i + 3) % 4;
            if (inputs[2])
                i = (i + 1) % 4;
        }
        if (inputs[5])
        {
            if (i == 0)
                zone_choice_menu();
            if (i == 1)
                character_menu();
            if (i == 3)
                quit = 1;
        }
        if (inputs[6])
            quit = 1;
    }
    clean_inputs();
}

void project_menu()
{
    char quit = 0;
    while(!inputs[0] && !quit)
    {
        int i = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        print_text_centered(0, 10, project_data.project_name, 0, 1, 1104);
        print_text_centered(0, 50, project_data.author_name, 1, 1, 1104);
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5] && !inputs[6])
        {
            print_text_centered(0, 200, "Modifier le projet", 1, 1 + (i == 0), 1104);
            print_text_centered(0, 250, "Analyser", 1, 1 + (i == 1), 1104);
            print_text_centered(0, 300, "Sauvegarder", 1, 1 + (i == 2), 1104);
            print_text_centered(0, 350, "Exporter", 1, 1 + (i == 3), 1104);
            print_text_centered(0, 400, "Retourner au menu principal", 1, 1 + (i == 4), 1104);
            print_refresh();
            load_input_long();
            if (inputs[1])
                i = (i + 4) % 5;
            if (inputs[2])
                i = (i + 1) % 5;
        }
        if (inputs[5])
        {
            if (i == 0)
                modify_project_menu();
            if (i == 2)
            {
                if (save_project(project_data))
                    project_data.modified = 0;
            }
            if (i == 4)
                quit = 1;
        }
        if (inputs[6])
            quit = 1;
    }
    clean_inputs();
}

void main_menu_editor()
{
    while(!inputs[0])
    {
        int i = 0;
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5])
        {
            rect(0, 0, 1104, 704, 0, 0, 0);
            print_text_centered(0, 10, "DEDALES I", 0, 1, 1104);
            print_text_centered(0, 50, "La Prison Selenite", 1, 1, 1104);
            print_text_centered(0, 100, "- EDITEUR -", 1, 1, 1104);
            print_text_centered(0, 300, "Charger un projet", 1, 1 + (i == 0), 1104);
            print_text_centered(0, 350, "Creer un nouveau projet", 1, 1 + (i == 1), 1104);
            print_text_centered(0, 400, "Quitter le logiciel", 1, 1 + (i == 2), 1104);
            print_text(0, 645, "Options :", 1, 1);
            print_text(0, 675, get_key_name(8), 1, 1);
            print_text(1020, 675, "v0.2.0", 1, 1);
            print_refresh();
            load_input_long();
            if (inputs[1])
                i = (i + 2) % 3;
            if (inputs[2])
                i = (i + 1) % 3;
        }
        if (!inputs[0])
        {
            if (i == 0)
                project_load();
            if (i == 1)
                project_create();
            if (i == 2)
                inputs[0] = 1;
        }
    }
}