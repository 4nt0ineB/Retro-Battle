//
// Created by antoine on 22/12/2021.
//

#ifndef L2_PROJET_C_SETTINGS_H
#define L2_PROJET_C_SETTINGS_H
#include <stdlib.h>
#include "enemy.h"
#include "entity_type.h"
#include "DListe.h"
#include "stdio.h"

#define MAX_LINE 7 ///< C'est le nombre max de lignes
/**
 * On prétend qu'une ligne à une taille max (pour l'instant),
 * car on a un écran fixe et une fonctionnalité
 * qui noue permet de voir le niveau
 *
 */
#define MAX_LINE_LENGTH 26
#define GO_BACK 'b'
#define MAX_OPTIONS 128

typedef enum {
    GUI = 'g',
    CLI = 'a',
    HELP = 'h',
    PATH = 'p'
} GAME_OPTIONS;

/**
 * Lit un fichier de niveau
 * @param nom_fichier
 * @param money retour du montant d'argent disponible au début du niveau
 * @return Structure d'ennemis (type, line et turn initialisés)
 */
Enemy * lire_fichier_niveau(char * nom_fichier, int * money);

/**
 * Lis le fichier de définition
 * des types d'ennemis ou tourelles
 * @param nom_fichier
 * @return DListe des types d'ennemis indiqués dans le fichier
 */
DListe lire_fichier_types(char * nom_fichier);

/**
 * Lit le fichier de définitions des effets par type et les associes
 * et ajoute les effets aux types, d'une liste de types d'ennemis
 * ou de tourelles
 * @param nom_fichier
 * @return 1 si tous les effets on pu être ajoutés
 */
int lire_fichier_effets(char * nom_fichier, DListe types);


char ** read_options(int argc, char *argv[]);

#endif //L2_PROJET_C_SETTINGS_H
