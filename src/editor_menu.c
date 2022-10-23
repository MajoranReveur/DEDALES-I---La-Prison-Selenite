#include "editor_menu.h"

void map_choice_menu(int zone)
{
    char quit = 0;
    while(!inputs[9] && !quit)
    {
        int map_numbers = project_data.parameters[5 + zone];
        int i = 0;
        int camera = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        inputs[0] = 0;
        while (!inputs[9] && !inputs[0])
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
                display_minimap_full(0, 0, project_data.maps[zone][i]);
            print_refresh();
            load_input_long();
            if (inputs[2])
            {
                if (i)
                    i--;
                else
                    i = map_numbers + 1;
            }
            if (inputs[3])
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
        if (!inputs[9])
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
    }
}

void zone_choice_menu()
{
    char quit = 0;
    while(!inputs[9] && !quit)
    {
        int i = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        print_text_centered(0, 10, project_data.project_name, 0, 1, 1104);
        print_text_centered(0, 50, project_data.author_name, 1, 1, 1104);
        print_text_centered(0, 100, "- Cartes -", 1, 1, 1104);
        inputs[0] = 0;
        while (!inputs[9] && !inputs[0])
        {
            print_text_centered(0, 200, "Entrepot", 1, 1 + (i == 0), 1104);
            print_text_centered(0, 250, "Cles", 1, 1 + (i == 1), 1104);
            print_text_centered(0, 300, "Cauchemars", 1, 1 + (i == 2), 1104);
            print_text_centered(0, 350, "Songes", 1, 1 + (i == 3), 1104);
            print_text_centered(0, 400, "Oubliettes", 1, 1 + (i == 4), 1104);
            print_text_centered(0, 450, "Retour", 1, 1 + (i == 5), 1104);
            print_refresh();
            load_input_long();
            if (inputs[2])
                i = (i + 5) % 6;
            if (inputs[3])
                i = (i + 1) % 6;
        }
        if (!inputs[9])
        {
            if (i < 5)
                map_choice_menu(i);
            if (i == 5)
                quit = 1;
        }
    }
}

void modify_project_menu()
{
    char quit = 0;
    while(!inputs[9] && !quit)
    {
        int i = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        print_text_centered(0, 10, project_data.project_name, 0, 1, 1104);
        print_text_centered(0, 50, project_data.author_name, 1, 1, 1104);
        print_text_centered(0, 100, "- Modification de Projet -", 1, 1, 1104);
        inputs[0] = 0;
        while (!inputs[9] && !inputs[0])
        {
            print_text_centered(0, 200, "Cartes", 1, 1 + (i == 0), 1104);
            print_text_centered(0, 250, "Personnages", 1, 1 + (i == 1), 1104);
            print_text_centered(0, 300, "Parametres", 1, 1 + (i == 2), 1104);
            print_text_centered(0, 350, "Retour", 1, 1 + (i == 3), 1104);
            print_refresh();
            load_input_long();
            if (inputs[2])
                i = (i + 3) % 4;
            if (inputs[3])
                i = (i + 1) % 4;
        }
        if (!inputs[9])
        {
            if (i == 0)
                zone_choice_menu();
            if (i == 3)
                quit = 1;
        }
    }
}

void project_menu()
{
    char quit = 0;
    while(!inputs[9] && !quit)
    {
        int i = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        print_text_centered(0, 10, project_data.project_name, 0, 1, 1104);
        print_text_centered(0, 50, project_data.author_name, 1, 1, 1104);
        inputs[0] = 0;
        while (!inputs[9] && !inputs[0])
        {
            print_text_centered(0, 200, "Modifier le projet", 1, 1 + (i == 0), 1104);
            print_text_centered(0, 250, "Analyser", 1, 1 + (i == 1), 1104);
            print_text_centered(0, 300, "Sauvegarder", 1, 1 + (i == 2), 1104);
            print_text_centered(0, 350, "Exporter", 1, 1 + (i == 3), 1104);
            print_text_centered(0, 400, "Retourner au menu principal", 1, 1 + (i == 4), 1104);
            print_refresh();
            load_input_long();
            if (inputs[2])
                i = (i + 4) % 5;
            if (inputs[3])
                i = (i + 1) % 7;
        }
        if (!inputs[9])
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
    }
}

void main_menu_editor()
{
    while(!inputs[9])
    {
        int i = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        print_text_centered(0, 10, "DEDALES I", 0, 1, 1104);
        print_text_centered(0, 50, "La Prison Selenite", 1, 1, 1104);
        print_text_centered(0, 100, "- EDITEUR -", 1, 1, 1104);
        inputs[0] = 0;
        while (!inputs[9] && !inputs[0])
        {
            print_text_centered(0, 300, "Charger un projet", 1, 1 + (i == 0), 1104);
            print_text_centered(0, 350, "Creer un nouveau projet", 1, 1 + (i == 1), 1104);
            print_text_centered(0, 400, "Quitter le logiciel", 1, 1 + (i == 2), 1104);
            print_text(0, 675, "Controles : Escape", 1, 1);
            print_text(1020, 675, "v0.2.0", 1, 1);
            print_refresh();
            load_input_long();
            if (inputs[2])
                i = (i + 2) % 3;
            if (inputs[3])
                i = (i + 1) % 3;
        }
        if (!inputs[9])
        {
            if (i == 0)
                project_load();
            if (i == 1)
                project_create();
            if (i == 2)
                inputs[9] = 1;
        }
    }
}