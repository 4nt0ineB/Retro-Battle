//
// Created by antoine on 26/12/2021.
//

#ifndef L2_PROJET_C_GAME_MASTER_H
#define L2_PROJET_C_GAME_MASTER_H
#include <stdio.h>
#include <time.h>

#include "cli.h"
#include "gui.h"
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
 * Ajouter une entité sur le plateau
 * @param game
 * @param entities (Enemy/Tower) **
 * @param ntt le type de l'entité
 * @return
 */
int gm_add_entities(Game * game, void * entities, ENTITY ntt);

/**
 * Joue l'effet donné d'une entité donné
 * @param entity l'entité
 * @param effect l'effet
 * @param ntt le type de l'entité jouant
 * @return
 */
int gm_entity_play_effect(Game game, void * entity, ENTITY ntt, Effect effect, DListe tower_types, DListe enemy_types);

/**
 * Appliquer un effet sur une liste d'entité d'un type donné
 * @param entities DListe d'adresses d'entité
 * @param ntt le type des entité de la liste
 * @param effect l'effet à appliquer
 * @param tower_types les types de tourelles
 * @param enemy_types les types d'ennemis
 * @return
 */
int gm_apply_effect_on_entities(DListe entities, ENTITY ntt, Effect effect, DListe tower_types, DListe enemy_types);

/**
 * Appliquer un effet sur une entité d'un type donné
 * @param entities DListe d'adresses d'entité
 * @param ntt le type des entité de la liste
 * @param effect l'effet à appliquer
 * @param tower_types les types de tourelles
 * @param enemy_types les types d'ennemis
 * @return
 */
int gm_apply_effect_on_entity(void * entity, ENTITY ntt, Effect effect, DListe tower_types, DListe enemy_types);

/**
 * Joue les effets d'une entité
 * @param game
 * @param entity l'entité
 * @param ntt le type de l'entité
 * @param tower_types les types de tourelles
 * @param enemy_types les types d'ennemis
 * @return
 */
int gm_entity_play_effects(Game game, void * entity, ENTITY ntt, DListe tower_types, DListe enemy_types);

/**
 * Joue les effets d'une liste d'entités
 * @param game
 * @param entity Liste des entités à faire jouer. (void *) = Enemy ** ou Tower **
 * @param ntt le type de l'entité
 * @param tower_types les types de tourelles
 * @param enemy_types les types d'ennemis
 * @return
 */
int gm_entities_play_effects(Game game, void * entity, ENTITY ntt, DListe tower_types, DListe enemy_types);

/**
 * Extrait et retourne les ennemis sur le plateau n'ayant plus de vie
 * @param game
 * @return
 */
Enemy * gm_remove_dead_enemies(Game * game);

/**
 * Extrait et retourne les tourelles sur le plateau n'ayant plus de vie
 * @param game
 * @return
 */
Tower * gm_remove_dead_towers(Game * game);

/**
 * Renvoie 1 si les ennemis on gagné, 0 sinon
 * @param game
 * @return
 */
int gm_ennemis_won(Game game);

/**
 * Renvoie 1 si le joueur à gagné, 0 sinon
 * @param game
 * @return
 */
int gm_player_won(Game game);

/**
 * Renvoie 1 si la partie est terminée, 0 sinon.
 * Joueur à gagné ou ennemis on gagné
 * @param game
 * @return
 */
int gm_is_game_over(Game game);

/**
 * Lancer le niveau.
 * L'interface est le terminal
 * @param waiting_enemies
 * @param e_types
 * @param t_types
 * @param money
 * @return
 */
int gm_level_cli(Enemy ** waiting_enemies, DListe e_types, DListe t_types, int money);

/**
 * Lance le niveau.
 * L'interface est une fenêtre.
 * @param waiting_enemies
 * @param e_types
 * @param t_types
 * @param money
 * @return
 */
int gm_level_gui(Enemy ** waiting_enemies, DListe e_types, DListe t_types, int money);

#endif //L2_PROJET_C_GAME_MASTER_H
