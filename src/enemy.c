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

int enemy_add_next_line(Enemy * to_e, Enemy * from_e){
    if(!from_e)
        return 0;
    // pas la même ligne
    if(to_e != NULL && to_e->line != from_e->line)
        return 0;
    if(!to_e || !to_e->next_line){
        to_e->next_line = from_e;
        from_e->prev_line = to_e;
        return 1;
    }
    return enemy_add_next_line(to_e->next_line, from_e);
}

Enemy * enemy_extract_from_line(Enemy * enemy){
    enemy->prev_line->next_line = enemy->next_line;
    enemy->next_line->prev_line = enemy->prev_line;
    enemy->prev_line = enemy->next_line = NULL;
    return enemy;
}

void enemy_delete_from_line(Enemy * enemy){
    free(enemy_extract_from_line(enemy));
}


int enemy_add_next(Enemy * to_e, Enemy * from_e){
    if(!from_e){
        return 0;
    }
    if(!to_e || !to_e->next_line){
        to_e->next = from_e;
        return 1;
    }
    return enemy_add_next_line(to_e->next, from_e);
}






