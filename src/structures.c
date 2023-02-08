#include "structures.h"

void free_container(struct container c)
{
    free(c.items);
}

void free_map(struct map r)
{
    int i = 0;
    while (i < r.x)
    {
        free(r.cells[i]);
        free(r.items[i]);
        i++;
    }
    free(r.cells);
    free(r.items);
}

void free_project(struct project p)
{
    int i = 0;
    while (i < 5)
    {
        free(p.requests[i]);
        int j = 0;
        while (j < p.parameters[i + 5])
        {
            free_map(p.maps[i][j]);
            j++;
        }
        free(p.maps[i]);
        i++;
    }
    i = 0;
    while (i < p.parameters[15])
    {
        free_container(p.containers[i]);
        i++;
    }
    free(p.containers);
    file_loaded = 0;
}

void delete_container(struct project p, long ID)
{
    free(p.containers[ID].items);
    p.containers[ID] = p.containers[p.parameters[15] - 1];
    p.parameters[15]--;
    struct position last_container = p.containers[ID].position;
    if (last_container.zone == 0) // Inventory
        p.inventories[last_container.map][last_container.x].ID = ID;
    else
        p.maps[last_container.zone - 1][last_container.map].items[last_container.x][last_container.y].ID = ID;
    if (p.parameters[15])
    {
        struct container *new_list = realloc(p.containers, sizeof(struct container) * p.parameters[15]);
        if (new_list)
            p.containers = new_list; // In case of fail, the memory just won't be free
    }
    else
    {
        free(p.containers);
        p.containers = NULL;
    }
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