#include "structures.h"

void free_container(struct container c)
{
    free(c.items);
    c.items = NULL;
}

void free_map(struct map r)
{
    int i = 0;
    while (i < r.x)
    {
        free(r.cells[i]);
        free(r.items[i]);
        free(r.thoughts[i]);
        free(r.cinematic_triggers[i]);
        i++;
    }
    free(r.cells);
    free(r.items);
    free(r.thoughts);
    free(r.cinematic_triggers);
    free(r.color_sequency);
}

void free_map_backup(int zone, int map)
{
    int i = 0;
    
    if (backup_data.zones[zone].maps[map].cells != NULL && backup_data.zones[zone].maps[map].items != NULL)
    {
        while (i < project_data.zones[zone].maps[map].x)
        {
            free(backup_data.zones[zone].maps[map].cells[i]);
            free(backup_data.zones[zone].maps[map].items[i]);
            i++;
        }
    }
    free(backup_data.zones[zone].maps[map].cells);
    free(backup_data.zones[zone].maps[map].items);
}

void free_zone_backup(int zone)
{
    int i = 0;
    if (backup_data.zones[zone].maps != NULL)
    {
        while (i < project_data.zones[zone].map_number)
        {
            free_map_backup(zone, i);
            i++;
        }
    }
    free(backup_data.zones[zone].maps);
}

void free_backup()
{
    int i = 0;
    if (backup_data.zones != NULL)
    {
        while (i < project_data.parameters[11])
        {
            free_zone_backup(i);
            i++;
        }
    }
    free(backup_data.zones);
}

void free_map_knowledge(struct map_knowledge m, int zone, int map)
{
    int i = 0;
    while (i < project_data.zones[zone].maps[map].x)
    {
        free(m.cells[i]);
        i++;
    }
    free(m.cells);
}

void free_zone_knowledge(struct zone_knowledge m, int zone)
{
    int i = 0;
    while (i < project_data.zones[zone].map_number)
    {
        free_map_knowledge(m.maps[i], zone, i);
        i++;
    }
    free(m.maps);
}

void free_character_knowledge(struct character_knowledge m)
{
    int i = 0;
    while (i < project_data.parameters[11])
    {
        free_zone_knowledge(m.zones[i], i);
        i++;
    }
    free(m.zones);
}

void free_zone(struct zone z)
{
    int i = 0;
    while (i < z.map_number && z.maps != NULL)
    {
        free_map(z.maps[i]);
        i++;
    }
    free(z.maps);
}

void free_text(struct text *t)
{
    free(t->string);
    t->string = NULL;
    t->length = 0;
}

void free_cinematic_event(struct cinematic_event *c)
{
    free_text(&(c->dialog));
}

void free_cinematic(struct cinematic *c)
{
    int i = 0;
    while (i < c->length)
    {
        free_cinematic_event(c->events + i);
        i++;
    }
    free(c->events);
    c->events = NULL;
    c->length = 0;
}

void free_save_data(struct savedatas s)
{
    free(s.request_states);
    int i = 0;
    while (i < 5)
    {
        free_character_knowledge(s.knowledge[i]);
        i++;
    }
}

void free_project(struct project p)
{
    int i = 0;
    while (i < 5)
    {
        free(p.requests[i]);
        i++;
    }
    i = 0;
    while (i < p.parameters[11])
    {
        free_zone(p.zones[i]);
        i++;
    }
    free(p.zones);
    i = 0;
    while (i < p.parameters[10])
    {
        free_container(p.containers[i]);
        i++;
    }
    i = 0;
    while (i < p.parameters[12])
    {
        free_cinematic(p.cinematics + i);
        i++;
    }
    free(p.containers);
    file_loaded = 0;
}

void delete_container(long ID)
{
    //print_error("Trying to delete container");
    //print_error_int(project_data.parameters[10]);
    free_container(project_data.containers[ID]);
    project_data.containers[ID] = project_data.containers[project_data.parameters[10] - 1];
    project_data.parameters[10]--;
    struct position last_container = project_data.containers[ID].position;
    if (last_container.zone == 0) // Inventory
        project_data.inventories[last_container.map][last_container.x].ID = ID;
    else
        project_data.zones[last_container.zone - 1].maps[last_container.map].items[last_container.x][last_container.y].ID = ID;
    if (project_data.parameters[10])
    {
        //print_error_int(project_data.parameters[10]);
        struct container *new_list = realloc(project_data.containers, sizeof(struct container) * project_data.parameters[10]);
        if (new_list)
            project_data.containers = new_list; // In case of fail, the memory just won't be free
    }
    else
    {
        free(project_data.containers);
        project_data.containers = NULL;
    }
    //print_error("Delete succeeded");
}

void add_item(struct container container, struct item o)
{
    int j = 0;
    while (container.items[j].value < o.value && container.items[j].value)
        j++;
    int k = container.size - 1;
    while (k > j)
    {
        k--;
        container.items[k + 1].type = container.items[k].type;
        container.items[k + 1].value = container.items[k].value;
    }
    container.items[j].type = o.type;
    container.items[j].value = o.value;
    container.items[j].activation = o.activation;
    container.items[j].ID = o.ID;
}

void remove_item(struct container container, int value)
{
    int j = 0;
    while (j < container.size && container.items[j].value < value && container.items[j].value)
        j++;
    while (j + 1 < container.size)
    {
        container.items[j].type = container.items[j + 1].type;
        container.items[j].value = container.items[j + 1].value;
        j++; 
    }
    if (j < container.size)
    {
        container.items[j].type = 0;
        container.items[j].value = 0;
    }
}

char allocate_knowledge(struct savedatas *s)
{
    int perso = 0;
    char valid = 1;
    s->request_states = malloc(sizeof(struct request_state) * (4 + project_data.zones[0].map_number + project_data.zones[4].map_number));
    if (s->request_states == NULL)
        return 0;
    int i = 0;
    while (i < 4 + project_data.zones[0].map_number + project_data.zones[4].map_number)
    {
        s->request_states[i].active = 0;
        s->request_states[i].value = 0;
        i++;
    }
    while (perso < 5 && valid)
    {
        int i = 0;
        s->knowledge[perso].zones = malloc(sizeof(struct character_knowledge) * project_data.parameters[11]);
        char valid = s->knowledge[perso].zones != NULL;
        while (i < project_data.parameters[11] && valid)
        {
            s->knowledge[perso].zones[i].maps = malloc(sizeof(struct map_knowledge) * project_data.zones[i].map_number);
            valid = s->knowledge[perso].zones[i].maps != NULL;
            int j = 0;
            while (j < project_data.zones[i].map_number && valid)
            {
                s->knowledge[perso].zones[i].maps[j].cells = malloc(sizeof(char*) * project_data.zones[i].maps[j].x);
                s->knowledge[perso].zones[i].maps[j].has_map = 0;
                valid = (s->knowledge[perso].zones[i].maps[j].cells != NULL);
                int x = 0;
                while (x < project_data.zones[i].maps[j].x && valid)
                {
                    s->knowledge[perso].zones[i].maps[j].cells[x] = malloc(sizeof(char) * project_data.zones[i].maps[j].y);
                    valid = (s->knowledge[perso].zones[i].maps[j].cells[x] != NULL);
                    int y = 0;
                    while (y < project_data.zones[i].maps[j].y && valid)
                    {
                        s->knowledge[perso].zones[i].maps[j].cells[x][y] = 0;
                        y++;
                    }
                    x++;
                }
                while (x && !valid)
                {
                    x--;
                    free(s->knowledge[perso].zones[i].maps[j].cells[x]);
                }
                if (!valid)
                {
                    free(s->knowledge[perso].zones[i].maps[j].cells);
                    s->knowledge[perso].zones[i].maps[j].cells = NULL;
                }
                j++;
            }
            while (j && !valid)
            {
                j--;
                if (s->knowledge[perso].zones[i].maps[j].cells)
                {
                    int x = 0;
                    while (x < project_data.zones[i].maps[j].x)
                    {
                        free(s->knowledge[perso].zones[i].maps[j].cells[x]);
                        x++;
                    }
                    free(s->knowledge[perso].zones[i].maps[j].cells);
                }
            }
            if (!valid)
            {
                free(s->knowledge[perso].zones[i].maps);
                s->knowledge[perso].zones[i].maps = NULL;
            }
            i++;
        }
        while (i && !valid)
        {
            i--;
            if (s->knowledge[perso].zones[i].maps)
            {
                int j = 0;
                while (j < project_data.zones[i].map_number)
                {
                    int x = 0;
                    while (x < project_data.zones[i].maps[j].x)
                    {
                        free(s->knowledge[perso].zones[i].maps[j].cells[x]);
                        x++;
                    }
                    free(s->knowledge[perso].zones[i].maps[j].cells);
                    j++;
                }
                free(s->knowledge[perso].zones[i].maps);
            }
        }
        if (!valid)
            free(s->knowledge[perso].zones);
        if (valid)
            perso++;
    }
    while (perso && !valid)
    {
        perso--;
        int i = 0;
        while (i < project_data.parameters[11])
        {
            if (s->knowledge[perso].zones[i].maps)
            {
                int j = 0;
                while (j < project_data.zones[i].map_number)
                {
                    int x = 0;
                    while (x < project_data.zones[i].maps[j].x)
                    {
                        free(s->knowledge[perso].zones[i].maps[j].cells[x]);
                        x++;
                    }
                    free(s->knowledge[perso].zones[i].maps[j].cells);
                    j++;
                }
                free(s->knowledge[perso].zones[i].maps);
            }
            i++;
        }
        free(s->knowledge[perso].zones);
    }
    return valid;
}