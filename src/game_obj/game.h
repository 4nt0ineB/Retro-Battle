//
// Created by antoine on 23/12/2021.
//

#ifndef L2_PROJET_C_GAME_H
#define L2_PROJET_C_GAME_H
#include <stdlib.h>
#include "../entities/enemy.h"
#include "../entities/tower.h"
#include "../settings/settings.h"

/**
 * Modélise le jeu
 */
typedef struct {
    Enemy* enemies; ///< Structure des ennemis en jeu
    Tower* towers; ///< Structure des tourelles en jeu
    int turn;
    int money;
} Game;

/**
 * Ajoute un ennemi dans le jeu.
 * Remarque l'ennemi doit être extrait de la précédente
 * structure dans laquelle il était si l'ajout à été réalisé.
 * @param game
 * @param enemy Remarque: Enemy ** car il est possiblement la tête d'une liste d'ennemis
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
void * game_get_entity_by_position(Game game, int line, int position, ENTITY * ntt);

/**
 * Ajoute une entité d'un type donné en jeu
 * @param game
 * @param entity
 * @param ntt
 * @return
 */
int game_add_entity(Game * game, void * entity, ENTITY ntt);

/**
 *
 * Change de position une entité, si possible
 * @param game
 * @param entity
 * @param ntt
 * @param line
 * @param position
 * @return 0 si le changement n'a pu être fait
 */
int game_move_entity(Game * game, void * entity, ENTITY ntt, int line, int position);

/**
 * Incrémente l'argent en jeu
 * @param game
 * @param money
 */
void game_incr_money(Game * game, int money);

//################ Fonctions des effets implémentés ################//
int game_effect_damage(void * entity, ENTITY ntt, Effect effects);
int game_effect_slow(void * entity, ENTITY ntt, Effect effect);
int game_effect_heal(void  * entity, ENTITY ntt, Effect effect, DListe tower_types, DListe enemy_types);
int game_effect_speed(void  * entity, ENTITY ntt, Effect effect);

#endif //L2_PROJET_C_GAME_H
