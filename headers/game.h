//
// Created by antoine on 23/12/2021.
//

#ifndef L2_PROJET_C_GAME_H
#define L2_PROJET_C_GAME_H
#include "enemy.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Modélise le jeu
 */
typedef struct {
    Enemy* enemies;
    //Tower* towers;
} Game;

#endif //L2_PROJET_C_GAME_H
