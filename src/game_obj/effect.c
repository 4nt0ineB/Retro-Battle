//
// Created by Antoine Bastos on 24/12/2021.
//

#include "effect.h"

char * effect_type_toString(EFFECT_TYPE effect){
    if(effect == 0) return NULL;
    char * libelle[] = {
            "UNDEFINED"
            , "DAMAGE"
            , "HEAL"
            , "SLOW"
            , "SPEED"
            /*, "FREEZE"
            , "SWITCHPLACE"*/
    };
    return libelle[effect];

    /*// on veut éviter de remplir la pile à chaque appel
    // pour des comparaisons
    char **libelles = (char **) malloc(sizeof (char*));
    int i = 0;
    for(; i < 5; i++){
        libelles[i] = (char *) malloc(MAX_EFFECT_LIBELLE * sizeof(char));
    }
    stpcpy(libelles[0], "DAMAGE");
    stpcpy(libelles[1], "HEAL");
    stpcpy(libelles[2], "SLOW");
    stpcpy(libelles[3], "SPEED");
    stpcpy(libelles[4], "FREEZE");
    return libelles[effect];
     */
}

int string_to_effect_type(char * libelle){
    int i = DAMAGE;
    for(; i <= SPEED; i++){
        if(strcmp(effect_type_toString(i), libelle) == 0)
            return i;
    }
    return 0;
}

Effect * effect_alloue(
        EFFECT_TYPE type,
        int increment,
        int set,
        int target,
        int self,
        int range,
        int l_range,
        int h_range,
        int r_range,
        int b_range,
        int front
        ){

    Effect * effect = (Effect *) malloc(sizeof (Effect));
    if(effect){
        effect->type = type;
                effect->increment = increment;
                effect->set = set;
                effect->target = target;
                effect->self = self;
                effect->range = range;
                effect->l_range = l_range;
                effect->h_range = h_range;
                effect->r_range = r_range;
                effect->b_range = b_range;
                effect->front = front;
    }
    return effect;
}

void effect_display_full(Effect effect){
    printf(
            "{Type: %s}"
            , effect_type_toString(effect.type)
    );
}

int effect_entity_in_circular_range(int t_line, int t_pos, int e_line, int e_pos, Effect effect){
    int lr, hr, rr, br = 0;
    lr = e_pos - effect.range;
    rr = e_pos + effect.range;
    hr = e_line - effect.range;
    br = e_line + effect.range;
    return (t_pos >= lr && t_pos <= rr) && (t_line >= br && t_line <= hr);
}

int effect_entity_in_range(int t_line, int t_pos, int e_line, int e_pos, Effect effect){
    int lr, hr, rr, br = 0;
    /*effect_display_full(effect);printf("\n");
    printf("----------------------------\n");
    printf("Position de la l'ennemi. Line: %d, Pos: %d\n", e_line, e_pos);
    printf("Position de la target. Line: %d, Pos: %d\n", t_line, t_pos);
    printf("Range droite de l'effet: %d\n", effect.r_range);
    printf("rr = %d\n",  e_pos + effect.r_range);*/
    lr = e_pos - effect.l_range;
    rr = e_pos + effect.r_range;
    hr = e_line - effect.h_range;
    br = e_line + effect.b_range;
    // range de line / range de colonne
    /*printf("Resultat: %d\n", (t_line == e_line && t_pos >= lr && t_pos <= rr));*/
    return (t_line == e_line && t_pos >= lr && t_pos <= rr)
           || (t_pos == e_pos && t_line >= br && t_line <= hr) ;
}
