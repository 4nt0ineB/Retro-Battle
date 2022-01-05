//
// Created by antoine on 04/01/2022.
//

#include "button.h"


Button * alloue_btn(BTN_TYPE type, Point p1, Point p2, MLV_Image * image, char * text, void * data, MLV_Color color){
    Button * btn = (Button *) malloc(sizeof(Button));
    if(btn){
        btn->type = type;
        btn->p1 = p1;
        btn->p2 = p2;
        btn->image = image;
        btn->text = text;
        btn->data = data;
        btn->color = color;
    }
    return btn;
}

int btn_coord_is_over(Button btn, int x, int y){
    if(btn.p1.x <= x && x <= btn.p2.x
       && btn.p1.y <= y && y <= btn.p2.y)
        return 1;
    return 0;
}

void free_btn(Button * btn){

    if(btn->data)
        free(btn->data);
    free(btn);
}