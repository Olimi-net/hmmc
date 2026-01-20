#pragma once
#include "cmp_basic.h"
#include "../world/world.h"

class cmp_map : public cmp_basic, public hmmc_callback{
    public:
        cmp_map(cmp_basic* parent);
        ~cmp_map();
        int on_event(cmp_basic *s, hmmc_event *e) override;
        void on_draw(int anime)  override;;
        int on_press(int c, int k)  override;;
        int on_tick(int anime)  override;
        int on_callback(hmmc_event *e, int a) override;
        int init(int map_id, const char* scen);
    private:
        world* map;
};
