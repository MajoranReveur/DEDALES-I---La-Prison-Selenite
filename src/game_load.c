#include "game_load.h"

void game_load()
{
    char *title = NULL;
    char *author = NULL;
    char done = 0;
    while (!done && !inputs[0])
    {
        if (file_choice("levels/projects/*[*].txt", &title, &author))
        {
            project_data.author_name = author;
            project_data.project_name = title;
            if (open_project(&project_data))
            {
                launch_game(0, 0);
                done = 1;
                free_project(project_data);
            }
        }
        else
            done = 1;
    }
}