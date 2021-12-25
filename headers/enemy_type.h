//
// Created by antoine on 24/12/2021.
//

#ifndef L2_PROJET_C_ENEMY_TYPE_H
#define L2_PROJET_C_ENEMY_TYPE_H
#include "DListe.h"
#include "effect.h"
#include "stdio.h"

/**
 * Représentation d'un type d'ennemi
 */
typedef struct{
    int id; ///< sont identifiant
    int life; ///< la vie initiale
    int speed; ///< sa vitesse initiale
    DListe effects; ///< la liste des effets
} Enemy_type;

/**
 * Alloue un type d'ennemi
 * @param id
 * @param life
 * @param speed
 * @return
 */
Enemy_type * enemy_type_alloue(int id, int life, int speed);

/**
 * Ajoute un effet au type d'ennemi
 * @param e_type
 * @param effect
 * @return
 */
int enemy_type_add_effect(Enemy_type * e_type, Effect * effect);

/**
 * Libère une structure Enemy_type
 * @param e_type
 */
void type_enemy_free(Enemy_type * e_type);

/**
 * Libère une DListe de type d'ennemis
 * @param l
 */
void enemy_type_dliste_free(DListe * l);

/**
 * Affiche la description du type d'ennemi
 * @param e_type
 */
void enemy_type_display_full(Enemy_type e_type);
#endif //L2_PROJET_C_ENEMY_TYPE_H
