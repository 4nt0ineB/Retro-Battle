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
    char * nom_fichier = "./data/level1.txt";
    lire_fichier_niveau(nom_fichier, &x, &e_1);
    printf("\n\n##### Test lecture fichier: \n");
    CLI_debug_display_next(e_1);
    printf("\n");
    CLI_display_full_enemy(*e_1);
    printf("\n");

    // test lecture type enemy //
    Enemy * enemy_types_list = NULL;
    nom_fichier = "./data/enemy_types.txt";
    printf("\n\n##### Test lecture fichier types: \n");
    lire_fichier_types_enemy(nom_fichier, &enemy_types_list);
    CLI_debug_display_next(enemy_types_list);
    printf("\n");

    // association des caractéristiques des ennemis par type //
    init_enemy(e_1, enemy_types_list);
    CLI_display_full_enemy(*e_1);

    // suppression
    enemy_add_next_line(&enemy_types_list->next, &(* enemy_types_list));
    Enemy * e_6 = alloue_enemy(ENEMY_M, 4, -1, 1, 1);
    enemy_add_next_line(&enemy_types_list->next, e_6);
    printf("\n\n##### Object récursif à libérer:\n");
    CLI_debug_display_next(enemy_types_list);
    printf("\nAvec ligne:\n");
    CLI_debug_display_line(enemy_types_list->next);
    printf("\n");

   /*//printf("\n%p : %p", &(*enemy_types_list), &(*enemy_types_list));
    enemy_extract_from_all(&enemy_types_list, &(* enemy_types_list));
    //printf("adresse %p\n", enemy_types_list);
    //printf("\n\nSuppression premier enemy: %d\n", enemy_types_list->speed);
    printf("\n\nSuppression du premier type: \n");
    CLI_debug_display_next(enemy_types_list);
    printf("\nSuppression de la ligne du premier element (précédemment second) type: \n");
    //enemy_extract_from_line(&enemy_types_list, &(* enemy_types_list));
    CLI_debug_display_line(enemy_types_list);*/

    return 0;
}
//enemy->prev_line = enemy->next_line = NULL;
