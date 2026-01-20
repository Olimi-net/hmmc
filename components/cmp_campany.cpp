#include "cmp_campany.h"


cmp_campany::cmp_campany(cmp_basic* parent):cmp_basic(parent){
    search_shema();
}

cmp_campany::~cmp_campany(){

}

void cmp_campany::search_shema(){
    //get all files in directory
}

int cmp_campany::on_event(cmp_basic *s, hmmc_event *e){
    return 0;
}

void cmp_campany::on_draw(int anime) {
    if(is_load){
        draw_clear(511);
        is_load = false;
    }else{
        draw_clear(REGULAR);
    }
    draw_select_buffer(REGULAR);
}

int cmp_campany::on_press(int c, int k){
    return 0;
}

int cmp_campany::on_tick(int anime){
    return 0;
}
