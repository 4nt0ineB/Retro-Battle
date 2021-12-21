#include <stdio.h>
#include "../headers/liste.h"




int main() {
    int x = 4;
    Liste l = alloue_cellule(&x, sizeof(x));
    int w = *(int*) l->element;
    printf("Hello, World adresse : %d!\n", w);

    return 0;
}
