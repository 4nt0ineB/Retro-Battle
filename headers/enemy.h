

#ifndef L2_PROJET_C_ENEMY_H
#define L2_PROJET_C_ENEMY_H

/**
 * Les types d'ennemis.
 * La valeur d'un type
 * est l'indice
 * de la lettre le représentant
 * en ASCII
 */
enum ENEMY_TYPE{
    E = (int) 'E',
    D = (int) 'D',
    S = (int) 'S',
    M = (int) 'M',
    B = (int) 'B'
};

/**
 * Modélise les ennemi.
 *
 */
typedef struct enemy{
    int type; /**< Le type de l'ennemi*/
    int life;
    int line;
    int position;
    int speed;
    int turn;
    struct enemy* next; /**< l'ennemi courant est apparu avant l'ennemi pointé par *next */
    struct enemy *next_line, *prev_line; /**< les ennemis suivant (droite) et précédent (gauche) sur la ligne */
}Enemy;

/**
 * Alloue un ennemi
 * @param type le type de l'ennemi
 * @param life les points de vie
 * @param line la ligne initiale
 * @param speed sa vitesse
 * @param turn le tour d'apparition
 * @return un pointeur sur un ennemi
 */
Enemy * alloue_enemy(int type, int life, int line, int speed, int turn);

/**
 * Lie l'ennemi à un suivant sur la ligne (à la fin de la liste)
 * @param to_e l'ennemi d'origine
 * @param from_e l'ennemi à ajouter
 * @return 0 ou 1
 */
int enemy_add_next_line(Enemy * to_e, Enemy * from_e);

void CLI_debug_display_enemies_line(Enemy * enemy);
void CLI_display_enemy(Enemy enemy);
#endif //L2_PROJET_C_ENEMY_H
