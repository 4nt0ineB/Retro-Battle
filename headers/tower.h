//
// Created by debian on 27/12/2021.
//

#ifndef L2_PROJET_C_TOWER_H
#define L2_PROJET_C_TOWER_H
#include <stdlib.h>

/**
 * Modélise une tourelle.
 */
typedef struct tower{
    int type; ///< le type de la tourelle
    int life;
    int line;
    int position;
    int price; ///< coût d'achat de la tourelle

    struct tower * next; ///< tourelle suivante dans l'ordre
} Tower;

/**
 *
 * @param type
 * @param life
 * @param line
 * @param position
 * @param price
 * @return
 */
Tower * alloue_tower(int type, int life, int line, int position, int price);

/**
 *
 * @param dest
 * @param src
 * @return
 */
int tower_add(Tower ** dest, Tower * src);

#endif //L2_PROJET_C_TOWER_H
