#include <stdio.h>
#include <time.h>
/*
#include <MLV/MLV_all.h>
*/

#include "../headers/entity_type.h"
#include "../headers/enemy.h"
#include "../headers/cli.h"
#include "../headers/game_master.h"
#include "../headers/game_utils.h"


int main(int argc, char *argv[]) {

    char ** options = NULL;
    options = read_options(argc, argv);
    if(strlen(options[PATH]) == 0){
        printf("No given level file\n");
        free_string_table(options, MAX_OPTIONS);
        return 0;
    }
    //printf("CHEMIN = %s\n", options[PATH]);
    // test fichier niveau //
    //printf("\n[#########] ---------- Lectures d'un niveau ---------- [#########]\n");
    int money = 0;
    //char * nom_fichier = "./data/level1";
    char * nom_fichier = NULL;
    Enemy * waiting_enemies = lire_fichier_niveau(options[PATH], &money);

    //printf("\n[#########] ---------- Configuration des ennemis ---------- [#########]\n");
    // test lecture type enemy //
    nom_fichier = "./data/enemy_types";
    //printf("\n lecture fichier types ennemis \n");
    DListe types = lire_fichier_types(nom_fichier);

    // association des effets aux types
    nom_fichier = "./data/effects_by_enemy_types";
    lire_fichier_effets(nom_fichier, types);

    // association des caractéristiques des ennemis par type //
    //printf("\n initialisation des ennemis d'après leur type\n");
    init_enemies(waiting_enemies, types);
    //printf("\n[#########] ---------- Configuration des tourelles ---------- [#########]\n");
    // test lecture type tower //
    nom_fichier = "./data/tower_types";
    //printf("\n Lecture fichier des types de tourelles: \n");
    DListe t_types = lire_fichier_types(nom_fichier);

    // association des effets aux types
    nom_fichier = "./data/effects_by_tower_types";
    //printf("\n association des effets aux type de tourelles: \n");
    lire_fichier_effets(nom_fichier, t_types);

    if(*options[CLI] == CLI)
        gm_level_cli(&waiting_enemies, types, t_types, money);
    /*else if(*options[GUI] == GUI)
        gm_level_gui(&waiting_enemies, types, t_types, money);
*/

    // free //
    printf("\n");
    free_string_table(options, MAX_OPTIONS);
    enemy_free_all(&waiting_enemies);
    entity_type_dliste_free(&types);
    entity_type_dliste_free(&t_types);
    printf("\n");
    return 0;
}
//enemy->prev_line = enemy->next_line = NULL;