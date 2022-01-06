//
// Created by Antoine Bastos on 26/12/2021.
//

#include "game_master.h"


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
                for (j = 0; tmp_pos && j < effect.front;) {
                    if (tmp_pos->line == entity_line && tmp_pos->position == entity_pos) {
                        tmp_pos = tmp_pos->next_line;
                        continue;
                    }
                    // la tourelle ne peut pas attaquer derrières elles
                    if(ntt == TOWER && tmp_pos->position <= entity_pos){
                        tmp_pos = tmp_pos->next_line;
                        continue;
                    }
                    if (!pos[tmp_pos->line][tmp_pos->position]) {
                        cel = alloue_DCellule(&(*tmp_pos));
                        pos[tmp_pos->line][tmp_pos->position] = 1;
                        DListe_ajouter_fin(&enemies, cel);
                        j++;
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
                // l'ennemi ne peut pas attaquer derrières elles
                if(ntt == ENEMY && ttmp->position >= entity_pos){
                    // on skip
                } else if(tmp_grid[ttmp->line][ttmp->position]){
                    cel = alloue_DCellule(&(*ttmp));
                    tmp_grid[ttmp->line][ttmp->position] = 0;
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
        if((*tmp).life <= 0)
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

int gm_level_cli(char ** options, Enemy ** waiting_enemies, DListe e_types, DListe t_types, int money){
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

int gm_level_gui(char ** options, Enemy ** waiting_enemies, DListe e_types, DListe t_types, int money){

    Game game = {NULL, NULL, 0, money};
    int WIDTH, HEIGHT;
    taille_fenetre(&WIDTH, &HEIGHT);

    // création de la fenêtre
    init_fenetre();
    if( MLV_init_audio() ){
        fprintf(
                stderr,
                "Could not load audio lib."
                            );
        exit(1);
    }
    // On fice le nombre d'image par seconde
    MLV_change_frame_rate(24);
    // chargement de la police d'écriture
    float fontsize = 1.5f;
    gui_rel_length(NULL, &fontsize);
    // chargement des images des entités
    DListe enemy_images = read_ntt_images(ENEMY, IMG_PATH "enemy_imgs");
    DListe tower_images = read_ntt_images(ENEMY, IMG_PATH "tower_imgs");
    MLV_Font* font = MLV_load_font( "./data/fonts/Old_R.ttf" , (int) fontsize );
    MLV_Font* bigger_font = MLV_load_font( "./data/fonts/Old_R.ttf" , (int) fontsize * 3 );
    //
    MLV_Image *image = MLV_load_image( "./data/img/bg.jpg" );
    MLV_resize_image( image, WIDTH, HEIGHT);
    MLV_draw_image( image, 0, 0 );
    MLV_actualise_window();
    // premier affichage du jeu
    GUI_display_game(game, e_types, enemy_images, t_types, tower_images);
    //
    // music: ref: royalty-free-music-lost-in-paradise-synthwave
    MLV_Music* music = MLV_load_music( "./data/sounds/music/music1.ogg" );
    MLV_play_music( music, 0.5f, -1 );
    MLV_Sound* selector = MLV_load_sound( "./data/sounds/effects/selector.ogg" );
    MLV_Sound* put_tower = MLV_load_sound( "./data/sounds/effects/put_tower.ogg" );
    MLV_Sound* remove_tower = MLV_load_sound( "./data/sounds/effects/remove_tower.ogg" );

    // ajout du bouton d'affichage de la vague
    DListe btn_list = NULL; // liste des boutons dans la fenêtre
    DListe btn_pick_list = gui_create_tower_selection(t_types, tower_images);
    DListe btn_box_list =  gui_create_board_box_btns(&btn_list);

    Button * btn_tmp = NULL;
    float tmp_x, tmp_y;
    float tmp_x2, tmp_y2;
    // bouton quitter
    tmp_x = tmp_y = HEADER_PADDING;
    gui_rel_length(&tmp_x, &tmp_y);
    tmp_x2 = 15; tmp_y2 = 7;
    gui_rel_length(&tmp_x2, &tmp_y2);
    btn_tmp = btn_create_responsive(WIDTH - (int) tmp_x - (int) tmp_x2, (int) tmp_y, 15, 7, BTN_QUIT, NULL, "Quit", NULL, MLV_COLOR_DARK_RED);
    gui_add_btn(&btn_list, btn_tmp);
    // bouton show wave
    tmp_x = tmp_y = HEADER_PADDING;
    gui_rel_length(&tmp_x, &tmp_y);
    btn_tmp = btn_create_responsive((int) ((float) WIDTH * 0.65) , (int) tmp_y, 15, 7, BTN_SHOW_WAVE, NULL, "Show wave", NULL, MLV_COLOR_GREY);
    gui_add_btn(&btn_list, btn_tmp);
    // bouton start
    tmp_x = tmp_y = HEADER_PADDING;
    gui_rel_length(&tmp_x, &tmp_y);
    btn_tmp = btn_create_responsive((int) ((float) WIDTH * 0.55 ), (int) tmp_y, 7, 7, BTN_START, NULL, "Start", NULL, MLV_COLOR_GREY);
    gui_add_btn(&btn_list, btn_tmp);
    // bouton decrement
    tmp_x = tmp_y = HEADER_PADDING;
    gui_rel_length(&tmp_x, &tmp_y);
    btn_tmp = btn_create_responsive((int) tmp_x, (int) tmp_y, 2, (float) (w_case()*100)/MAX_H, BTN_DECR_PICK, NULL, "<", NULL, MLV_COLOR_DARK_CYAN);
    gui_add_btn(&btn_list, btn_tmp);
    // bouton increment
    tmp_x = tmp_y = HEADER_PADDING;
    gui_rel_length(&tmp_x, &tmp_y);
    btn_tmp = btn_create_responsive((WIDTH * HEADER_PADDING*14.5f)/100, (int) tmp_y, 2, (float) (w_case()*100)/MAX_H, BTN_INCR_PICK, NULL, ">", NULL, MLV_COLOR_DARK_CYAN);
    gui_add_btn(&btn_list, btn_tmp);

    MLV_actualise_window();
    //printf("BTN: x1: %d y1: %d, x2: %d y2: %d\n", btn_tmp->p1.x, btn_tmp->p1.y, btn_tmp->p2.x, btn_tmp->p2.y);

    char strmoney[100] = {0};
    char textmoney[200] = {0};
    sprintf(strmoney, "%d", game.money);
    strcpy(textmoney, "Money ");
    strcat(textmoney, strmoney);
    LEVEL_MENU_ACTION act = -1;
    Button * clicked_btn = NULL;
    Tower * tower_add_tmp = NULL;
    ENTITY ntt_tmp;
    int selected_tower_type = *(int *) ((Button *) btn_pick_list->element)->data;
    int pick_menu_origin = 0;
    int max_pick_menu = DListe_nbr_elements(btn_pick_list);
    int click_cool_down = 100;
    int click_t = MLV_get_time();

    //
    // Avant le lancement du jeu.
    // Pose de tourelles / voir la vague
    //
    while(1){
        MLV_draw_image( image, 0, 0 );
        GUI_display_game(game, e_types, enemy_images, t_types, tower_images);
        gui_display_btns(btn_list, font);
        gui_display_btns(btn_pick_list, font);
        gui_update_tower_selection(btn_pick_list,
                                   (WIDTH * HEADER_PADDING*3)/100,
                                   (HEIGHT * HEADER_PADDING)/100, pick_menu_origin);
        gui_enhance_btns_over(btn_list, font);
        gui_enhance_btns_over(btn_pick_list, font);
        // money
        MLV_draw_text_with_font(
                (WIDTH * HEADER_PADDING*25.5f)/100,
                tmp_y*2,
                textmoney,
                font, MLV_COLOR_YELLOW
        );
        MLV_update_window();

        if(MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED) {

            if ((clicked_btn = gui_get_clicked_btn(btn_list))) {
                if (clicked_btn->type == BTN_START) {
                    MLV_play_sound( selector, 1.0 );
                    act = START_LEVEL;
                    break;
                } else if (clicked_btn->type == BTN_QUIT) {
                    MLV_play_sound( selector, 1.0 );
                    break;
                } else if (clicked_btn->type == BTN_INCR_PICK) {
                    MLV_play_sound( selector, 1.0 );
                    pick_menu_origin += pick_menu_origin > max_pick_menu - 3 ? 0 : 1;
                } else if (clicked_btn->type == BTN_DECR_PICK && (MLV_get_time() - click_t) > click_cool_down) {
                    MLV_play_sound( selector, 1.0 );
                    click_t = MLV_get_time();
                    pick_menu_origin -= pick_menu_origin == 0 ? 0 : 1;
                }else if(clicked_btn->type == BTN_SHOW_WAVE && (MLV_get_time() - click_t) > click_cool_down){
                    MLV_play_sound( selector, 1.0 );
                    click_t = MLV_get_time();
                    DListe tmp_btn_list = NULL;
                    Button * back = NULL;
                    tmp_x = tmp_y = HEADER_PADDING;
                    gui_rel_length(&tmp_x, &tmp_y);
                    back = btn_create_responsive((int) ((float) WIDTH * 0.65) , (int) tmp_y, 15, 7, BTN_SHOW_WAVE, NULL, "Back", NULL, MLV_COLOR_GREY);
                    gui_add_btn(&tmp_btn_list, back);
                    while(1){
                        MLV_draw_image( image, 0, 0 );
                        gui_display_btns(tmp_btn_list, font);
                        gui_enhance_btns_over(tmp_btn_list, font);
                        GUI_display_wave(game, *waiting_enemies, e_types, enemy_images);
                        MLV_update_window();
                        MLV_delay_according_to_frame_rate();
                        if(MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED) {
                            if ((clicked_btn = gui_get_clicked_btn(tmp_btn_list)) && (MLV_get_time() - click_t) > 500){
                                MLV_play_sound( selector, 1.0 );
                                break;
                            }

                        }

                    }
                }

            } else if ((clicked_btn = gui_get_clicked_btn(btn_pick_list))) {
                if (clicked_btn->type == BTN_PICK_TOWER) {
                    MLV_play_sound( selector, 1.0 );
                    selected_tower_type = *((int *) clicked_btn->data);
                }
            } else if ((clicked_btn = gui_get_clicked_btn(btn_box_list))) {
                // ajout de la tourelle sur le plateau
                tower_add_tmp = alloue_tower(selected_tower_type, 0, ((Point *) clicked_btn->data)->x,
                                             ((Point *) clicked_btn->data)->y, 0);
                init_towers(tower_add_tmp, t_types);
                if(game.money - tower_add_tmp->price < 0 ){
                    free(tower_add_tmp);
                }else{
                    game_incr_money(&game, -1 * tower_add_tmp->price);
                    if (!game_add_entity(&game, &tower_add_tmp, TOWER)) {
                        game_incr_money(&game, tower_add_tmp->price);
                        free(tower_add_tmp);
                    }else{
                        MLV_play_sound( put_tower, 0.5f );
                    }
                    sprintf(strmoney, "%d", game.money);
                    strcpy(textmoney, "Money ");
                    strcat(textmoney, strmoney);
                }

            }
        }else if(MLV_get_mouse_button_state(MLV_BUTTON_RIGHT) == MLV_PRESSED){

         if((clicked_btn = gui_get_clicked_btn(btn_box_list))){
             // suppression de la tourelle du plateau
             tower_add_tmp = game_get_entity_by_position(game, ((Point *) clicked_btn->data)->x, ((Point *) clicked_btn->data)->y, &ntt_tmp);
             tower_extract(&game.towers, tower_add_tmp);
             if(tower_add_tmp){
                 MLV_play_sound( remove_tower, 0.5f );

                 game_incr_money(&game, tower_add_tmp->price);
             }
             sprintf(strmoney, "%d", game.money);
             strcpy(textmoney, "Money ");
             strcat(textmoney, strmoney);
         }

       }

        MLV_delay_according_to_frame_rate();
    }

    //
    // Lancement du jeu
    //
    if(act == START_LEVEL) {
        DListe cel_btn = NULL;
        // on enlève le bouton show wave
        cel_btn = DListe_extract(&btn_list, btn_list->suivant);
        gui_free_btn_list(&cel_btn);
        // on enlève le bouton start
        cel_btn = DListe_extract(&btn_list, btn_list->suivant);
        gui_free_btn_list(&cel_btn);
        // jouer le niveau

        Enemy *dead_e = NULL;
        Tower *dead_t = NULL;
        int star_t = MLV_get_time();
        while (1) {
            if(MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED) {
                if ((clicked_btn = gui_get_clicked_btn(btn_list))) {
                    if (clicked_btn->type == BTN_QUIT) {
                        MLV_play_sound(selector, 1.0f);
                        break;
                    } else if (clicked_btn->type == BTN_INCR_PICK) {
                        MLV_play_sound(selector, 1.0f);
                        pick_menu_origin += pick_menu_origin > max_pick_menu - 3 ? 0 : 1;
                    } else if (clicked_btn->type == BTN_DECR_PICK && (MLV_get_time() - click_t) > click_cool_down) {
                        MLV_play_sound(selector, 1.0f);
                        click_t = MLV_get_time();
                        pick_menu_origin -= pick_menu_origin == 0 ? 0 : 1;
                    }
                } else if ((clicked_btn = gui_get_clicked_btn(btn_pick_list))) {
                    if (clicked_btn->type == BTN_PICK_TOWER) {
                        MLV_play_sound(selector, 1.0f);
                        selected_tower_type = *((int *) clicked_btn->data);
                    }
                } else if ((clicked_btn = gui_get_clicked_btn(btn_box_list))) {
                    // ajout de la tourelle sur le plateau
                    tower_add_tmp = alloue_tower(selected_tower_type, 0, ((Point *) clicked_btn->data)->x,
                                                 ((Point *) clicked_btn->data)->y, 0);
                    init_towers(tower_add_tmp, t_types);
                    if (game.money - tower_add_tmp->price < 0) {
                        free(tower_add_tmp);
                    } else {
                        game_incr_money(&game, -1 * tower_add_tmp->price);
                        if (!game_add_entity(&game, &tower_add_tmp, TOWER)) {
                            game_incr_money(&game, tower_add_tmp->price);
                            free(tower_add_tmp);
                        } else {
                            MLV_play_sound(put_tower, 0.5f);
                        }
                        sprintf(strmoney, "%d", game.money);
                        strcpy(textmoney, "Money ");
                        strcat(textmoney, strmoney);
                    }

                }
            }

            if(MLV_get_time() - star_t >= 1000){
                // retirer les ennemis à court de vies
                enemy_add(&dead_e, gm_remove_dead_enemies(&game));
                tower_add(&dead_t, gm_remove_dead_towers(&game));
                //
                game.turn += 1;
                // ajouter les ennemis du tour courant (mais d'abord ceux ayants un ou des tours de retard)
                gm_add_entities(&game, waiting_enemies, ENEMY);
                //CLI_display_game(game);
                //printf("\n");

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

            MLV_draw_image( image, 0, 0 );
            GUI_display_game(game, e_types, enemy_images, t_types, tower_images);
            gui_display_btns(btn_list, font);
            gui_display_btns(btn_pick_list, font);
            gui_update_tower_selection(btn_pick_list,
                                       (WIDTH * HEADER_PADDING*3)/100,
                                       (HEIGHT * HEADER_PADDING)/100, pick_menu_origin);
            gui_enhance_btns_over(btn_list, font);
            gui_enhance_btns_over(btn_pick_list, font);
            // money
            MLV_draw_text_with_font(
                    (WIDTH * HEADER_PADDING*25.5f)/100,
                    tmp_y*2,
                    textmoney,
                    font, MLV_COLOR_YELLOW
            );
            MLV_actualise_window();
            MLV_delay_according_to_frame_rate();
        }

        //
        // Fin de partie
        //
        int txt_w = 0;
        int txt_h = 0;

        cel_btn = NULL;
        // on enlève le bouton à gauche de la selection de tourelles
        cel_btn = DListe_extract(&btn_list, btn_list->suivant);
        gui_free_btn_list(&cel_btn);
        // on enlève le bouton à droite de la selection de tourelles
        cel_btn = DListe_extract(&btn_list,
                                 btn_list->suivant);
        gui_free_btn_list(&cel_btn);

        while(1){
            MLV_draw_image( image, 0, 0 );
            GUI_display_game(game, e_types, enemy_images, t_types, tower_images);
            gui_display_btns(btn_list, font);
            gui_enhance_btns_over(btn_list, font);
            if(MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED) {
                if ((clicked_btn = gui_get_clicked_btn(btn_list))) {
                    break;
                }
            }

            if(gm_ennemis_won(game)){
                MLV_get_size_of_text_with_font("WASTED", &txt_w, &txt_h, bigger_font);
                MLV_draw_text_with_font(
                        WIDTH/2-(txt_w/2),
                        HEIGHT * 0.10f,
                        "WASTED",
                        bigger_font, MLV_COLOR_RED
                );
            }else if(gm_player_won(game)){
                MLV_get_size_of_text_with_font("WASTED", &txt_w, &txt_h, bigger_font);
                MLV_draw_text_with_font(
                        WIDTH/2-(txt_w/2),
                        HEIGHT * 0.10f,
                        "You won",
                        bigger_font, MLV_COLOR_GREEN
                );
            }
            MLV_actualise_window();
            MLV_delay_according_to_frame_rate();
        }
        MLV_actualise_window();
        enemy_free_all(&dead_e);
        tower_free_all(&dead_t);
    }

    gui_free_btn_list(&btn_list);
    gui_free_btn_list(&btn_pick_list);
    gui_free_btn_list(&btn_box_list);
    gui_free_img_list(&enemy_images);
    gui_free_img_list(&tower_images);
    //
    enemy_free_all(&game.enemies);
    tower_free_all(&game.towers);
    //MLV_wait_mouse(NULL, NULL);
    //
    // Ferme la fenêtre
    //

    MLV_stop_all_sounds();
    MLV_free_sound( selector);
    MLV_free_sound(put_tower);
    MLV_free_sound(remove_tower);
    MLV_stop_music();
    MLV_free_music(music);

    MLV_free_audio();
    MLV_free_image(image);
    MLV_free_font(font);
    MLV_free_font(bigger_font);
    MLV_free_window();
    return 1;
}
