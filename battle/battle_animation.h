#pragma once
#include "hmmc_queue_item.h"

const int AM_DROP = 16; // not edit

const int BA_ATTACK = 1;
const int BA_MOVE = 2;
const int BA_MELEE = 4;
const int BA_RANGE = 8;
const int BA_HERO = 16;


const int BA_POSITIVE = 32;
const int BA_NEGATIVE = 64;


const int BA_MAGIC = 128;
const int BA_MORALE = 256;
const int BA_LUCK = 512;


const int BA_RANGE_ARROW = BA_RANGE | BA_ATTACK;
const int BA_STAND_ATTACK = BA_MELEE | BA_ATTACK;

const int BA_HERO_ATTACK = BA_HERO | BA_STAND_ATTACK;
const int BA_HERO_RANGE = BA_HERO | BA_RANGE_ARROW;
const int BA_MOVE_ATTACK = BA_MOVE | BA_STAND_ATTACK;

const int BA_NEGATIVE_MAGIC = BA_MAGIC | BA_NEGATIVE;
const int BA_POSITIVE_MAGIC = BA_MAGIC | BA_POSITIVE;

const int BA_LOW_MORALE = BA_MORALE | BA_NEGATIVE;
const int BA_UP_MORALE = BA_MORALE | BA_POSITIVE;

const int BA_LOW_LUCK = BA_LUCK | BA_NEGATIVE;
const int BA_UP_LUCK = BA_LUCK | BA_POSITIVE;

const int BA_OTHER_EVENT = 511;

class battle_animation{
    public:
        battle_animation(hmmc_callback* e, hmmc_callback* field);
        ~battle_animation();
        void init(int anim, int at, hmmc_queue_item* one, hmmc_queue_item* two, int flag);
        bool is_end();
        hmmc_queue_item* get_one();
        hmmc_queue_item* get_two();
        int get_flag();
        bool on_tick();
        int get_id();
    private:
        hmmc_callback* field;
        hmmc_callback* event;
        hmmc_queue_item* one;
        hmmc_queue_item* two;
        int state;
        int flag;
        int unit;
        int at;
        int luck;
        int id;
        void drop();
};
