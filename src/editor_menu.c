#include "editor_menu.h"

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
            print_text_centered(0, 200, "Cartes", 1, 1 + (i == 0), 1104);
            print_text_centered(0, 250, "Personnages", 1, 1 + (i == 1), 1104);
            print_text_centered(0, 300, "Configurations", 1, 1 + (i == 2), 1104);
            print_text_centered(0, 350, "Analyse", 1, 1 + (i == 3), 1104);
            print_text_centered(0, 400, "Sauvegarde", 1, 1 + (i == 4), 1104);
            print_text_centered(0, 450, "Exportation", 1, 1 + (i == 5), 1104);
            print_text_centered(0, 500, "Retour au menu principal", 1, 1 + (i == 6), 1104);
            print_refresh();
            load_input_long();
            if (inputs[2])
                i = (i + 6) % 7;
            if (inputs[3])
                i = (i + 1) % 7;
        }
        if (!inputs[9])
        {
            if (i == 4)
            {
                if (save_project(project_data))
                    project_data.modified = 0;
            }
            if (i == 6)
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