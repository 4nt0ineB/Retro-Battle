//
// Created by antoine on 24/12/2021.
//

#include "DListe.h"



DListe alloue_DCellule(void * data){
    DListe l = (DListe) malloc(sizeof(DCellule));
    if(l){
        l->element = data;
        l->suivant = NULL;
        l->precedent = NULL;
    }
    return l;
}

int DListe_ajouter_fin(DListe * l, DListe cel){
    if(!cel)
        return 0;
    // pas la même ligne
    if((* l) == NULL){
        (* l) = cel;
        return 1;
    }
    if(!(*l)->suivant){
        (*l)->suivant = cel;
        cel->precedent = (* l);
        return 1;
    }
    return DListe_ajouter_fin(&(* l)->suivant, cel);
}

DListe DListe_extract(DListe * l, DListe DCellule) {
    if((*l) == DCellule){
        l = &(*l)->suivant;
        DCellule->suivant = NULL;
        if(!l){
            (*l)->precedent = NULL;
        }
    }else{
        if(!DCellule->suivant){
            DCellule->precedent->suivant = NULL;
            DCellule->precedent = NULL;
        }else{
            DCellule->precedent->suivant = DCellule->suivant;
            DCellule->suivant->precedent = DCellule->precedent;
            DCellule->precedent = NULL;
            DCellule->suivant = NULL;
        }
    }
    return DCellule;
}

int DListe_nbr_elements(DListe l){
    DListe tmp = l;
    int count = 0;
    while(tmp){
        count += 1;
        tmp = tmp->suivant;
    }
    return count;
}

void DListe_delete(DListe * l){
    DListe tmp = (*l);
    DListe suivant;
    while(tmp){
        suivant = tmp->suivant;
        free(tmp->element);
        free(tmp);
        tmp = suivant;
    }
    *l = NULL;
}
