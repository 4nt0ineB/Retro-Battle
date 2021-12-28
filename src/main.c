#include <stdio.h>

#include "../headers/entity_type.h"
#include "../headers/enemy.h"
#include "../headers/cli.h"
#include "../headers/settings.h"
#include "../headers/entity_type.h"
#include "../headers/game.h"
#include "../headers/effect.h"
#include "../headers/DListe.h"
#include "../headers/game_master.h"


int main() {


    // test fichier niveau //
    printf("\n[#########] ---------- Lectures d'un niveau ---------- [#########]\n");
    int money = 0;
    char * nom_fichier = "./data/level1";
    Enemy * waiting_enemies = lire_fichier_niveau(nom_fichier, &money);

    printf("\n[#########] ---------- Configuration des ennemis ---------- [#########]\n");
    // test lecture type enemy //
    nom_fichier = "./data/enemy_types";
    printf("\n lecture fichier types ennemis \n");
    DListe types = lire_fichier_types(nom_fichier);

    // association des effets aux types
    nom_fichier = "./data/effects_by_enemy_types";
    lire_fichier_effets(nom_fichier, types);

    // association des caractéristiques des ennemis par type //
    printf("\n initialisation des ennemis d'après leur type\n");
    init_enemies(waiting_enemies, types);

    printf("\n[#########] ---------- Configuration des tourelles ---------- [#########]\n");
    // test lecture type tower //
    nom_fichier = "./data/tower_types";
    printf("\n Lecture fichier des types de tourelles: \n");
    DListe t_types = lire_fichier_types(nom_fichier);

    // association des effets aux types
    nom_fichier = "./data/effects_by_tower_types";
    printf("\n association des effets aux type de tourelles: \n");
    lire_fichier_effets(nom_fichier, t_types);

    printf("\n[#########] ---------- Lancement du jeu ---------- [#########]\n");
    // Game
    // à déclarer dans play() dans game_master.c
    Game game = {NULL, NULL, money};
    printf("AJOUT TOURELLE: \n");
    Tower * towers = NULL;
    Tower * t1 = alloue_tower('X', 0, 2, 1, 0);
    tower_add(&towers,t1);
    game_add_entity(&game, &towers, TOWER);
    gm_add_entities(&game, &towers, TOWER, 0);

    // affichage du jeu
    char choice = '.';
    int turn = 1; /*  @todo créer une classe game_state pour le tour, les points, les stats, etc...  */

    LEVEL_MENU_ACTION act = 0;
    while((act = CLI_scan_choice_level_menu())!= START_LEVEL){
        switch (act) {
            case SHOW_WAVE:
                CLI_show_wave(waiting_enemies);
                break;
            case BUILD_DEFENSE:
                break;
            case ENEMIES_INFO:
                CLI_menu_entities_types(types);
                break;
            case TOWERS_INFO:
                break;
            default:
                continue;
        }
        act = 0;
    }

    while((1)){
        CLI_display_game(game);
        CLI_menu_entities_types(t_types);
        scanf(" %c", &choice);
        if(choice == 'n') {
            gm_move_all(&game);
            gm_add_entities(&game, &waiting_enemies, ENEMY, turn);
            turn += 1;
        }else if(choice == 'q'){
            break;
        }
    }

    // free //
    printf("\n");
    enemy_free_all(&waiting_enemies);
    enemy_free_all(&game.enemies);
    entity_type_dliste_free(&types);
    entity_type_dliste_free(&t_types);
    //free(extr);
    printf("\n");
    return 0;
}
//enemy->prev_line = enemy->next_line = NULL;