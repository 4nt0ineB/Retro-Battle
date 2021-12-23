//
// Created by antoine on 22/12/2021.
//

#include "../headers/cli.h"

void CLI_debug_display_line(Enemy * enemy){
    while(enemy){
        CLI_display_enemy(*enemy);
        if(enemy->next_line)
            printf("<=>");
        enemy = enemy->next_line;
    }
}

void CLI_debug_display_next(Enemy * enemy){
    while(enemy){
        CLI_display_enemy(*enemy);
        if(enemy->next)
            printf("->");
        enemy = enemy->next;
    }
}

void CLI_display_enemy(Enemy enemy){
    //printf("{Life:%d,Type:%c,Line:%d}", enemy.life, (char) enemy.type, enemy.line);
    printf("%d%c", enemy.life, (char) enemy.type);
}

void CLI_display_full_enemy(Enemy enemy){
    printf("{Type: %c"
           ", Life: %d"
           ", Line: %d"
           ", Speed: %d"
           ", Turn: %d}"
            , (char) enemy.type, enemy.life, enemy.line, enemy.speed, enemy.turn);
}