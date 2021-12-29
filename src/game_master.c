//
// Created by antoine on 26/12/2021.
//

#include "../headers/game_master.h"


int gm_move(Game * game, void * entity, ENTITY ntt){
    if(ntt == ENEMY){
        // déplacement de "speed" case vers la gauche pour l'ennemi
        return game_move_entity(game, entity, ntt, ((Enemy *)entity)->line, ((Enemy *)entity)->position - ((Enemy *)entity)->speed);
    }
    return 0;
}

int gm_move_all(Game * game){
    // mettre à jour la position des ennemis
    Enemy * tmp = game->enemies;
    ENTITY ntt = ENEMY;
   // CLI_display_full_enemy(*tmp);

    while(tmp){
        gm_move(game, tmp, ntt);
        tmp = tmp->next;
    }
    return 1;
}

int gm_add_entities(Game * game, void * entities, ENTITY ntt, int turn){
    if(ntt == ENEMY){
        Enemy ** entity_tmp = (Enemy **) entities;
        if(!*entity_tmp) return 0;
        Enemy ** entity_tmp_next = &(*entity_tmp)->next;
        // on veut placer les ennemis dont le tour d'apparition a été passé
        // ou dont c'est le tour d'apparition (file d'attente)
        if ((*entity_tmp)->turn > turn || !game_add_entity(game, entity_tmp, ntt))
            // échec d'ajout, on passe au suivant
            entity_tmp = entity_tmp_next;
        // si succès d'ajout, on ne fait rien (l'ennemi courant est l'ancien suivant)
        gm_add_entities(game, entity_tmp, ntt, turn);
    } else if(ntt == TOWER) {
        Tower ** entity_tmp = (Tower **) entities;
        if(!*entity_tmp) return 0;
        Tower ** entity_tmp_next = &(*entity_tmp)->next;
        if (!(game_add_entity(game, entity_tmp, ntt)))
            entity_tmp = entity_tmp_next;
        gm_add_entities(game, entity_tmp, ntt, turn);
    }
    return 1;
}

int gm_entity_play_effect(Game game, void * entity, ENTITY ntt, Effect effect){
    // on récupère les targets
    if(effect.target != 0 && effect.target != 2) return 0;
    int pos[MAX_LINE+1][MAX_LINE_LENGTH+1] = {0};
    DListe towers = NULL;
    DListe enemies = NULL;
    DListe cel = NULL;
    Enemy * etmp = NULL;
    Tower * ttmp = NULL;


    int i, j = 0;
    int entity_line, entity_pos = 0;
    int range, zrange = 0;
    if(effect.front){

    }else{
        // tourelles
        ttmp = game.towers;
        while(ttmp){
            if(ntt == ENEMY){
                entity_line = ((Enemy *) entity)->line;
                entity_pos = ((Enemy *) entity)->position;
            }else if(ntt == TOWER){
                entity_line = ((Tower *) entity)->line;
                entity_pos = ((Tower *) entity)->position;
            }
            if(ttmp->line == entity_line && ttmp->position == entity_pos){
                ttmp = ttmp->next;
                continue;
            }
            zrange = effect_entity_in_circle_range(
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
        etmp = game.enemies;
        while(etmp){
            if(ntt == ENEMY){
                entity_line = ((Enemy *) entity)->line;
                entity_pos = ((Enemy *) entity)->position;
            }else if(ntt == TOWER){
                entity_line = ((Tower *) entity)->line;
                entity_pos = ((Tower *) entity)->position;
            }
            if(etmp->line == entity_line && etmp->position == entity_pos){
                etmp = etmp->next;
                continue;
            }
            zrange = effect_entity_in_circle_range(
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
                DListe_ajouter_fin(&towers, cel);
            }
            etmp = etmp->next;
        }
    }

    DListe m = towers;
    while(m){
        //printf("%p->", m->element);
        m = m->suivant;
    }
    //printf("\n");

    switch (effect.type) {
        case DAMAGE:
            printf("HELLO !! \n");
            //game_effect_damage(void * entity, ntt, effects);
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