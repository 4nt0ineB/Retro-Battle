//
// Created by antoine on 24/12/2021.
//

#ifndef L2_PROJET_C_ENTITY_TYPE_H
#define L2_PROJET_C_ENTITY_TYPE_H
#include "DListe.h"
#include "effect.h"
#include "stdio.h"

typedef enum {
  TOWER,
  ENEMY
} ENTITIES;

/**
 * Représentation d'un type d'entité (tower ou enemy)
 * les entité on des valeur d'initialisation et des
 * effets dans le jeu.
 * L'Entity_type est un conteneur de valeurs
 * et d'effets.
 */
typedef struct{
    int id; ///< sont identifiant
    int v1; ///< la vie initiale
    int v2; ///< sa vitesse initiale
    DListe effects; ///< la liste des effets
} Entity_type;

/**
 * Alloue un type d'ennemi
 * @param id
 * @param life
 * @param speed
 * @return
 */
Entity_type * entity_type_alloue(int id, int v1, int v2);

/**
 * Ajoute un effet au type d'ennemi
 * @param e_type
 * @param effect
 * @return
 */
int entity_type_add_effect(Entity_type * e_type, Effect * effect);

/**
 * Libère une structure Enemy_type
 * @param e_type
 */
void entity_type_free(Entity_type * e_type);

/**
 * Libère une DListe de type d'ennemis
 * @param l
 */
void entity_type_dliste_free(DListe * l);

/**
 * Affiche la description du type d'ennemi
 * @param e_type
 */
void entity_type_display_full(Entity_type e_type, ENTITIES ntt);

void enemy_type_display(Entity_type e_type);
void tower_type_display(Entity_type e_type);
#endif //L2_PROJET_C_ENTITY_TYPE_H
