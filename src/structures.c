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
        i++;
    }
    free(r.cells);
    free(r.items);
    free(r.thoughts);
    free(r.color_sequency);
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

void add_item(struct container container, int value, int type)
{
    int j = 0;
    while (container.items[j].value < value && container.items[j].value)
        j++;
    int k = container.size - 1;
    while (k > j)
    {
        k--;
        container.items[k + 1].type = container.items[k].type;
        container.items[k + 1].value = container.items[k].value;
    }
    container.items[j].type = type;
    container.items[j].value = value;
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