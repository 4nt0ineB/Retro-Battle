//
// Created by antoine on 22/12/2021.
//

#include "../headers/settings.h"
#include "../headers/gui.h"
extern struct GUI_dimension G_DIMENSION;

Enemy * lire_fichier_niveau(char * nom_fichier, int * money){
    Enemy * enemy_list = NULL;
    FILE * fichier = NULL;
    Enemy * enemy = NULL;
    char e_type = 0;
    int e_line =-1, e_tour = -1;
    // permet de vérifier facilement la cohérence du fichier de niveau
    // : pas deux ennemis au même endroit au même tour
    int * histo_ennemy[MAX_LINE+1] = {0};
    int maxline[MAX_LINE+1] = {0};

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
         * @todo à placer dans une fonction de classe de gestions des erreurs ?
         */
        if(last_tour > e_tour){
            fprintf(stderr
                    , "\nMauvais formatage du niveau:"
                      "\n[%d %d %c] La définition des ennemis se fait dans l'ordre croissant des tours.\n"
                    , e_tour, e_line, e_type);
            fclose(fichier);
            return NULL;
        }else if(e_line < 1 || e_line > MAX_LINE) {
            fprintf(stderr, "\nMauvais formatage du niveau:"
                            "\n[%d %d %c] La maximum de lignes est %d, le minimum est 1.\n", e_tour, e_line, e_type, MAX_LINE);
            fclose(fichier);
            return NULL;
        }
        if(maxline[e_line] < e_tour) {
            maxline[e_line] = e_tour;
            histo_ennemy[e_line] = (int *) realloc(histo_ennemy[e_line], (maxline[e_line] + 1) * sizeof(int));
            histo_ennemy[e_line][e_tour] = 0;
        }
        if(histo_ennemy[e_line][e_tour]){
            fprintf(stderr
                    , "\nMauvais formatage du niveau:"
                            "\n[%d %d %c] il existe déjà un ennemi sur cette ligne pour ce tour.\n"
                    , e_tour, e_line, e_type);
            fclose(fichier);
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
    int i = 0;
    for(; i <= MAX_LINE; i++){
        free(histo_ennemy[i]);
    }

    return enemy_list;
}

DListe lire_fichier_types(char * nom_fichier){
    /*
     * @todo limite la vie à 99 et restreindre les champs en général (ex: pas de speed négative)
     */
    DListe types = NULL;
    DListe cel;
    Entity_type * enemy_type = NULL;
    FILE * fichier = NULL;
    char name[25];
    char * alloc_name = NULL;
    char type_id = 0;
    int v1 = -1, v2 = -1;
    // ouverture du fichier
    if(!(fichier = fopen(nom_fichier, "r")))
        return NULL;
    // lecture des types
    while(fscanf(fichier, "%25s %c %d %d", name, &type_id, &v1, &v2) == 4){
         // allocation d'un nouveau nom sur le tas
         alloc_name = NULL;
         alloc_name = (char *) malloc(26 * sizeof(char));
         strcpy(alloc_name, name);
         DListe e_types_tmp = types;
         if(type_id == 'q' || type_id == GO_BACK){
             fprintf(stderr
                      , "\n[%c %d %d] Identifiant non autorisé.\n"  // voir menu (CLI) de choix des tourelles
                     , type_id, v1, v2);
             fclose(fichier);
             free(alloc_name);
             return NULL;
         }
         // erreurs
         // on regarde si l'identifiant a déjà été défini
         while(e_types_tmp){
             if( ((Entity_type *) e_types_tmp->element)->id == (int) type_id){
                 fprintf(stderr
                         , "\nMauvais formatage des types"
                           "\n[%c %d %d] ce type_id d'ennemis à déjà été déclaré.\n"
                         , type_id, v1, v2);
                 fclose(fichier);
                 free(alloc_name);
                 return NULL;
             }
             e_types_tmp = e_types_tmp->suivant;
         }
         if((enemy_type = entity_type_alloue(alloc_name, type_id, v1, v2))){
             cel = alloue_DCellule(enemy_type);
             if(!cel){
                 fprintf(stderr,"Impossible d'allouer le type_id d'entité.");
                 fclose(fichier);
                 free(alloc_name);
                 return NULL;
             }
         }else{
             fprintf(stderr,"Impossible d'allouer le type_id d'entité.");
             fclose(fichier);
             free(alloc_name);
             return NULL;
         }
        DListe_ajouter_fin(&types, cel);
     }
    fclose(fichier);
    return types;
}

int lire_fichier_effets(char * nom_fichier, DListe types){
    /*
     * @todo restreindre les champs
     */
    if(!types) return 0;
    FILE * fichier = NULL;
    Effect * effect = NULL;
    DListe l = NULL;
    if(!(fichier = fopen(nom_fichier, "r")))
        return 0;

    char e_type;
    // :/ risque de segfault
    char effect_type[MAX_EFFECT_LIBELLE] = {0};
    int increment, set, target, self, range, l_range, h_range, r_range,
    b_range, front;
    EFFECT_TYPE type_effet = 0;

    // lecture des effets par type
    while(fscanf(fichier, "%c %s %d %d %d %d %d %d %d %d %d %d ",
                 &e_type, effect_type, &increment, &set, &target, &self, &range, &l_range,
                 &h_range, &r_range,&b_range, &front
                 ) == 12){
        type_effet = string_to_effect_type(effect_type);
        if(!type_effet){
            fprintf(stderr,"Le type d'effet %s n'existe pas.", effect_type);
            fclose(fichier);
            return 0;
        }
        effect = effect_alloue(
                type_effet, increment, set, target, self, range, l_range, h_range, r_range,
                b_range, front
                );
        if(!effect){
            fprintf(stderr,"Impossible d'allouer l'effet.\n");
            fclose(fichier);
            return 0;
        }
        // on récupère le type d'ennemi
        l = types;
        while(l && ((Entity_type *) l->element)->id != e_type){
            l = l->suivant;
        }
        if(!l){
            fprintf(stderr,"Le type %c n'a pas été définit.", e_type);
            free(effect);
            fclose(fichier);
            return 0;
        }
        if(!entity_type_add_effect(((Entity_type *) l->element), effect)){
            fprintf(stderr,"Impossible d'ajouter l'effet.\n");
            free(effect);
            fclose(fichier);
            return 0;
        }
    }
    fclose(fichier);
    return 1;
}

char ** read_options(int argc, char *argv[]) {
    /*           TRAITEMENT OPTIONS            */
    char ** options = (char **) malloc(128 * sizeof(char *));
    if(!options) return NULL;
    int i = 0;
    for(; i < MAX_OPTIONS; i++){
        options[i] = (char *) malloc(100 * sizeof(char));
        if(!options[i]) return NULL;
    }
    // index de l'argument
    i = 1;
    // pointeur sur la chaine de l'argument
    char *str_option;
    long tmp_w = MAX_W;
    long tmp_h = MAX_W;
    char * reste;
    // iteration sur les arguments
    for (; i < argc; i++) {
        str_option = argv[i];
        // si est une option
        if (*str_option == '-') {
            str_option++;
            // iteration sur la chaine de l'argument
            for (; *str_option; str_option++) {
                // enregistre choix option
                if(*str_option == HELP){
                    options[HELP][0] = HELP;
                }else if(*str_option == GUI){
                    options[CLI][0] = '\0';
                    options[GUI][0] = GUI;
                }else if(*str_option == CLI){
                    options[GUI][0] = '\0';
                    options[CLI][0] = CLI;
                }else if(*str_option == WIDTH){
                    if(str_option[1] == '='){
                        strcpy(options[WIDTH],&str_option[2]);
                        tmp_w = strtol(options[WIDTH],&reste, 10);
                        if(tmp_w >= 0){
                            G_DIMENSION.WIDTH = (int) tmp_w;
                        }
                    }
                    break;
                }else if(*str_option == HEIGHT){
                    if(str_option[1] == '='){
                        strcpy(options[HEIGHT],&str_option[2]);
                        tmp_h = strtol(options[HEIGHT],&reste, 10);
                        if(tmp_h >= 0){
                            G_DIMENSION.HEIGHT = (int) tmp_h;
                        }
                    }else{
                        break;
                    }
                }
            }
        }else{
            free(options[PATH]);
            options[PATH] = (char *) malloc((strlen(str_option)+1) * sizeof(char));
            strcpy(options[PATH], str_option);
        }

    }
    return options;
}


DListe read_ntt_images(ENTITY ntt, char * path){

    DListe ntt_imgs = NULL;
    MLV_Image * image = NULL;
    Entity_img * ntt_img = NULL;
    FILE * fichier = NULL;
    if(!(fichier = fopen(path, "r")))
        return 0;

    char id = '\0';
    char img_name[150];
    char img_path[1500];
    while(fscanf(fichier, " %c %150s", &id, img_name) == 2){
        strcpy(img_path, IMG_PATH);
        strcat(img_path, img_name);
        image = MLV_load_image( img_path);
        MLV_resize_image_with_proportions (image, w_case(), w_case());

        if(!image){
            printf(BORED "Could not load image\n" RESET);
            continue;
        }
        ntt_img = alloue_ntt_img(ntt, id, image);
        if(!ntt_img){
            printf(BORED "Could not allocate image\n" RESET);
            continue;
        }
        if(!DListe_ajouter_fin(&ntt_imgs, alloue_DCellule(ntt_img))){
            printf(BORED "Could not allocate image in image list.\n" RESET);
            continue;
        }
    }

    fclose(fichier);
    return ntt_imgs;
}