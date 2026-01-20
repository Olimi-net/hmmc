#pragma once
#include "../legacy/pair.h"

const int BTL_ARG_ALL = 511;
const int BTL_ARG_SMALL = 1;
const int BTL_ARG_BIG = 2;
const int BTL_ARG_NONLIVING = 4; // ent
const int BTL_ARG_MACHINE = 8; // machine
const int BTL_ARG_HEROS = 16;

class battle_args{
    public:
        battle_args(int flag, int w, int h);
        ~battle_args();
        void info_left(int fr, int hero_id);
        void info_right(int fr, int hero_id);
        int get_start_column(bool is_right);
        int width;
        int height;
        int columns;
        int rows;
        int type_field;
        int type_city;
        int unit_arg;
        int max_width;
        int max_height;
        int left;
        int right;
        int top;
        int bottom;
        int fraction_left;
        int fraction_right;
        int hero_left;
        int hero_right;
};
