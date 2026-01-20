#pragma once
#include "cmp_basic.h"
#include <vector>

class cmp_mgr : public cmp_basic{
    public:
        cmp_mgr(cmp_basic* parent);
        ~cmp_mgr();
        int on_event(cmp_basic *s, hmmc_event *e) override;
        void on_draw(int anime)  override;;
        int on_press(int c, int k)  override;;
        int on_tick(int anime)  override;;

    private:
        int cur_page;
        std::vector<cmp_basic*> pages;
        cmp_basic* get_page();
        void crop_pages(int max);
};
