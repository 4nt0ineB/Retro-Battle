//
// Created by antoine on 21/12/2021.
//

#include "../headers/enemy.h"
#include <stdlib.h>
#include <stdio.h>


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
    if((*to_e) != NULL && (*to_e)->line != from_e->line)
        return 0;
    if(!(*to_e)->next_line){
        (*to_e)->next_line = from_e;
        from_e->prev_line = (* to_e);
        return 1;
    }
    return enemy_add_next_line(&(* to_e)->next_line, from_e);
}

Enemy * enemy_extract_from_line(Enemy** enemy){
    if(!(*enemy)->prev_line){
        Enemy * tmp = (* enemy);
        enemy = NULL;
        return tmp;
    }
    (* enemy)->prev_line->next_line = (* enemy)->next_line;
    (* enemy)->next_line->prev_line = (* enemy)->prev_line;
    (* enemy)->prev_line = (* enemy)->next_line = NULL;
    return *enemy;
}

void enemy_delete_from_line(Enemy** enemy){
    free(enemy_extract_from_line(enemy));
}


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


