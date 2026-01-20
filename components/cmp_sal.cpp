#include "cmp_sal.h"


cmp_sal::cmp_sal(cmp_basic* parent):cmp_basic(parent){

}

cmp_sal::~cmp_sal(){

}


int cmp_sal::on_event(cmp_basic *s, hmmc_event *e){
    return 0;
}

void cmp_sal::on_draw(int anime) {
    if(is_load){
        draw_clear(511);
        is_load = false;
    }else{
        draw_clear(REGULAR);
    }
    draw_select_buffer(REGULAR);
    draw_text("Load here or not", 0);
}
int cmp_sal::on_press(int c, int k){
    if(c) return 1;
    return 0;
}
int cmp_sal::on_tick(int anime){
    return 0;
}
