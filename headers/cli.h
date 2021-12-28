//
// Created by antoine on 22/12/2021.
//

#ifndef L2_PROJET_C_CLI_H
#define L2_PROJET_C_CLI_H
#include "enemy.h"
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "settings.h"
#include <string.h>

//** actions des menus
typedef enum {
    SHOW_WAVE = 1,
    BUILD_DEFENSE,
    ENEMIES_INFO,
    TOWERS_INFO,
    START_LEVEL
} LEVEL_MENU_ACTION;
//**


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

void CLI_enemy_type_display(Entity_type e_type);
void CLI_tower_type_display(Entity_type e_type);


/**
 * Affiche la description du type d'entité
 * @param e_type
 */
void CLI_entity_type_display_full(Entity_type e_type, ENTITY ntt);

/**
 * @todo affichage des aides
 */
void CLI_display_help();

/**
 * Affiche le menu
 */
void CLI_display_level_menu();

LEVEL_MENU_ACTION CLI_scan_choice_level_menu();

void CLI_show_wave(Enemy * l);

DListe * CLI_menu_entities_types(DListe entity_types);



/**
 * Affiche le jeu
 * @param game
 */
void CLI_display_game(Game game);


#endif //L2_PROJET_C_CLI_H


