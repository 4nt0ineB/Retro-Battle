//
// Created by antoine on 23/12/2021.
//

#ifndef L2_PROJET_C_GAME_H
#define L2_PROJET_C_GAME_H
#include "enemy.h"

#include <stdlib.h>

/**
 * Modélise le jeu
 */
typedef struct {
    Enemy* enemies; ///< Structure des ennemis en jeu
    //Tower* towers; ///< structure des tourelles en jeu
    int money;
} Game;

/**
 * Ajoute un ennemi dans le jeu.
 * Remarque l'ennemi doit être extrait de la précédente
 * structure dans laquelle il était si l'ajout à été réalisé.
 * @param game
 * @param level_enemies
 * @return 1 si la mise en jeu à été réalisé
 */
int game_add_enemy(Game * game, Enemy ** enemy);

/**
 * Renvoie l'entité à une position donnée, s'il elle existe.
 * @param game
 * @param line
 * @param position
 * @param ntt valeur de retour : le type d'entité à été renvoyé via @return
 * @return pointeur void * sur l'entité (enemy ou tower) de type @param ntt, ou NULL
 */
void * game_get_entity_by_position(Game game, int line, int position, ENTITIES * ntt);

#endif //L2_PROJET_C_GAME_H
