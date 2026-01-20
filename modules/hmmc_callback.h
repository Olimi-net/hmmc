#pragma once
#include "hmmc_event.h"

class hmmc_callback{
    public:
        hmmc_callback();
        virtual ~hmmc_callback();
        virtual int on_callback(hmmc_event *e, int a) = 0;
};
