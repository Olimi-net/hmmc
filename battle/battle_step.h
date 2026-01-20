#pragma once

class battle_step{
    public:
        battle_step();
        ~battle_step();
        int attacker;
        int defender;
        int at;
        int state;
        bool is_at(int f);
};
