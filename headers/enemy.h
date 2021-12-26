

#ifndef L2_PROJET_C_ENEMY_H
#define L2_PROJET_C_ENEMY_H
#include <stdlib.h>
#include "effect.h"
#include "entity_type.h"

/**
 * Modélise un ennemi.
 */
typedef struct enemy{
    int type;  ///< Le type de l'ennemi
    int life;
    int line;
    int position; ///<  et sur la ligne
    int speed;
    int turn;

    struct enemy* next; ///< l'ennemi courant est apparu avant l'ennemi pointé par *next
    struct enemy *next_line; ///< les ennemis suivant (droite) sur la ligne
    struct enemy *prev_line; ///< les ennemis  précédent (gauche) sur la ligne
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
Enemy * alloue_enemy(int type, int life, int line, int position, int speed, int turn);

/**
 * Lie l'ennemi à un suivant sur la ligne (à la fin de la liste)
 * @param to_e La ligne d'ennemis
 * @param from_e l'ennemi à lier
 * @return 0 si l'ennemi à ajouté n'est pas sur la même ligne
 */
int enemy_add_next_line(Enemy** to_e, Enemy * from_e);

/**
 * Ajoute un ennemi en fin d'une liste d'ennemis (next)
 * (fin de liste)
 * @param to_e tête de liste
 * @param from_e l'ennemi à ajouter
 * @return 0 ou 1
 */
int enemy_add_next(Enemy** to_e, Enemy* from_e);


/**
 * Ajoute un ennemi à une liste d'ennemis
 * en respectant le concept de l'implémentation
 *
 * Apparition (next):
 *      ->(x ligne 1)->(y ligne 2)->(z ligne1) \n
 * Ligne (next_line):
 *      (x ligne 1) <=> (z ligne1)
 *
 * @param to_e
 * @param from_e
 */
void enemy_add(Enemy ** to_e, Enemy * from_e);

/**
 * Extrait un ennemi de sa ligne
 * @param e_from adresse de la ligne d'ennemi depuis laquelle retirer l'ennemi
 * @param enemy adresse de l'ennemi à retirer
 * @return l'ennemi s'il se trouvait dans la ligne, NULL sinon
 */
Enemy* enemy_extract_from_line(Enemy** e_from, Enemy * enemy);

/**
 * Extrait un ennemi d'une liste (next)
 * @param e_from adresse de la ligne d'ennemi depuis laquelle retirer l'ennemi
 * @param enemy adresse de l'ennemi à retirer
 * @return l'adresse de l'ennemi s'il se trouvait dans la ligne, NULL sinon
 * @param e_from
 * @param enemy
 * @return
 */
Enemy * enemy_extract_from_next(Enemy** e_from, Enemy * enemy);

/**
 * Extrait un ennemi de toute la structure
 * @param e_from
 * @param enemy
 * @return l'adresse de l'ennemi enlevé, NULL sinon
 */
Enemy * enemy_extract(Enemy ** e_from, Enemy * enemy);


/**
 * Remplit les caractéristiques des ennemis
 * en fonction de leur type
 * @param enemy un pointeur sur ennemi
 * @param e_type liste des types d'ennemis
 * @return 0 si une initialisation n'a pas pu être réalisée
 */
int init_enemies(Enemy * enemy, DListe enemy_types);

/**
 *
 * Renvoie l'adresse du premier ennemi d'un type donné
 *  (next)
 * @param enemy
 * @param e_type
 * @return
 */
Enemy * find_first_type(Enemy * enemy, int e_type);

/**
 * Retourne l'adresse de l'ennemi pour pour coordonnées
 * données, s'il existe
 * @param enemy
 * @param line
 * @param position
 * @return un pointeur sur Enemy
 */
Enemy * get_enemy_by_position(Enemy * enemy, int line, int position);


/**
 * Déplace l'ennemi dans la direction souhaitée
 * @param enemy
 * @param x
 * @param y
 * @return
 */
int enemy_set_position(Enemy * enemy, int x, int y);


/**
 * Libère toute une structure d'ennemis
 * itère sur toute la chaîne (next) et
 * free désalloue l'ennemi
 * @param enemy
 */
void enemy_free_all(Enemy ** enemy);

/**
 * Retourne un pointeur sur la chaine de
 * caractère de la représentation de l'ennemi
 * @param enemy
 * @return
 */
char * enemy_toString(Enemy enemy);

#endif //L2_PROJET_C_ENEMY_H
