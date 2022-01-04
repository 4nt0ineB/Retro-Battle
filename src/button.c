//
// Created by antoine on 04/01/2022.
//

#include "../headers/button.h"


Button * alloue_btn(BTN_TYPE type, Point p1, Point p2, void * data){
    Button * btn = (Button *) malloc(sizeof(Button));
    if(btn){
        btn->type = type;
        btn->p1 = p1;
        btn->p2 = p2;
        btn->data = data;
    }
    return btn;
}

void free_btn(Button * btn){
    free(btn->data);
    free(btn);
}