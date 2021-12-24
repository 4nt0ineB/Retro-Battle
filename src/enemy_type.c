//
// Created by antoine on 24/12/2021.
//

#include "../headers/enemy_type.h"

Enemy_type * enemy_type_alloue(int id, int life, int speed){
    Enemy_type * e_type = (Enemy_type *) malloc(sizeof(Enemy_type));
    if(e_type){
        e_type->id = id;
        e_type->speed = speed;
        e_type->life = life;
        e_type->effects = NULL;
    }
    return e_type;
}
int enemy_type_add_effect(Enemy_type * e_type, Effect * effect){
    if(!e_type) return 0;
    DListe effect_tmp = *e_type->effects;
    while(effect_tmp){
        if( (*((Effect *) effect_tmp->element)).type == effect->type){
            return 0;
        }
    }
    DListe cel = alloue_DCellule(effect);
    if(!cel){
        return 0;
    }
    DListe_ajouter_fin(e_type->effects, cel);
    return 1;
}



void type_enemy_free(Enemy_type * e_type){
    if(e_type->effects) DListe_delete(e_type->effects);
    free(e_type);
}

void enemy_type_dliste_free(DListe * l){
    DListe tmp = (*l);
    DListe suivant;
    while(tmp){
        suivant = tmp->suivant;
        type_enemy_free((Enemy_type *) tmp->element);
        free(tmp);
        tmp = suivant;
    }
    *l = NULL;
}