//
// Created by antoine on 24/12/2021.
//

#ifndef L2_PROJET_C_ENEMY_TYPE_H
#define L2_PROJET_C_ENEMY_TYPE_H
#include "DListe.h"
#include "effect.h"

/**
 * Les types d'ennemis.
 * La valeur d'un type
 * est l'indice de la lettre
 * le représentant en ASCII
 */
typedef enum{
    ENEMY_E = (int) 'E',
    ENEMY_D = (int) 'D',
    ENEMY_S = (int) 'S',
    ENEMY_M = (int) 'M',
    ENEMY_B = (int) 'B'
} ENEMY_TYPE_INDEX;

typedef struct{
    int id;
    int life;
    int speed;
    DListe * effects;
} Enemy_type;

Enemy_type * enemy_type_alloue(int id, int life, int speed);

int enemy_type_add_effect(Enemy_type * e_type, Effect * effect);

void type_enemy_free(Enemy_type * e_type);

void enemy_type_dliste_free(DListe * l);

#endif //L2_PROJET_C_ENEMY_TYPE_H
