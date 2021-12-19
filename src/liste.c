//
// Created by antoine on 19/12/2021.
//

#include "../headers/liste.h"
#include <stdlib.h>

Liste creer_cellule(int val){
    Liste l = (Liste) malloc(sizeof (Cellule));
    if(l){
       l->val = val;
       l->next = NULL;
    }
    return l;
}