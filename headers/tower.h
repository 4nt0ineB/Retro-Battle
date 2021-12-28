//
// Created by debian on 27/12/2021.
//

#ifndef L2_PROJET_C_TOWER_H
#define L2_PROJET_C_TOWER_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "DListe.h"
#include "entity_type.h"

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
 * Remplit les caractéristiques des tourelles
 * en fonction de leur type
 * @param enemy un pointeur sur ennemi
 * @param e_type liste des types d'ennemis
 * @return 0 si une initialisation n'a pas pu être réalisée
 */
int init_towers(Tower * towers, DListe tower_types);

/**
 *
 * @param dest
 * @param src
 * @return
 */
int tower_add(Tower ** dest, Tower * src);


/**
 *
 * @param from
 * @param tower
 * @return
 */
Tower * tower_extract(Tower ** from, Tower * tower);

/**
 * Retourne l'adresse d'une tourelle pour des coordonnées
 * données, s'elle existe
 * @param towers
 * @param line
 * @param position
 * @return tower *
 */
Tower * tower_get_by_position(Tower * towers, int line, int position);

/**
 * Déplace la tourelle dans la direction souhaitée
 * @param tower
 * @param line
 * @param position
 * @return
 */
int tower_set_position(Tower * tower, int line, int position);

/**
 * Renvoie un pointeur sur la chaine de caractère
 * de la représentation de la tourelle
 * @param tower
 * @return
 */
char * tower_toString(Tower tower);

/**
 * Libère une structure de tourelles
 * (next)
 * @param towers
 */
void tower_free_all(Tower ** towers);

#endif //L2_PROJET_C_TOWER_H
