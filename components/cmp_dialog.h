#pragma once
#include "cmp_basic.h"
#include <vector>
#include <string>

const int DIALOG_BTN_YES = 1;
const int DIALOG_BTN_OK = 2;

const int DIALOG_BTN_NO = 4;
const int DIALOG_BTN_CLOSE = 8;

const int DIALOG_BTN_ONE = 16;
const int DIALOG_BTN_TWO = 32;
const int DIALOG_BTN_THREE = 64;
const int DIALOG_BTN_FOUR = 128;

const int DIALOG_BTN_YES_NO = DIALOG_BTN_YES | DIALOG_BTN_NO;

class cmp_dialog : public cmp_basic{
    public:
        cmp_dialog(cmp_basic *parent);
        ~cmp_dialog();
        int on_event(cmp_basic *s, hmmc_event *e) override;
        void on_draw(int anime)  override;;
        int on_press(int c, int k)  override;;
        int on_tick(int anime)  override;
        void set_title(const char *str);
        void set_title(const char *str1, const char *str2);
        void set_title(const char *str1, const char *str2, const char *str3);
        void set_row(const char *str);
        void set_row(const char *str1, const char *str2);
        void set_row(const char *str, int num);
        void set_row(int num, const char *str);
        void set_buttons(int btn);
        void clear();
        void update_button(int btn, const char* text, char key);
    private:
        cmp_basic* parent;
        std::vector<std::string> rows;
        std::vector<std::string> custom;
        std::string keys;
        std::string title;
        int max_width;
        int buttons;
};
