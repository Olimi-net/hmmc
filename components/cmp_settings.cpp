#include "cmp_settings.h"


cmp_settings::cmp_settings(cmp_basic* parent):cmp_basic(parent){

}

cmp_settings::~cmp_settings(){

}


int cmp_settings::on_event(cmp_basic *s, hmmc_event *e){
    return 0;
}

void cmp_settings::on_draw(int anime) {
    if(is_load){
        draw_clear(511);
        is_load = false;
    }else{
        draw_clear(REGULAR);
    }
    draw_select_buffer(REGULAR);
    draw_text("Settings here or not", 0);
}

int cmp_settings::on_press(int c, int k){
    if(c) return 1;
    return 0;
}

int cmp_settings::on_tick(int anime){
    return 0;
}
