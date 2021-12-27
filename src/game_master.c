//
// Created by antoine on 26/12/2021.
//

#include "../headers/game_master.h"


int gm_move(Game * game, void * entity, ENTITY ntt){
    if(ntt == ENEMY){
        // déplacement de "speed" case vers la gauche pour l'ennemi
        return game_move_entity(game, entity, ntt, ((Enemy *)entity)->line, ((Enemy *)entity)->position - ((Enemy *)entity)->speed);
    }
    return 0;
}

int gm_move_all(Game * game){
    // mettre à jour la position des ennemis
    Enemy * tmp = game->enemies;
    ENTITY ntt = ENEMY;
   // CLI_display_full_enemy(*tmp);

    while(tmp){
        gm_move(game, tmp, ntt);
        tmp = tmp->next;
    }
    return 1;
}

int gm_add_entities(Game * game, Enemy ** entities, int turn){
    Enemy ** enemy_tmp = (Enemy **)entities;
    Enemy ** enemy_tmp_next;
    while((*enemy_tmp)) {
        enemy_tmp_next = &(*enemy_tmp)->next;
        // on veut tours les ennemis dont le tour d'apparition a été passé
        // ou dont c'est le tour d'apparition
        // (file d'attente)
        if ((*enemy_tmp)->turn <= turn && game_add_enemy(game, enemy_tmp)) {
            // succès d'ajout, l'ennemi courant est l'ancien suivant
            // on ne fait rien
        } else {
            // échec d'ajout, on passe au suivant
            enemy_tmp = enemy_tmp_next;
        }
    }
    return 1;
}

/*int gm_add_entities(Game * game, void * entities, ENTITY ntt){
    printf("ICICICI : ");
    CLI_debug_display_next((*(Enemy **) entities));
    printf("\n");
    Enemy ** enemy_tmp = (Enemy **)entities;
    Enemy ** enemy_tmp_next;
    while(enemy_tmp) {
        printf("Bopi\n");
        enemy_tmp_next = &((*enemy_tmp)->next);
        if ((*enemy_tmp_next) && ntt == ENEMY && game_add_enemy(game, enemy_tmp)) {
            printf("SUCCESS\n");
        } else {
            printf("ECHEC \n");
            enemy_tmp = enemy_tmp_next;
            if(!(*enemy_tmp_next)) break;

        }
    }

    return 1;
}*/

/*int gm_add_entities(Game * game, void * entities, ENTITY ntt){
    printf("ICICICI : ");
    printf("Adresse liste attente : %p\n", &(*entities));
    printf("\n");
    Enemy ** enemy_tmp = (Enemy **)entities;
    Enemy ** enemy_tmp_next;
    while((*enemy_tmp)) {
        printf("Bopi\n");
        enemy_tmp_next = &(*enemy_tmp)->next;
        printf("Adresse liste attente : %p\n", (*enemy_tmp_next));
        if(enemy_tmp_next && (*enemy_tmp_next))
            printf("Adresse liste attente : %c\n", (char) (*enemy_tmp_next)->type);
        printf("bop\n");
        if ((*enemy_tmp_next) && ntt == ENEMY &&
        game_add_enemy(game, enemy_tmp)) {
            printf("SUCCESS\n");
        } else {
            printf("ECHEC \n");
            enemy_tmp = enemy_tmp_next;
            if(!(*enemy_tmp_next)) break;

        }
    }

    return 1;
}*/