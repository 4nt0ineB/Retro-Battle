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

int init_towers(Tower * towers, DListe tower_types){
    if(!towers) return 0;
    DListe e_type_tmp = NULL;
    Tower * tmp = towers;

    while(tmp){
        e_type_tmp = tower_types;
        while(e_type_tmp && ((Entity_type *) e_type_tmp->element)->id == tmp->type){
            e_type_tmp = e_type_tmp->suivant;
        }
        if(!e_type_tmp){
            fprintf(stderr
                    , "\nLe type %c n'a pas été définit.\n"
                    , tmp->type);
            return 0;
        }
        tmp->life = ((Entity_type *) e_type_tmp->element)->v1;
        tmp->price = ((Entity_type *) e_type_tmp->element)->v2;
        tmp = tmp->next;
    }
    return 1;
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

Tower * tower_extract(Tower ** from, Tower * tower){
    Tower * tmp = (* from);
    if(tmp == tower){
        *from = tmp->next ? tmp->next : NULL;
        return tower;
    }
    while(tmp->next){
        if(tmp->next == tower){
            (* from)->next = tower->next ? tower->next : NULL;
            return tower;
        }
        tmp = tmp->next;
    }
    return NULL;
}

Tower * tower_get_by_position(Tower * towers, int line, int position){
    if(!towers) return NULL;
    Tower * tmp = towers;
    while(tmp){
        if(tmp->line == line && tmp->position == position){
            return towers;
        }
        tmp = tmp->next;
    }
    return NULL;
}

int tower_set_position(Tower * tower, int line, int position){
    if(!tower) return 0;
    tower->line = line;
    tower->position = position;
    return 1;
}

char * tower_toString(Tower tower){
    //    printf("{Life:%d,Type:%c,Line:%d,Position:%d}\n", tower.life, (char) tower.type, tower.line, tower.position);
    char * str = (char *) malloc(20 * sizeof(char));
    sprintf(str, "[%d%c", tower.life, (char) tower.type);
    return str;
}

void tower_free_all(Tower ** towers){
    Tower *tmp = (*towers);
    Tower *tmp2;
    while (tmp) {
        tmp2 = tmp->next;
        free(tmp);
        tmp = tmp2;
    }
    *towers = NULL;
}

int tower_count(Tower * towers){
    Tower * tmp = towers;
    int count = 0;
    while(tmp){
        count += 1;
        tmp = tmp->next;
    }
    return count;
}