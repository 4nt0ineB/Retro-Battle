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

/**
 * Déplacement naturelle de l'entité
 * @param game
 * @param entity
 * @param ntt
 * @return
 */
int gm_move(Game * game, void * entity, ENTITY ntt);

/**
 * Déplace toutes les entité selon leur speed
 * selon leur direction naturelle de déplacement
 * @param game
 * @return
 */
int gm_move_all(Game * game);


int gm_add_entities(Game * game, Enemy ** entities);

#endif //L2_PROJET_C_GAME_MASTER_H
