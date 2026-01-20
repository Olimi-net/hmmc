#pragma once
#include "battle_args.h"
#include "battle_field.h"
#include "battle_back.h"

class battle : public hmmc_callback{
    public:
        battle(group *attack, group * defence, float difficult, int flag);
        ~battle();
        int on_callback(hmmc_event* e, int a) override;
        void draw(int anime);
        int on_press(int c, int k);
        int on_tick(int anime);
    private:
        battle_field* field;
        bool keyboard_enable();
        pair ai;
        void draw_queue(int ow, int w, int low, int ho, int anime);
        void draw_field(int ow, int w, int top, int anime);
        void draw_currents_unit(int ow, int w, int top, int anime);
        void draw_book(int page);
        battle_args* game_args;
        battle_back* ground;
        bool is_back;
        int move_arrow(int v);
        quadra* move_obj;
        int move_unit(int v, bool r);
        void draw_effects(int left, int top);
        bool calc_ammo(int val);
        int fast_event(int e, int a);
        int event_value;

};
