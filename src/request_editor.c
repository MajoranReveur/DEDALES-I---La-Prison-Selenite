#include "request_editor.h"

char* request_types[18] = {
    "Parler",
    "Montrer un/des objets",
    "Donner un/des objets",
    "Fermer un/des Cauchemars",
    "Appaiser un/des Songes",
    "Ranger un/des Songes",
    "Completer un/des Piedestaux",
    "Valider un/des Piedestaux",
    "Convertir une/des Pensees",
    "Fermer un/des Delires",
    "Fermer une Fissure",
    "Detruire Nihil",
    "Creer Nihil",
    "Reveiller un Endormi",
    "Activer le courant",
    "Desactiver le courant",
    "Aller a un endroit sur la carte",
    "Trouver quelqu'un"
};

void print_request(struct request *r, int character)
{
    rect(0, 0, 704, 704, 0, 0, 0);
    if (is_in_map(r->objective.p.x, r->objective.p.y, r->objective.p.map, r->objective.p.zone))
        display_map_cells(r->objective.p.x * 8 - 40, r->objective.p.y * 8 - 40, project_data.zones[r->objective.p.zone - 1].maps[r->objective.p.map]);
    display_sprite(4, 320, 320, 64, character * 4 + r->objective.p.orientation, 0);
    rect(0, 0, 200, 120, 0, 0, 0);
    print_text(0, 0, "Mission :", 1, 1);
    print_text(0, 30, request_types[r->objective.type], 1, 1);
    print_text(0, 60, "Recompense :", 1, 1);
    print_text(0, 90, items_texts[r->reward.type], 1, 1);
}

void change_mission_type(struct mission *m)
{
    clean_inputs();
    while(!inputs[0] && !inputs[5] && !inputs[6])
    {
        int i = 0;
        int camera = 0;
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5] && !inputs[6])
        {
            int j = 0;
            rect(104, 92, 496, 436, 0, 0, 255);
            rect(108, 96, 488, 428, 0, 0, 0);
            print_text_centered(108, 104, "- Missions -", 1, 1, 488);
            while (j < 12)
            {
                if (camera + j < 18)
                    print_text_centered(108, 150 + j * 30, request_types[camera + j], 1, 1 + (camera + j == i), 488);
                if (camera + j == 18)
                    print_text_centered(108, 150 + j * 30, "Retour", 1, 1 + (camera + j == i), 488);
                j++;
            }
            print_refresh();
            load_input_long();
            if (inputs[1])
            {
                if (i)
                    i--;
                else
                    i = 18;
            }
            if (inputs[2])
            {
                if (i < 18)
                    i++;
                else
                    i = 0;
            }
            if (camera >= i)
            {
                if (i)
                    camera = i - 1;
                else
                    camera = 0;
            }
            if (camera + 10 < i)
            {
                if (i < 18)
                    camera = i - 10;
                else
                    camera = i - 11;
            }
        }
        if (inputs[5] && i < 18)
            m->type = i;
    }
}

void mission_editor(struct mission *m)
{
    clean_inputs();
    char end = 0;
    int i = 0;
    while (!inputs[0] && !inputs[6] && !inputs[10] && !end)
    {
        rect(104, 92, 496, 436, 0, 0, 255);
        rect(108, 96, 488, 428, 0, 0, 0);
        char options[10] = {0}; // 10 is way larger than needed, the maximum is theorically 6.
        options[0] = 1;
        //display_sprite(3, 320, 104, 64, item->type - 1, 0);
        print_text_centered(112, 104, "MISSION", 0, 1, 480);
        print_text_centered(112, 180, "Type :", 1, 1 + (i == 0), 480);
        print_text_centered(112, 205, request_types[m->type], 1, 6, 480);
        int j = 1; // Change type
        if (m->type == 1 || m->type == 2)
        {
            print_text_centered(112, 180 + j * 70, "Objet :", 1, 1 + (i == j), 480);
            if (m->value1 / 2)
                print_text_centered(112, 205 + j * 70, items_texts[m->value1 / 2], 1, 6, 480);
            else
                print_text_centered(112, 205 + j * 70, "Non defini", 1, 6, 480);
            options[j] = 2; // Change item
            j++;
        }
        if (m->type >= 1 && m->type <= 9)
        {
            print_text_centered(112, 180 + j * 70, "Quantite :", 1, 1 + (i == j), 480);
            print_int_centered(112, 205 + j * 70, m->value4, 1, 1, 6, 480);
            options[j] = 3; // Change quantity
            j++;
        }
        if (m->type >= 3 && m->type <= 9)
        {
            print_text_centered(112, 180 + j * 70, "Zone :", 1, 1 + (i == j), 480);
            print_text_centered(112, 205 + j * 70, "TODO", 1, 6, 480);
            options[j] = 4; // Change zone
            j++;
        }
        if (m->type == 13 || m->type == 17)
        {
            print_text_centered(112, 180 + j * 70, "Personnage :", 1, 1 + (i == j), 480);
            print_text_centered(112, 205 + j * 70, "TODO", 1, 6, 480);
            options[j] = 5; // Change character
            j++;
        }
        if (m->type == 14 || m->type == 15)
        {
            print_text_centered(112, 180 + j * 70, "Etage :", 1, 1 + (i == j), 480);
            print_text_centered(112, 205 + j * 70, "TODO", 1, 6, 480);
            options[j] = 6; // Change stage
            j++;
        }
        if (m->type == 10)
        {
            print_text_centered(112, 180 + j * 70, "FISSURE :", 1, 1 + (i == j), 480);
            print_text_centered(112, 205 + j * 70, "TODO", 1, 6, 480);
            options[j] = 7; // Change fissure
            j++;
        }
        print_text_centered(112, 180 + j * 70, "Retour", 1, 1 + (i == j), 480);
        j++;
        print_refresh();
        load_input_long();
        if (inputs[1])
            i = (i + j - 1) % j;
        if (inputs[2])
            i = (i + 1) % j;
        if (inputs[5])
        {
            if (options[i] == 1)
                change_mission_type(m);
            if (options[i] == 2)
            {
                struct item objet;
                objet.activation = m->value1 % 2;
                objet.type = m->value1 / 2;
                objet.ID = m->value2;
                objet.value = m->value3;
                modify_item(&objet, m->p, 2);
                m->value1 = objet.activation + objet.type * 2;
                m->value2 = objet.ID;
                m->value3 = objet.value;
            }
            if (options[i] == 3)
            {
                m->value4 = int_input(m->value4, 330, 300);
            }
            if (i == j - 1)
                inputs[6] = 1;
        }
    }
    clean_inputs();
}

void request_editor(struct request *r, int character)
{
    inputs[5] = 0;
    inputs[6] = 0;
    inputs[10] = 0;
    char end = 0;
    int i = 0;
    while (!inputs[0] && !inputs[6] && !inputs[10] && !end)
    {
        print_request(r, character);
        rect(704, 0, 400, 704, 0, 0, 0);
        print_text_centered(704, 150, " Requete ", 1, 1, 400);
        print_text_centered(704, 250, "Modifier la Mission", 1, 1 + (i == 0), 400);
        print_text_centered(704, 300, "Modifier la Position", 1, 1 + (i == 1), 400);
        print_text_centered(704, 350, "Modifier l'Orientation", 1, 1 + (i == 2), 400);
        print_text_centered(704, 400, "Modifier la Recompense", 1, 1 + (i == 3), 400);
        if (r->objective.activated)
            print_text_centered(704, 450, "Statut : Active", 1, 1 + (i == 4), 400);
        else
            print_text_centered(704, 450, "Statut : Inactive", 1, 1 + (i == 4), 400);
        print_text_centered(704, 500, "Retour", 1, 1 + (i == 5), 400);
        print_refresh();
        load_input_long();
        if (inputs[1])
            i = (i + 5) % 6;
        if (inputs[2])
            i = (i + 1) % 6;
        if (inputs[5])
        {
            if (i == 0)
                mission_editor(&(r->objective));
            if (i == 1)
                position_choice_zone(&(r->objective.p));
            if (i == 2)
            {
                clean_inputs();
                while (!inputs[0] && !inputs[5])
                {
                    print_request(r, character);
                    rect(704, 0, 400, 704, 0, 0, 0);
                    print_text_centered(704, 150, " Requete ", 1, 1, 400);
                    print_text_centered(704, 400, "Modification de l'Orientation", 1, 1, 400);
                    print_refresh();
                    load_input();
                    if (inputs[1])
                        r->objective.p.orientation = 1;
                    if (inputs[2])
                        r->objective.p.orientation = 0;
                    if (inputs[3])
                        r->objective.p.orientation = 2;
                    if (inputs[4])
                        r->objective.p.orientation = 3;
                }
                clean_inputs();
            }
            if (i == 3)
                modify_item(&(r->reward), r->objective.p, 2);
            if (i == 4)
                r->objective.activated = !(r->objective.activated);
            if (i == 5)
                inputs[6] = 1;
        }
    }
    clean_inputs();
}

void delete_request_character(int character)
{
    char* character_names[] = {"Nihil", "Sarah", "Mathias", "Saihtam", "Emma"};
    char quit = 0;
    while(!inputs[0] && !quit)
    {
        int request_number = project_data.parameters[character + 5];
        int i = 0;
        int camera = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5] && !inputs[6])
        {
            int j = 0;
            rect(0, 0, 1104, 704, 0, 0, 0);
            if (i < request_number)
                print_request(project_data.requests[character] + i, character);
            rect(704, 0, 400, 704, 0, 0, 0);
            print_text_centered(704, 100, character_names[character], 1, 1, 400);
            print_text_centered(704, 150, "- Suppression de Requetes -", 1, 1, 400);
            while (j < 16)
            {
                if (camera + j < request_number)
                    print_int_centered(704, 200 + j * 30, camera + j + 1, 2, 1, 1 + (camera + j == i), 400);
                if (camera + j == request_number)
                    print_text_centered(704, 200 + j * 30, "Retour", 1, 1 + (camera + j == i), 400);
                j++;
            }
            print_refresh();
            load_input_long();
            if (inputs[1])
            {
                if (i)
                    i--;
                else
                    i = request_number + 1;
            }
            if (inputs[2])
            {
                if (i < request_number + 1)
                    i++;
                else
                    i = 0;
            }
            if (camera >= i)
            {
                if (i)
                    camera = i - 1;
                else
                    camera = 0;
            }
            if (camera + 14 < i)
            {
                if (i < request_number + 1)
                    camera = i - 14;
                else
                    camera = i - 15;
            }
        }
        if (inputs[5])
        {
            if (i < request_number)
            {
                while (i + 1 < request_number)
                {
                    project_data.requests[character][i] = project_data.requests[character][i + 1];
                    i++;
                }
                if (request_number > 1)
                {
                    struct request *new_list = realloc(project_data.requests[character], sizeof(struct request) * (request_number - 1));
                    if (new_list == NULL)
                        print_error("Probleme de memoire...");
                    else
                    {
                        project_data.requests[character] = new_list;
                        project_data.parameters[character + 5]--;
                    }
                }
                else
                {
                    free(project_data.requests[character]);
                    project_data.requests[character] = NULL;
                    project_data.parameters[character + 5] = 0;
                }
            }
            if (i == request_number)
                quit = 1;
        }
        if (inputs[6])
            quit = 1;
    }
    clean_inputs();
}

void request_character_editor(int character)
{
    char* character_names[] = {"Nihil", "Sarah", "Mathias", "Saihtam", "Emma"};
    char quit = 0;
    while(!inputs[0] && !quit)
    {
        int request_number = project_data.parameters[character + 5];
        int i = 0;
        int camera = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5] && !inputs[6])
        {
            int j = 0;
            rect(0, 0, 1104, 704, 0, 0, 0);
            if (i < request_number)
                print_request(&(project_data.requests[character][i]), character);
            rect(704, 0, 400, 704, 0, 0, 0);
            print_text_centered(704, 100, character_names[character], 1, 1, 400);
            print_text_centered(704, 150, "- Requetes -", 1, 1, 400);
            while (j < 16)
            {
                if (camera + j < request_number)
                    print_int_centered(704, 200 + j * 30, camera + j + 1, 2, 1, 1 + (camera + j == i), 400);
                if (camera + j == request_number)
                    print_text_centered(704, 200 + j * 30, "Nouvelle Requete", 1, 1 + (camera + j == i), 400);
                if (camera + j == request_number + 1)
                    print_text_centered(704, 200 + j * 30, "Mode Suppression", 1, 1 + (camera + j == i), 400);
                if (camera + j == request_number + 2)
                    print_text_centered(704, 200 + j * 30, "Retour", 1, 1 + (camera + j == i), 400);
                j++;
            }
            print_refresh();
            load_input_long();
            if (inputs[1])
            {
                if (i)
                    i--;
                else
                    i = request_number + 2;
            }
            if (inputs[2])
            {
                if (i < request_number + 2)
                    i++;
                else
                    i = 0;
            }
            if (camera >= i)
            {
                if (i)
                    camera = i - 1;
                else
                    camera = 0;
            }
            if (camera + 14 < i)
            {
                if (i < request_number + 2)
                    camera = i - 14;
                else
                    camera = i - 15;
            }
        }
        if (inputs[5])
        {
            if (i < request_number)
                request_editor(project_data.requests[character] + i, character);
            if (i == request_number)
            {
                if (i < 99)
                {
                    print_error("alloc");
                    struct request *new_list = realloc(project_data.requests[character], sizeof(struct request) * (request_number + 1));
                    print_error("alloc done");
                    if (new_list == NULL)
                        print_error("Pas assez de memoire...");
                    else
                    {
                        project_data.requests[character] = new_list;
                        project_data.parameters[character + 5]++;
                        project_data.requests[character][i].reward.activation = 0;
                        project_data.requests[character][i].reward.ID = 0;
                        project_data.requests[character][i].reward.type = 0;
                        project_data.requests[character][i].reward.value = 0;
                        project_data.requests[character][i].objective.type = 0;
                        project_data.requests[character][i].objective.activated = 0;
                        project_data.requests[character][i].objective.p.map = 0;
                        project_data.requests[character][i].objective.p.orientation = 0;
                        project_data.requests[character][i].objective.p.x = 0;
                        project_data.requests[character][i].objective.p.y = 0;
                        project_data.requests[character][i].objective.p.zone = 0;
                        project_data.requests[character][i].objective.tp.map = 0;
                        project_data.requests[character][i].objective.tp.orientation = 0;
                        project_data.requests[character][i].objective.tp.x = 0;
                        project_data.requests[character][i].objective.tp.y = 0;
                        project_data.requests[character][i].objective.tp.zone = 0;
                        project_data.requests[character][i].objective.value1 = 0;
                        project_data.requests[character][i].objective.value2 = 0;
                        project_data.requests[character][i].objective.value3 = 0;
                        project_data.requests[character][i].objective.value4 = 0;
                        print_error("init done");
                        request_editor(&(project_data.requests[character][i]), character);
                    }
                }
                else
                    print_error("Limite de Requêtes atteinte !");
            }
            if (i == request_number + 2)
                quit = 1;
            if (i == request_number + 1)
                delete_request_character(character);
        }
        if (inputs[6])
            quit = 1;
    }
    clean_inputs();
}