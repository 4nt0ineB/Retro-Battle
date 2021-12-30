//
// Created by antoine on 22/12/2021.
//

#include "../headers/cli.h"

void CLI_clear_screen(){
    printf(CLEAR "");
}

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
    printf(RED "┌─────── Effects ──────────" RESET);
    CLI_display_entity_type_effects(e_type);
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

void CLI_display_level_menu(){
    printf("──── MENU ────────────────────────────"
           "\n 1" GREEN " ► " RESET "See the incoming wave"
           "\n 2" GREEN " ► " RESET "Prepare defense"
           "\n 3" GREEN " ► " RESET "Enemies info"
           "\n 4" GREEN " ► " RESET "Towers info"
           "\n 5" GREEN " ► " RESET "Start"
    );
    printf("\n\nChoice ? : ");
}

LEVEL_MENU_ACTION CLI_scan_choice_level_menu(){
    char choice[2] = {0};
    CLI_display_title();printf("\n\n");
    CLI_display_level_menu();
    while((scanf(" %1s", choice) != 1)  || *choice < SHOW_WAVE || *choice > START_LEVEL){
        CLI_clear_screen();
        printf(BORED "Wrong choice, try again.\n" RESET);
        CLI_display_level_menu();
        choice[0] = 0;
    }
    return (LEVEL_MENU_ACTION) *choice;
}

void CLI_show_wave(Enemy * l){
    printf(BOGREEN "Incoming wave of enemies \n\n" RESET);
    char * view[MAX_LINE+1][MAX_LINE_LENGTH+1] = {0};
    int i, j;
    // initialisation de la vue
    for(i = 1; i <= MAX_LINE; i++){
        for(j = 1; j <= MAX_LINE_LENGTH; j++){
            view[i][j] = (char *) malloc(10 * sizeof(char));
            strcpy(view[i][j], ".");
        }
    }

    // Ajout des ennemis
    Enemy * e_tmp = l;
    while(e_tmp){
        free(view[e_tmp->line][((MAX_LINE_LENGTH+1) - e_tmp->turn)]);
        view[e_tmp->line][((MAX_LINE_LENGTH+1) - e_tmp->turn)] = enemy_toString(*e_tmp);
        e_tmp = e_tmp->next;
    }

    // affichage de la vue
    for(i = 1; i <= MAX_LINE; i++){
        printf("%d|", i);
        for(j = 1; j <= MAX_LINE_LENGTH; j++){
            if(*view[i][j] == '('){
                printf(BORED" %2s"RESET, &view[i][j][1]);
            }else{
                printf(" %2s", view[i][j]);
            }
        }
        printf("\n");
    }

    // free
    for(i = 1; i <= MAX_LINE; i++){
        for(j = 1; j <= MAX_LINE_LENGTH; j++){
            if(view[i][j]){
                free(view[i][j]);
            }
        }
    }

}

void CLI_menu_entities_types(DListe entity_types){
    DListe t_tmp = entity_types;
    while(t_tmp){
        printf("  %c : %s\n",(char) ((Entity_type *) t_tmp->element)->id, ((Entity_type *) t_tmp->element)->name);
        t_tmp = t_tmp->suivant;
    }

}

DListe CLI_scan_choice_entity_types_menu(DListe * entity_types){
    char choice[2] = {0};
    DListe tmp = NULL;
    do{
        tmp = *entity_types;
        printf("◄ b: back\n");
        printf("────────────\n");
        CLI_menu_entities_types(*entity_types);
        printf("\n Your choice ? : ");
        if(scanf(" %1s", choice) == 1){
            while(tmp){
                if((int) *choice == ((Entity_type *) tmp->element)->id)
                    break;
                tmp = tmp->suivant;
            }
        }
        if(*choice != GO_BACK && !tmp){
            CLI_clear_screen();
            CLI_display_title();printf("\n\n");
            printf(BORED "Wrong choice, try again.\n" RESET);
        }

    }while(*choice != GO_BACK && !tmp);
    return &(*tmp);
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

    Tower * t_tmp = game.towers;
    while(t_tmp){
        free(view[t_tmp->line][t_tmp->position]);
        view[t_tmp->line][t_tmp->position] = tower_toString(*t_tmp);
        t_tmp = t_tmp->next;
    }

    // Ajout des ennemis
    Enemy * e_tmp = game.enemies;
    while(e_tmp){
        free(view[e_tmp->line][e_tmp->position]);
        view[e_tmp->line][e_tmp->position] = enemy_toString(*e_tmp);
        e_tmp = e_tmp->next;
    }

    // affichage de la vue
    for(i = 1; i <= MAX_LINE; i++){
        printf("%d|", i);
        for(j = 1; j <= MAX_LINE_LENGTH; j++){
            if(*view[i][j] == '['){
                printf(BOGREEN" %3s"RESET, &view[i][j][1]);
            }else if(*view[i][j] == '('){
                printf(BORED" %3s"RESET, &view[i][j][1]);
            }else{
                printf(" %3s", view[i][j]);
            }
        }
        printf("\n");
    }

    // free
    for(i = 1; i <= MAX_LINE; i++){
        for(j = 1; j <= MAX_LINE_LENGTH; j++){
            if(view[i][j]){
                free(view[i][j]);
            }
        }
    }
    //printf("\e[1;1H\e[2J");
}

void CLI_display_title(){
    printf(""
           GREEN
           "\n ██████╗██╗  ██╗██╗██████╗ ███████╗    ██╗   ██╗███████╗    ██╗   ██╗██╗██████╗ ██╗   ██╗███████╗"
           "\n██╔════╝██║  ██║██║██╔══██╗██╔════╝    ██║   ██║██╔════╝    ██║   ██║██║██╔══██╗██║   ██║██╔════╝"
           "\n██║     ███████║██║██████╔╝███████╗    ██║   ██║███████╗    ██║   ██║██║██████╔╝██║   ██║███████╗"
           "\n██║     ██╔══██║██║██╔═══╝ ╚════██║    ╚██╗ ██╔╝╚════██║    ╚██╗ ██╔╝██║██╔══██╗██║   ██║╚════██║"
           "\n╚██████╗██║  ██║██║██║     ███████║     ╚████╔╝ ███████║     ╚████╔╝ ██║██║  ██║╚██████╔╝███████║"
           "\n ╚═════╝╚═╝  ╚═╝╚═╝╚═╝     ╚══════╝      ╚═══╝  ╚══════╝      ╚═══╝  ╚═╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝"
           RESET
           );
}


void CLI_display_effect(Effect effect){
    printf("\n┌──────────────────────────");
    char str[300] = "";
    strcat(str, "\n├ Name: %s");
    strcat(str, "\n├ Target: %s");
    if(effect.set){
        strcat(str, "\n├ Set: %d");
    }else{
        strcat(str, "\n├ Increment: %d");
    }

    if(effect.front){
        strcat(str, "\n├ Target front depth: %d.");
        printf(str
                , effect_type_toString(effect.type)
                , (effect.target == 0 ? "Enemies": effect.target == 1 ? "Towers": "Enemies & Towers")
                , (effect.set ? effect.set: effect.increment)
                , effect.front
        );
        printf("\n│");
        if(effect.b_range >= 1){
            printf(" %d line%s above %s", effect.b_range, (effect.b_range > 1 ? "s" : ""), (effect.b_range >= 1 ? "and": ""));
        }
        if(effect.b_range >= 1){
            printf(" %d line%s below.", effect.b_range, (effect.b_range > 1 ? "s" : ""));
        }
    }else{
        strcat(str, "\n├ Range: %d");
        strcat(str, "\n├ Left range: %d");
        strcat(str, "\n├ Right range: %d");
        strcat(str, "\n├ Range above: %d");
        strcat(str, "\n└ Range below: %d");

        printf(str
                , effect_type_toString(effect.type)
                , (effect.target == 0 ? "Enemies": effect.target == 1 ? "Towers": "Enemies & Towers")
                , (effect.set ? effect.set: effect.increment)
                , effect.range
                , effect.l_range, effect.r_range, effect.h_range, effect.b_range
        );
    }
    printf("\n└─────────────────────────");
}

void CLI_display_entity_type_effects(Entity_type ntt_type){
    DListe l = ntt_type.effects;
    while(l){
        CLI_display_effect(*((Effect *) l->element));
        printf("\n");
        l = l->suivant;
    }
}

int CLI_ask_continue(){
    char tmp2[2] = {0};
    printf("\n\nContinue (y): ");
    tmp2[0] = 0;
    while(scanf(" %1s", tmp2) != 1 || *tmp2 != 'y');
    printf("\n");
    return 1;
}