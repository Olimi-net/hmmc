#pragma once
#include "../legacy/pair.h"


class module_static{
    public:
        module_static();
        ~module_static();
        pair unit_cost(int unit_type);
        const char* unit_text(int unit_type, int row);
        void draw_book(int magic, int x, int y, int cost, int mana, bool is_double);
        void draw_unit(int unit_type, int x, int y, int angle);
};

