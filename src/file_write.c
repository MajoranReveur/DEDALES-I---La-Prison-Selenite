#include "file_write.h"

void write_int(int value)
{
    fprintf(file, "%d ", value);
}

void write_id(long ID)
{
    fprintf(file, "%ld ", ID);
}

void write_int_list(int* list, int size)
{
    int i = 0;
    while (i < size)
    {
        write_int(list[i]);
        i++;
    }
    fputc('\n', file);
}


void write_char_list(char* list, int size)
{
    int i = 0;
    while (i < size)
    {
        write_int(list[i]);
        i++;
    }
    fputc('\n', file);
}

void write_position(struct position p)
{
    write_int(p.zone);
    write_int(p.map);
    write_int(p.x);
    write_int(p.y);
    fputc('\n', file);
}

void write_item(struct item i)
{
    write_int(i.type);
    write_int(i.value);
    write_id(i.ID);
    write_int(i.activation);
    fputc('\n', file);
}

void write_item_list(struct item *list, int size)
{
    int i = 0;
    while (i < size)
    {
        write_item(list[i]);
        i++;
    }
    fputc('\n', file);
}

void write_container(struct container c)
{
    write_int(c.size);
    write_position(c.position);
    write_item_list(c.items, c.size);
    fputc('\n', file);
}

void write_container_list(struct container *list, long size) //Size is the maximal ID here. So it is a long while other size, which are limited to 100, are int
{
    long i = 0;
    while (i < size)
    {
        write_container(list[i]);
        i++;
    }
    fputc('\n', file);
}

void write_request(struct request r)
{
    write_int(r.type);
    write_int(r.orientation);
    write_int(r.item_quantity);
    write_int(r.vision_field);
    write_int(r.wanted_character);
    write_int(r.active);
    write_position(r.place);
    write_position(r.destination);
    write_item(r.item_wanted);
    write_item(r.reward_item);
    fputc('\n', file);
}

void write_request_list(struct request *list, int size)
{
    int i = 0;
    while (i < size)
    {
        write_request(list[i]);
        i++;
    }
    fputc('\n', file);
}

void write_map(struct map m)
{
    write_int(m.x);
    write_int(m.y);
    write_int(m.initial_delay);
    write_int(m.color_length);
    if (m.color_length)
        write_int_list(m.color_sequency, m.color_length);
    int i = 0;
    while (i < m.x)
    {
        write_int_list(m.cells[i], m.y);
        write_item_list(m.items[i], m.y);
        write_char_list(m.thoughts[i], m.y);
        i++;
    }
    fputc('\n', file);
}

void write_map_list(struct map *list, int size)
{
    write_int(size);
    int i = 0;
    while (i < size)
    {
        write_map(list[i]);
        i++;
    }
    fputc('\n', file);
}

void write_project(struct project p)
{
    write_int_list(p.parameters, 12);
    int i = 0;
    while (i < p.parameters[11])
    {
        write_map_list(p.zones[i].maps, p.zones[i].map_number);
        i++;
    }
    i = 0;
    while (i < 5)
    {
        write_position(p.character_positions[i]);
        write_request_list(p.requests[i], p.parameters[i + 5]);
        write_item_list(p.inventories[i], 50);
        i++;
    }
    write_container_list(p.containers, p.parameters[10]);
}

char save_project(struct project p)
{
    char* file_fields[5] = {
        "levels/projects/",
        p.project_name,
        "[",
        p.author_name,
        "].txt"
    };
    char filename[451] = { 0 };
    concat_str(filename, file_fields, 450, 5);
    file = fopen(filename, "w");
    if (!file)
    {
        print_error("Echec...");
        print_error("    Sources possibles d'erreur :    ");
        print_error("      - Nom du projet invalide      ");
        print_error("         - Pseudo invalide          ");
        print_error("- Dossier levels/projects inexistant");
        return 0;
    }
    write_project(p);
    file_saved = 1;
    print_error("Projet sauvegarde !");
    fclose(file);
    file = NULL;
    return 1;
}