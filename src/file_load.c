
#include "file_load.h"

char check_endline()
{
    char c = fgetc(file);
    return (c == '\n');
}

char load_int(int *value)
{
    char c = fgetc(file);
    char empty = 1;
    *value = 0;
    while (c >= '0' && c <= '9')
    {
        empty = 0;
        *value = *value * 10 + c - '0';
        c = fgetc(file);
    }
    return (c == ' ' && !empty);
}

char load_char(char *value)
{
    char c = fgetc(file);
    char empty = 1;
    *value = 0;
    while (c >= '0' && c <= '9')
    {
        empty = 0;
        *value = *value * 10 + c - '0';
        c = fgetc(file);
    }
    return (c == ' ' && !empty);
}

char load_id(long *ID)
{
    char c = fgetc(file);
    char empty = 1;
    *ID = 0;
    while (c >= '0' && c <= '9')
    {
        empty = 0;
        *ID = *ID * 10 + c - '0';
        c = fgetc(file);
    }
    return (c == ' ' && !empty);
}

char load_int_list(int* list, int size)
{
    int i = 0;
    char valid = 1;
    while (i < size && valid)
    {
        valid = load_int(list + i);
        i++;
    }
    if (!valid)
        return 0;
    return check_endline();
}

char load_char_list(char* list, int size)
{
    int i = 0;
    char valid = 1;
    while (i < size && valid)
    {
        valid = load_char(list + i);
        i++;
    }
    if (!valid)
        return 0;
    return check_endline();
}

char load_position(struct position *p)
{
    int value = 0;
    if (!load_int(&value))
        return 0;
    p->zone = value;
    if (!load_int(&value))
        return 0;
    p->map = value;
    if (!load_int(&value))
        return 0;
    p->x = value;
    if (!load_int(&value))
        return 0;
    p->y = value;
    if (!load_int(&value))
        return 0;
    p->orientation = value;
    return check_endline();
}

char load_item(struct item *i)
{
    int value = 0;
    long ID = 0;
    if (!load_int(&value))
        return 0;
    i->type = value;
    if (!load_int(&value))
        return 0;
    i->value = value;
    if (!load_id(&ID))
        return 0;
    i->ID = ID;
    if (!load_int(&value))
        return 0;
    i->activation = value;
    return check_endline();
}

char load_item_list(struct item *list, int size)
{
    int i = 0;
    char valid = 1;
    while (i < size && valid)
    {
        valid = load_item(list + i);
        i++;
    }
    if (!valid)
        return 0;
    return check_endline();
}

char load_container(struct container *c)
{
    int capacity = 0;
    if (!load_int(&capacity))
        return 0;
    c->size = capacity;
    c->items = malloc(sizeof(struct item) * capacity);
    if (c->items == NULL)
        return 0;
    struct position p;
    if (!load_position(&p))
        return 0;
    c->position = p;
    if (!load_item_list(c->items, capacity))
    {
        free(c->items);
        c->items = NULL;
        return 0;
    }
    return check_endline();
}

char load_container_list(struct container *list, long size)
{
    long i = 0;
    char valid = 1;
    while (i < size && valid)
    {
        valid = load_container(list + i);
        i++;
    }
    if (!valid)
    {
        while (i)
        {
            i--;
            free(list[i].items);
            list[i].items = NULL;
        }
        return 0;
    }
    return check_endline();
}

char load_text(struct text *t)
{
    int value = 0;
    if (!load_int(&value))
        return 0;
    t->length = value;
    t->string = malloc(sizeof(char) * (value + 1));
    if (t->string == NULL)
        return 0;
    char c = fgetc(file);
    int i = 0;
    while (c != 0 && c != EOF && i < value)
    {
        t->string[i] = c;
        c = fgetc(file);
        i++;
    }
    t->string[i] = 0;
    if (c != '\n')
        return 0;
    return 1;
}

char load_mission(struct mission *m)
{
    int value = 0;
    struct position p;
    if (!load_int(&value))
        return 0;
    m->type = value;
    if (!load_position(&p))
        return 0;
    m->p = p;
    if (!load_position(&p))
        return 0;
    m->tp = p;
    if (!load_int(&value))
        return 0;
    m->value1 = value;
    if (!load_int(&value))
        return 0;
    m->value2 = value;
    if (!load_int(&value))
        return 0;
    m->value3 = value;
    if (!load_int(&value))
        return 0;
    m->value4 = value;
    if (!load_int(&value))
        return 0;
    m->activated = value;
    //m->activated = 1;
    return check_endline();
}

char load_request(struct request *r)
{
    struct mission m;
    struct item i;
    if (!load_mission(&m))
        return 0;
    r->objective = m;
    if (!load_item(&i))
        return 0;
    r->reward = i;
    return check_endline();
}

char load_request_list(struct request *list, int size)
{
    int i = 0;
    char valid = 1;
    while (i < size && valid)
    {
        valid = load_request(list + i);
        i++;
    }
    if (!valid)
        return 0;
    return check_endline();
}

char load_cinematic_event(struct cinematic_event *c)
{
    c->dialog.length = 0; c->dialog.string = NULL;
    int value = 0;
    if (!load_int(&value))
        return 0;
    c->type = value;
    if (!load_int(&value))
        return 0;
    c->value1 = value;
    if (!load_int(&value))
        return 0;
    c->value2 = value;
    if (!load_int(&value))
        return 0;
    c->value3 = value;
    if (!load_int(&value))
        return 0;
    c->value4 = value;
    struct position p;
    if (!load_position(&p))
        return 0;
    c->p = p;
    if (!load_position(&p))
        return 0;
    c->target = p;
    struct text t; t.length = 0; t.string = NULL;
    if (!load_text(&t))
        return 0;
    c->dialog = t;
    return check_endline();
}

char load_cinematic_event_list(struct cinematic_event *list, int size)
{
    int i = 0;
    char valid = 1;
    while (i < size && valid)
    {
        valid = load_cinematic_event(list + i);
        i++;
    }
    if (!valid)
    {
        while (i)
        {
            i--;
            free_cinematic_event(list + i);
        }
        return 0;
    }
    return 1;
}

char load_cinematic_trigger(struct cinematic_trigger *c)
{
    int value = 0;
    if (!load_int(&value))
        return 0;
    c->type = value;
    struct position p;
    if (!load_position(&p))
        return 0;
    c->p = p;
    if (!load_int(&value))
        return 0;
    c->value = value;
    long ID = 0;
    if (!load_id(&ID))
        return 0;
    c->ID = ID;
    char b = 0;
    if (!load_char(&b))
        return 0;
    c->active = b;
    return 1;
}

char load_cinematic(struct cinematic *c)
{
    c->length = 0; c->events = NULL;
    int value = 0;
    if (!load_int(&value))
        return 0;
    c->length = value;
    c->events = malloc(sizeof(struct cinematic_event) * value);
    if (c->events == NULL)
        return 0;
    if (!load_cinematic_event_list(c->events, value))
    {
        c->length = 0;
        free(c->events);
        c->events = NULL;
        return 0;
    }
    struct cinematic_trigger ct;
    if (!load_cinematic_trigger(&ct))
    {
        c->length = 0;
        free(c->events);
        c->events = NULL;
        return 0;
    }
    c->trigger = ct;
    return check_endline();
}

char load_cinematic_list(struct cinematic *list, int size)
{
    int i = 0;
    char valid = 1;
    while (i < size && valid)
    {
        valid = load_cinematic(list + i);
        i++;
    }
    if (!valid)
    {
        while (i)
        {
            i--;
            free_cinematic(list + i);
        }
        return 0;
    }
    return check_endline();
}


char load_map(struct map *m)
{
    int value = 0;
    if (!load_int(&value))
        return 0;
    m->x = value;
    if (!load_int(&value))
        return 0;
    m->y = value;
    if (!load_int(&value))
        return 0;
    m->x_start = value;
    if (!load_int(&value))
        return 0;
    m->y_start = value;
    if (!load_int(&value))
        return 0;
    m->initial_delay = value;
    if (!load_int(&value))
        return 0;
    m->color_length = value;
    m->color_sequency = NULL;
    m->cells = malloc(sizeof(int*) * m->x);
    if (m->cells == NULL)
        return 0;
    m->items = malloc(sizeof(struct item*) * m->x);
    if (m->items == NULL)
    {
        free(m->cells);
        return 0;
    }
    m->thoughts = malloc(sizeof(char*) * m->x);
    if (m->thoughts == NULL)
    {
        free(m->cells);
        free(m->items);
        return 0;
    }
    m->cinematic_triggers = malloc(sizeof(long*) * m->x);
    if (m->cinematic_triggers == NULL)
    {
        free(m->thoughts);
        free(m->cells);
        free(m->items);
        return 0;
    }
    if (m->color_length)
    {
        m->color_sequency = malloc(sizeof(int) * m->color_length);
        if (m->color_sequency == NULL)
        {
            free(m->cells);
            free(m->items);
            free(m->thoughts);
            return 0;
        }
    }
    int i = 0;
    while (i < m->x)
    {
        m->cells[i] = NULL;
        m->items[i] = NULL;
        m->thoughts[i] = NULL;
        i++;
    }
    i = 0;
    char valid = 1;
    while (i < m->x && valid)
    {
        m->cells[i] = malloc(sizeof(int) * m->y);
        m->thoughts[i] = malloc(sizeof(char) * m->y);
        m->items[i] = malloc(sizeof(struct item) * m->y);
        m->cinematic_triggers[i] = malloc(sizeof(long) * m->y);
        valid = (m->cells[i] != NULL && m->items[i] != NULL && m->thoughts[i] != NULL && m->cinematic_triggers[i] != NULL);
        i++;
    }
    if (valid)
    {
        i = 0;
        while (i < m->x && valid)
        {
            valid = load_int_list(m->cells[i], m->y);
            if (valid)
                valid = load_item_list(m->items[i], m->y);
            if (valid)
                valid = load_char_list(m->thoughts[i], m->y);
            i++;
        }
    }
    if (!valid)
    {
        i = 0;
        while (i < m->x)
        {
            free(m->cells[i]);
            free(m->items[i]);
            free(m->cinematic_triggers[i]);
            free(m->thoughts[i]);
            i++;
        }
        free(m->cells);
        free(m->items);
        free(m->thoughts);
        free(m->cinematic_triggers);
        free(m->color_sequency);
        m->x = 0;
        m->cells = NULL;
        m->items = NULL;
        m->thoughts = NULL;
        m->color_sequency = NULL;
        return 0;
    }
    return check_endline();
}

char load_map_list(struct zone *zone)
{
    long i = 0;
    char valid = load_int(&(zone->map_number));
    if (valid)
        zone->maps = malloc(sizeof(struct map) * zone->map_number);
    if (zone->maps == NULL)
        return 0; 
    while (i < zone->map_number && valid)
    {
        valid = load_map(zone->maps + i);
        i++;
    }
    if (!valid)
    {
        while (i)
        {
            i--;
            free_map(zone->maps[i]);
            zone->maps[i].x = 0;
            zone->maps[i].cells = NULL;
            zone->maps[i].items = NULL;
            zone->maps[i].color_sequency = NULL;
            zone->maps[i].thoughts = NULL;
        }
        free(zone->maps);
        zone->maps = NULL;
        return 0;
    }
    return check_endline();
}

char load_portal(struct portal *p)
{
    int value = 0;
    if (!load_int(&value))
        return 0;
    p->type = value;
    if (!load_int(&value))
        return 0;
    p->value = value;
    struct position pos;
    if (!load_position(&pos))
        return 0;
    p->last_position = pos;
    if (!load_position(&pos))
        return 0;
    p->portal_position = pos;
    return check_endline();
}

char load_knowledge_map(int zone, int map, struct map_knowledge *m)
{
    int value = 0;
    int i = 0;
    char valid = 1;
    while (i < project_data.zones[zone].maps[map].x && valid)
    {
        valid = load_char_list(m->cells[i], project_data.zones[zone].maps[map].y);
        i++;
    }
    if (valid)
        valid = load_int(&value);
    m->has_map = value;
    return check_endline() && valid;
}

char load_knowledge_zone(int zone, struct zone_knowledge *m)
{
    int i = 0;
    char valid = 1;
    while (i < project_data.zones[zone].map_number && valid)
    {
        valid = load_knowledge_map(zone, i, &(m->maps[i]));
        i++;
    }
    return check_endline() && valid;
}

char load_knowledge(struct character_knowledge *m)
{
    int i = 0;
    char valid = 1;
    while (i < project_data.parameters[11] && valid)
    {
        valid = load_knowledge_zone(i, &(m->zones[i]));
        i++;
    }
    return check_endline() && valid;
}

char load_request_state(struct request_state *r)
{
    int value = 0;
    if (!load_int(&value))
        return 0;
    r->active = value;
    if (!load_int(&value))
        return 0;
    r->value = value;
    return check_endline();
}

char load_request_state_list(struct request_state *r)
{
    int size = 4 + project_data.zones[0].map_number + project_data.zones[4].map_number;
    int i = 0;
    while (i < size)
    {
        if (!load_request_state(r + i))
            return 0;
        i++;
    }
    return check_endline();
}

char load_save()
{
    char c = fgetc(file);
    while (c != '\n' && c != EOF)
        c = fgetc(file);
    if (c == EOF)
        return 0;
    c = fgetc(file);
    while (c != '\n' && c != EOF)
        c = fgetc(file);
    if (c == EOF)
        return 0;
    struct project p;
    p.cinematics = NULL;
    struct savedatas s;
    long i = 0;
    while (i < 5)
    {
        p.requests[i] = NULL;
        i++;
    }
    i = 0;
    while (i < 13)
    {
        p.parameters[i] = project_data.parameters[i];
        i++;
    }
    p.parameters[12] = 0;
    i = 0;
    char valid = 1;
    while (i < 5 && valid)
    {
        p.requests[i] = malloc(sizeof(struct request) * project_data.parameters[i + 5]);
        valid = valid && (p.requests[i] != NULL);
        i++; 
    }
    while (!valid && i)
    {
        i--;
        free(p.requests[i]);
    }
    if (!valid)
        return 0;
    int player = 0;
    valid = load_int(&player);
    if (!valid)
    {
        i = 0;
        while (i < 5)
        {
            free(p.requests[i]);
            i++;
        }
        return 0;
    }
    s.request_states = NULL;
    valid = allocate_knowledge(&s);
    if (!valid)
    {
        i = 0;
        while (i < 5)
        {
            free(p.requests[i]);
            i++;
        }
        return 0;
    }
    p.zones = malloc(sizeof(struct zone) * project_data.parameters[11]);
    if (p.zones == NULL)
    {
        free_save_data(s);
        i = 0;
        while (i < 5)
        {
            free(p.requests[i]);
            i++;
        }
        return 0;
    }
    i = 0;
    while (i < project_data.parameters[11])
    {
        p.zones[i].map_number = 0;
        p.zones[i].maps = NULL;
        i++;
    }
    if (valid)
    {
        p.containers = malloc(sizeof(struct container) * project_data.parameters[10]);
        valid = p.containers != NULL;
    }
    if (!valid)
    {
        free_save_data(s);
        free(p.containers);
        free(p.zones);
        i = 0;
        while (i < 5)
        {
            free(p.requests[i]);
            i++;
        }
        return 0;
    }
    i = 0;
    while (i < project_data.parameters[10])
    {
        p.containers[i].items = NULL;
        i++;
    }
    i = 0;
    struct position pos;
    while (i < 5 && valid)
    {
        valid = load_portal(s.portals + i);
        if (valid)
            valid = load_knowledge(s.knowledge + i);
        if (valid)
            valid = load_position(&pos);
        p.character_positions[i] = pos;
        if (valid)
            valid = load_item_list(p.inventories[i], 40);
        i++;
    }
    i = 0;
    while (i < project_data.parameters[11] && valid)
    {
       valid = load_map_list(&(p.zones[i]));
       i++;
    }
    if (valid)
        valid = load_position(&pos);
    s.sleep_target = pos;
    if (valid)
        valid = load_container_list(p.containers, project_data.parameters[10]);
    if (valid)
        valid = load_request_state_list(s.request_states);
    i = 0;
    while (i < 5 && valid)
    {
        valid = load_request_list(p.requests[i], project_data.parameters[5 + i]);
        i++;
    }
    valid = valid && (fgetc(file) == EOF);
    if (!valid)
    {
        free_save_data(s);
        free_project(p);
    }
    i = 0;
    while (i < 5)
    {
        save_data.portals[i] = s.portals[i];
        project_data.character_positions[i] = p.character_positions[i];
        int j = 0;
        while (j < 40)
        {
            project_data.inventories[i][j] = p.inventories[i][j];
            j++;
        }
        j = 0;
        while (j < project_data.parameters[i + 5])
        {
            project_data.requests[i][j].objective.activated = p.requests[i][j].objective.activated;
            j++;
        }
        i++;
    }
    i = 0;
    while (i < 4 + project_data.zones[0].map_number + project_data.zones[4].map_number)
    {
        save_data.request_states[i] = s.request_states[i];
        i++;
    }
    i = 0;
    while (i < project_data.parameters[11])
    {
        int j = 0;
        while (j < project_data.zones[i].map_number)
        {
            int k = 0;
            while (k < project_data.zones[i].maps[j].x)
            {
                int l = 0;
                while (l < project_data.zones[i].maps[j].y)
                {
                    project_data.zones[i].maps[j].cells[k][l] = p.zones[i].maps[j].cells[k][l];
                    project_data.zones[i].maps[j].items[k][l] = p.zones[i].maps[j].items[k][l];
                    project_data.zones[i].maps[j].thoughts[k][l] = p.zones[i].maps[j].thoughts[k][l];
                    int c = 0;
                    while (c < 5)
                    {
                        save_data.knowledge[c].zones[i].maps[j].cells[k][l] = s.knowledge[c].zones[i].maps[j].cells[k][l];
                        c++;
                    }
                    l++;
                }
                k++;
            }
            int c = 0;
            while (c < 5)
            {
                save_data.knowledge[c].zones[i].maps[j].has_map = s.knowledge[c].zones[i].maps[j].has_map;
                c++;
            }
            j++;
        }
        i++;
    }
    save_data.sleep_target = s.sleep_target;
    i = 0;
    while (i < project_data.parameters[10])
    {
        int j = 0;
        while (j < project_data.containers[i].size)
        {
            project_data.containers[i].items[j] = p.containers[i].items[j];
            j++;
        }
        i++;
    }
    reload_with_character(player);
    free_save_data(s);
    free_project(p);
    return 1;
}

char load_project(struct project *p)
{
    char valid = load_int_list(p->parameters, 13);
    if (!valid)
        return 0;
    p->zones = malloc(sizeof(struct zone) * p->parameters[11]);
    if (p->zones == NULL)
        return 0;
    long i = 0;
    while (i < p->parameters[11])
    {
        p->zones[i].map_number = 0;
        p->zones[i].maps = NULL;
        i++;
    }
    i = 0;
    while (i < 5)
    {
        p->requests[i] = NULL;
        i++;
    }
    i = 0;
    while (i < 5 && valid)
    {
        p->requests[i] = malloc(sizeof(struct request) * p->parameters[i + 5]);
        valid = valid && (p->requests[i] != NULL);
        i++;
    }
    while (i && !valid)
    {
        i--;
        free(p->requests[i]);
    }
    if (!valid)
    {
        free(p->zones);
        return 0;
    }
    p->containers = malloc(sizeof(struct container) * p->parameters[10]);
    p->cinematics = malloc(sizeof(struct cinematic) * p->parameters[12]);
    valid = p->containers != NULL && p->cinematics;
    if (!valid)
    {
        i = 0;
        while (i < 5)
        {
            free(p->requests[i]);
            i++;
        }
        free(p->containers);
        free(p->cinematics);
        free(p->zones);
        return 0;
    }
    i = 0;
    while (i < p->parameters[10])
    {
        p->containers[i].items = NULL;
        i++;
    }
    i = 0;
    while (i < p->parameters[12])
    {
        p->cinematics[i].length = 0;
        p->cinematics[i].events = NULL;
        i++;
    }
    i = 0;
    while (i < p->parameters[11] && valid)
    {
       valid = load_map_list(&(p->zones[i]));
       i++;
    }
    i = 0;
    struct position pos;
    while (i < 5 && valid)
    {
        valid = load_position(&pos);
        p->character_positions[i] = pos;
        if (valid)
        {
            valid = load_request_list(p->requests[i], p->parameters[i + 5]);
        }
        if (valid)
        {
            valid = load_item_list(p->inventories[i], 40);
        }
        i++;
    }
    if (valid)
        valid = load_container_list(p->containers, p->parameters[10]);
    if (valid)
        valid = load_cinematic_list(p->cinematics, p->parameters[12]);
    if (!valid)
    {
        free_project(*p);
        return 0;
    }
    return (fgetc(file) == EOF);
}

char open_project(struct project *p)
{
    const char* file_fields[5] = {
        "levels/projects/",
        p->project_name,
        "[",
        p->author_name,
        "].txt"
    };
    char filename[451] = { 0 };
    concat_str(filename, file_fields, 450, 5);
    file = fopen(filename, "r");
    if (!file)
    {
        print_error("Echec...");
        print_error("    Sources possibles d'erreur :    ");
        print_error("      - Nom du projet invalide      ");
        print_error("         - Pseudo invalide          ");
        print_error("- Dossier levels/projects inexistant");
        return 0;
    }
    char result = load_project(p);
    fclose(file);
    file = NULL;
    if (result)
    {
        file_loaded = 1;
        file_saved = 0;
        print_error("Projet charge !");
        return 1;
    } 
    print_error("Mauvais format...");
    return 0;
}

char open_save(int spot)
{
    //print_error("loading");
    char number[2] = {spot + '0', 0};
    const char* file_fields[3] = {
        "levels/saves/save",
        number,
        ".txt"
    };
    char filename[451] = { 0 };
    concat_str(filename, file_fields, 450, 3);
    file = fopen(filename, "r");
    if (!file)
    {
        print_error("Echec...");
        print_error("    Sources possibles d'erreur :    ");
        print_error("      - Nom du projet invalide      ");
        print_error("         - Pseudo invalide          ");
        print_error("- Dossier levels/projects inexistant");
        return 0;
    }
    char result = load_save();
    fclose(file);
    file = NULL;
    if (result)
    {
        file_loaded = 1;
        file_saved = 0;
        print_error("Projet charge !");
        return 1;
    }
    print_error("Mauvais format...");
    return 0;
}