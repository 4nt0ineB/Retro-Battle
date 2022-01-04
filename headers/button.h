//
// Created by antoine on 04/01/2022.
//

#ifndef L2_PROJET_C_BUTTON_H
#define L2_PROJET_C_BUTTON_H
#include <stdlib.h>
#include "point.h"

/**
 * Types des boutons implémentés
 */
typedef enum {
    PICK_TOWER,
    SHOW_WAVE,
    START,
    QUIT
} BTN_TYPE;

/**
 * Structure d'un bouton pour l'affichage graphique
 */
typedef struct {
    BTN_TYPE type; ///< Type du bouton
    Point p1; ///< Coordonnée bas droite
    Point p2; ///< Coordonnée haut gauche
    void * data; ///< une donnée alloué portée par le bouton
} Button;

/**
 * Alloue un bouton
 * @param type
 * @param p1
 * @param p2
 * @param data
 * @return
 */
Button * alloue_btn(BTN_TYPE type, Point p1, Point p2, void * data);

/**
 * Libère un boutton alloué
 * @param btn
 */
void free_btn(Button * btn);

#endif //L2_PROJET_C_BUTTON_H
