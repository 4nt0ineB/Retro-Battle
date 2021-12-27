/**
 * @file tower.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/**
 * @brief 
 * modelisation des tours
 */
typedef struct Tower{
int type;
int life;
int line;
int position;
int price;
struct Tower* next;
}Tower;

/**
 * @brief
 * Type d'une tour
 */
enum Tower_TYPE{
    A = (int) 'A',
    R = (int) 'R',
    P = (int) 'P',
    X = (int) 'X',
    F = (int) 'F'
};

/**
 * @brief 
 * alloue une cellule de type int
 * @param type le type
 * @param life le nombre de pv
 * @param line la ligne ou se situe la tour
 * @param position la position de la tour
 * @param price le cout de la tour
 * @return Tower 
 */
Tower * tower_alloue_cel(int type, int life, int line, int position, int price);

/**
 * @brief 
 * ajout une cellule a la fin de la chaine
 * lie directement au pointeur next
 * @param t cellule de type Tower 
 * @param argent quantite d'argent
 * @param prix prix de la tour desirée 
 * @param add cellule ajouter de type Tower  
 * @return int 
 */
int ajout_tower(Tower * t, int * argent, int prix, Tower add);

/**
 * @brief 
 * supprime une tour
 * @param t chaine de tour 
 * @param del element a suprimmer
 * @return int 
 */
int del_tower(Tower * t, Tower del);
