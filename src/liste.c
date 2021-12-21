//
// Created by antoine on 19/12/2021.
//

#include <stdlib.h>
#include "../headers/liste.h"


/*
 * Le type size_t n'est apparemment
 * pas interchangeable avec unsigned int par exemple
 * size_t est plu susceptible d'être portable
 *
 * Rq: avec void *
 * pour lire la valeur d'un int passé par pointeur:
 * int x = *(int *) l->element;
 */

Liste alloue_cellule(void * data, size_t taille){
    Liste l = (Liste) malloc(sizeof(taille));
    if(l){
        l->element = data;
        l->suivant = NULL;
    }
    return l;
}
