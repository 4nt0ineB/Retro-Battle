//
// Created by antoine on 19/12/2021.
//

#ifndef L2_PROJET_C_LISTE_H
#define L2_PROJET_C_LISTE_H

typedef struct cel{
    void* element;
    struct cel* suivant;
}Cellule, *Liste;

/**
 * Créer une cellule de liste chaînée
 * @param data un pointeur sur une donnée
 * @param taille taille de la donnée
 * @return
 */
Liste alloue_cellule(void * data, size_t taille);

#endif //L2_PROJET_C_LISTE_H
