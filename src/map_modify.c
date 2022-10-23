#include "map_modify.h"

int x_camera = 0;
int y_camera = 0;
int x_cursor = 0;
int y_cursor = 0;

void update_camera()
{
    if (x_camera > x_cursor + 10)
        x_camera = x_cursor + 10;
    if (x_camera < x_cursor - 10)
        x_camera = x_cursor - 10;
    if (y_camera > y_cursor + 10)
        y_camera = y_cursor + 10;
    if (y_camera < y_cursor - 10)
        y_camera = y_cursor - 10;
}

void modify_cell_map(int zone, int map)
{
    int type = 1;
    inputs[0] = 0;
    inputs[1] = 0;
    inputs[6] = 0;
    while (!inputs[9] && !inputs[1])
    {
        display_map_full(x_camera, y_camera, project_data.maps[zone][map]);
        rect(704, 0, 400, 704, 0, 0, 0);
        print_text_centered(704, 10, zones_texts[zone], 0, 1, 400);
        print_int_centered(704, 50, map + 1, 3, 1, 1, 400);
        print_text_centered(704, 250, "Backspace : Changer de Type", 1, 1, 400);
        print_text_centered(704, 300, "Espace : Affecter le Type", 1, 1, 400);
        print_text_centered(704, 350, "Entree : Retour", 1, 1, 400);
        print_text_centered(704, 450, "Type actuel :", 1, 1, 400);
        cursor((x_cursor - x_camera) * 64, (y_cursor - y_camera) * 64, 64, 64, 255, 0, 0);
        display_sprite(0, 874, 500, 64, type, 0);
        print_refresh();
        load_input_long();
        if (inputs[0])
        {
            if (x_cursor >= 0 && x_cursor < project_data.maps[zone][map].x && y_cursor >= 0 && y_cursor < project_data.maps[zone][map].y)
                project_data.maps[zone][map].cells[x_cursor][y_cursor] = type;
        }
        if (inputs[2] && y_cursor > 0)
            y_cursor--;
        if (inputs[3] && y_cursor < project_data.maps[zone][map].y - 1)
            y_cursor++;
        if (inputs[4] && x_cursor > 0)
            x_cursor--;
        if (inputs[5] && x_cursor < project_data.maps[zone][map].x - 1)
            x_cursor++;
        update_camera();
    }
}

void modify_map(int zone, int map)
{
    char quit = 0;
    if (x_cursor >= project_data.maps[zone][map].x)
        x_cursor = project_data.maps[zone][map].x - 1;
    if (y_cursor >= project_data.maps[zone][map].y)
        y_cursor = project_data.maps[zone][map].y - 1;
    update_camera();
    int i = 0;
    while (!inputs[9] && !quit)
    {
        inputs[0] = 0;
        while (!inputs[9] && !inputs[0])
        {
            display_map_full(x_camera, y_camera, project_data.maps[zone][map]);
            rect(704, 0, 400, 704, 0, 0, 0);
            print_text_centered(704, 10, zones_texts[zone], 0, 1, 400);
            print_int_centered(704, 50, map + 1, 3, 1, 1, 400);
            print_text_centered(704, 250, "Cases", 1, 1 + (i == 0), 400);
            print_text_centered(704, 300, "Objets", 1, 1 + (i == 1), 400);
            print_text_centered(704, 350, "Redimensionner", 1, 1 + (i == 2), 400);
            print_text_centered(704, 400, "Supprimer la Carte", 1, 1 + (i == 3), 400);
            print_text_centered(704, 450, "Deplacer le Curseur", 1, 1 + (i == 4), 400);
            print_text_centered(704, 500, "Retour", 1, 1 + (i == 5), 400);
            print_refresh();
            load_input_long();
            if (inputs[2])
                i = (i + 5) % 6;
            if (inputs[3])
                i = (i + 1) % 6;
        }
        if (!inputs[9])
        {
            if (i == 5)
                quit = 1;
            if (i == 0)
                modify_cell_map(zone, map);
        }
    }
}

char create_map(int zone)
{
    if (project_data.parameters[5 + zone] >= 99)
    {
        print_error("99 Cartes ont deja ete creees !");
        return 0;
    }
    rect(0, 0, 1104, 704, 0, 0, 0);
    print_text_centered(0, 10, project_data.project_name, 0, 1, 1104);
    print_text_centered(0, 50, project_data.author_name, 1, 1, 1104);
    print_text_centered(0, 100, "- Nouvelle Carte -", 1, 1, 1104);
    print_text_centered(0, 170, "Coordonnee X :", 1, 1, 1104);
    int x = int_input(0, 530, 200);
    // print_int_centered(0, 200, x, 3, 1, 1, 1104);
    print_text_centered(0, 230, "Coordonnee Y :", 1, 1, 1104);
    int y = int_input(0, 530, 260);
    // print_int_centered(0, 260, y, 3, 1, 1, 1104);
    if (!x || !y)
    {
        print_error("Une carte ne peut pas avoir 0 cases !");
        return 0;
    }
    int **x_cells = malloc(sizeof(int *) * x);
    struct item **x_items = NULL;
    if (x_cells != NULL)
        x_items = malloc(sizeof(struct item *) * x);
    char valid = x_items != NULL;
    int i = 0;
    while (i < x)
    {
        x_cells[i] = NULL;
        x_items[i] = NULL;
        i++;
    }
    i = 0;
    while (i < x && valid)
    {
        x_cells[i] = malloc(sizeof(int) * y);
        x_items[i] = malloc(sizeof(struct item) * y);
        valid = (x_cells[i] != NULL && x_items[i] != NULL);
        i++;
    }
    struct map *new_map_list = NULL;
    if (valid)
        new_map_list = realloc(project_data.maps[zone], sizeof(struct map) * (project_data.parameters[zone + 5] + 1));
    valid = new_map_list != NULL;
    while (i && !valid)
    {
        i--;
        free(x_cells[i]);
        free(x_items[i]);
    }
    if (!valid)
    {
        free(x_cells);
        free(x_items);
        print_error("Pas assez de memoire !");
        return 0;
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
            j++;
        }
        i++;
    }
    new_map_list[project_data.parameters[5 + zone]].cells = x_cells;
    new_map_list[project_data.parameters[5 + zone]].items = x_items;
    new_map_list[project_data.parameters[5 + zone]].x = x;
    new_map_list[project_data.parameters[5 + zone]].y = y;
    project_data.parameters[5 + zone]++;
    project_data.maps[zone] = new_map_list;
    print_error("Carte creee !");
    return 1;
}