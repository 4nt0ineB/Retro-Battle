//
// Created by antoine on 23/12/2021.
//

#include "../headers/game.h"
#include "../headers/enemy.h"
#include "../headers/cli.h"

void * game_get_entity_by_position(Game game, int line, int position, ENTITY * ntt){
    void * entity = NULL;
    if(((entity = get_enemy_by_position(game.enemies, line, position)) )){
        *ntt = ENEMY;
        return entity;
    }
    /*
     * @todo vérifier s'il existe une tourelle
     */
    /*if(((entity = get_tower_by_position(game.towers, line, position)) )){
        *ntt = TOWER;
        return entity;
    }*/
    return entity;
}

int game_add_enemy(Game * game, Enemy ** enemy){
    if(!enemy) return 0;
    // on vérifie qu'une entité n'existe pas déjà à cette position
    ENTITY ntt;
    CLI_display_full_enemy(*(*enemy));printf("\n");
    if(game_get_entity_by_position(*game, (*enemy)->line, (*enemy)->position,&ntt)){
        return 0;
    }
    enemy_add(&game->enemies, enemy_extract(&(*enemy), *enemy));
    return 1;
}

int game_move_entity(Game * game, void * entity, ENTITY ntt, int line, int position){

    if(ntt == ENEMY){
        if(game_get_entity_by_position(*game, line, position,&ntt)) {
                   return 0;
        }
        enemy_set_position(((Enemy *) entity), line, position);
        return 1;
    }
    return 1;
}