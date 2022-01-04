//
// Created by antoine on 03/01/2022.
//

#ifndef L2_PROJET_C_GUI_H
#define L2_PROJET_C_GUI_H
#include <stdio.h>
#include <MLV/MLV_all.h>

#include "settings.h"
#include "game.h"
#include "button.h"

// http://www-igm.univ-mlv.fr/~boussica/mlv/api/French/html/index.html

#define TAILLE_RELATIVE 5
#define BACKGROUND_COLOR MLV_COLOR_MIDNIGHTBLUE

#define MAX_W 1920
#define MAX_H 1080
#define HEADER 20
#define CASE_PADDING 0
#define CASE_MARGING 0
#define BOARD_PADDING 2
#define HEADER_PADDING 1


/*
 * |───────────────────────────|
 * |          Header           |
 * |───────────────────────────|
 * |        Game board         |
 * |                           |
 * |───────────────────────────|
 */

void affichage();

/**
 * Renvoie la taille de la fenêtre
 * @param width
 * @param height
 */
void taille_fenetre(int * width, int * height);

/**
 * Initialise la fenêtre
 */
void init_fenetre();

int w_case();

void game_board_origin(int * x, int * y);

void gui_rel_length(float * width, float * height);

Button * btn_create_responsive(int x, int y, float w, float h, BTN_TYPE btn_type, MLV_Image * image, char * text, void * data, MLV_Color color);

int gui_add_btn(DListe * btn_list, Button * btn);

void gui_display_btn(Button btn,  MLV_Font *font);

void gui_display_btns(DListe btn_list,  MLV_Font *font);

void gui_enhance_btn_over(Button btn,  MLV_Font *font);

void gui_enhance_btns_over(DListe btn,  MLV_Font *font);

Button * gui_get_clicked_btn(DListe btn_list);

void gui_display_entity(void * entity, ENTITY ntt, Entity_type ntt_type, Entity_img ntt_img, int x, int y);

DListe gui_create_tower_selection(DListe t_types,  DListe ntt_img);

void gui_update_tower_selection(DListe tower_btn, int x, int y, int pick_menu_origin);

DListe gui_create_board_box_btns();

/**
 * Affiche le jeu
 * @param game
 */
void GUI_display_game(Game game, DListe e_types, DListe enemy_images, DListe t_types, DListe tower_images);

#endif //L2_PROJET_C_GUI_H
