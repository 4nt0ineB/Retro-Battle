
#include <stdio.h>
#include "./game_obj/entities/entity_type.h"
#include "./game_obj/entities/enemy.h"
#include "graphics/cli.h"
#include "game_master.h"
#include "other/game_utils.h"

int main(int argc, char *argv[]) {
    // lecture des options
    char ** options = NULL;
    options = read_options(argc, argv);
    if(strlen(options[PATH]) == 0){
        printf("No given level file\n");
        free_string_table(options, MAX_OPTIONS);
        return 0;
    }
    if(*options[HELP] == HELP){
        CLI_display_help();
    }

    // Lectures du niveau
    int money = 0;
    char * nom_fichier = NULL;
    Enemy * waiting_enemies = lire_fichier_niveau(options[PATH], &money);

    // ---------- Configuration des ennemis.
    // Lecture des types d'ennemis //
    nom_fichier = NTT_DEF_PATH "enemy_types";
    DListe types = lire_fichier_types(nom_fichier);
    // association des effets aux types
    nom_fichier = NTT_DEF_PATH "effects_by_enemy_types";
    lire_fichier_effets(nom_fichier, types);
    // association des caractéristiques des ennemis par type
    init_enemies(waiting_enemies, types);

    // ---------- Configuration des tourelles.
    // Lecture des types de tourelles //
    nom_fichier = NTT_DEF_PATH "tower_types";
    DListe t_types = lire_fichier_types(nom_fichier);
    // association des effets aux types
    nom_fichier = NTT_DEF_PATH "effects_by_tower_types";
    lire_fichier_effets(nom_fichier, t_types);

    // jeu
    if(*options[CLI] == CLI)
        gm_level_cli(options, &waiting_enemies, types, t_types, money);
    else if(*options[GUI] == GUI)
        gm_level_gui(options, &waiting_enemies, types, t_types, money);

    // free //
    free_string_table(options, MAX_OPTIONS);
    enemy_free_all(&waiting_enemies);
    entity_type_dliste_free(&types);
    entity_type_dliste_free(&t_types);
    printf("\n");
    return 0;
}
//enemy->prev_line = enemy->next_line = NULL;