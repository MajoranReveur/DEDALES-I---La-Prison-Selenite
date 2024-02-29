#include "game_menu.h"

int x_inventory = 0;
int y_inventory = 0;

void card_menu(long ID)
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
        rect(120 + 8, 130 + 8, 32, 64, 100, 100, 100);
        rect(i * 60 + 120, j * 100 + 130, 48, 80, 0, 0, 255);
        while (y < 3)
        {
            int x = 0;
            while (x < 8)
            {
                if (x || y)
                {
                    rect(x * 60 + 120 + 8, y * 100 + 130 + 8, 32, 64, 0, 0, 0);
                    if (project_data.containers[ID].items[x + y * 8 - 1].type == 15)
                        display_cardsprite(x * 60 + 120 + 8, y * 100 + 130 + 8, x + y * 8);
                }
                x++;
            }
            y++;
        }
        if (i || j)
        {
            if (project_data.containers[ID].items[i + j * 8 - 1].value)
                print_text_centered(112, 450, cards_texts[i + j * 8], 1, 1, 480);
            else
                print_text_centered(112, 450, "Rien", 1, 1, 480);
        }
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
    }
    clean_inputs();
}

void container_basic_menu(long ID, int type)
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
        while (y < 11 && y + camera < category_count + 1)
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
            inputs[5] = (i == category_count);
        }
    }
    clean_inputs();
}

void inventory()
{
    int player = get_player();
    clean_inputs();
    while (!inputs[0] && !inputs[14] && !inputs[6])
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
        if (inputs[5])
        {
            struct item objet = project_data.inventories[player][x_inventory + y_inventory * 8];
            if (objet.type)
            {
                if (objet.type == 9 ||objet.type == 7)
                    container_basic_menu(objet.ID, objet.type);
                if (objet.type == 8)
                    card_menu(objet.ID);
            }
        }
    }
    clean_inputs();
}

void pause_menu()
{
    int player = get_player();
    int i = 0;
    clean_inputs();
    while (!inputs[0] && !inputs[6])
    {
        clean_inputs();
        display_current_screen(project_data.character_positions[player]);
        rect_alpha(0, 0, 704, 704, 0, 0, 0, 150);
        while (!inputs[0] && !inputs[6] && !inputs[5])
        {
            rect(56, 56, 592, 592, 255, 255, 255);
            rect(60, 60, 584, 584, 0, 0, 0);
            print_text_centered(60, 70, "MENU", 0, 1, 584);
            print_text_centered(60, 150, "Inventaire", 1, 1 + (i == 0), 584);
            print_text_centered(60, 200, "Carte", 1, 1 + (i == 1), 584);
            print_text_centered(60, 250, "Quitter le Portail", 1, 1 + (i == 2), 584);
            print_text_centered(60, 300, "Sauvegarder", 1, 1 + (i == 3), 584);
            print_text_centered(60, 350, "Charger une Sauvegarde", 1, 1 + (i == 4), 584);
            print_text_centered(60, 400, "Reprendre", 1, 1 + (i == 5), 584);
            print_text_centered(60, 450, "Retour au Menu Principal", 1, 1 + (i == 6), 584);
            print_refresh();
            load_input_long();
            if (inputs[1])
                i = (i + 6) % 7;
            if (inputs[2])
                i = (i + 1) % 7;
        }
        if (inputs[5])
        {
            if (i == 0)
                inventory();
            if (i == 1)
                map_menu(project_data.character_positions[player].zone);
            if (i == 2)
            {
                int player = get_player();		
                struct position p_player = project_data.character_positions[player];
                quit_portal(p_player.zone, p_player.map, player);
                inputs[6] = 1;
            }
            if (i == 3)
                save_choice_for_save();
            if (i == 4)
                save_choice_for_load();
            if (i >= 5)
                inputs[6] = 1;
            if (i == 6)
                end_game();
        }
    }
    clean_inputs();
}

void main_menu_game()
{
    while(!inputs[0])
    {
        project_data.author_name = "";
        project_data.project_name = "";
        int i = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        print_text_centered(0, 10, "DEDALES I", 0, 1, 704);
        print_text_centered(0, 50, "La Prison Selenite", 1, 1, 704);
        print_text_centered(0, 100, "- EDITEUR -", 1, 1, 704);
        clean_inputs();
        while (!inputs[0] && !inputs[5])
        {
            print_text_centered(0, 300, "Continuer", 1, 1 + (i == 0), 704);
            print_text_centered(0, 350, "Charger une sauvegarde", 1, 1 + (i == 1), 704);
            print_text_centered(0, 400, "Nouvelle Partie", 1, 1 + (i == 2), 704);
            print_text_centered(0, 450, "Quitter le logiciel", 1, 1 + (i == 3), 704);
            print_text(0, 645, "Options :", 1, 1);
            print_text(0, 675, get_key_name(8), 1, 1);
            print_text(1020, 675, "v0.2.0", 1, 1);
            print_refresh();
            load_input_long();
            if (inputs[1])
                i = (i + 3) % 4;
            if (inputs[2])
                i = (i + 1) % 4;
        }
        if (!inputs[0])
        {
            if (i == 0)
                open_latest_game();
            if (i == 1)
                save_choice_for_load_with_game();
            if (i == 2)
                game_load();
            if (i == 3)
                inputs[0] = 1;
        }
    }
}