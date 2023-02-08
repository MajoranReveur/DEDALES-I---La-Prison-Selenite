#include "options.h"

void options_menu()
{
	int screen_size = 1104 - software_mode * 400;
    copy_current_screen();
    char quit = 0;
    clean_inputs();
    while(!inputs[0] && !quit && !inputs[9] && !inputs[6])
    {
        int i = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        print_text_centered(0, 10, "Options", 0, 1, screen_size);
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5] && !inputs[9] && !inputs[6])
        {
            print_text_centered(0, 200, "Configuration des touches", 1, 1 + (i == 0), screen_size);
            print_text_centered(0, 250, "Volume", 1, 1 + (i == 1), screen_size);
            print_text_centered(0, 300, "Langue", 1, 1 + (i == 2), screen_size);
            print_text_centered(0, 350, "Retour", 1, 1 + (i == 3), screen_size);
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
                input_editor();
            if (i == 3)
                quit = 1;
        }
    }
    clean_inputs();
    paste_last_screen();
}