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
#include "effect.h"

/**
 * Déplacement naturelle de l'entité
 * @param game
 * @param entity (Enemy/Tower) **
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

/**
 *
 * @param game
 * @param entities (Enemy/Tower) **
 * @param ntt
 * @param turn
 * @return
 */
int gm_add_entities(Game * game, void * entities, ENTITY ntt);

/**
 *
 * @param entity
 * @param effect
 * @param ntt
 * @return
 */
int gm_entity_play_effect(Game game, void * entity, ENTITY ntt, Effect effect);

int gm_apply_effect_on_entities(DListe entities, ENTITY ntt, Effect effect);

int gm_apply_effect_on_entity(void * entity, ENTITY ntt, Effect effect);

/**
 *
 * @param entity
 * @param ntt
 * @param entity_types
 * @return
 */
int gm_entity_play_effects(Game game, void * entity, ENTITY ntt, DListe entity_types);

Enemy * gm_remove_dead_enemies(Game * game);

#endif //L2_PROJET_C_GAME_MASTER_H
