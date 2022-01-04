//
// Created by antoine on 26/12/2021.
//

#include "../headers/game_master.h"


int gm_move(Game * game, void * entity, ENTITY ntt){
    int line = 0, speed = 0, pos = 0;
    int orig_pos = ((Enemy *)entity)->position;
    if(ntt == ENEMY){
        // déplacement de "speed" case vers la gauche pour l'ennemi
        line = ((Enemy *)entity)->line;
        speed = ((Enemy *)entity)->speed;
        if(!speed) return 0;
        pos = ((Enemy *)entity)->position;
        // on cherche une position possible
        while(pos == orig_pos || (game_move_entity(game, entity, ntt, line, pos) && (pos != (orig_pos - speed)))){
            pos -= 1;
        }
        return 1;
    }
    return 0;
}

int gm_move_all(Game * game){
    // mettre à jour la position des ennemis
    Enemy * tmp = game->enemies;
    ENTITY ntt = ENEMY;
    while(tmp){
        gm_move(game, tmp, ntt);
        tmp = tmp->next;
    }
    return 1;
}

int gm_add_entities(Game * game, void * entities, ENTITY ntt){
    if(ntt == ENEMY){
        Enemy ** entity_tmp = (Enemy **) entities;
        if(!*entity_tmp) return 0;
        Enemy ** entity_tmp_next = &(*entity_tmp)->next;
        // on veut placer les ennemis dont le tour d'apparition a été passé
        // ou dont c'est le tour d'apparition (file d'attente)
        if ((*entity_tmp)->turn > game->turn || !game_add_entity(game, entity_tmp, ntt))
            // échec d'ajout, on passe au suivant
            entity_tmp = entity_tmp_next;
        // si succès d'ajout, on ne fait rien (l'ennemi courant est l'ancien suivant)
        gm_add_entities(game, entity_tmp, ntt);
    } else if(ntt == TOWER) {
        Tower ** entity_tmp = (Tower **) entities;
        if(!*entity_tmp) return 0;
        Tower ** entity_tmp_next = &(*entity_tmp)->next;
        if (!(game_add_entity(game, entity_tmp, ntt)))
            entity_tmp = entity_tmp_next;
        gm_add_entities(game, entity_tmp, ntt);
    }
    return 1;
}

int gm_entity_play_effect(Game game, void * entity, ENTITY ntt, Effect effect, DListe tower_types, DListe enemy_types){
    // on récupère les targets
    if(effect.target < 0 || effect.target > 3) return 0;
    int pos[MAX_LINE+1][MAX_LINE_LENGTH+1] = {0};
    DListe towers = NULL;
    DListe enemies = NULL;
    DListe cel = NULL;
    Enemy * etmp = NULL;
    Tower * ttmp = NULL;

    int i, j = 0;
    int entity_line, entity_pos = 0;
    int range, zrange = 0;
    int hr,br = 0;

    if(ntt == ENEMY){
        entity_line = ((Enemy *) entity)->line;
        entity_pos = ((Enemy *) entity)->position;
    }else if(ntt == TOWER){
        entity_line = ((Tower *) entity)->line;
        entity_pos = ((Tower *) entity)->position;
    }

    if(effect.front){ /* @todo détailler pourquoi on a une séparation (profondeur du front etc...)*/
        // ennemis
        if(effect.target == 0 || effect.target == 2){
            etmp = game.enemies;
            hr = ((Enemy *) entity)->line - effect.h_range;
            br = ((Enemy *) entity)->line + effect.b_range;
            Enemy * tmp_line = NULL;
            Enemy * tmp_pos = NULL;
            // on itère sur les lignes souhaitées
            for (i = hr; i <= br; i++) {
                // on récupère la ligne
                if (!(tmp_line = enemy_get_first_in_line(game.enemies, i)))
                    continue;
                // on souhaite les "effect.front" premières entités de la ligne de front ennemie
                tmp_pos = tmp_line;
                for (j = 0; j < effect.front; j++) {
                    if (tmp_pos->line == entity_line && tmp_pos->position == entity_pos) {
                        tmp_pos = tmp_pos->next_line;
                        continue;
                    }
                    if (!pos[tmp_pos->line][tmp_pos->position]) {
                        cel = alloue_DCellule(&(*tmp_pos));
                        pos[tmp_pos->line][tmp_pos->position] = 1;
                        DListe_ajouter_fin(&enemies, cel);
                    }
                }
            }
        }

        //tourelles
        if(effect.target == 1 || effect.target == 2){
            ttmp = game.towers;
            hr = ((Tower *) entity)->line - effect.h_range;
            br = ((Tower *) entity)->line + effect.b_range;
            int tmp_grid[MAX_LINE+1][MAX_LINE_LENGTH+1] = {0};
            // on itère sur les lignes souhaitées
            for (i = hr; i <= br; i++) {
                for (j = 0; j < effect.front; j++) {
                    tmp_grid[i][j] = 1;
                }
            }
            while(ttmp){
                if(!tmp_grid[ttmp->line][ttmp->position]){
                    cel = alloue_DCellule(&(*ttmp));
                    tmp_grid[ttmp->line][ttmp->position] = 1;
                    DListe_ajouter_fin(&towers, cel);
                }
                ttmp = ttmp->next;
            }
        }


    }else{
        // tourelles
        if(effect.target == 1 || effect.target == 2){
            ttmp = game.towers;
            while (ttmp) {
                if (ttmp->line == entity_line && ttmp->position == entity_pos) {
                    ttmp = ttmp->next;
                    continue;
                }
                zrange = effect_entity_in_circular_range(
                        ttmp->line, ttmp->position,
                        entity_line, entity_pos,
                        effect);
                range = effect_entity_in_range(
                        ttmp->line, ttmp->position,
                        entity_line, entity_pos,
                        effect);
                if (!pos[ttmp->line][ttmp->position] && (zrange || range)) {
                    cel = alloue_DCellule(&(*ttmp));
                    pos[ttmp->line][ttmp->position] = 1;
                    DListe_ajouter_fin(&towers, cel);
                }
                ttmp = ttmp->next;
            }
        }
        // ennemis
        if(effect.target == 0 || effect.target == 2){
            etmp = game.enemies;
            while(etmp){
                if(etmp->line == entity_line && etmp->position == entity_pos){
                    etmp = etmp->next;
                    continue;
                }
                zrange = effect_entity_in_circular_range(
                        etmp->line, etmp->position,
                        entity_line, entity_pos,
                        effect);
                range = effect_entity_in_range(
                        etmp->line, etmp->position,
                        entity_line, entity_pos,
                        effect);
                if(!pos[etmp->line][etmp->position] && (zrange || range)){
                    cel = alloue_DCellule(&(*etmp));
                    pos[etmp->line][etmp->position] = 1;
                    DListe_ajouter_fin(&enemies, cel);
                }
                etmp = etmp->next;
            }
        }

    }
    /*DListe m = enemies;
    while(m){
        printf("%p->", m->element);
        m = m->suivant;
    }
    printf("\n");*/


    if(effect.self){
        // si self effect et nécessite des target pour s'activer
        if(effect.target >= 0){
            if(enemies || towers){
                cel = alloue_DCellule(entity);
                gm_apply_effect_on_entities(cel, ntt, effect, tower_types, enemy_types);
                free(cel);
            }
        }else{    // si self effect seulement
            cel = alloue_DCellule(entity);
            gm_apply_effect_on_entities(cel, ntt, effect, tower_types, enemy_types);
            free(cel);
        }
    }else if(effect.target >= 0){
        gm_apply_effect_on_entities(enemies, ENEMY, effect, tower_types, enemy_types);
        gm_apply_effect_on_entities(towers, TOWER, effect, tower_types, enemy_types);
    }



    DListe tmp = enemies;
    DListe suivant;
    while(tmp){
        suivant = tmp->suivant;
        free(tmp);
        tmp = suivant;
    }

    tmp = towers;
    while(tmp){
        suivant = tmp->suivant;
        free(tmp);
        tmp = suivant;
    }

    //free(towers);
    //free(enemies);
    return 1;
}

int gm_apply_effect_on_entities(DListe entities, ENTITY ntt, Effect effect, DListe tower_types, DListe enemy_types){
    DListe tmp = entities;
    while(tmp){
        gm_apply_effect_on_entity(tmp->element, ntt, effect, tower_types, enemy_types);
        tmp = tmp->suivant;
    }
    return 0;
}

int gm_apply_effect_on_entity(void * entity, ENTITY ntt, Effect effect, DListe tower_types, DListe enemy_types){
    switch (effect.type) {
        case DAMAGE:
            game_effect_damage(entity, ntt, effect);
            break;
        case HEAL:
            game_effect_heal(entity, ntt, effect, tower_types, enemy_types);
            break;
        case SLOW:
            game_effect_slow(entity, ntt, effect);
            break;
        case SPEED:
            game_effect_speed(entity, ntt, effect);
            break;
        /*case FREEZE:
            break;
        case SWITCHPLACE:
            break;*/
        default:
            return 0;
    }
    return 1;
}

int gm_entity_play_effects(Game game, void * entity, ENTITY ntt, DListe tower_types, DListe enemy_types){
    DListe type = ntt == ENEMY ? enemy_types : tower_types;
    DListe effect = NULL;
    int id;

    if(ntt == TOWER){
        id = ((Tower *) entity)->type;
    }else if(ntt == ENEMY){
        id = ((Enemy *) entity)->type;
    }
    while(type){
        if(((Entity_type *) type->element)->id == id)
            break;
        type = type->suivant;
    }
    if(!type) return 0;
    effect = ((Entity_type *) type->element)->effects;

    while(effect){
        gm_entity_play_effect(game, entity, ntt, *(Effect *) effect->element, tower_types, enemy_types);
        effect = effect->suivant;
    }
    return 1;
}

int gm_entities_play_effects(Game game, void * entity, ENTITY ntt, DListe tower_types, DListe enemy_types){
    void * tmp = entity;
    while(tmp){
        gm_entity_play_effects(game, tmp, ntt, tower_types, enemy_types);
        switch (ntt) {
            case TOWER:
                tmp = ((Tower *) tmp)->next;
                break;
            case ENEMY:
                tmp = ((Enemy *) tmp)->next;
                break;
            default:
                return 0;
        }
    }
    return 1;
}

Enemy * gm_remove_dead_enemies(Game * game){
    Enemy * dead = NULL;
    Enemy * tmp = &(*game->enemies);
    Enemy ** pp = &*&game->enemies;
    Enemy * tmp_next = NULL;
    //printf("Adresse ennemis fonction: %p\n", &(game->enemies));
    while(tmp){
        tmp_next = tmp->next;
        if((*tmp).life <= 0 || (*tmp).position == 1)
            enemy_add(&dead, enemy_extract(pp, &(*tmp)));
        tmp = tmp_next;
    }
    return &(*dead);
}
Tower * gm_remove_dead_towers(Game * game){
    Tower * dead = NULL;
    Tower * tmp = &(*game->towers);
    Tower ** pp = &*&game->towers;
    Tower * tmp_next = NULL;
    //printf("Adresse ennemis fonction: %p\n", &(game->enemies));
    while(tmp){
        tmp_next = tmp->next;
        if((*tmp).life <= 0)
            tower_add(&dead, tower_extract(pp, &(*tmp)));
        tmp = tmp_next;
    }
    return &(*dead);
}

int gm_ennemis_won(Game game){
    Enemy * tmp = game.enemies;
    while(tmp){
        if(tmp->position == 1)
            return 1;
        tmp = tmp->next;
    }
    return 0;
}

int gm_player_won(Game game){
    if(!game.enemies)
        return 1;
    return 0;
}

int gm_is_game_over(Game game){
    if(gm_ennemis_won(game)
        || gm_player_won(game)){
        return 1;
    }
    return 0;
}

int gm_level_cli(Enemy ** waiting_enemies, DListe e_types, DListe t_types, int money){
    Game game = {NULL, NULL, 0, money};
    // ça râle si on ne crée pas de fenêtre alors qu'on importe libMLV (voir: valgrind ./main

    // affichage du jeu
    /*  @todo créer une classe game_state pour le tour, les points, les stats, etc...  */

    LEVEL_MENU_ACTION act = 0;
    DListe tmp = NULL;
    CLI_clear_screen();
    CLI_clear_screen();
    do{
        CLI_clear_screen();
        CLI_display_title();printf("\n\n");
        switch (act) {
            case SHOW_WAVE:
                CLI_show_wave(*waiting_enemies);
                CLI_ask_continue();
                break;
            case BUILD_DEFENSE:
                CLI_build_defense(&game, t_types);
                break;
            case ENEMIES_INFO:
                do{
                    CLI_clear_screen();
                    CLI_display_title();printf("\n\n");
                    if((tmp = CLI_scan_choice_entity_types_menu(&e_types))){
                        CLI_clear_screen();
                        CLI_display_title();printf("\n\n");
                        CLI_entity_type_display_full(*((Entity_type *) tmp->element), ENEMY);
                        CLI_ask_continue();
                    }
                }while(tmp);
                break;
            case TOWERS_INFO:
                do{
                    CLI_clear_screen();
                    CLI_display_title();printf("\n\n");
                    if((tmp = CLI_scan_choice_entity_types_menu(&t_types))){
                        CLI_clear_screen();
                        CLI_display_title();printf("\n\n");
                        CLI_entity_type_display_full(*((Entity_type *) tmp->element), ENEMY);
                        CLI_ask_continue();
                    }
                }while(tmp);
                break;
            default:
                break;
        }
        act = 0;
        CLI_clear_screen();
        act = CLI_scan_choice_level_menu();
    }while(act != START_LEVEL && act != LEAVE);


    if(act == START_LEVEL){
        // jouer le niveau
        clock_t t_1 = clock();
        clock_t t_2;
        Enemy * dead_e = NULL;
        Tower * dead_t = NULL;
        while(1) {
            // ----Si on se renseigne sur time.h (par ex: https://fr.wikipedia.org/wiki/Time.h
            // on apprend l'existence de clock_t CLOCKS_PER_SEC (nombre de tick d'horloge par seconde).
            // (ce qu'on préfère, car time_t donné par time(NULL) est trop grand (seconde))
            // Comme nous souhaitons avoir des millisecondes on multiplie par 1000,
            // puis par 500 pour la latence souhaitée-----

            // Rectificatif. Impossible d'obtenir une précision inférieure à la seconde
            // de plus cela va dépendre de la capacité de la machine à traiter le processus en cours (le jeu)
            // alternative : utiliser POSIX (clock_gettime())
            t_2 = clock();
            if(((unsigned int)(t_2 - t_1) % CLOCKS_PER_SEC) == 0){
                // retirer les ennemis à court de vies
                enemy_add(&dead_e,gm_remove_dead_enemies(&game));
                tower_add(&dead_t,gm_remove_dead_towers(&game));
                // vérifier si la partie est finie, savoir qui a gagné n'est pas important ici

                //
                game.turn += 1;
                // ajouter les ennemis du tour courant (mais d'abord ceux ayants un ou des tours de retard)
                gm_add_entities(&game, waiting_enemies, ENEMY);
                CLI_clear_screen();
                CLI_display_title();printf("\n\n");
                CLI_display_game(game);printf("\n");
                // on fait jouer les tourelles
                gm_entities_play_effects(game, game.towers, TOWER, t_types, e_types);
                // on fait jouer les ennemis
                gm_entities_play_effects(game, game.enemies, ENEMY, t_types, e_types);
                if(gm_is_game_over(game))
                    break;
                // déplacement des ennemis
                gm_move_all(&game);
            }

        }

        if(gm_ennemis_won(game)){
            printf(BORED "Wasted\n" RESET);
        }else if(gm_player_won(game)){
            printf(GREEN "You won !\n" RESET);
        }

        // détail de la partie
        /*printf("\n┌ Wave recap \n"
               "▄ Killed enemies: %d\n"
               "▄ Destroyed towers: %d\n"
                , enemy_count(dead_e), tower_count(dead_t));*/

        enemy_free_all(&dead_e);
        tower_free_all(&dead_t);
        enemy_free_all(&game.enemies);
        tower_free_all(&game.towers);
    }
    return 1;
}



int gm_level_gui(Enemy ** waiting_enemies, DListe e_types, DListe t_types, int money){

    Game game = {NULL, NULL, 0, money};
    int WIDTH, HEIGHT;
    MLV_Image *image;
    DListe enemy_images = NULL;
    DListe tower_images = NULL;
    taille_fenetre(&WIDTH, &HEIGHT);
    // création de la fenêtre
    init_fenetre();
    // On fice le nombre d'image par seconde
    MLV_change_frame_rate( 24 );
    // affichage du fond
    // chargement de la police d'écriture
    float fontsize = 2;
    gui_rel_length(NULL, &fontsize);
    MLV_Font* font = MLV_load_font( "./data/fonts/Old_R.ttf" , (int) fontsize );
    image = MLV_load_image( "./data/img/bg.jpg" );
    // chargement des images des entités
    enemy_images = read_ntt_images(ENEMY, IMG_PATH "enemy_imgs");
    tower_images = read_ntt_images(ENEMY, IMG_PATH "tower_imgs");
    //printf("\nIMAGE: %p \n", ((Entity_img *) tower_images)->image);

    MLV_resize_image( image, 1920, 1080);
    MLV_draw_image( image, 0, 0 );
    MLV_actualise_window();
    // premier affichage du jeu
    GUI_display_game(game, enemy_images, tower_images);

    /* Tower * towers = NULL;
     Tower * t1 = alloue_tower('F', 10, 3, 1, 0);
     tower_add(&towers,t1);
     game_add_entity(&game, &towers, TOWER);
     gm_add_entities(&game, &towers, TOWER);*/

   // ajout du bouton d'affichage de la vague
    DListe btn_list = NULL;
    Button * btn_tmp = NULL;
    float btn_w = 15, btn_h = 7; // en pourcentage des bords de la fenêtre
    float tmp_x, tmp_y;

    gui_rel_length(&btn_w, &btn_h);
    tmp_x = tmp_y = HEADER_PADDING;
    gui_rel_length(&tmp_x, &tmp_y);
    Point ptmp1 = {WIDTH - (int) btn_w - (int) tmp_x, (int) tmp_y};
    Point ptmp2 = { (WIDTH  - (int) tmp_x), (int) (tmp_y + btn_h)};
    btn_tmp = alloue_btn(BTN_QUIT, ptmp1, ptmp2 , NULL, "Quitter",NULL);
    gui_add_btn(&btn_list, btn_tmp);
    MLV_actualise_window();
    //printf("BTN: x1: %d y1: %d, x2: %d y2: %d\n", btn_tmp->p1.x, btn_tmp->p1.y, btn_tmp->p2.x, btn_tmp->p2.y);


    // header
    LEVEL_MENU_ACTION act = -1;
    Button * clicked_btn = NULL;
    while(1){
        MLV_draw_image( image, 0, 0 );
        GUI_display_game(game, enemy_images, tower_images);
        gui_display_btns(btn_list, font);
        if(MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED
            && (clicked_btn = gui_get_clicked_btn(btn_list))
            ){
            if(clicked_btn->type == BTN_QUIT){
                break;
            }
        }
        gui_enhance_btns_over(btn_list, font);
        MLV_actualise_window();
        MLV_delay_according_to_frame_rate();
    }


    act = START_LEVEL;
    if(act == START_LEVEL) {
        // jouer le niveau

        Enemy *dead_e = NULL;
        Tower *dead_t = NULL;
        int star_t = MLV_get_time();
        while (1) {

            if(MLV_get_time() - star_t >= 500){
                // retirer les ennemis à court de vies
                enemy_add(&dead_e, gm_remove_dead_enemies(&game));
                tower_add(&dead_t, gm_remove_dead_towers(&game));
                // vérifier si la partie est finie, savoir qui a gagné n'est pas important ici

                //
                game.turn += 1;
                // ajouter les ennemis du tour courant (mais d'abord ceux ayants un ou des tours de retard)
                gm_add_entities(&game, waiting_enemies, ENEMY);
                //CLI_display_game(game);
                //printf("\n");
                MLV_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, BACKGROUND_COLOR);
                MLV_draw_image( image, 0, 0 );
                GUI_display_game(game, enemy_images, tower_images);
                // on fait jouer les tourelles
                gm_entities_play_effects(game, game.towers, TOWER, t_types, e_types);
                // on fait jouer les ennemis
                gm_entities_play_effects(game, game.enemies, ENEMY, t_types, e_types);
                if (gm_is_game_over(game))
                    break;
                // déplacement des ennemis
                gm_move_all(&game);
                star_t = MLV_get_time();
            }

            gui_display_btns(btn_list, font);
            gui_enhance_btns_over(btn_list, font);
            MLV_actualise_window();
            MLV_delay_according_to_frame_rate();
        }

        enemy_free_all(&dead_e);
        tower_free_all(&dead_t);
        enemy_free_all(&game.enemies);
        tower_free_all(&game.towers);
    }

    //MLV_wait_mouse(NULL, NULL);
    //
    // Ferme la fenêtre
    //
    MLV_free_window();
    MLV_free_image( image );
    return 1;
}
