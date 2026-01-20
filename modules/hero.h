#pragma once
#include "../legacy/pair.h"
#include "../legacy/static_hero.h"

const int HERO_SPELL_HEAL = 65;
const int HERO_COLOR = 66;



const int HERO_ATTR_BALLISTIC = 12;
const int HERO_ATTR_ARCHERY = 11;
const int HERO_ATTR_LIDER = 10;
const int HERO_ATTR_SPECIALITY = 7;
const int HERO_ATTR_SORCERY = 6;
const int HERO_ATTR_INTELLECT = 5;
const int HERO_ATTR_LEVEL = 4;
const int HERO_ATTR_DEFENSE = 3;
const int HERO_ATTR_ATTACK = 2;
const int HERO_ATTR_ID = 1;

class hero{
    public:
        hero(int id);
        ~hero();
        void set_attr(int attr, int val);
        int get_attr(int attr);
    private:
        int id;
        int level;
        int speciality;

        //todo WTF?
        int branch;
        int talants;
        int book; //magic book
        pair base; //base attack | defense
        pair damage; // damage min|max | magic power
        pair mana; // mana current|max| refresh
};
