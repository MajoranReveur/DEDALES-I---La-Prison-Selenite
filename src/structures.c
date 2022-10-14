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
}