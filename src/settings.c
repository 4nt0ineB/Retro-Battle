//
// Created by antoine on 22/12/2021.
//

#include "../headers/settings.h"

int lire_fichier_niveau(char * nom_fichier, int * money, Enemy ** enemy_list){
    FILE * fichier = NULL;
    int nbr_enemies = 0;
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
        return 0;
    }
    //lecture de l'argent de départ pour le niveau
    if(!fscanf(fichier, "%d", money))
        return 0;
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
            exit(0);
        }else if(e_tour < 1 || e_tour > MAX_LINE_LENGTH){
            fprintf(stderr
                    , "\nMauvais formatage du niveau:"
                      "\n[%d %d %c] Le maximum de tours est %d.\n"
                    , e_tour, e_line, e_type, MAX_LINE_LENGTH);
            exit(1);
        }else if(e_line < 1 || e_line > MAX_LINE) {
            fprintf(stderr, "\nMauvais formatage du niveau:"
                            "\n[%d %d %c] La maximum de ligne est %d.\n", e_tour, e_line, e_type, MAX_LINE);
            exit(1);
        }
        if(histo_ennemy[e_line][e_tour]){
            fprintf(stderr
                    , "\nMauvais formatage du niveau:"
                            "\n[%d %d %c] il existe déjà un ennemi sur cette ligne pour ce tour.\n"
                    , e_tour, e_line, e_type);
            exit(1);
        }else{
            histo_ennemy[e_line][e_tour] = 1;
        }
        if(!(enemy = alloue_enemy(e_type, 0, e_line, MAX_LINE_LENGTH, 0, e_tour))){
            return 0;
        }
        enemy_add(enemy_list, enemy);
        nbr_enemies += 1;
        last_tour = e_tour;
    }
    fclose(fichier);
    return nbr_enemies;
}

DListe lire_fichier_types_enemy(char * nom_fichier){
    DListe e_types = NULL;
    DListe cel;
    Enemy_type * enemy_type = NULL;
    FILE * fichier = NULL;
    int nbr_types = 0;

    char e_type = 0;
    int e_life = -1, e_speed = -1;
    // ouverture du fichier
    if(!(fichier = fopen(nom_fichier, "r")))
        return 0;
    // lecture des types
    while(fscanf(fichier, "%c %d %d ", &e_type, &e_life, &e_speed)== 3){
         DListe e_types_tmp = e_types;
         while(e_types_tmp){
             if( (*((Enemy_type *) e_types_tmp->element)).id == (int) e_type){
                 fprintf(stderr
                         , "\nMauvais formatage des types"
                           "\n[%c %d %d] ce type d'ennemis à déjà été déclaré.\n"
                         , e_type, e_life, e_speed);
                 exit(1);
             }
             e_types_tmp = e_types_tmp->suivant;
         }
         if((enemy_type = enemy_type_alloue(e_type, e_life, e_speed))){
             cel = alloue_DCellule(enemy_type);
             if(!cel){
                 fprintf(stderr,"Impossible d'allouer le type d'ennemi.");
                 exit(0);
             }
             DListe_ajouter_fin(&e_types, cel);
             nbr_types += 1;
         }else{
             fprintf(stderr,"Impossible d'allouer le type d'ennemi.");
             exit(1);
         }
     }
    fclose(fichier);
    return e_types;
}

