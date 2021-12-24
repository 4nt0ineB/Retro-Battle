//
// Created by antoine on 21/12/2021.
//

#include "../headers/enemy.h"



Enemy * alloue_enemy(int type, int life, int line, int position, int speed, int turn){
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


void enemy_add(Enemy ** to_e, Enemy * from_e){
    enemy_add_next(&(* to_e), from_e);
    Enemy * tmp = *to_e;
    while(tmp){
        if(tmp != from_e){
            enemy_add_next_line(&tmp, from_e);
        }
        tmp = tmp->next;
    }
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
    if((*e_from) == enemy){
        e_from = &(*e_from)->next_line;
        enemy->next_line = NULL;
        if(!e_from){
            (*e_from)->prev_line = NULL;
        }
    }else{
        if(!enemy->next_line){
            enemy->prev_line->next_line = NULL;
            enemy->prev_line = NULL;
        }else{
            enemy->prev_line->next_line = enemy->next_line;
            enemy->next_line->prev_line = enemy->prev_line;
            enemy->prev_line = NULL;
            enemy->next_line = NULL;
        }
    }
    return enemy;
}

Enemy * enemy_extract_from_next(Enemy** e_from, Enemy * enemy){
    Enemy * tmp = (* e_from);
    if(tmp == enemy){
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

Enemy * enemy_extract(Enemy ** e_from, Enemy * enemy){
    if(!e_from || !enemy) return NULL;
    enemy_extract_from_line(e_from, enemy);
    enemy_extract_from_next(&(* e_from), enemy);
    return enemy;
}

Enemy * find_first_type_next(Enemy * enemy, int e_type){
    if(!enemy || enemy->type == e_type)
        return enemy;
    return find_first_type_next(enemy->next, e_type);
}


Enemy * get_enemy_by_position(Enemy * enemy, int line, int position){
    if(!enemy) return NULL;
    if(enemy->line == line && enemy->position == position){
        return enemy;
    }
    return NULL;
}

int init_enemies(Enemy * enemy_list, Enemy * type_list){
    if(!enemy_list) return 0;
    Enemy * e_type = NULL;
    Enemy * tmp = enemy_list;
    while(tmp){
        e_type = find_first_type_next(type_list, enemy_list->type);
        if(!e_type) return 0;
        tmp->life = e_type->life;
        tmp->speed = e_type->speed;
        tmp = tmp->next;
    }
    return 1;
}

void enemy_free_all(Enemy ** enemy) {
    Enemy *tmp = (*enemy);
    Enemy *tmp2;
    while (tmp) {
        tmp2 = tmp->next;
        free(tmp);
        tmp = tmp2;
    }
    *enemy = NULL;
}

