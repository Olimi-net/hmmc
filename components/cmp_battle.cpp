#include "cmp_battle.h"
#include "../modules/group.h"
#include "../modules/map_const.h"

/**
bm = is city && wall ? CITY : ATTACK
*/
cmp_battle::cmp_battle(cmp_basic* parent, group* gl, group* gr, float df, int bm):cmp_basic(parent){
    this->cur_battle = new battle(gl, gr, df, bm);
}

cmp_battle::~cmp_battle(){
    delete cur_battle;
}

int cmp_battle::on_event(cmp_basic *s, hmmc_event *e){
    return 0;
}

void cmp_battle::on_draw(int anime) {
    if(is_load){
        draw_clear(511);
        is_load = false;
    }
    //if show_back
    // cur_back->draw(anime);
    cur_battle->draw(anime);
}

int cmp_battle::on_press(int c, int k){
    cur_battle->on_press(c, k);
    return 0;
}

int cmp_battle::on_tick(int anime){
    cur_battle->on_tick(anime);
    return 0;
}
