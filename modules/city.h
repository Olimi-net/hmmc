#pragma once
#include "hmmc_callback.h"
#include "group.h"

const int CITY_GROWL = 32;
const int CITY_RESERVE = 64;
const int CITY_FORTED = 128;


class city{
    public:
        city(int fraction, int builded, int max_build, int magic, int x, int y, hmmc_callback *c);
        ~city();
        bool has_xy(int x, int y);
        int get_city();
        int get_fraction();
        void set_fraction(int f);
        void raw();
        int get_x();
        int get_y();
        int get_level();
        void set_level(int b);
        int get_max_level();
        int magic;
        pair get_growl_today(int unit);
        int get_available(int unit);
        char* get_name();
        group* get_garrison();
        bool join_group(group *g);
        void remove(int unit, int count);
        pair* week_info();
    private:
        pair* build;
        pair units[4]; // growl, available, unit_type
        pair position;
        pair resources;
        int hero_id;
        void update_growl();
        void week_growl();
        hmmc_callback* event;
        group* garrison;
};
