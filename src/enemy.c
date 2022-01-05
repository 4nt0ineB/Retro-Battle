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


void enemy_add(Enemy ** dest, Enemy * src){
    // pas de vérification de position dans cette fonction
    enemy_add_next(&(* dest), src);
    Enemy * tmp = *dest;
    while(tmp){
        if(tmp != src){
            enemy_add_next_line(&tmp, src);
            return;
        }
        tmp = tmp->next;
    }
}

int enemy_add_next(Enemy** dest, Enemy* src){
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
    return enemy_add_next(&(* dest)->next, src);
}

int enemy_add_next_line(Enemy ** dest, Enemy * src){
    if(!src)
        return 0;
    if((* dest) == NULL){
        (* dest) = src;
        return 1;
    }
    // l'ennemi n'est pas ajouté si pas la même ligne
    if((*dest) != NULL && (*dest)->line != src->line)
        return 0;
    if(!(*dest)->next_line){
        (*dest)->next_line = src;
        src->prev_line = (* dest);
        return 1;
    }
    return enemy_add_next_line(&(* dest)->next_line, src);
}

Enemy * enemy_extract_from_line(Enemy** dest, Enemy * src) {
    if((*dest) == src){
        dest = &(*dest)->next_line;
        src->next_line = NULL;
        if(!dest){
            (*dest)->prev_line = NULL;
        }
    }else{
        if(!src->next_line){
            if(src->prev_line){
                src->prev_line->next_line = NULL;
            }
            src->prev_line = NULL;
        }else{
            src->prev_line->next_line = src->next_line;
            src->next_line->prev_line = src->prev_line;
            src->prev_line = NULL;
            src->next_line = NULL;
        }
    }
    return src;
}

Enemy * enemy_extract_from_next(Enemy** e_from, Enemy * enemy){
    Enemy * tmp = (* e_from);
    if(tmp == enemy){
        *e_from = tmp->next ? tmp->next : NULL;
        return enemy;
    }
    while(tmp->next){
        if(tmp->next == enemy){
            tmp->next = enemy->next ? enemy->next : NULL;
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

Enemy * enemy_find_first_type(Enemy * enemy, int e_type){
    if(!enemy || enemy->type == e_type)
        return enemy;
    return enemy_find_first_type(enemy->next, e_type);
}

Enemy * enemy_get_by_position(Enemy * enemy, int line, int position){
    if(!enemy) return NULL;
    Enemy * tmp = enemy;
    while(tmp){
        if(tmp->line == line && tmp->position == position){
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

Enemy * enemy_get_first_in_line(Enemy * enemy, int line){
    if(!enemy) return NULL;
    Enemy * tmp = enemy;
    while(tmp){
        if(tmp->line == line){
            return tmp;
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
        while(e_type_tmp && ((Entity_type *) e_type_tmp->element)->id != tmp->type){
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
    char * str = (char *) malloc(20 * sizeof(char));
    sprintf(str, "(%d%c", enemy.life, (char) enemy.type);
    return str;
}

int enemy_count(Enemy * enemy){
    Enemy * tmp = enemy;
    int count = 0;
    while(tmp){
        count += 1;
        tmp = tmp->next;
    }
    return count;
}

