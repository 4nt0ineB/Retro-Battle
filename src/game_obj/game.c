//
// Created by Antoine Bastos on 23/12/2021.
//

#include "game.h"


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
            Enemy * test = enemy_extract((&*e), *e);
            enemy_add(&game->enemies, test);
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
    if(line <= 0 || position <= 0 || line > MAX_LINE || position > MAX_LINE_LENGTH)
        return 0;
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

void game_incr_money(Game * game, int money){
    game->money += money;
}

//** Fonction des effets implémentés **//
int game_effect_damage(void * entity, ENTITY ntt, Effect effect){
    if(effect.increment < 0){
        switch (ntt) {
            case TOWER:
                ((Tower *) entity)->life += effect.increment;
                break;
            case ENEMY:
                ((Enemy *) entity)->life += effect.increment;
                break;
            default:
                return 0;
        }
    }else if(effect.set < 0){
        switch (ntt) {
            case TOWER:
                ((Tower *) entity)->life = effect.set;
                break;
            case ENEMY:
                ((Enemy *) entity)->life = effect.set;
                break;
            default:
                return 0;
        }
    }
    return 1;
}

int game_effect_slow(void * entity, ENTITY ntt, Effect effect){
    if(effect.increment){
        switch (ntt) {
            case TOWER:
                break;
            case ENEMY:
                ((Enemy *) entity)->speed += effect.increment;
                break;
            default:
                return 0;
        }
    }else if(effect.set){
        switch (ntt) {
            case TOWER:
                break;
            case ENEMY:
                ((Enemy *) entity)->speed = effect.set;
                break;
            default:
                return 0;
        }
    }
    return 1;
}

int game_effect_heal(void  * entity, ENTITY ntt, Effect effect, DListe tower_types, DListe enemy_types){
    int maxlife = 0;
    switch (ntt) {
        case TOWER:
            maxlife = entity_type_get( &tower_types, ((Enemy *) entity)->type)->v1;
            break;
        case ENEMY:
            maxlife = entity_type_get( &enemy_types, ((Enemy *) entity)->type)->v1;
            break;
        default:
            return 0;
    }
    if(effect.increment > 0){
        switch (ntt) {
            case TOWER:
                ((Tower *) entity)->life += effect.increment % maxlife;

                break;
            case ENEMY:
                ((Enemy *) entity)->life += effect.increment % maxlife;;
                break;
            default:
                return 0;
        }
    }else if(effect.set > 0){
        switch (ntt) {
            case TOWER:
                ((Tower *) entity)->life = effect.set % maxlife;
                break;
            case ENEMY:
                ((Enemy *) entity)->speed = effect.set % maxlife;;
                break;
            default:
                return 0;
        }
    }
    return 1;
}

int game_effect_speed(void  * entity, ENTITY ntt, Effect effect){
    if(effect.increment > 0){
        switch (ntt) {
            case TOWER:
                break;
            case ENEMY:
                ((Enemy *) entity)->speed += effect.increment;
                break;
            default:
                return 0;
        }
    }else if(effect.set > 0){
        switch (ntt) {
            case TOWER:
                break;
            case ENEMY:
                ((Enemy *) entity)->speed = effect.set;
                break;
            default:
                return 0;
        }
    }
    return 1;
}