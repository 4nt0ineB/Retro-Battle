//
// Created by antoine on 24/12/2021.
//

#ifndef L2_PROJET_C_LISTDC_H_H
#define L2_PROJET_C_LISTDC_H_H
#include <stdlib.h>

/**
 * Implémentation d'une liste doublement chainée
 * générique
 * ( Note perso:
 * le type size_t n'est apparemment
 * pas interchangeable avec unsigned int par exemple
 * size_t est plu susceptible d'être portable)
 *
 * On choisira ici de stocker un pointeur
 * sur une donnée déjà allouée.
 */
typedef struct DCel{
    void * element;
    struct DCel * suivant;
    struct DCel * precedent;
} DCellule, * DListe;

/**
 * Alloue une DCellule
 * @param data
 * @return un pointeur sur la cellule
 */
DListe alloue_DCellule(void * data);

/**
 * Ajout en fin de liste
 * @param to_dc
 * @param from_dc
 * @return
 */
int DListe_ajouter_fin(DListe * l, DListe cel);

/**
 * Extrait une DCellule d'une DListe
 * @param dl_from
 * @param DCellule
 * @return
 */
DListe DListe_extract(DListe * l, DListe cel);


/**
 * Libère toute une liste
 * @param l
 */
void DListe_delete(DListe * l);

#endif //L2_PROJET_C_LISTDC_H_H
