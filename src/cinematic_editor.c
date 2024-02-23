#include "cinematic_editor.h"

char* event_types[19] = {
    "Dialogue", //0 : Personnage, Phrase, Visage
    "Deplacement de Personnage", //1 : Personnage, Direction, Distance, Marche/glisse
    "Teleportation de Personnage", //2 : Personnage, Position
    "Deplacement d'Objet", //3 : Position de depart, direction, distance
    "Teleportation d'Objet", //4 : Position de depart, Position d'arrivee
    "Activation de Requete", //5 : Personnage, Requete
    "Desactivation de Requete", //6 : Personnage, Requete
    "Activation de Cinematique", //7 : Cinematique
    "Desactivation de Cinematique", //8 : Cinematique
    "Ramasser un Objet", //9 : Personnage
    "Laisser un Objet", //10 : Personnage, Objet
    "Echange d'Objet", //11 : Donneur, Receveur, Objet 
    "Deplacement de Camera", //12 : Position
    "Camera sur Personnage", //13 : Personnage
    "Changement de Joueur", //14 : Personnage
    "Activation de Courant", //15 : Zone, Etage
    "Désactivation de Courant", //16 : Zone, Etage
    "Lancement de Cinematique", //17 : Cinematique
    "Fin de Partie" //18
};

char* trigger_types[13] = {
    "Debut du jeu", //0
    "Passer sur une case", //1
    "S'arreter sur une case", //2
    "Resolution de Cle", //3
    "Resolution de Songe", //4
    "Resolution de Cauchemar", //5
    "Resolution de Delire", //6
    "Resolution de Piedestal", //7
    "Restauration de Piedestal", //8
    "Annihilation de Sceau", //9
    "Resolution d'Objet", //10
    "Completion de Requete", //11
    "Parler a un personnage", //12
};

char* character_names[6] = {
    "Personne",
    "Nihil",
    "Sarah",
    "Mathias",
    "Saihtam",
    "Emma"
};

char* directions_names[4] = {
    "Haut",
    "Droite",
    "Bas",
    "Gauche"
};

void change_character(int *character)
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
            print_text_centered(108, 104, "- Personnages -", 1, 1, 488);
            while (j < 12)
            {
                if (camera + j < 6)
                    print_text_centered(108, 150 + j * 30, character_names[camera + j], 1, 1 + (camera + j == i), 488);
                if (camera + j == 6)
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
                    i = 6;
            }
            if (inputs[2])
            {
                if (i < 6)
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
                if (i < 6)
                    camera = i - 10;
                else
                    camera = i - 11;
            }
        }
        if (inputs[5] && i < 6)
            *character = i;
    }
    clean_inputs();
}

void change_event_type(struct cinematic_event *c)
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
            print_text_centered(108, 104, "- Events -", 1, 1, 488);
            while (j < 12)
            {
                if (camera + j < 19)
                    print_text_centered(108, 150 + j * 30, event_types[camera + j], 1, 1 + (camera + j == i), 488);
                if (camera + j == 19)
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
                    i = 19;
            }
            if (inputs[2])
            {
                if (i < 19)
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
                if (i < 19)
                    camera = i - 10;
                else
                    camera = i - 11;
            }
        }
        if (inputs[5] && i < 19)
            c->type = i;
    }
    clean_inputs();
}

void change_trigger_type(struct cinematic_trigger *c)
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
            print_text_centered(108, 104, "- Triggers -", 1, 1, 488);
            while (j < 12)
            {
                if (camera + j < 13)
                    print_text_centered(108, 150 + j * 30, trigger_types[camera + j], 1, 1 + (camera + j == i), 488);
                if (camera + j == 13)
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
                    i = 13;
            }
            if (inputs[2])
            {
                if (i < 13)
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
                if (i < 13)
                    camera = i - 10;
                else
                    camera = i - 11;
            }
        }
        if (inputs[5] && i < 11)
            c->type = i;
    }
    clean_inputs();
}

void event_editor(struct cinematic_event *c)
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
        print_text_centered(112, 104, "EVENT", 0, 1, 480);
        print_text_centered(112, 180, "Type :", 1, 1 + (i == 0), 480);
        print_text_centered(112, 205, event_types[c->type], 1, 6, 480);
        int j = 1; // Change type
        if (c->type <= 2 || c->type == 5 || c-> type == 6 || c-> type == 9 || c-> type == 10 || c-> type == 13 || c-> type == 14)
        {
            print_text_centered(112, 180 + j * 70, "Personnage :", 1, 1 + (i == j), 480);
            print_text_centered(112, 205 + j * 70, character_names[c->value1], 1, 6, 480);
            options[j] = 2; // Change character
            j++;
        }
        if (c->type == 11)
        {
            print_text_centered(112, 180 + j * 70, "Donneur :", 1, 1 + (i == j), 480);
            print_text_centered(112, 205 + j * 70, character_names[c->value1], 1, 6, 480);
            options[j] = 2; // Change character
            j++;
        }
        if (c->type == 11)
        {
            print_text_centered(112, 180 + j * 70, "Receveur :", 1, 1 + (i == j), 480);
            print_text_centered(112, 205 + j * 70, character_names[c->value2], 1, 6, 480);
            options[j] = 3; // Change receiver
            j++;
        }
        if (c->type == 0)
        {
            print_text_centered(112, 180 + j * 70, "Texte :", 1, 1 + (i == j), 480);
            options[j] = 4; // Change phrase
            j++;
        }
        if (c->type == 0)
        {
            //TODO
            print_text_centered(112, 180 + j * 70, "Visage :", 1, 1 + (i == j), 480);
            print_int_centered(112, 205 + j * 70, c->value2, 0, 1, 6, 480);
            options[j] = 5; // Change face
            j++;
        }
        if (c->type == 2 || c->type == 3 || c->type == 12)
        {
            //TODO
            print_text_centered(112, 180 + j * 70, "Position", 1, 1 + (i == j), 480);
            options[j] = 6; // Change position
            j++;
        }
        if (c->type == 4)
        {
            //TODO
            print_text_centered(112, 180 + j * 70, "Position de Depart", 1, 1 + (i == j), 480);
            options[j] = 6; // Change position
            j++;
        }
        if (c->type == 4)
        {
            //TODO
            print_text_centered(112, 180 + j * 70, "Position d'Arrivee", 1, 1 + (i == j), 480);
            options[j] = 7; // Change position 2
            j++;
        }
        if (c->type == 1 || c->type == 3)
        {
            //TODO
            print_text_centered(112, 180 + j * 70, "Direction :", 1, 1 + (i == j), 480);
            print_text_centered(112, 205 + j * 70, directions_names[c->value2], 1, 6, 480);
            options[j] = 8; // Change direction
            j++;
        }
        if (c->type == 1 || c->type == 3)
        {
            //TODO
            print_text_centered(112, 180 + j * 70, "Distance :", 1, 1 + (i == j), 480);
            print_int_centered(112, 205 + j * 70, c->value4, 0, 1, 6, 480);
            options[j] = 9; // Change distance
            j++;
        }
        /*
        if (c->type == 1)
        {
            print_text_centered(112, 180 + j * 70, "Mouvement :", 1, 1 + (i == j), 480);
            if (c->p.x)
                print_text_centered(112, 205 + j * 70, "Glisse", 1, 6, 480);
            else
                print_text_centered(112, 205 + j * 70, "Marche", 1, 6, 480);
            options[j] = 10; // Change walking
            j++;
        }*/ // Not now
        if (c->type == 5 || c->type == 6)
        {
            //TODO
            print_text_centered(112, 180 + j * 70, "Requete :", 1, 1 + (i == j), 480);
            print_int_centered(112, 205 + j * 70, c->value2, 0, 1, 6, 480);
            options[j] = 11; // Change request
            j++;
        }
        if (c->type == 7 || c->type == 8 || c->type == 17)
        {
            //TODO
            print_text_centered(112, 180 + j * 70, "Cinematique :", 1, 1 + (i == j), 480);
            print_int_centered(112, 205 + j * 70, c->value2, 0, 1, 6, 480);
            options[j] = 12; // Change cinematic
            j++;
        }
        if (c->type == 10 || c->type == 11)
        {
            //TODO
            print_text_centered(112, 180 + j * 70, "Objet :", 1, 1 + (i == j), 480);
            print_text_centered(112, 205 + j * 70, items_texts[c->p.zone], 1, 6, 480);
            options[j] = 13; // Change item
            j++;
        }
        if (c->type == 15 || c->type == 16)
        {
            //TODO
            print_text_centered(112, 180 + j * 70, "Zone :", 1, 1 + (i == j), 480);
            print_int_centered(112, 205 + j * 70, c->p.zone, 0, 1, 6, 480);
            options[j] = 14; // Change zone
            j++;
        }
        if (c->type == 15 || c->type == 16)
        {
            //TODO
            print_text_centered(112, 180 + j * 70, "Etage :", 1, 1 + (i == j), 480);
            print_int_centered(112, 205 + j * 70, c->p.map, 0, 1, 6, 480);
            options[j] = 15; // Change level
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
                change_event_type(c);
            if (options[i] == 2)
                change_character(&(c->value1));
            if (options[i] == 3)
                change_character(&(c->value2));
            if (options[i] == 4)
                text_input(&(c->dialog), "Dialogue de cinematique");
            /*
            if (options[i] == 3)
            {
                m->value4 = int_input(m->value4, 330, 300);
            }*/
            if (i == j - 1)
                inputs[6] = 1;
        }
    }
    clean_inputs();
}

void trigger_editor(struct cinematic_trigger *c)
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
        print_text_centered(112, 104, "TRIGGER", 0, 1, 480);
        print_text_centered(112, 180, "Type :", 1, 1 + (i == 0), 480);
        print_text_centered(112, 205, trigger_types[c->type], 1, 6, 480);
        int j = 1; // Change type
        if (c->type == 1 || c->type == 2 || c-> type == 10)
        {
            print_text_centered(112, 180 + j * 70, "Position :", 1, 1 + (i == j), 480);
            options[j] = 2; // Change position
            j++;
        }
        if (c->type >= 3 && c->type <= 9)
        {
            //TODO
            print_text_centered(112, 180 + j * 70, "Valeur :", 1, 1 + (i == j), 480);
            options[j] = 3; // Change value
            j++;
        }
        if (c->type == 11 || c->type == 12)
        {
            print_text_centered(112, 180 + j * 70, "Personnage :", 1, 1 + (i == j), 480);
            print_text_centered(112, 205 + j * 70, character_names[c->value], 1, 6, 480);
            options[j] = 4; // Change character
            j++;
        }
        if (c->type == 11)
        {
            //TODO
            print_text_centered(112, 180 + j * 70, "Requete :", 1, 1 + (i == j), 480);
            options[j] = 5; // Change request ID
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
                change_trigger_type(c);
            if (options[i] == 2)
                position_choice_all_zone(&(c->p));
            if (options[i] == 4)
                change_character(&(c->value));
            if (i == j - 1)
                inputs[6] = 1;
        }
    }
    clean_inputs();
}

void event_editor_menu(struct cinematic *c)
{
    char quit = 0;
    while(!inputs[0] && !quit)
    {
        int i = 0;
        int camera = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5] && !inputs[6])
        {
            int j = 0;
            rect(0, 0, 1104, 704, 0, 0, 0);
            /*
            if (i < project_data.parameters[12])
                print_request(&(project_data.requests[character][i]), character);*/
            rect(704, 0, 400, 704, 0, 0, 0);
            print_text_centered(704, 150, " Cinematique ", 1, 1, 400);
            while (j < 16)
            {
                if (camera + j < c->length)
                    print_int_centered(704, 200 + j * 30, camera + j + 1, 2, 1, 1 + (camera + j == i), 400);
                if (camera + j == c->length)
                    print_text_centered(704, 200 + j * 30, "Nouvel Event", 1, 1 + (camera + j == i), 400);
                if (camera + j == c->length + 1)
                    print_text_centered(704, 200 + j * 30, "Mode Suppression", 1, 1 + (camera + j == i), 400);
                if (camera + j == c->length + 2)
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
                    i = c->length + 2;
            }
            if (inputs[2])
            {
                if (i < c->length + 2)
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
                if (i < c->length + 2)
                    camera = i - 14;
                else
                    camera = i - 15;
            }
        }
        if (inputs[5])
        {
            if (i < c->length)
                event_editor(c->events + i);
            if (i == c->length)
            {
                if (i < 99)
                {
                    print_error("alloc");
                    struct cinematic_event *new_list = realloc(c->events, sizeof(struct cinematic_event) * (c->length + 1));
                    print_error("alloc done");
                    if (new_list == NULL)
                        print_error("Pas assez de memoire...");
                    else
                    {
                        c->events = new_list;
                        c->length++;
                        c->events[i].type = 0;
                        c->events[i].value1 = 0;
                        c->events[i].value2 = 0;
                        c->events[i].value3 = 0;
                        c->events[i].value4 = 0;
                        c->events[i].p.x = 0;
                        c->events[i].p.y = 0;
                        c->events[i].p.orientation = 0;
                        c->events[i].p.map = 0;
                        c->events[i].p.zone = 0;
                        c->events[i].target.x = 0;
                        c->events[i].target.y = 0;
                        c->events[i].target.orientation = 0;
                        c->events[i].target.map = 0;
                        c->events[i].target.zone = 0;
                        c->events[i].dialog.length = 0;
                        c->events[i].dialog.string = NULL;
                        print_error("init done");
                        event_editor(c->events + i);
                    }
                }
                else
                    print_error("Limite d'Events atteinte !");
            }
            if (i == c->length + 2)
                quit = 1;
            /*if (i == c->length + 1)
                delete_cinematic();*/
        }
        if (inputs[6])
            quit = 1;
    }
    clean_inputs();
}

void cinematic_editor(struct cinematic *c)
{
    inputs[5] = 0;
    inputs[6] = 0;
    inputs[10] = 0;
    char end = 0;
    int i = 0;
    while (!inputs[0] && !inputs[6] && !inputs[10] && !end)
    {
        //print_request(r, character);
        rect(704, 0, 400, 704, 0, 0, 0);
        print_text_centered(704, 150, " Cinematique ", 1, 1, 400);
        print_text_centered(704, 250, "Modifier les Events", 1, 1 + (i == 0), 400);
        print_text_centered(704, 300, "Modifier le Trigger", 1, 1 + (i == 1), 400);
        if (c->trigger.active)
            print_text_centered(704, 450, "Statut : Active", 1, 1 + (i == 2), 400);
        else
            print_text_centered(704, 450, "Statut : Inactive", 1, 1 + (i == 2), 400);
        print_text_centered(704, 500, "Retour", 1, 1 + (i == 3), 400);
        print_refresh();
        load_input_long();
        if (inputs[1])
            i = (i + 3) % 4;
        if (inputs[2])
            i = (i + 1) % 4;
        if (inputs[5])
        {
            if (i == 0)
                event_editor_menu(c);
            if (i == 1)
                trigger_editor(&(c->trigger));
            if (i == 2)
                c->trigger.active = !(c->trigger.active);
            if (i == 3)
                inputs[6] = 1;
        }
    }
    clean_inputs();
}

void delete_cinematic()
{
    char quit = 0;
    while(!inputs[0] && !quit)
    {
        int i = 0;
        int camera = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5] && !inputs[6])
        {
            int j = 0;
            rect(0, 0, 1104, 704, 0, 0, 0);
            /*if (i < project_data.parameters[12])
                print_request(project_data.requests[character] + i, character);*/
            rect(704, 0, 400, 704, 0, 0, 0);
            print_text_centered(704, 150, "- Suppression de Cinematiques -", 1, 1, 400);
            while (j < 16)
            {
                if (camera + j < project_data.parameters[12])
                    print_int_centered(704, 200 + j * 30, camera + j + 1, 2, 1, 1 + (camera + j == i), 400);
                if (camera + j == project_data.parameters[12])
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
                    i = project_data.parameters[12];
            }
            if (inputs[2])
            {
                if (i < project_data.parameters[12])
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
                if (i < project_data.parameters[12] + 1)
                    camera = i - 14;
                else
                    camera = i - 15;
            }
        }
        if (inputs[5])
        {
            if (i == project_data.parameters[12])
                quit = 1;
            if (i < project_data.parameters[12])
            {
                while (i + 1 < project_data.parameters[12])
                {
                    project_data.cinematics[i] = project_data.cinematics[i + 1];
                    i++;
                }
                if (project_data.parameters[12] > 1)
                {
                    free_cinematic(project_data.cinematics + i);
                    struct cinematic *new_list = realloc(project_data.cinematics, sizeof(struct cinematic) * (project_data.parameters[12] - 1));
                    if (new_list == NULL)
                        print_error("Probleme de memoire...");
                    else
                    {
                        project_data.cinematics = new_list;
                        project_data.parameters[12]--;
                    }
                }
                else
                {
                    free(project_data.cinematics);
                    project_data.cinematics = NULL;
                    project_data.parameters[12] = 0;
                }
            }
        }
        if (inputs[6])
            quit = 1;
    }
    clean_inputs();
}

void cinematic_editor_menu()
{
    char quit = 0;
    while(!inputs[0] && !quit)
    {
        int i = 0;
        int camera = 0;
        rect(0, 0, 1104, 704, 0, 0, 0);
        inputs[5] = 0;
        while (!inputs[0] && !inputs[5] && !inputs[6])
        {
            int j = 0;
            rect(0, 0, 1104, 704, 0, 0, 0);
            /*
            if (i < project_data.parameters[12])
                print_request(&(project_data.requests[character][i]), character);*/
            rect(704, 0, 400, 704, 0, 0, 0);
            print_text_centered(704, 150, "- Cinematiques -", 1, 1, 400);
            while (j < 16)
            {
                if (camera + j < project_data.parameters[12])
                    print_int_centered(704, 200 + j * 30, camera + j + 1, 2, 1, 1 + (camera + j == i), 400);
                if (camera + j == project_data.parameters[12])
                    print_text_centered(704, 200 + j * 30, "Nouvelle Cinematique", 1, 1 + (camera + j == i), 400);
                if (camera + j == project_data.parameters[12] + 1)
                    print_text_centered(704, 200 + j * 30, "Mode Suppression", 1, 1 + (camera + j == i), 400);
                if (camera + j == project_data.parameters[12] + 2)
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
                    i = project_data.parameters[12] + 2;
            }
            if (inputs[2])
            {
                if (i < project_data.parameters[12] + 2)
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
                if (i < project_data.parameters[12] + 2)
                    camera = i - 14;
                else
                    camera = i - 15;
            }
        }
        if (inputs[5])
        {
            if (i < project_data.parameters[12])
                cinematic_editor(project_data.cinematics + i);
            if (i == project_data.parameters[12])
            {
                if (i < 99)
                {
                    print_error("alloc");
                    struct cinematic *new_list = realloc(project_data.cinematics, sizeof(struct cinematic) * (project_data.parameters[12] + 1));
                    print_error("alloc done");
                    if (new_list == NULL)
                        print_error("Pas assez de memoire...");
                    else
                    {
                        project_data.cinematics = new_list;
                        project_data.parameters[12]++;
                        project_data.cinematics[i].length = 0;
                        project_data.cinematics[i].events = NULL;
                        project_data.cinematics[i].trigger.active = 0;
                        project_data.cinematics[i].trigger.ID = 0;
                        project_data.cinematics[i].trigger.type = 0;
                        project_data.cinematics[i].trigger.value = 0;
                        project_data.cinematics[i].trigger.p.x = 0;
                        project_data.cinematics[i].trigger.p.y = 0;
                        project_data.cinematics[i].trigger.p.orientation = 0;
                        project_data.cinematics[i].trigger.p.map = 0;
                        project_data.cinematics[i].trigger.p.zone = 0;
                        print_error("init done");
                        cinematic_editor(project_data.cinematics + i);
                    }
                }
                else
                    print_error("Limite de Cinematiques atteinte !");
            }
            if (i == project_data.parameters[12] + 2)
                quit = 1;
            if (i == project_data.parameters[12] + 1)
                delete_cinematic();
        }
        if (inputs[6])
            quit = 1;
    }
    clean_inputs();
}