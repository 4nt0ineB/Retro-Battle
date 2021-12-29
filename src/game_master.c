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

int gm_entity_play_effect(Game game, void * entity, ENTITY ntt, Effect effect){

    // on récupère les targets
    if(effect.target < 0 || effect.target > 2) return 0;
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
        return 0;
        entity_line = ((Tower *) entity)->line;
        entity_pos = ((Tower *) entity)->position;
    }


    if(effect.front){ /* @todo détailler pourquoi on a une séparation (profondeur du front etc...)*/
        // enemy
        etmp = game.enemies;
        hr = ((Enemy *) entity)->line - effect.h_range;
        br = ((Enemy *) entity)->line + effect.b_range;
        Enemy * tmp_line = NULL;
        Enemy * tmp_pos = NULL;
        // on itère sur les lignes souhaitées
        for(i = hr; i <= br ; i++){
            // on récupère la ligne
            if(!(tmp_line = enemy_get_first_in_line(game.enemies, i)))
                continue;
            // on souhaite les "effect.front" premières entités de la ligne de front ennemie
            tmp_pos = tmp_line;
            for(j = 0; j < effect.front; j++){
                if(tmp_pos->line == entity_line && tmp_pos->position == entity_pos){
                    tmp_pos = tmp_pos->next_line;
                    continue;
                }
                if(!pos[tmp_pos->line][tmp_pos->position]){
                    cel = alloue_DCellule(&(*tmp_pos));
                    pos[tmp_pos->line][tmp_pos->position] = 1;
                    DListe_ajouter_fin(&enemies, cel);
                }
            }
        }
    }else{
        // tourelles
        ttmp = game.towers;

        while(ttmp){
            if(ttmp->line == entity_line && ttmp->position == entity_pos){
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
            if(!pos[ttmp->line][ttmp->position] && (zrange || range)){
                cel = alloue_DCellule(&(*ttmp));
                pos[ttmp->line][ttmp->position] = 1;
                DListe_ajouter_fin(&towers, cel);
            }
            ttmp = ttmp->next;
        }
        // ennemis
        /*etmp = game.enemies;
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
        }*/
    }
    /*DListe m = towers;
    while(m){
        printf("%p->", m->element);
        m = m->suivant;
    }*/
    printf("\n");
    gm_apply_effect_on_entities(enemies, ntt, effect);
    gm_apply_effect_on_entities(towers, ntt, effect);

    free(towers);
    free(enemies);
    return 1;
}

int gm_apply_effect_on_entities(DListe entities, ENTITY ntt, Effect effect){
    DListe tmp = entities;
    while(tmp){
        gm_apply_effect_on_entity(tmp->element, ntt, effect);
        tmp = tmp->suivant;
    }
    return 0;
}

int gm_apply_effect_on_entity(void * entity, ENTITY ntt, Effect effect){
    switch (effect.type) {
        case DAMAGE:
            game_effect_damage(entity, ntt, effect);
            break;
        case HEAL:
            break;
        case SLOW:
            break;
        case SPEED:
            break;
        case FREEZE:
            break;
        case SWITCHPLACE:
            break;
    }
    return 1;
}

int gm_entity_play_effects(Game game, void * entity, ENTITY ntt, DListe entity_types){
    DListe type = entity_types;
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
        gm_entity_play_effect(game, entity, ntt, *(Effect *) effect->element);
        effect = effect->suivant;
    }
    return 1;
}

int gm_entities_play_effects(Game game, void * entity, ENTITY ntt, DListe entity_types){
    void * tmp = entity;
    while(tmp){
        gm_entity_play_effects(game, tmp, ntt, entity_types);
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