#pragma once
#include "../modules/hmmc_callback.h"
#include "../legacy/pair.h"
#include "../legacy/hmmc_math.h"
#include "../legacy/static_unit.h"

const int UNIT_EVENT_WAGON = 1;
const int UNIT_EVENT_NEAR_ENEMY = 2;
const int UNIT_EVENT_MORALE = 3;
const int UNIT_EVENT_HERO_LUCK = 4;
const int UNIT_EVENT_HERO_LEVEL = 5;
const int UNIT_EVENT_UNIT_SPECIALITY = 6;
const int UNIT_EVENT_ATTACK_DIFICULT = 7;
const int UNIT_EVENT_DEFENSE_DIFICULT = 8;
const int UNIT_EVENT_DEAD = 9;

//magic attack group - exclude one
const int UNIT_ATTACK_DRAIN_LIFE = 1;
const int UNIT_ATTACK_LIGHTNING = 2;
const int UNIT_ATTACK_FIREBALL = 4;

//range and melee
const int UNIT_AFTER_MUTILATE = 1;
const int UNIT_ATTACK_RAGE = 2;
const int UNIT_ATTACK_ROUND = 4;

//range and magic
const int UNIT_ATTACK_NO_MELEE_PENALTY = 8;
// only melee
const int UNIT_ATTACK_JOUSTING = 8;

const int UNIT_HAS_MAGIC = 103;
const int UNIT_HAS_DRAIN_LIFE = 33;

const int UNIT_FLAG_RANGER = 16;
const int UNIT_FLAG_MAGIC = 32;
const int UNIT_FLAG_MELEE = 64;
const int UNIT_FLAG_NO_RETALATION = 128;
const int UNIT_FLAG_LUCK = 256;
const int UNIT_FLAG_UNLUCK = 512;

const int UNIT_FLAG_ATTACK = 8192;
const int UNIT_ATTACK_HEAL = 1024 | UNIT_FLAG_ATTACK;
const int UNIT_ATTACK_RESURRECT = 2048 | UNIT_FLAG_ATTACK;
const int UNIT_ATTACK_MORALE = 4096 | UNIT_FLAG_ATTACK;

const int UNIT_FLAG_AFTER = 16384;
const int UNIT_AFTER_REGENERATION = 1024 | UNIT_FLAG_AFTER; // this = heal
const int UNIT_AFTER_RETURNS = 2048 | UNIT_FLAG_AFTER;
const int UNIT_AFTER_ROOT = 4096 | UNIT_FLAG_AFTER;




class hmmc_unit{
    public:
        hmmc_unit(hmmc_callback *e, pair unit, int id, int angle);
        ~hmmc_unit();
        bool is_big();
        void set_pos(int x, int y);
        pair get_pos();
        int before_step();
        int after_step();
        int get_id();
        int get_morale();
        int get_type();
        bool is_dead();
        int get_level();
        int calc_assign(bool enemy_near);
        int calc_retaliation(bool enemy_near);
        void rem_shoots(int count);
        int cur_attack();
        int cur_defense();
        int cur_min();
        int cur_max();
        float get_index_attack();
        float get_index_defense();
        float get_index_positive();
        int set_damage(int dmg);
        bool nonliving();
        void heal(int val);
        void up_rage(int val, int dead);
        int get_rage();
        bool range_retaliation();
        void down_retaliated();
        int get_heal();
        int get_fast();
        bool is_ranger();
        bool is_mage();
        int get_count();
        void set_debaf(int arg);
        bool is_machine();
        int get_type_attack();
        int max_hp();
        int cur_hp();
        int max_speed();
        int cur_speed();
        void refresh();
        bool is_fly();
        bool can_swim();
        int get_angle();
        pair get_coord();
        int move(int x, int y);
        int step_low(int val);
        void set_hp(int hp);
        void set_huge();
    private:
        pair x; // current, target,, start start
        pair y; // current, target
        pair* unit;  // type | count | id
        quadra* attr; // level | attack | defense | hp
        pair* damage; // min - max
        pair* speed; // current | max | debaf
        quadra* prms; // attack-type | after-attack | effect | amount
        quadra* admh; //attack | defense | max damage | hp
        pair* cur_unit; // count | hp | dead
        pair* cur_prms; // count retaliation | rage | rage max

        int id;
        int state;
        int relation;
        bool huge;
        bool spec;
        int angle;
        hmmc_callback* event;
        float index_attack;
        float index_defense;
        float index_positive;

        float calc_attack(int hLvl, bool spec);
        float calc_defense(int hLvl, bool spec);
        int calc_luck();
        void refresh_retalation();
        void next_angle(int a);
        int on_call_event(int e, int a);
};
