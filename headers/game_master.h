//
// Created by antoine on 26/12/2021.
//

#ifndef L2_PROJET_C_GAME_MASTER_H
#define L2_PROJET_C_GAME_MASTER_H
#include <stdio.h>
#include "cli.h"
#include "game.h"
#include "DListe.h"
#include "enemy.h"
#include "settings.h"

int gm_move(Game * game, void * entity, ENTITY ntt);
int gm_move_all(Game * game);

#endif //L2_PROJET_C_GAME_MASTER_H
