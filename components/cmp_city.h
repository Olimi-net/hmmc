#pragma once
#include "cmp_basic.h"
#include "cmp_dialog.h"
#include "../modules/city.h"
#include "../town/town.h"
#include "../modules/module_static.h"

class cmp_city : public cmp_basic, public hmmc_callback{
    public:
        cmp_city(cmp_basic* parent, group *gout, city* c, pair* res);
        ~cmp_city();
        int on_event(cmp_basic *s, hmmc_event *e) override;
        int on_callback(hmmc_event *e, int a) override;
        void on_draw(int anime)  override;;
        int on_press(int c, int k)  override;;
        int on_tick(int anime)  override;
    private:
        group* group_out;
        cmp_basic *parent;
        city* cur_city;
        void draw_resources_list();
        void draw_build_list();
        void on_draw_page_main(int anime);
        void on_draw_tavern(int anime);
        void on_draw_library(int anime);
        void on_draw_market(int anime);
        void on_draw_recruting(int anime);
        void on_draw_city(int anime);
        void draw_city_img(int anime, int city, int room);
        int page;
        int subpage;
        char get_build(int build, int level, int max_lvl, int v);
        cmp_dialog* dialog;
        pair* resources;
        int is_dialog;
        bool enough_resources(pair p, int count);
        bool is_build(char page);
        void on_draw_village(int anime);
        void on_draw_manufacture(int room, int anime);
        void on_draw_machinery(int anime);
        void on_draw_units(int arg, int pos, pair heroes, int anime);
        bool buy_or_move(int a);
        group* get_army(int arg, int toogle);
        bool on_press_dialog(int r);
        town the_town;
        module_static ms;
        void spend_resources(pair cost, int count);
        bool dialog_not_enough(pair cost, int count);
        void on_draw_room(int city, int anime);
        void on_draw_armory(int anime);
};
