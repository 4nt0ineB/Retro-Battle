//
// Created by antoine on 22/12/2021.
//

#include "../headers/enemy.h"
#include "stdio.h"
#include "../headers/cli.h"

void CLI_debug_display_enemies_line(Enemy * enemy){
    while(enemy){
        CLI_display_enemy(*enemy);
        if(enemy->next_line)
            printf("->");
        enemy = enemy->next_line;
    }
}

void CLI_display_enemy(Enemy enemy){
    printf("%d%c", enemy.life, (char) enemy.type);
}