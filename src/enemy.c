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
    if(!from_e){
        return 0;
    }
    if(!to_e || !to_e->next_line){
        to_e->next_line = from_e;
        return 1;
    }
    return enemy_add_next_line(to_e->next_line, from_e);
}

/**
 * A DEPLACER DANS cli.c
 * DEBUG : Affiche les ennemis d'une même ligne même ligne
 * formatage : "LifeType"
 * @param enemy tête de ligne
 */
void CLI_debug_display_enemies_line(Enemy * enemy){
    while(enemy){
        CLI_display_enemy(*enemy);
        if(enemy->next_line)
            printf("->");
        enemy = enemy->next_line;
    }
}

/**
 * A DEPLACER DANS cli.c
 * Affiche un ennemi
 */
void CLI_display_enemy(Enemy enemy){
    printf("%d%c", enemy.life, (char) enemy.type);
}