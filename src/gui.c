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

void gui_rel_length(float * width_percent, float * height_percent){
    int w, h;
    taille_fenetre(&w, &h);
    if(width_percent && *width_percent > 0)
        *width_percent = (*width_percent * (float) w) / 100;
    if(height_percent && *height_percent > 0)
        *height_percent = (*height_percent * (float) h) / 100;
}

int gui_add_btn(DListe * btn_list, Button * btn){
    DListe cel = alloue_DCellule(btn);
    if(cel && DListe_ajouter_fin(btn_list, cel)){
        return 1;
    }
    return 0;
}

Button * btn_create_responsive(int x, int y, float w, float h, BTN_TYPE btn_type, MLV_Image * image, char * text, void * data, MLV_Color color){
    float btn_w = w, btn_h = h; // en pourcentage des bords de la fenêtre
    gui_rel_length(&btn_w, &btn_h);
    Point ptmp1 = {x, y};
    Point ptmp2 = { x  + (int) btn_w, y + (int) btn_h};
    return alloue_btn(btn_type, ptmp1, ptmp2 , image, text, data, color);
}

void gui_display_btn(Button btn, MLV_Font * font){
    MLV_draw_filled_rectangle(
            btn.p1.x,
            btn.p1.y,
            btn.p2.x - btn.p1.x,
            btn.p2.y - btn.p1.y,
            btn.color
    );

    if(btn.image && (btn.p2.x - btn.p1.x) > 0  ){
        MLV_draw_image(btn.image, btn.p1.x,
                       btn.p1.y);
    }

    if(btn.text){
        int fw, fh ;
        MLV_get_size_of_text_with_font (btn.text, &fw, &fh, font);
        MLV_draw_text_with_font(
                btn.p1.x + (((btn.p2.x - btn.p1.x) - fw) /2),
                btn.p1.y + (((btn.p2.y - btn.p1.y) - fh) /2),
                btn.text,
                font, MLV_COLOR_BLACK
        );
    }

}

void gui_display_btns(DListe btn_list,  MLV_Font * font){
    DListe cel = btn_list;
    while(cel){
        gui_display_btn(*(Button *) cel->element, font);
        cel = cel->suivant;
    }
}

void gui_enhance_btn_over(Button btn,  MLV_Font * font){
    float border_w = 0.2f, border_y = 0.2f;
    gui_rel_length(&border_w, &border_y);
    MLV_draw_filled_rectangle(
            btn.p1.x - (int) border_w,
            btn.p1.y - (int) border_y,
            (btn.p2.x - btn.p1.x) + (int) (border_w * 2),
            (btn.p2.y - btn.p1.y) + (int) (border_y * 3),
            MLV_COLOR_WHITE
    );
    gui_display_btn(btn, font);
}

void gui_enhance_btns_over(DListe btn_list,  MLV_Font * font){
    DListe cel = btn_list;
    int x = 0, y = 0;
    MLV_get_mouse_position(&x, &y);
    while(cel){
        if(btn_coord_is_over(*(Button *) cel->element, x, y))
            gui_enhance_btn_over(*(Button *) cel->element, font);
        cel = cel->suivant;
    }
}



Button * gui_get_clicked_btn(DListe btn_list){
    DListe cel = btn_list;
    int x = 0, y = 0;
    MLV_get_mouse_position(&x, &y);
    while(cel){
        if(btn_coord_is_over(*(Button *) cel->element, x, y))
            return (Button *) cel->element;
        cel = cel->suivant;
    }
    return NULL;
}

void gui_display_entity(void * entity, ENTITY ntt, Entity_type ntt_type, Entity_img ntt_img, int x, int y){
    MLV_draw_image(ntt_img.image, x, y);
    int maxlife = ntt_type.v1;
    int life = 0;
    float lifebar_h = 0.5; // pourcentages
    float paddinglife = 0.7;
    gui_rel_length(&paddinglife, &lifebar_h);
    switch (ntt) {
        case TOWER:
            life = ((Tower *) entity)->life;
            break;
        case ENEMY:
            life = ((Enemy *) entity)->life;
            break;
    }
    float pourcent = ((float) life*100)/((float) maxlife);
    int calc =  (int) ( (float) (w_case()-(int) paddinglife*2) * pourcent) / 100;
    MLV_draw_filled_rectangle(x+(int) paddinglife, y + w_case(), w_case()-(int) paddinglife*2, (int) lifebar_h,  MLV_COLOR_RED2);
    MLV_draw_filled_rectangle(x+(int) paddinglife, y + w_case(), calc, (int) lifebar_h, MLV_COLOR_GREEN1);
}


DListe gui_create_tower_selection(DListe t_types, DListe ntt_img){
    DListe l = NULL;
    Button * btn_tmp = NULL;
    DListe t_tmp = t_types;
    Entity_img * ntt_img_tmp = NULL;
    int * data = NULL;
    while(t_tmp){
        ntt_img_tmp = ntt_img_get(&ntt_img, ((Entity_type *) t_tmp->element)->id);
        if(ntt_img_tmp){
            data = (int *) malloc(sizeof(int));
            *data = ntt_img_tmp->id;
            btn_tmp = btn_create_responsive(0, 0, 0, 0, BTN_PICK_TOWER, ntt_img_tmp->image, NULL, data, MLV_COLOR_GREY);
            //(float) (w_case()*100)/MAX_H,             (float) (w_case()*100)/MAX_W,
            gui_add_btn(&l, btn_tmp);
        }
        t_tmp = t_tmp->suivant;
    }
    return l;
}

void gui_update_tower_selection(DListe tower_btn, int x, int y, int pick_menu_origin){
    int maxdisplayed = 3;
    DListe btn_tmp = tower_btn;
    Button * btn = NULL;
    while(btn_tmp){
        btn = ((Button *) btn_tmp->element);
        if(pick_menu_origin < 0){
            pick_menu_origin++;
        }else if (pick_menu_origin < maxdisplayed){
            btn->p1.x = x;
            btn->p1.y = y;
            btn->p2.x = x + w_case();
            btn->p2.y = y + w_case();
            x += w_case();
            pick_menu_origin++;
        }
        btn_tmp = btn_tmp->suivant;
    }
}

DListe gui_create_board_box_btns() {
    DListe btn_list = NULL;
    int WIDTH, HEIGHT;
    taille_fenetre(&WIDTH, &HEIGHT);
    // origine du plateau de jeu
    int gmb_x = 0, gmb_y = 0;
    game_board_origin(&gmb_x, &gmb_y);
    Point * p1 = NULL;
    int i = 1;
    int j = 1;
    int cw = w_case();
    int cx = gmb_x, cy = gmb_y;
    for (; i <= MAX_LINE; i++) {
        for (j = 1; j <= MAX_LINE_LENGTH; j++) {
            p1 = (Point *) malloc(sizeof(Point));
            p1->x = i;
            p1->y = j;
            gui_add_btn(&btn_list,  btn_create_responsive(cx, cy, ( w_case() * 100) / WIDTH, ( w_case() * 100) / HEIGHT, BTN_BOARD_BOX, NULL, NULL, p1, MLV_ALPHA_TRANSPARENT));
            cx += cw + CASE_MARGING;
        }
        cx = gmb_x;
        cy += cw + CASE_MARGING;
    }
    return btn_list;
}

void GUI_display_game(Game game, DListe e_types, DListe enemy_images, DListe t_types, DListe tower_images){

    Enemy * e_view[MAX_LINE+1][MAX_LINE_LENGTH+1] = {0};
    Tower * t_view[MAX_LINE+1][MAX_LINE_LENGTH+1] = {0};
    int i, j;

    // Ajout des tourelles
    Tower * t_tmp = game.towers;
    while(t_tmp){
        t_view[t_tmp->line][t_tmp->position] = t_tmp;
        t_tmp = t_tmp->next;
    }

    // Ajout des ennemis
    Enemy * e_tmp = game.enemies;
    while(e_tmp){
        e_view[e_tmp->line][e_tmp->position] = e_tmp;
        e_tmp = e_tmp->next;
    }

    // origine du plateau de jeu
    int gmb_x = 0, gmb_y = 0;
    game_board_origin(&gmb_x, &gmb_y);
    // affichage des enemy
    i = 1;
    j = 1;
    int cw = w_case();
    int cx = gmb_x, cy = gmb_y;
    for(; i <= MAX_LINE; i++){
        for(j = 1; j <= MAX_LINE_LENGTH; j++){

            if(e_view[i][j]){
                gui_display_entity(e_view[i][j], ENEMY,
                                   *(Entity_type *) entity_type_get(&e_types, e_view[i][j]->type),
                                   *ntt_img_get(&enemy_images, e_view[i][j]->type),
                                   cx, cy);
                //MLV_draw_image(ntt_img_get(&enemy_images, e_view[i][j]->type)->image, cx, cy );
            }else{
                MLV_draw_filled_circle(cx+(cw/2), cy+(cw/2), 3, MLV_COLOR_LIGHT_GREEN);
            }
            cx += cw + CASE_MARGING;
        }
        cx = gmb_x;
        cy += cw + CASE_MARGING;
    }

    // affichage des tourelles
    i = 1;
    j = 1;
    cw = w_case();
    cx = gmb_x, cy = gmb_y;
    for(; i <= MAX_LINE; i++){
        for(j = 1; j <= MAX_LINE_LENGTH; j++){

            if(t_view[i][j]) {
                gui_display_entity(t_view[i][j], TOWER,
                                   *(Entity_type *) entity_type_get(&t_types, t_view[i][j]->type),
                                   *ntt_img_get(&tower_images, t_view[i][j]->type),
                                   cx, cy);
            }
            cx += cw + CASE_MARGING;
        }
        cx = gmb_x;
        cy += cw + CASE_MARGING;
    }
    MLV_actualise_window();

}