#include "editor_load.h"

void project_load()
{
    char *title = NULL;
    char *author = NULL;
    char done = 0;
    while (!done && !inputs[9])
    {
        if (file_choice("levels/projects/*[*].txt", &title, &author))
        {
            project_data.author_name = author;
            project_data.project_name = title;
            if (open_project(&project_data))
            {
                project_menu();
                done = 1;
            }
        }
        else
            done = 1;
    }
}