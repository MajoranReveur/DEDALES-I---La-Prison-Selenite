#include "editor_create.h"

void project_create()
{
    char name[101];
    string_input(name, 100, "Quel est votre pseudo ?");
    char project[301];
    string_input(project, 300, "Quel nom donner au projet ?");
    print_error("A !");
    //Check if the project already exists
    char project_file_name[451];
    const char* file_name_parts[5] = {"levels/projects/", project, "[", name, "].txt"};
    concat_str(project_file_name, file_name_parts, 450, 5);
    print_error("B !");
	FILE* file = fopen(project_file_name, "r");
    if (file != NULL)
    {
        fclose(file);
        rect(0,0,1104,704,0,0,0);
        print_text_centered(0, 300, "Un projet du meme nom et du meme auteur existe deja.", 1, 2, 1104);
        print_refresh();
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5])
            load_input();
    }
    else
    {
        print_error("C !");
        int i = 0;
        project_data.zones = malloc(sizeof(struct zone) * 5);
        if (!project_data.zones)
        {
            rect(0,0,1104,704,0,0,0);
            print_error("Erreur de memoire !");
            return;
        }
        print_error("1");
        while (i < 11)
        {
            project_data.parameters[i] = 0;
            i++;
        }
        project_data.parameters[11] = 5;
        i = 0;
        print_error("2");
        while (i < 5)
        {
            project_data.character_positions[i].zone = 0;
            project_data.character_positions[i].map = 0;
            project_data.character_positions[i].x = 0;
            project_data.character_positions[i].y = 0;
            project_data.requests[i] = NULL;
            project_data.zones[i].maps = NULL;
            project_data.zones[i].map_number = 0;
            int j = 0;
            while (j < 40)
            {
                project_data.inventories[i][j].activation = 0;
                project_data.inventories[i][j].ID = 0;
                project_data.inventories[i][j].type = 0;
                project_data.inventories[i][j].value = 0;
                j++;
            }
            i++;
        }
        print_error("3");
        project_data.author_name = name;
        project_data.project_name = project;
        project_data.containers = NULL;
        project_data.modified = 1;
        project_data.valid = 0;
        file_loaded = 1;
        print_error("D !");
        project_menu();
    }
}