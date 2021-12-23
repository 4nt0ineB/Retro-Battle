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
    enemy_add(&e_1, e_2);
    enemy_add(&e_1, e_3);
    enemy_add(&e_1, e_4); // pas ajouté, car par la même ligne
    printf("Ligne %d: ", e_1->line);
    CLI_debug_display_line(e_1);
    // free
    enemy_free_all(&e_1);


    // test fichier niveau //
    e_1 = NULL;
    int x = 0;
    char * nom_fichier = "./data/level1.txt";
    lire_fichier_niveau(nom_fichier, &x, &e_1);
    printf("\n----------Test lecture fichier: \n");
    CLI_debug_display_next(e_1);
    printf("\n");
    CLI_display_full_enemy(*e_1);
    printf("\n");

    // test lecture type enemy //
    Enemy * enemy_types_list = NULL;
    nom_fichier = "./data/enemy_types.txt";
    printf("\n---------- Test lecture fichier types: \n");
    lire_fichier_types_enemy(nom_fichier, &enemy_types_list);
    CLI_debug_display_next(enemy_types_list);
    printf("\n");

    // association des caractéristiques des ennemis par type //
    init_enemies(e_1, enemy_types_list);
    printf("Caractéristiques du premier ennemis remplies:\n");
    CLI_display_full_enemy(*e_1);
    printf("\nAffichage de la liste d'apparition initialisée:\n");
    CLI_debug_display_next(e_1);

    // verification implémentation //
    printf("\n---------- Effectivité de l'implémentation:\n");
    printf("Ligne 1 premier element: \n");
    CLI_debug_display_line(e_1);
    printf("\nLigne 1 deuxième element: \n");
    CLI_debug_display_line(e_1->next);

    // suppression //
    printf("\n----------Test de suppression\n");
    printf("On souhaite extraire le deuxième élément de tout le chainage:\n");
    Enemy * extr = enemy_extract(&e_1, e_1);
    CLI_debug_display_next(e_1);
    printf("\nLigne 1 premier element: \n");
    CLI_debug_display_line(e_1);
    printf("\nL'élément à bien été supprimé\n");


    // free
    free(extr);
    enemy_free_all(&e_1);
    enemy_free_all(&enemy_types_list);

    printf("\n");
    return 0;
}
//enemy->prev_line = enemy->next_line = NULL;
