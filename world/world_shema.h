#pragma once
#include "town_info.h"
#include "../legacy/pair.h"

class world_shema{
    public:
        world_shema(const char* scen);
        ~world_shema();
        town_info* city_found(pair p);
        int max_build();
        int map_id();
        void parse(int count, char* line);
};
