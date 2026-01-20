#pragma once
#include "../legacy/pair.h"

class hmmc_event{
    public:
        hmmc_event(int id);
        ~hmmc_event();
        int id;
        pair p;
};
