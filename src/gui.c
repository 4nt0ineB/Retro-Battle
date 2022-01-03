//
// Created by antoine on 03/01/2022.
//

#include "../headers/gui.h"

void taille_fenetre(int * width, int * height) {
    unsigned int w, h;
    MLV_get_desktop_size( &w, &h);
    *width = w > MAX_W ? MAX_W : (int) w;
    *height = h > MAX_H ? MAX_H : (int) h;
}

void init_fenetre() {
    int WIDTH, HEIGHT;
    taille_fenetre(&WIDTH, &HEIGHT);
    MLV_create_window("Attax", "", WIDTH, HEIGHT);
    MLV_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, BACKGROUND_COLOR);
    MLV_actualise_window();
}

int w_case(){
    int WIDTH, HEIGHT;
    taille_fenetre(&WIDTH, &HEIGHT);
    int cw = (WIDTH - (BOARD_PADDING * WIDTH / 100) * 2 - ((CASE_MARGING * WIDTH) / 100) * MAX_LINE_LENGTH * 2) / MAX_LINE_LENGTH;
    int ch = (HEIGHT - (BOARD_PADDING * HEIGHT / 100) * 2 - ((CASE_MARGING * HEIGHT) / 100) * MAX_LINE_LENGTH * 2) / MAX_LINE;
    //printf("CW: %d CH: %d\n", cw, ch);
    return cw < ch ? cw : ch;
}

void game_board_origin(int * x, int * y){
    int WIDTH, HEIGHT;
    taille_fenetre(&WIDTH, &HEIGHT);
    *x = (BOARD_PADDING * WIDTH) / 100;
    *y = (HEADER * HEIGHT) / 100;
    *y += (BOARD_PADDING * HEIGHT) / 100;
    //printf("CW: %d CH: %d\n", *x, *y);
}

void GUI_display_game(Game game){

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

    // origine du plateau de jeu
    int gmb_x = 0, gmb_y = 0;
    game_board_origin(&gmb_x, &gmb_y);
    // affichage des cases
    i = 1;
    j = 1;
    int cw = w_case();
    int cx = gmb_x, cy = gmb_y;
    for(; i <= MAX_LINE; i++){
        for(j = 1; j <= MAX_LINE_LENGTH; j++){

            if(*view[i][j] == '['){
                //printf(BOGREEN" %3s"RESET, &view[i][j][1]);
                MLV_draw_filled_rectangle(cx, cy, cw, cw, MLV_COLOR_DARK_GREEN);
                MLV_draw_rectangle(cx, cy, cw, cw, MLV_COLOR_BLACK);
            }else if(*view[i][j] == '('){
                //printf(BORED" %3s"RESET, &view[i][j][1]);
                MLV_draw_filled_rectangle(cx, cy, cw, cw, MLV_COLOR_DARK_RED);
                MLV_draw_rectangle(cx, cy, cw, cw, MLV_COLOR_BLACK);
            }else{
                MLV_draw_filled_circle(cx+(cw/2), cy+(cw/2), 3, MLV_COLOR_LIGHT_GREEN);
                //printf(" %3s", view[i][j]);
            }
            cx += cw + CASE_MARGING;
        }
        cx = gmb_x;
        cy += cw + CASE_MARGING;
    }
    MLV_actualise_window();

    // free
    for(i = 1; i <= MAX_LINE; i++){
        for(j = 1; j <= MAX_LINE_LENGTH; j++){
            if(view[i][j]){
                free(view[i][j]);
            }
        }
    }
}