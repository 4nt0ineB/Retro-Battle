//
// Created by debian on 28/12/2021.
//

#include "../headers/game-utils.h"

// d'après https://fr.wikipedia.org/wiki/Time.h
long mmsecondes_diff_clocks(clock_t t1, clock_t t2){
    return (t2 - t1)/(CLOCKS_PER_SEC/1000);
}