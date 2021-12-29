//
// Created by antoine on 23/12/2021.
//

#include "../headers/game.h"
#include "../headers/enemy.h"
#include "../headers/cli.h"

void * game_get_entity_by_position(Game game, int line, int position, ENTITY * ntt){
    void * entity = NULL;
    if(((entity = enemy_get_by_position(game.enemies, line, position)) )){
        *ntt = ENEMY;
        return entity;
    }
    if(((entity = tower_get_by_position(game.towers, line, position)) )){
        *ntt = TOWER;
        return entity;
    }
    return entity;
}

int game_add_enemy(Game * game, Enemy ** enemy){
    if(!*enemy) return 0;
    // on vérifie qu'une entité n'existe pas déjà à cette position
    ENTITY ntt;
    if(game_get_entity_by_position(*game, (*enemy)->line, (*enemy)->position,&ntt)){
        return 0;
    }
    enemy_add(&game->enemies, enemy_extract(&(*enemy), *enemy));
    return 1;
}

int game_add_entity(Game * game, void * entity, ENTITY ntt){
    // on vérifie qu'une entité n'existe pas déjà à cette position
    ENTITY ntt_tmp;
    Tower** t = NULL; Enemy** e = NULL;
    switch(ntt){
        case ENEMY:
            e = (Enemy **) entity;
            if(game_get_entity_by_position(*game, (*e)->line, (*e)->position, &ntt_tmp))
                return 0;
            enemy_add(&game->enemies, enemy_extract((&*e), *e));
            break;
        case TOWER:
            t = (Tower **) entity;
            if(game_get_entity_by_position(*game, (*t)->line, (*t)->position, &ntt_tmp))
                return 0;
            tower_add(&game->towers, tower_extract((&*t), *t));
            break;
        default:
            return 0;
    }
    return 1;
}

int game_move_entity(Game * game, void * entity, ENTITY ntt, int line, int position){
    if(game_get_entity_by_position(*game, line, position,&ntt))
        return 0;
    switch(ntt){
        case ENEMY:
            enemy_set_position(((Enemy *) entity), line, position);
            break;
        case TOWER:
            tower_set_position(((Tower *) entity), line, position);
            break;
        default:
            return 0;
    }
    return 1;
}


//** Fonction des effets implémentés **//
int game_effect_damage(void * entity, ENTITY ntt, Effect effects){
    return 0;
}