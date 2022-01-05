//
// Created by antoine on 03/01/2022.
//

#ifndef L2_PROJET_C_GUI_H
#define L2_PROJET_C_GUI_H
#include <stdio.h>
#include <MLV/MLV_all.h>

#include "../settings/settings.h"
#include "../game_obj/game.h"
#include "./gui_obj/button.h"

// http://www-igm.univ-mlv.fr/~boussica/mlv/api/French/html/index.html

#define TAILLE_RELATIVE 5
#define BACKGROUND_COLOR MLV_COLOR_MIDNIGHTBLUE

#define MAX_W 1920
#define MAX_H 1080
#define HEADER 20
#define CASE_PADDING 0
#define CASE_MARGING 0
#define BOARD_PADDING 2
#define HEADER_PADDING 1

/**
 * Variable globale des dimensions
 * d'affichage du jeu.
 */
struct GUI_dimension {
    int WIDTH;
    int HEIGHT;
};


/*
 * |───────────────────────────|
 * |          Header           |
 * |───────────────────────────|
 * |        Game board         |
 * |                           |
 * |───────────────────────────|
 */


/**
 * Renvoie la taille de la fenêtre
 * @param width
 * @param height
 */
void taille_fenetre(int * width, int * height);

/**
 * Initialise la fenêtre
 */
void init_fenetre();

/**
 * Renvoie la taille d'une case
 * d'entité sur le plateau de jeu
 * @return
 */
int w_case();

/**
 * Renvoie les coordonnées
 * d'origine du plateau de jeu
 * @param x
 * @param y
 */
void game_board_origin(int * x, int * y);

/**
 * Renvoie la valeur en pixel
 * pour une distance en pourcentage
 * @param width pourcentage d'une longueur
 * @param height pourcentage d'une hauteur
 */
void gui_rel_length(float * width, float * height);

/**
 * Renvoie un bouton de dimension
 * relative à la taille de la fenêtre
 * pour des données passées en paramètre
 * @param x position sup gauche x
 * @param y position sup gauche y
 * @param w longueur
 * @param h hauteur
 * @param btn_type type du bouton
 * @param image texte à afficher
 * @param text image à afficher
 * @param data donnée à porter
 * @param color couleur du bouton
 * @return
 */
Button * btn_create_responsive(int x, int y, float w, float h, BTN_TYPE btn_type, MLV_Image * image, char * text, void * data, MLV_Color color);

/**
 * Ajoute un bouton à la fin d'une DListe
 * @param btn_list
 * @param btn
 * @return
 */
int gui_add_btn(DListe * btn_list, Button * btn);

/**
 * Affiche un bouton
 * @param btn
 * @param font police d'affichage du texte
 */
void gui_display_btn(Button btn,  MLV_Font *font);

/**
 * Affiche une liste de boutons
 * @param btn_list
 * @param font police d'affichage du texte
 */
void gui_display_btns(DListe btn_list,  MLV_Font *font);

/**
 * Met en surbrillance le bouton
 * @param btn
 * @param font
 */
void gui_enhance_btn_over(Button btn,  MLV_Font *font);

/**
 * Met en surbrillance le bouton survolé par la souris
 * @param btn
 * @param font
 */
void gui_enhance_btns_over(DListe btn,  MLV_Font *font);

/**
 * Renvoie l'adresse du bouton aux coordonnées
 * de la souris si elle le survole
 * @param btn_list une DListe de boutons
 * @return
 */
Button * gui_get_clicked_btn(DListe btn_list);

/**
 * Affiche une entité
 * @param entity l'entité
 * @param ntt le type de l'entité
 * @param ntt_type Dliste des types  (du type d'entité donné via ntt)
 * @param ntt_img Dliste des images par type (du type d'entité donné via ntt)
 * @param x pos x d'affichage (sup gauche)
 * @param y pos y d'affichage (sup gauche)
 */
void gui_display_entity(void * entity, ENTITY ntt, Entity_type ntt_type, Entity_img ntt_img, int x, int y);

/**
 * Renvoie une DListe de boutons pour les type de tourelles
 * disponibles (avec image)
 * @param t_types
 * @param ntt_img
 * @return
 */
DListe gui_create_tower_selection(DListe t_types,  DListe ntt_img);

/**
 * Met à jour les boutons pour rendre
 * l'affichage du choix des tourelles dynamique
 * @param tower_btn liste des boutons de choix de tourelle
 * @param x
 * @param y
 * @param pick_menu_origin
 */
void gui_update_tower_selection(DListe tower_btn, int x, int y, int pick_menu_origin);

/**
 * Renvoie une DListe de boutons transparent
 * représentant la grille des positions du plateau de jeu
 * @return
 */
DListe gui_create_board_box_btns();

/**
 * Libère une DListe de boutons
 * @param btns
 */
void gui_free_btn_list(DListe * btns);

/**
 * Libère une liste DListe d'images
 * @param img_list
 */
void gui_free_img_list(DListe * img_list);

/**
 * Affiche la vague du niveau
 * @param game
 * @param waiting_enemies
 * @param e_types
 * @param enemy_images
 */
void GUI_display_wave(Game game, Enemy * waiting_enemies, DListe e_types, DListe enemy_images);

/**
 * Affiche le jeu
 * @param game
 */
void GUI_display_game(Game game, DListe e_types, DListe enemy_images, DListe t_types, DListe tower_images);

#endif //L2_PROJET_C_GUI_H
