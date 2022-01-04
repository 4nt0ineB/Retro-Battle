//
// Created by antoine on 24/12/2021.
//

#include "../headers/entity_type.h"

Entity_type * entity_type_alloue(char * name, int id, int v1, int v2){
    Entity_type * e_type = (Entity_type *) malloc(sizeof(Entity_type));
    if(e_type){
        e_type->name = name;
        e_type->id = id;
        e_type->v1 = v1;
        e_type->v2 = v2;
        e_type->effects = NULL;
    }
    return e_type;
}

int entity_type_add_effect(Entity_type * e_type, Effect * effect){
    if(!e_type) return 0;
    //DListe effect_tmp = e_type->effects;
    /*while(effect_tmp){
        if( (*((Effect *) effect_tmp->element)).type == effect->type)
            return 0;
        effect_tmp = effect_tmp->suivant;
    }*/
    DListe cel = alloue_DCellule(effect);
    if(!cel)
        return 0;
    DListe_ajouter_fin(&e_type->effects, cel);
    return 1;
}

void entity_type_free(Entity_type * e_type){
    free(e_type->name);
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

Entity_type * entity_type_get(DListe * l, int id){
    DListe tmp = (*l);
    while(tmp){
        if(((Entity_type *) tmp->element)->id == id )
            return tmp->element;
        tmp = tmp->suivant;
    }
    return NULL;
}

//
Entity_img * alloue_ntt_img(ENTITY ntt, int id, MLV_Image * image){
    Entity_img * ntt_img = (Entity_img *) malloc(sizeof(Entity_img));
    if(ntt_img){
        ntt_img->ntt = ntt;
        ntt_img->id = id;
        ntt_img->image = image;
    }
    return ntt_img;
}

Entity_img * ntt_img_get(DListe * l, int id){
    DListe tmp = (*l);
    while(tmp){
        if(((Entity_img *) tmp->element)->id == id )
            return tmp->element;
        tmp = tmp->suivant;
    }
    return NULL;
}

void free_ntt_img(Entity_img * ntt_img){
    if(!ntt_img) return;
    if(ntt_img->image)
        MLV_free_image(ntt_img->image);
    free(ntt_img);
}