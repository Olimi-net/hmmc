#pragma once
#include "hero.h"
#include "../legacy/pair.h"

const int GROUP_GET = 1;
const int GROUP_SET = 2;
const int GROUP_LEFT = 4;
const int GROUP_RIGHT = 8;
const int GROUP_DEAD = 16;


class battle_group;

class group{
    friend battle_group;
    public:
        group(int id, int fraction, hero *h);
        ~group();
        void set_position(int x, int y);
        bool append_unit(int type, int count);
        int can_append(int type);
        int get_faction();
        void set_faction(int f);
        pair* get_position();
        int get_x();
        int get_y();
        int get_creature();
        int get_animation();
        hero* get_hero();
        int get_hero_val_or_def(int flag, int def);
        int get_group_id();
        pair* get_unit(int i);
        int slots_count();
        void remove_dead();
    protected:
        int fraction;
        pair units[6];
        pair position;
        pair step; // angle:L|R, step:move|stay, left step count
        hero *current;
        int group_id;
};
