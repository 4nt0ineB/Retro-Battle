#include <stdio.h>

#include "../headers/enemy.h"
#include "../headers/cli.h"
#include "../headers/settings.h"


int main() {
    // test enemy //
    Enemy * e_1 = NULL;
    Enemy * e_2 = alloue_enemy(ENEMY_D, 4, 2, 1, 1);
    Enemy * e_3 = alloue_enemy(ENEMY_M, 4, 2, 1, 1);
    Enemy * e_4 = alloue_enemy(ENEMY_M, 4, 1, 1, 1);
    enemy_add_next_line(&e_1, e_2);
    enemy_add_next_line(&e_1, e_3);
    enemy_add_next_line(&e_1, e_4); // pas ajouté, car par la même ligne
    printf("Ligne %d: ", e_1->line);
    CLI_debug_display_line(e_1);


    // test fichier niveau //
    e_1 = NULL;
    int x = 0;
    char * nom_fichier = "./assets/level1.txt";
    lire_fichier_niveau(nom_fichier, &x, &e_1);
    printf("\nTest lecture fichier: \n");
    CLI_debug_display_next(e_1);
    printf("\n");
    CLI_display_full_enemy(*e_1);
    printf("\n");

    // test lecture type enemy //
    Enemy * enemy_types_list = NULL;
    nom_fichier = "./assets/enemy_types.txt";
    printf("Test lecture fichier types: \n");
    lire_fichier_types_enemy(nom_fichier, &enemy_types_list);
    CLI_debug_display_next(enemy_types_list);
    printf("\n");

    // association des caractéristiques des ennemis par type //
    init_enemy(e_1, enemy_types_list);
    CLI_display_full_enemy(*e_1);

    return 0;
}
