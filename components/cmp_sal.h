#pragma once
#include "cmp_basic.h"

class cmp_sal : public cmp_basic{
    public:
        cmp_sal(cmp_basic* parent);
        ~cmp_sal();
        int on_event(cmp_basic *s, hmmc_event *e) override;
        void on_draw(int anime)  override;;
        int on_press(int c, int k)  override;;
        int on_tick(int anime)  override;;

    private:

};
