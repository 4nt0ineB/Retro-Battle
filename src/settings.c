//
// Created by antoine on 22/12/2021.
//

#include "../headers/settings.h"

Enemy * lire_fichier_niveau(char * nom_fichier, int * money){
    Enemy * enemy_list = NULL;
    FILE * fichier = NULL;
    Enemy * enemy = NULL;
    char e_type = 0;
    int e_line =-1, e_tour = -1;
    // permet de vérifier facilement la cohérence du fichier de niveau
    // : pas deux ennemis au même endroit au même tour
    int histo_ennemy[MAX_LINE+1][MAX_LINE_LENGTH+1] = {0};
    int last_tour = 1;
    // ouverture du fichier
    if(!(fichier = fopen(nom_fichier, "r")) ){
        printf("Le fichier %s ne peut pas être lu.\n", nom_fichier);
        return NULL;
    }
    //lecture de l'argent de départ pour le niveau
    if(!fscanf(fichier, "%d", money))
        return NULL;
    // lecture des apparitions d'ennemis dans le niveau
    while(fscanf(fichier, "%d %d %c ", &e_tour, &e_line, &e_type) == 3){
        /**
         * @todo à placer dans une fonction de gestion des erreurs
         */
        if(last_tour > e_tour){
            fprintf(stderr
                    , "\nMauvais formatage du niveau:"
                      "\n[%d %d %c] La définition des ennemis se fait dans l'ordre croissant des tours.\n"
                    , e_tour, e_line, e_type);
            return NULL;
        }else if(e_tour < 1 || e_tour > MAX_LINE_LENGTH){
            fprintf(stderr
                    , "\nMauvais formatage du niveau:"
                      "\n[%d %d %c] Le maximum de tours est %d.\n"
                    , e_tour, e_line, e_type, MAX_LINE_LENGTH);
            exit(1);
        }else if(e_line < 1 || e_line > MAX_LINE) {
            fprintf(stderr, "\nMauvais formatage du niveau:"
                            "\n[%d %d %c] La maximum de ligne est %d.\n", e_tour, e_line, e_type, MAX_LINE);
            return NULL;
        }
        if(histo_ennemy[e_line][e_tour]){
            fprintf(stderr
                    , "\nMauvais formatage du niveau:"
                            "\n[%d %d %c] il existe déjà un ennemi sur cette ligne pour ce tour.\n"
                    , e_tour, e_line, e_type);
            return NULL;
        }else{
            histo_ennemy[e_line][e_tour] = 1;
        }
        if(!(enemy = alloue_enemy(
                e_type, 0, e_line,
                MAX_LINE_LENGTH, 0, e_tour)
            )){
            return NULL;
        }
        enemy_add(&enemy_list, enemy);
        last_tour = e_tour;
    }
    fclose(fichier);
    return enemy_list;
}

DListe lire_fichier_types_enemy(char * nom_fichier){
    DListe e_types = NULL;
    DListe cel;
    Enemy_type * enemy_type = NULL;
    FILE * fichier = NULL;
    char e_type = 0;
    int e_life = -1, e_speed = -1;
    // ouverture du fichier
    if(!(fichier = fopen(nom_fichier, "r")))
        return NULL;
    // lecture des types
    while(fscanf(fichier, "%c %d %d ", &e_type, &e_life, &e_speed)== 3){
         DListe e_types_tmp = e_types;
         // erreurs
         while(e_types_tmp){
             if( ((Enemy_type *) e_types_tmp->element)->id == (int) e_type){
                 fprintf(stderr
                         , "\nMauvais formatage des types"
                           "\n[%c %d %d] ce type d'ennemis à déjà été déclaré.\n"
                         , e_type, e_life, e_speed);
                 return NULL;
             }
             e_types_tmp = e_types_tmp->suivant;
         }
         //
         if((enemy_type = enemy_type_alloue(e_type, e_life, e_speed))){
             cel = alloue_DCellule(enemy_type);
             if(!cel){
                 fprintf(stderr,"Impossible d'allouer le type d'ennemi.");
                 return NULL;
             }
             DListe_ajouter_fin(&e_types, cel);
         }else{
             fprintf(stderr,"Impossible d'allouer le type d'ennemi.");
             return NULL;
         }
     }
    fclose(fichier);
    return e_types;
}

int lire_fichier_effets(char * nom_fichier, DListe enemy_types){
    if(!enemy_types) return 0;
    FILE * fichier = NULL;
    Effect * effect = NULL;
    DListe l = NULL;
    if(!(fichier = fopen(nom_fichier, "r")))
        return 0;

    char e_type;
    // :/ risque de segfault
    char * effect_type = malloc((MAX_EFFECT_LIBELLE+1) * sizeof (char));
    int increment, set, target, range, l_range, h_range, r_range,
    b_range, front;
    EFFECT_TYPE type_effet = 0;

    // lecture des effets par type
    while(fscanf(fichier, "%c %s %d %d %d %d %d %d %d %d %d ",
                 &e_type, effect_type, &increment, &set, &target, &range, &l_range,
                 &h_range, &r_range,&b_range, &front
                 ) == 11){
        type_effet = string_to_effect_type(effect_type);
        if(!type_effet){
            fprintf(stderr,"Le type d'effet %s n'existe pas.", effect_type);
            return 0;
        }
        effect = effect_alloue(
                type_effet, increment, set, target, range, l_range, h_range, r_range,
                b_range, front
                );
        if(!effect){
            fprintf(stderr,"Impossible d'allouer l'effet.\n");
            return 0;
        }
        // on récupère le type d'ennemi
        l = enemy_types;
        while(l && ((Enemy_type *) l->element)->id != e_type){
            l = l->suivant;
        }
        if(!l){
            fprintf(stderr,"Le type d'ennemi %c n'a pas été définit.", e_type);
            return 0;
        }
        if(!enemy_type_add_effect(((Enemy_type *) l->element), effect)){
            fprintf(stderr,"Impossible d'ajouter l'effet.\n");
            return 0;
        }
    }
    printf("success\n");
    fclose(fichier);
    return 1;
}

