#pragma once
#include "cmp_basic.h"

class cmp_about : public cmp_basic{
    public:
        cmp_about(cmp_basic* parent);
        ~cmp_about();
        int on_event(cmp_basic *s, hmmc_event *e) override;
        void on_draw(int anime)  override;
        int on_press(int c, int k)  override;
        int on_tick(int anime)  override;

    private:
        int tab_indx;
        int scroll_offset;
        void about_unit(int city);
        void about_heroes();
        void about_book();
};
