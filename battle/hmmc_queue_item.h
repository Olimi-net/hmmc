#pragma once
#include "battle_group.h"

class hmmc_queue_item{
    public:
        hmmc_queue_item(battle_group* g, hmmc_unit* u, int f);
        ~hmmc_queue_item();
        battle_group* group;
        hmmc_unit* unit;
        int id;
        int fac;
        bool is_hero();
        int assign;
        int get_assign(bool near);
        void interaction(int at, hmmc_queue_item* q);
        bool not_next();
        int get_type();
        bool is_rem;
        void anim_attack();
        void anim_damage();
        int set_damage(int dmg);
        int get_anime();
        int dmg_min();
        int dmg_max();
        float get_archery();
        bool is_city_archer;
        int city_unit;
        bool is_pass();
    private:
        int hero_angle;
        int attack_anime;

};
