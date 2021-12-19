//
// Created by antoine on 19/12/2021.
//

#ifndef L2_PROJET_C_LISTE_H
#define L2_PROJET_C_LISTE_H
typedef struct cel{
    int val;
    struct cel* next;
}Cellule, *Liste;

/**
 * Description
 * @param val
 * @return
 */
Liste creer_cellule(int val);
#endif //L2_PROJET_C_LISTE_H
