//
// Created by debian on 28/12/2021.
//

#include "../headers/game_utils.h"

// d'après https://fr.wikipedia.org/wiki/Time.h
/*
long mmsecondes_diff_clocks(clock_t t1, clock_t t2){
    //long x = ((t1 - t2)%CLOCKS_PER_SEC)%1000;
    //long x = ((t2%CLOCKS_PER_SEC - t1%CLOCKS_PER_SEC)%1000);
    return (t2 - t1);
}
*/

void free_string_table(char ** t, int taille){
    int i = 0;
    for(; i < taille; i++)
        free(t[i]);
    free(t);
}