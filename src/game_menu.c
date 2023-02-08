#include "game_menu.h"

void main_menu_game()
{
    while(!inputs[0])
    {
        int i = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        print_text_centered(0, 10, "DEDALES I", 0, 1, 1104);
        print_text_centered(0, 50, "La Prison Selenite", 1, 1, 1104);
        print_text_centered(0, 100, "- EDITEUR -", 1, 1, 1104);
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5])
        {
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
            if (i == 2)
                inputs[0] = 1;
        }
    }
}