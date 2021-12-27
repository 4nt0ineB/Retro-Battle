//
// Created by antoine on 21/12/2021.
//

#include "../headers/enemy.h"
#include "../headers/cli.h"


Enemy * alloue_enemy(int type, int life, int line, int position, int speed, int turn){
    Enemy * enemy = (Enemy *) malloc(sizeof(Enemy));
    if(enemy){
        enemy->type = type;
        enemy->life = life;
        enemy->line = line;
        enemy->position = position;
        enemy->speed = speed;
        enemy->turn = turn;

        enemy->next = NULL;
        enemy->next_line = NULL;
        enemy->prev_line = NULL;
    }
    return enemy;
}


void enemy_add(Enemy ** to_e, Enemy * from_e){
    // pas de vérification de position dans cette fonction
    enemy_add_next(&(* to_e), from_e);
    Enemy * tmp = *to_e;
    while(tmp){
        if(tmp != from_e){
            enemy_add_next_line(&tmp, from_e);
            return;
        }
        tmp = tmp->next;
    }
}

int enemy_add_next(Enemy** to_e, Enemy* from_e){
    if(!from_e)
        return 0;
    if(!(* to_e)){
        (*to_e) = from_e;
        from_e->next = NULL;
        return 1;
    }
    if(!(*to_e)->next){
        (* to_e)->next = from_e;
        from_e->next = NULL;
        return 1;
    }
    return enemy_add_next(&(* to_e)->next, from_e);
}

int enemy_add_next_line(Enemy ** to_e, Enemy * from_e){
    if(!from_e)
        return 0;
    if((* to_e) == NULL){
        (* to_e) = from_e;
        return 1;
    }
    // l'ennemi n'est pas ajouté si pas la même ligne
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

Enemy * find_first_type(Enemy * enemy, int e_type){
    if(!enemy || enemy->type == e_type)
        return enemy;
    return find_first_type(enemy->next, e_type);
}

Enemy * get_enemy_by_position(Enemy * enemy, int line, int position){
    if(!enemy) return NULL;
    Enemy * tmp = enemy;
    while(tmp){
        if(tmp->line == line && tmp->position == position){
            return enemy;
        }
        tmp = tmp->next;
    }
    return NULL;
}

int enemy_set_position(Enemy * enemy, int line, int position){
    /*// on prévient la modification erronée de l'agencement
    // interne de la structure enemy
    if(!enemy || enemy->next || enemy->next_line || enemy->prev_line)
        return 0;*/
    if(!enemy) return 0;
    enemy->line = line;
    enemy->position = position;
    return 1;
}

int init_enemies(Enemy * enemy_list, DListe enemy_types){
    if(!enemy_list) return 0;
    DListe e_type_tmp = NULL;
    Enemy * tmp = enemy_list;

    while(tmp){
        e_type_tmp = enemy_types;
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
        tmp->speed = ((Entity_type *) e_type_tmp->element)->v2;
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

char * enemy_toString(Enemy enemy){
    //printf("{Life:%d,Type:%c,Line:%d}", enemy.life, (char) enemy.type, enemy.line);
    char * str = (char *) malloc(20 * sizeof(char));
    sprintf(str, "%d%c", enemy.life, (char) enemy.type);
    return str;
}

