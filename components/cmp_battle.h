#pragma once
#include "cmp_basic.h"
#include "../battle/battle.h"

class cmp_battle : public cmp_basic{
    public:
        cmp_battle(cmp_basic* parent, group* gl, group* gr, float df, int bm);
        ~cmp_battle();
        int on_event(cmp_basic *s, hmmc_event *e) override;
        void on_draw(int anime)  override;;
        int on_press(int c, int k)  override;;
        int on_tick(int anime)  override;;

    private:
        battle* cur_battle;
};
