#pragma once
#include <vector>
#include "../modules/hmmc_callback.h"
#include "../legacy/draw_buffer.h"
#include "../modules/city.h"
#include "world_shema.h"

class world : public hmmc_callback {
    public:
        world(int map, const char* scen);
        ~world();
        int on_callback(hmmc_event *e, int a) override;
    private:
        draw_buffer* map_buf;
        std::vector<city*> cities;
        int shema_load(const char* scen);
        void create_town(pair p);
        void loaded();
        world_shema* shema;

};
