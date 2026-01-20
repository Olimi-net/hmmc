#pragma once
#include "cmp_basic.h"
#include "cmp_dialog.h"

class cmp_menu : public cmp_basic{
    public:
        cmp_menu(cmp_basic* parent);
        ~cmp_menu();
        int on_event(cmp_basic *s, hmmc_event *e) override;
        void on_draw(int anime)  override;;
        int on_press(int c, int k)  override;;
        int on_tick(int anime)  override;;

    private:
        void transleting(char c, int clr);
        void draw_title();
        void draw_menu(int slct);
        int selected_line;
        bool is_dialog;
        bool is_page;
        cmp_dialog* dialog;
        int resolve_key(int c, int k);
        cmp_basic* page;
};
