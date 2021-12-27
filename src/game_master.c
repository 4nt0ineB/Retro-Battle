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

int gm_add_entities(Game * game, void ** entities, ENTITY ntt){
    Enemy * enemy_tmp = (*entities);
    Enemy * enemy_tmp_next;
    while(enemy_tmp) {
        printf("Bop\n");
        enemy_tmp_next = enemy_tmp->next;
        if (ntt == ENEMY && game_add_enemy(game, &enemy_tmp)) {
            printf("SUCCESS\n");
        } else {
            printf("ECHEC \n");
        }
        enemy_tmp = enemy_tmp_next;
    }

    return 1;
}