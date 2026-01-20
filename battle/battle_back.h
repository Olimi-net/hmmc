#pragma once
#include "battle_args.h"

const int BTL_BACK_ON = 16;
const int BTL_BACK_OFF = 32;
const int BTL_BACK_TOOGLE = 48;
const int BTL_BACK_ALWAYS = 64;
const int BTL_BACK_BROKEN = 80;

class battle_back{
    public:
        battle_back(battle_args* a);
        ~battle_back();
        void draw(int anime);
        /** 1=door open, 2=door close, 4=catapult pull, 8=wall broken, 32=destroyed tower  */
        void on_anime_flag(int flag);
    private:
        bool is_floor;
        bool is_load;
        battle_args* game_args;
        int top_img;
        int top_name;
        bool is_standard;
        void init_header(int top, int flag);
        void draw_heros_name(int offset);
        void draw_heros();
        void draw_standard(bool angle, int clr, int ow, int ho, int anim);
        int anime_flag;
};
