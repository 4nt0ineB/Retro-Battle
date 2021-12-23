//
// Created by antoine on 22/12/2021.
//

#include "../headers/settings.h"
#include "../headers/cli.h"

int lire_fichier_niveau(char * nom_fichier, int * money, Enemy ** enemy_list){
    FILE * fichier = NULL;
    int nbr_enemies = 0;
    Enemy * enemy = NULL;
    char e_type = 0;
    int e_line =-1, e_tour = -1;
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
        if(!(enemy = alloue_enemy(e_type, 0, e_line, 0, e_tour))){
            return 0;
        }
        enemy_add_next(enemy_list, enemy);
        nbr_enemies += 1;
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
             enemy_add_next(enemy_list, enemy_type);
             nbr_types += 1;
         }
     }
    fclose(fichier);
    return nbr_types;
}