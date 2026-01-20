#pragma once
#include <vector>
#include "group.h"
#include "city.h"
#include "hmmc_callback.h"

class hmmc_game{
    public:
        hmmc_game(hmmc_callback* e);
        ~hmmc_game();
        /** return group or null if hero is busy */
        group* factory_group(int fracton, int hero_id, int unit_type);
    private:
        std::vector<hero*> heroes;
        std::vector<group*> groups;
        std::vector<city*> cities;
        hero* maybe_hero(int hero_id);
        void init_group_by_unit(group *g, int week_unit);
        int guid;
        hmmc_callback* error_event;
};
