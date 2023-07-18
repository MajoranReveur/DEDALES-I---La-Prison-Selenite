#include "game_menu.h"

int x_inventory = 0;
int y_inventory = 0;

void inventory()
{
    int player = get_player();
    clean_inputs();
    while (!inputs[0] && !inputs[14])
    {
        rect(56, 56, 592, 592, 255, 255, 255);
        rect(60, 60, 584, 584, 0, 0, 0);
        int i = 0;
        while (i < 8)
        {
            int j = 0;
            while (j < 5)
            {
                rect(68 + 72 * i, 68 + 72 * j, 64, 64, 50, 50, 50);
                if (project_data.inventories[player][i + j * 8].type)
                    display_sprite(3, 68 + 72 * i, 68 + 72 * j, 64, project_data.inventories[player][i + j * 8].type - 1, 0);
                j++;
            }
            i++;
        }
        rect(60, 428, 584, 4, 255, 255, 255);
        rect(64 + 72 * x_inventory, 64 + 72 * y_inventory, 72, 4, 0, 0, 255);
        rect(64 + 72 * x_inventory, 64 + 72 * y_inventory, 4, 72, 0, 0, 255);
        rect(132 + 72 * x_inventory, 64 + 72 * y_inventory, 4, 72, 0, 0, 255);
        rect(64 + 72 * x_inventory, 132 + 72 * y_inventory, 72, 4, 0, 0, 255);
        
        print_text_centered(112, 436, items_texts[project_data.inventories[player][x_inventory + y_inventory * 8].type], 1, 1, 480);
        print_refresh();
        load_input_long();
        if (inputs[3] && x_inventory)
            x_inventory--;
        if (inputs[4] && x_inventory < 7)
            x_inventory++;
        if (inputs[1] && y_inventory)
            y_inventory--;
        if (inputs[2] && y_inventory < 4)
            y_inventory++;
    }
    clean_inputs();
}

void main_menu_game()
{
    while(!inputs[0])
    {
        int i = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        print_text_centered(0, 10, "DEDALES I", 0, 1, 704);
        print_text_centered(0, 50, "La Prison Selenite", 1, 1, 704);
        print_text_centered(0, 100, "- EDITEUR -", 1, 1, 704);
        clean_inputs();
        while (!inputs[0] && !inputs[5])
        {
            print_text_centered(0, 300, "Continuer", 1, 1 + (i == 0), 704);
            print_text_centered(0, 350, "Nouvelle Partie", 1, 1 + (i == 1), 704);
            print_text_centered(0, 400, "Quitter le logiciel", 1, 1 + (i == 2), 704);
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
            if (i == 1)
                game_load();
            if (i == 2)
                inputs[0] = 1;
        }
    }
}