//
// Created by antoine on 21/12/2021.
//

#include "../headers/enemy.h"
#include <stdlib.h>
#include <stdio.h>
#include "../headers/cli.h"


Enemy * alloue_enemy(int type, int life, int line, int speed, int turn){
    Enemy * enemy = (Enemy *) malloc(sizeof(Enemy));
    if(enemy){
        enemy->type = type;
        enemy->life = life;
        enemy->line = line;
        enemy->position = 0;
        enemy->speed = speed;
        enemy->turn = turn;
        enemy->next = NULL;
        enemy->next_line = NULL;
        enemy->prev_line = NULL;
    }
    return enemy;
}

int enemy_add_next_line(Enemy ** to_e, Enemy * from_e){
    if(!from_e)
        return 0;
    // pas la même ligne
    if((* to_e) == NULL){
        (* to_e) = from_e;
        return 1;
    }
    // N'est pas ajouté si pas la même ligne
    if((*to_e) != NULL && (*to_e)->line != from_e->line)
        return 0;
    if(!(*to_e)->next_line){
        (*to_e)->next_line = from_e;
        from_e->prev_line = (* to_e);
        return 1;
    }
    return enemy_add_next_line(&(* to_e)->next_line, from_e);
}

Enemy * enemy_extract_from_line(Enemy** e_from, Enemy * enemy) {

    if(!(* e_from) || !enemy) return NULL;
    if ((*e_from) == enemy) {

        if(!(*e_from)->prev_line){
            *e_from = (*e_from)->next_line;
            enemy->next_line = NULL;
        } else {
            (*e_from)->next_line->prev_line = enemy->prev_line;
            ((*e_from)->prev_line)->next_line = enemy->next_line;
        }
        return enemy;
    }
    return enemy_extract_from_line(&(* e_from)->next_line, enemy);
}

Enemy * enemy_extract_from_next(Enemy** e_from, Enemy * enemy){

    Enemy * tmp = (* e_from);
    if(tmp == enemy){
        printf("\nlà %p : %p", *e_from, enemy);
        *e_from = tmp->next ? tmp->next : NULL;
        return enemy;
    }
    while(tmp->next){
        if(tmp->next == enemy){
            (* e_from)->next = enemy->next ? enemy->next : NULL;
            return enemy;
        }
        tmp = tmp->next;
    }
    return NULL;
}

Enemy * enemy_extract_from_all(Enemy** e_from, Enemy * enemy){
    if(!(* e_from) || !enemy) return NULL;

    printf("TOUR \n");
    Enemy * tmp = enemy;
    Enemy ** tmp2 = e_from;
    enemy_extract_from_all(&(* tmp2)->next, tmp);
    //CLI_display_enemy(*(*e_from));printf("\n");
    enemy_extract_from_all(&(* tmp2)->next_line, tmp);
    enemy_extract_from_next(tmp2, tmp);
    enemy_extract_from_line(tmp2, tmp);

    return enemy;
}

/*void enemy_delete_from_line(Enemy** enemy){
    free(enemy_extract_from_line(enemy));
}*/


int enemy_add_next(Enemy** to_e, Enemy* from_e){
    if(!from_e)
        return 0;
    if(!(* to_e)){
        (*to_e) = from_e;
        return 1;
    }
    if(!(*to_e)->next){
        (* to_e)->next = from_e;
        return 1;
    }
    return enemy_add_next(&(* to_e)->next, from_e);
}

Enemy * find_first_type_next(Enemy * enemy, ENEMY_TYPE e_type){
    if(!enemy || enemy->type == e_type)
        return enemy;
    return find_first_type_next(enemy->next, e_type);
}


int init_enemy(Enemy * enemy_list, Enemy * type_list){
    if(!enemy_list) return 0;
    Enemy * e_type = find_first_type_next(type_list, enemy_list->type);
    if(!e_type)
        return 0;
    enemy_list->life = e_type->life;
    enemy_list->speed = e_type->speed;
    return 1;
}

void enemy_delete_line(Enemy** enemy){
    if(!(* enemy)) return;
    enemy_delete_line(&(*enemy)->next_line);
    free((*enemy));
}

void delete_next(Enemy ** enemy){
    if(!(* enemy)) return;
    enemy_delete_line(&(*enemy)->next);
    free((*enemy));
}

void enemy_delete_all(Enemy** enemy){
    /*if(!(* enemy)) return;
    enemy_delete_all((* enemy)->next);
    enemy_delete_all((* enemy)->next_line);
    enemy_extract_from_all();
    enemy_extract_from_all((*enemy)->next_line);
    free((* enemy));*/
}