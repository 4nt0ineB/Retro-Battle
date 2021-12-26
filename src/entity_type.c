//
// Created by antoine on 24/12/2021.
//

#include "../headers/entity_type.h"

Entity_type * entity_type_alloue(int id, int v1, int v2){
    Entity_type * e_type = (Entity_type *) malloc(sizeof(Entity_type));
    if(e_type){
        e_type->id = id;
        e_type->v1 = v1;
        e_type->v2 = v2;
        e_type->effects = NULL;
    }
    return e_type;
}

int entity_type_add_effect(Entity_type * e_type, Effect * effect){
    if(!e_type) return 0;
    DListe effect_tmp = e_type->effects;
    while(effect_tmp){
        if( (*((Effect *) effect_tmp->element)).type == effect->type){
            return 0;
        }
        effect_tmp = effect_tmp->suivant;
    }
    DListe cel = alloue_DCellule(effect);
    if(!cel){
        return 0;
    }
    DListe_ajouter_fin(&e_type->effects, cel);
    return 1;
}

void entity_type_free(Entity_type * e_type){
    if(e_type->effects) DListe_delete(&e_type->effects);
    free(e_type);
}

void entity_type_dliste_free(DListe * l){
    DListe tmp = (*l);
    DListe suivant;
    while(tmp){
        suivant = tmp->suivant;
        entity_type_free((Entity_type *) tmp->element);
        free(tmp);
        tmp = suivant;
    }
    *l = NULL;
}

void entity_type_display_full(Entity_type e_type, ENTITY ntt){
    if(ntt == TOWER){
        tower_type_display(e_type);
    }else if(ntt == ENEMY){
        enemy_type_display(e_type);
    }
    printf("| Effects: ");
    DListe cel = e_type.effects;
    while(cel){
        effect_display_full(*((Effect *) cel->element));
        if(cel->suivant){
            printf(", ");
        }
        cel = cel->suivant;
    }
}

void enemy_type_display(Entity_type e_type){
    printf(
            "|---- Enemy type : %c\n"
            "| Life: %d\n"
            "| Speed: %d\n"
            , e_type.id, e_type.v1, e_type.v2
    );
}

void tower_type_display(Entity_type e_type){
    printf(
            "|---- Tower type : %c\n"
            "| Life: %d\n"
            "| Price: %d\n"
            , e_type.id, e_type.v1, e_type.v2
    );
}