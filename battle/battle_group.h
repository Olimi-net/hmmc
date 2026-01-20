#pragma once
#include <vector>
#include "../modules/group.h"
#include "hmmc_unit.h"

const int LEFT_GROUP = 32;
const int RIGHT_GROUP = 64;
const int GROUP_EVENT = 128;

const int GE_REVALIDATE_QUEUE = 1;

class battle_group : public hmmc_callback{
    public:
        battle_group(group *g, int side, float user_lvl, hmmc_callback* e);
        ~battle_group();
        int on_callback(hmmc_event* e, int a) override;
        void start_line(int x);
        int get_after(int id);
        int is_life(int id);
        int is_id(int id);
        int hero_id();
        void remove_shoots(int id, int count);
        int get_attack(int id);
        int get_defense(int id);
        float index_attack(int id);
        float index_defense(int id);
        int nonliving(int id);
        float get_sorcery();
        int get_ballistic();
        float get_archery();
        void heal(int id, int val);
        void up_rage(int id, int val, int dead);
        float get_difficult();
        bool is_shoot_retaliation(int id);
        void down_retaliated(int id);
        void interaction(int at, int one, int two);
        bool is_machine(int id);
        hmmc_unit* get_unit_by_id(int id);
        hmmc_unit* get_next_unit(int id);
        int get_assign(int id);
        void add_unit(hmmc_unit* unit);
        void rem_unit(int id);
        int get_heal(int id);
        int get_clr();
    private:
        group* parent;
        int offset;
        std::vector<hmmc_unit*> units;
        int calc_morale(int id);
        float get_positive(int id);
        float hero_spell;
        float difficult;
        int calculate_positive(int at, int val, float I6);
        hmmc_callback* event;
};
