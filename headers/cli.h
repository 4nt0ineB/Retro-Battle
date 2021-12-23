//
// Created by antoine on 22/12/2021.
//

#ifndef L2_PROJET_C_CLI_H
#define L2_PROJET_C_CLI_H
#include "enemy.h"
/**
 * A DEPLACER DANS cli.c
 * DEBUG : Affiche les ennemis d'une même ligne
 * @param enemy tête de ligne
 */
void CLI_debug_display_line(Enemy * enemy);

/**
 * Pour le debug
 * Affiche une liste d'ennemis (next)
 * @param enemy tête de ligne
 */
void CLI_debug_display_next(Enemy * enemy);

/**
 * A DEPLACER DANS cli.c
 * Affiche un ennemi
 * formatage : "LifeType"
 */
void CLI_display_enemy(Enemy enemy);

/**
 * Affiche toutes les infos d'un ennemi
 * @param enemy
 */
void CLI_display_full_enemy(Enemy enemy);


#endif //L2_PROJET_C_CLI_H


