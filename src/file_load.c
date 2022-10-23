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

char load_request(struct request *r)
{
    int value = 0;
    struct position p;
    struct item i;
    if (!load_int(&value))
        return 0;
    r->type = value;
    if (!load_int(&value))
        return 0;
    r->orientation = value;
    if (!load_int(&value))
        return 0;
    r->item_quantity = value;
    if (!load_int(&value))
        return 0;
    r->vision_field = value;
    if (!load_int(&value))
        return 0;
    r->wanted_character = value;
    if (!load_int(&value))
        return 0;
    r->active = value;
    if (!load_position(&p))
        return 0;
    r->place = p;
    if (!load_position(&p))
        return 0;
    r->destination = p;
    if (!load_item(&i))
        return 0;
    r->item_wanted = i;
    if (!load_item(&i))
        return 0;
    r->reward_item = i;
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

char load_map(struct map *m)
{
    int value = 0;
    if (!load_int(&value))
        return 0;
    m->x = value;
    if (!load_int(&value))
        return 0;
    m->y = value;
    m->cells = malloc(sizeof(int*) * m->x);
    if (m->cells == NULL)
        return 0;
    m->items = malloc(sizeof(struct item*) * m->x);
    if (m->items == NULL)
    {
        free(m->cells);
        return 0;
    }
    int i = 0;
    while (i < m->x)
    {
        m->cells[i] = NULL;
        m->items[i] = NULL;
        i++;
    }
    i = 0;
    char valid = 1;
    while (i < m->x && valid)
    {
        m->cells[i] = malloc(sizeof(int) * m->y);
        m->items[i] = malloc(sizeof(struct item) * m->y);
        valid = (m->cells[i] != NULL && m->items[i] != NULL);
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
            i++;
        }
        free(m->cells);
        free(m->items);
        m->x = 0;
        m->cells = NULL;
        m->items = NULL;
        return 0;
    }
    return check_endline();
}

char load_map_list(struct map *list, int size)
{
    long i = 0;
    char valid = 1;
    while (i < size && valid)
    {
        valid = load_map(list + i);
        i++;
    }
    if (!valid)
    {
        while (i)
        {
            i--;
            free_map(list[i]);
            list[i].x = 0;
            list[i].cells = NULL;
            list[i].items = NULL;
        }
        return 0;
    }
    return check_endline();
}

char load_project(struct project *p)
{
    char valid = load_int_list(p->parameters, 16);
    if (!valid)
        return 0;
    long i = 0;
    while (i < 5)
    {
        p->requests[i] = malloc(sizeof(struct request) * p->parameters[i + 10]);
        p->maps[i] = malloc(sizeof(struct map) * p->parameters[i + 5]);
        valid = valid && (p->requests[i] != NULL && p->maps[i] != NULL);
        i++; 
    }
    if (valid)
    {
        p->containers = malloc(sizeof(struct container) * p->parameters[15]);
        valid = p->containers != NULL;
    }
    if (!valid)
    {
        i = 0;
        while (i < 5)
        {
            free(p->requests[i]);
            free(p->maps[i]);
            i++;
        }
        free(p->containers);
        return 0;
    }
    i = 0;
    while (i < p->parameters[15])
    {
        p->containers[i].items = NULL;
        i++;
    }
    i = 0;
    while (i < 5)
    {
        int j = 0;
        while (j < p->parameters[i + 5])
        {
            p->maps[i][j].x = 0;
            p->maps[i][j].cells = NULL;
            p->maps[i][j].items = NULL;
            j++;
        }
        i++;
    }
    i = 0;
    struct position pos;
    while (i < 5 && valid)
    {
        valid = load_position(&pos);
        p->character_positions[i] = pos;
        if (valid)
            valid = load_request_list(p->requests[i], p->parameters[i + 10]);
        if (valid)
            valid = load_item_list(p->inventories[i], 50);
        if (valid)
            valid = load_map_list(p->maps[i], p->parameters[i + 5]);
        i++;
    }
    if (valid)
        valid = load_container_list(p->containers, p->parameters[15]);
    if (!valid)
    {
        free_project(*p);
        return 0;
    }
    return (fgetc(file) == EOF);
}

char open_project(struct project *p)
{
    char* file_fields[5] = {
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
        print_error("Projet charge !");
        return 1;
    }
    print_error("Mauvais format...");
    return 0;
}