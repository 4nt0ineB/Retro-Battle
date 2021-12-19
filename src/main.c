#include <stdio.h>
#include "../headers/liste.h"
#include "../headers/tower.h"

int main() {
    printf("Hello, World adresse : %lu!\n", (long) creer_cellule(4));
    affiche();
    return 0;
}
