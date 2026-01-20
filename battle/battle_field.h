#pragma once
#include "hmmc_path.h"
#include "../modules/hmmc_callback.h"
#include "../legacy/static_battle.h"
#include "battle_action.h"
#include "battle_queue.h"
#include "battle_step.h"

class battle_field: public hmmc_callback{
    public:
        battle_field(int cols, int rows, group* l, group* r, float dif, hmmc_callback* e);
        ~battle_field();
        int on_callback(hmmc_event* e, int a) override;
        void init(int flag);
        void start_line(int offsetL, int offsetR);
        int resolve_pulse(int user_faction);
        int get_clr(int a);
        int hero_id(int a);
        hmmc_queue_item* next_queue(int index);
        int unit_count();
        hmmc_queue_item* get_index(int i);
        pair get_pos(int id);
        int get_cols();
        bool select_unit();
        int one_id();
        int two_id();
        hmmc_queue_item* get_item(int id);
        bool key_on();
        void on_run();
        pair unit_move();
        void update_pos_for_unit(int id);
        void add_city_unit(int unit_type, int x, int y);
    private:
        void set_unit(int id, pair pos, int t, int p);
        int round(int x, int y, int a);
        int round_by_id(int id, int attr);
        bool is_near(int f, int s);
        int calc_far(int f, int s);
        int get_id_by_pos(int x, int y);
        hmmc_path* field;
        int cols;
        int rows;
        battle_group* left;
        battle_group* right;
        battle_queue* queue;
        battle_step* pulse;
        battle_action* run;
        bool valid_action(int flag);
        void round_attack(int dmg, int id, int t, int f, bool big);
        void fair_round(int dmg, int x, int y, int t);
        bool unit_is_valid(int flag, hmmc_unit* u);
        int get_simle_unit(int at, hmmc_queue_item* one);
        int get_unit_for_hero(hmmc_queue_item* hero);
        hmmc_queue_item* next_unit(int at, int id, int fac, int skip_id);
        int read_magic_book(hmmc_queue_item* one);
        void set_dmg_id(int id, int dmg, int t, int f);
        int damage;
        hmmc_callback* battle;
        std::vector<hmmc_unit*> city_units;
        int get_city_unit(int f);
};
