//
// Created by antoine on 24/12/2021.
//

#ifndef L2_PROJET_C_EFFECT_H
#define L2_PROJET_C_EFFECT_H
#include "DListe.h"
#include <string.h>
#define MAX_EFFECT_LIBELLE 25

typedef enum{
    DAMAGE,
    HEAL,
    SLOW,
    SPEED,
    FREEZE,
}EFFECT_TYPE;

/**
 * Renvoie la chaîne de caractère associé au type d'effet
 * @param effect
 * @return
 */
char * effect_toString(EFFECT_TYPE effect);

/**
 * Les ennemis sont comme des cartes.
 * Ces cartes on des effets..
 */
typedef struct {
    EFFECT_TYPE type;
    int increment; ///< valeur à incrémenter/décrémenter
    int set; ///< Valeur à définir != incrémenter
    int target; ///< 0=Enemy, 1=Tower, 2=Tout le monde
    int range; ///< portée (zone de rayon range) de l'effet sur les target
    int l_range; ///< portée de l'effet (sur les targets), à gauche
    int h_range; ///< portée de l'effet (sur les targets), en haut
    int r_range; ///< portée de l'effet (sur les targets), à droite
    int b_range; ///< portée de l'effet (sur les targets), en bas
    int front; ///< profondeur de l'effet sur la ligne de front des targets.
} Effect;

/**
 * Alloue un effet
 * @param type
 * @param increment
 * @param set
 * @param target
 * @param range
 * @param l_range
 * @param h_range
 * @param r_range
 * @param b_range
 * @param front
 * @return
 */
Effect * effect_alloue(
        EFFECT_TYPE type,
        int increment,
        int set,
        int target,
        int range,
        int l_range,
        int h_range,
        int r_range,
        int b_range,
        int front
        );



#endif //L2_PROJET_C_EFFECT_H
