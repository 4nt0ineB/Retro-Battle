//
// Created by antoine on 03/01/2022.
//

#ifndef L2_PROJET_C_GUI_H
#define L2_PROJET_C_GUI_H
#include <stdio.h>
#include <MLV/MLV_all.h>

#include "settings.h"
#include "game.h"

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

void game_board_origin(int * x, int * y);

/**
 * Affiche le jeu
 * @param game
 */
void GUI_display_game(Game game);

#endif //L2_PROJET_C_GUI_H
