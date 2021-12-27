//
// Created by debian on 27/12/2021.
//

#include "../headers/tower.h"

Tower * alloue_tower(int type, int life, int line, int position, int price){
    Tower  *t = (Tower *) malloc(sizeof(Tower));
    if(t){
        t->type = type;
        t->life = life;
        t->line = line;
        t->position = position;
        t->price = price;
        t->next = NULL;
    }
    return t;
}


int tower_add(Tower ** dest, Tower * src){
    if(!src)
        return 0;
    if(!(* dest)){
        (*dest) = src;
        src->next = NULL;
        return 1;
    }
    if(!(*dest)->next){
        (* dest)->next = src;
        src->next = NULL;
        return 1;
    }
    return tower_add(&(* dest)->next, src);
}