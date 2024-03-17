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
    write_int(p.orientation);
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

void write_cell(struct cell c)
{
    write_int(c.type);
    write_int(c.thought);
    write_item(c.item);
    write_int(c.sprite);
    write_int(c.first_sprite);
}

void write_cell_list(struct cell *list, int size)
{
    int i = 0;
    while (i < size)
    {
        write_cell(list[i]);
        i++;
    }
    fputc('\n', file);
}

void write_cinematic_states_list(struct cinematic *list, int size)
{
    int i = 0;
    while (i < size)
    {
        write_int(list[i].trigger.active);
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

void write_mission(struct mission m)
{
    write_int(m.type);
    write_position(m.p);
    write_position(m.tp);
    write_int(m.value1);
    write_int(m.value2);
    write_int(m.value3);
    write_int(m.value4);
    write_int(m.activated);
    fputc('\n', file);
}

void write_request(struct request r)
{
    write_mission(r.objective);
    write_item(r.reward);
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
    write_int(m.x_start);
    write_int(m.y_start);
    write_int(m.initial_delay);
    write_int(m.color_length);
    if (m.color_length)
        write_int_list(m.color_sequency, m.color_length);
    int i = 0;
    while (i < m.x)
    {
        write_cell_list(m.cells[i], m.y);
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

void write_portal(struct portal p)
{
    write_int(p.type);
    write_int(p.value);
    write_position(p.last_position);
    write_position(p.portal_position);
    fputc('\n', file);
}

void write_knowledge_map(int character, int zone, int map)
{
    int i = 0;
    while (i < project_data.zones[zone].maps[map].x)
    {
        write_char_list(save_data.knowledge[character].zones[zone].maps[map].cells[i], project_data.zones[zone].maps[map].y);
        i++;
    }
    write_int(save_data.knowledge[character].zones[zone].maps[map].has_map);
    fputc('\n', file);
}

void write_knowledge_zone(int character, int zone)
{
    int i = 0;
    while (i < project_data.zones[zone].map_number)
    {
        write_knowledge_map(character, zone, i);
        i++;
    }
    fputc('\n', file);
}

void write_knowledge(int character)
{
    int i = 0;
    while (i < project_data.parameters[11])
    {
        write_knowledge_zone(character, i);
        i++;
    }
    fputc('\n', file);
}

void write_text(struct text t)
{
    write_int(t.length);
    fputs(t.string, file);
    fputc('\n', file);
}

void write_cinematic_event(struct cinematic_event c)
{
    write_int(c.type);
    write_int(c.value1);
    write_int(c.value2);
    write_int(c.value3);
    write_int(c.value4);
    write_position(c.p);
    write_position(c.target);
    write_text(c.dialog);
    fputc('\n', file);
}

void write_cinematic_trigger(struct cinematic_trigger c)
{
    write_int(c.type);
    write_position(c.p);
    write_int(c.value);
    write_id(c.ID);
    write_int(c.active);
}

void write_cinematic(struct cinematic c)
{
    write_int(c.length);
    int i = 0;
    while (i < c.length)
    {
        write_cinematic_event(c.events[i]);
        i++;
    }
    write_cinematic_trigger(c.trigger);
    fputc('\n', file);
}

void write_cinematic_list()
{
    int i = 0;
    while (i < project_data.parameters[12])
    {
        write_cinematic(project_data.cinematics[i]);
        i++;
    }
    fputc('\n', file);
}

void write_project(struct project p)
{
    write_int_list(p.parameters, 13);
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
        write_item_list(p.inventories[i], 40);
        i++;
    }
    write_container_list(p.containers, p.parameters[10]);
    write_cinematic_list();
}

void write_request_state(struct request_state r)
{
    write_int(r.active);
    write_int(r.value);
    fputc('\n', file);
}

void write_request_state_list()
{
    int size = 4 + project_data.zones[0].map_number + project_data.zones[4].map_number;
    int i = 0;
    while (i < size)
    {
        write_request_state(save_data.request_states[i]);
        i++;
    }
    fputc('\n', file);
}

void write_save_data()
{
    fprintf(file, "%s\n%s\n", project_data.project_name, project_data.author_name);
    write_int(get_player());
    int i = 0;
    while (i < 5)
    {
        write_portal(save_data.portals[i]);
        write_knowledge(i);
        write_position(project_data.character_positions[i]);
        write_item_list(project_data.inventories[i], 40);
        i++;
    }
    i = 0;
    while (i < project_data.parameters[11])
    {
        write_map_list(project_data.zones[i].maps, project_data.zones[i].map_number);
        i++;
    }
    write_position(save_data.sleep_target);
    write_container_list(project_data.containers, project_data.parameters[10]);
    write_request_state_list();
    i = 0;
    while (i < 5)
    {
        write_request_list(project_data.requests[i], project_data.parameters[i + 5]);
        i++;
    }
    write_cinematic_states_list(project_data.cinematics, project_data.parameters[12]);
}

char save_project(struct project p)
{
    const char* file_fields[5] = {
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

char save_game(int spot)
{
    print_error("saving");
    char number[2] = {spot + '0', 0};
    const char* file_fields[3] = {
        "levels/saves/save",
        number,
        ".txt"
    };
    char filename[451] = { 0 };
    concat_str(filename, file_fields, 450, 3);
    print_error(filename);
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
    write_save_data();
    fclose(file);
    print_error("Partie sauvegardée !");
    file = NULL;
    return 1;
}

char export_project(struct project p)
{
    const char* file_fields[5] = {
        "levels/games/",
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
        print_error("- Dossier levels/games inexistant");
        return 0;
    }
    write_project(p);
    file_saved = 1;
    print_error("Projet exporte !");
    fclose(file);
    file = NULL;
    return 1;
}