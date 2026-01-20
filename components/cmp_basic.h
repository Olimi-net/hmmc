#pragma once
#include "../modules/hmmc_event.h"
#include "../legacy/drawing.h"
#include "../keyboard.h"

const int CMP_NEW = 'N', CMP_LOAD = 'L', CMP_SETTINGS = 'S', CMP_ABOUT = 'A', CMP_EXIT = 'E';

class cmp_basic{
    public:
        cmp_basic(cmp_basic* parent);
        virtual ~cmp_basic();
        virtual int on_event(cmp_basic *s, hmmc_event *e) = 0;
        virtual void on_draw(int anime) = 0;
        virtual int on_press(int c, int k) = 0;
        virtual int on_tick(int anime) = 0;
    protected:
        cmp_basic* parent;
        bool is_load;
};

