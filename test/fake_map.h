#pragma once
#include "../components/cmp_basic.h"
#include "../modules/city.h"
#include "../modules/group.h"

const int FAKE_MAP_BATTLE = 32;
const int FAKE_MAP_CITY = 64;

class fake_map : public cmp_basic, public hmmc_callback {
    public:
        fake_map(cmp_basic* parent, int flags);
        ~fake_map();
        int on_event(cmp_basic *s, hmmc_event *e) override;
        int on_callback(hmmc_event *e, int a) override;
        void on_draw(int anime)  override;;
        int on_press(int c, int k)  override;;
        int on_tick(int anime)  override;;
        void init(int flags);
        void clear();

    private:
        group* gl;
        group* gr;
        hero* hl;
        hero* hr;
        cmp_basic* current_cmp;
        city* cur_city;
        int flag;
        int city_flag;
        pair* res;
        void battle_cells();
};
