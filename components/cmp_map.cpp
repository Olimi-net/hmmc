#include "cmp_map.h"


cmp_map::cmp_map(cmp_basic* parent):cmp_basic(parent),hmmc_callback(){
    map = nullptr;

}

cmp_map::~cmp_map(){
    if(map != nullptr)
        delete map;
}

int cmp_map::on_event(cmp_basic *s, hmmc_event *e){
    return 0;
}

int cmp_map::on_callback(hmmc_event *e, int a){
    if(e == nullptr){
        if(a > 900){
            draw_echo(a);
        }
        return 0;
    }
    if(e->id == 'G'){ //city
        //growl for week
    }
    return 0;
}

void cmp_map::on_draw(int anime) {
    if(is_load){
        draw_clear(511);
        is_load = false;
    }else{
        draw_clear(BACK | REGULAR);
    }
    draw_select_buffer(BACK);

}
int cmp_map::on_press(int c, int k){
    return 0;
}
int cmp_map::on_tick(int anime){
    return 0;
}
