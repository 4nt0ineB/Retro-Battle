#include <stdio.h>

#include "../headers/enemy.h"




int main() {
    Enemy * e_1 = alloue_enemy(E, 3, 2, 1, 1);
    Enemy * e_2 = alloue_enemy(D, 4, 2, 1, 1);
    Enemy * e_3 = alloue_enemy(M, 4, 2, 1, 1);
    enemy_add_next_line(e_1, e_2);
    enemy_add_next_line(e_1, e_3);
    CLI_debug_display_enemies_line(e_1);

    return 0;
}
