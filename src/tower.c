#include "../headers/tower.h"
#include <stdlib.h>

/**
 * @file tower.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */


Tower * tower_alloue_cel(int type, int life, int line, int position, int price){
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


int ajout_tower(Tower * t, int * argent, int prix, Tower * add){
    if(*argent < prix) return 0;

    *argent = *argent - prix;

    t->next = add;

    return 1;

}


int del_tower(Tower * t, Tower * del){
    while(t->next != del){
        t = t->next;
    }

    if(t->next != del) return 0;
    free(t->next);
    t->next = del->next;

    return 1;
}
