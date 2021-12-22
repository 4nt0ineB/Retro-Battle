//
// Created by antoine on 22/12/2021.
//

#ifndef L2_PROJET_C_CLI_H
#define L2_PROJET_C_CLI_H
#include "enemy.h"
/**
 * A DEPLACER DANS cli.c
 * DEBUG : Affiche les ennemis d'une même ligne même ligne
 * formatage : "LifeType"
 * @param enemy tête de ligne
 */
void CLI_debug_display_line(Enemy * enemy);

/**
 * A DEPLACER DANS cli.c
 * Affiche un ennemi
 */
void CLI_display_enemy(Enemy enemy);
#endif //L2_PROJET_C_CLI_H
