//
// Created by antoine on 04/01/2022.
//

#ifndef L2_PROJET_C_BUTTON_H
#define L2_PROJET_C_BUTTON_H
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include "point.h"

/**
 * Types des boutons implémentés
 */
typedef enum {
    BTN_PICK_TOWER = 1,
    BTN_SHOW_WAVE,
    BTN_START,
    BTN_QUIT,
    BTN_INCR_PICK,
    BTN_DECR_PICK,
    BTN_BOARD_BOX,
} BTN_TYPE;

/**
 * Structure d'un bouton pour l'affichage graphique
 */
typedef struct {
    BTN_TYPE type; ///< Type du bouton
    Point p1; ///< Coordonnée bas droite
    Point p2; ///< Coordonnée haut gauche
    MLV_Image * image;
    char * text;
    void * data; ///< une donnée alloué portée par le bouton
    MLV_Color color;
} Button;

/**
 * Alloue un bouton
 * @param type
 * @param p1
 * @param p2
 * @param image
 * @param text
 * @param data
 * @return
 */
Button * alloue_btn(BTN_TYPE type, Point p1, Point p2, MLV_Image * image, char * text, void * data, MLV_Color color);


int btn_coord_is_over(Button btn, int x, int y);

/**
 * Libère un boutton alloué
 * @param btn
 */
void free_btn(Button * btn);

#endif //L2_PROJET_C_BUTTON_H
