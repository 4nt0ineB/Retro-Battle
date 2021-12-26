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


void CLI_enemy_type_display(Entity_type e_type){
    printf(
            "|---- Enemy type : %c\n"
            "| Name: %s\n"
            "| Life: %d\n"
            "| Speed: %d\n"
            , e_type.id, e_type.name, e_type.v1, e_type.v2
    );
}

void CLI_tower_type_display(Entity_type e_type){
    printf(
            "|---- Tower type : %c\n"
            "| Name: %s\n"
            "| Life: %d\n"
            "| Price: %d\n"
            , e_type.id, e_type.name, e_type.v1, e_type.v2
    );
}

void CLI_entity_type_display_full(Entity_type e_type, ENTITY ntt){

    if(ntt == TOWER){
        CLI_tower_type_display(e_type);
    }else if(ntt == ENEMY){
        CLI_enemy_type_display(e_type);
    }
    printf("| Effects: ");
    DListe cel = e_type.effects;
    while(cel){
        effect_display_full(*((Effect *) cel->element));
        if(cel->suivant){
            printf(", ");
        }
        cel = cel->suivant;
    }
}

void CLI_display_enemy(Enemy enemy){
    char * str =  enemy_toString(enemy);
    printf("%s", str);
    free(str);
}

void CLI_display_full_enemy(Enemy enemy){
    printf("{Type: %c"
           ", Life: %d"
           ", Line: %d"
           ", Position %d"
           ", Speed: %d"
           ", Turn: %d}"
            , (char) enemy.type, enemy.life, enemy.line, enemy.position, enemy.speed, enemy.turn);
}

void CLI_display_help(){

}

void CLI_display_menu(){
    printf("MENU"
           "\n] Next turn (n)\n");
    printf("\nChoice ? :");
}

void CLI_display_tower_menu(){

}

void CLI_display_game(Game game){
    char * view[MAX_LINE+1][MAX_LINE_LENGTH+1] = {0};
    int i, j;
    // initialisation de la vue
    for(i = 1; i <= MAX_LINE; i++){
        for(j = 1; j <= MAX_LINE_LENGTH; j++){
            view[i][j] = (char *) malloc(10 * sizeof(char));
            strcpy(view[i][j], ".");
        }
    }
    // Ajout des tourelles
    /*
    Tower * t_tmp = game.towers;
    while(e_tmp){
        free(view[t_tmp->line][e_tmp->position]);
        view[t_tmp->line][e_tmp->position] = tower_toString(*t_tmp);
        t_tmp = t_tmp->next;
    }
     */
    // Ajout des ennemis
    Enemy * e_tmp = game.enemies;
    while(e_tmp){
        free(view[e_tmp->line][e_tmp->position]);
        view[e_tmp->line][e_tmp->position] = enemy_toString(*e_tmp);
        e_tmp = e_tmp->next;
    }

    // affichage de la vue
    printf("\e[1;1H\e[2J");
    for(i = 1; i <= MAX_LINE; i++){
        printf("%d|", i);
        for(j = 1; j <= MAX_LINE_LENGTH; j++){
            printf(" %2s", view[i][j]);
        }
        printf("\n");
    }

    // free
    for(i = 1; i <= MAX_LINE; i++){
        for(j = 1; j <= MAX_LINE_LENGTH; j++){
            free(view[i][j]);
        }
    }
    //printf("\e[1;1H\e[2J");
}