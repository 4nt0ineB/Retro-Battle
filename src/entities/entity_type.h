//
// Created by antoine on 24/12/2021.
//

#ifndef L2_PROJET_C_ENTITY_TYPE_H
#define L2_PROJET_C_ENTITY_TYPE_H
#include <MLV/MLV_all.h>
#include "../game_obj/DListe.h"
#include "../game_obj/effect.h"
#include "stdio.h"

typedef enum {
  TOWER,
  ENEMY
} ENTITY;

/**
 * Représentation d'un type d'entité (tower ou enemy)
 * les entité on des valeur d'initialisation et des
 * effets dans le jeu.
 * L'Entity_type est un conteneur de valeurs
 * et d'effets.
 */
typedef struct{
    char * name;
    int id; ///< sont identifiant
    int v1; ///< la vie initiale
    int v2; ///< sa vitesse initiale
    DListe effects; ///< la liste des effets
} Entity_type;


/**
 * Alloue un type d'entité
 * @param id
 * @param life
 * @param speed
 * @return
 */
Entity_type * entity_type_alloue(char * name, int id, int v1, int v2);

/**
 * Ajoute un effet au type d'entité
 * @param e_type
 * @param effect
 * @return
 */
int entity_type_add_effect(Entity_type * e_type, Effect * effect);

/**
 * Libère une un type d'entité
 * @param e_type
 */
void entity_type_free(Entity_type * e_type);

/**
 * Libère une DListe de types d'entités
 * @param l
 */
void entity_type_dliste_free(DListe * l);

Entity_type * entity_type_get(DListe * l, int id);


typedef struct{
    ENTITY ntt;
    int id;
    MLV_Image * image;
} Entity_img;

Entity_img * alloue_ntt_img(ENTITY ntt, int id, MLV_Image * image);

Entity_img * ntt_img_get(DListe * l, int id);

void free_ntt_img(Entity_img * ntt_img);

#endif //L2_PROJET_C_ENTITY_TYPE_H
