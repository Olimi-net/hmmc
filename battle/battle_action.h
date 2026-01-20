#pragma once
#include "battle_animation.h"

const int AC_FIRE = 1;
const int AC_STUN = 2;
const int AC_ROUND = 4;
const int AC_DAMAGE = 8;
// const int AM_DROP = 16; // not edit using for field event from animation

const int BATTLE_NEXT = 0;
const int BATTLE_MELEE = 4;
const int BATTLE_MAGIC = 8;
const int BATTLE_RANGER = 16;
const int MOVE_HOME = 32;
const int BATTLE_INTERACTION = 64;

const int REGENERATION = 128;
const int BATTLE_ACTION = 256;

const int BATTLE_AFTER = 512;
const int BATTLE_ASSIGN = 1024;
const int BATTLE_RETALATION = 1024;

const int BATTLE_EFFECT = 4096;

class battle_action{
    public:
        battle_action(hmmc_callback* battle, hmmc_callback* field);
        ~battle_action();
        bool action(int at, hmmc_queue_item* one, hmmc_queue_item* two);
        void end_action(int flag, int at, hmmc_queue_item* one, hmmc_queue_item* two);
        void range_action(int at, hmmc_queue_item* one, hmmc_queue_item* two);
        void magic_action(int at, hmmc_queue_item* one, hmmc_queue_item* two);
        void melee_action(int at, hmmc_queue_item* one, hmmc_queue_item* two);
        bool after_action(int at, hmmc_queue_item* one, hmmc_queue_item* two);
        bool assign_or_retalation(int at, hmmc_queue_item* one, hmmc_queue_item* two, int flag);
        int one_id();
        void anime(int anim, int at, hmmc_queue_item* one, hmmc_queue_item* two, int flag);
        bool on_tick();
        int get_flag();
    private:
        battle_animation* run;
        int get_damage(int at, hmmc_queue_item* one, hmmc_queue_item* two, float I6);
        hmmc_callback* event;
};


