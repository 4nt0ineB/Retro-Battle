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
        if(!(enemy = alloue_enemy(e_type, 0, e_line, 0, e_tour))){
            return 0;
        }
        enemy_add(enemy_list, enemy);
        nbr_enemies += 1;
        last_tour = e_tour;
    }
    fclose(fichier);
    return nbr_enemies;
}

int lire_fichier_types_enemy(char * nom_fichier, Enemy ** enemy_list){
    FILE * fichier = NULL;
    int nbr_types = 0;
    Enemy * enemy_type = NULL;
    char e_type = 0;
    int e_life = -1, e_speed = -1;
    // ouverture du fichier
    if(!(fichier = fopen(nom_fichier, "r")))
        return 0;
    // lecture des types
    while(fscanf(fichier, "%c %d %d ", &e_type, &e_life, &e_speed)== 3){
         enemy_type = find_first_type_next((*enemy_list), e_type);
         if((enemy_type)){
             enemy_type->life = e_life;
             enemy_type->speed = e_speed;
         }else if((enemy_type = alloue_enemy(e_type, e_life, -1, e_speed, -1))){
             enemy_add(enemy_list, enemy_type);
             nbr_types += 1;
         }
     }
    fclose(fichier);
    return nbr_types;
}