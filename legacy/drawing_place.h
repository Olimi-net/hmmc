#pragma once

const int DB_OPT_RESET = 256;
const int DB_OPT_CUT = 1024;
const int DB_OPT_CLR = 2048;
const int DB_OPT_BEGIN = 4096;
const int DB_OPT_END = 8192;

class drawing_place{
    public:
        drawing_place(int len, int rows, char clr);
        virtual ~drawing_place();
        void set_cursor(int x, int y);
        int get_cursor_x();
        int get_cursor_y();
        int get_width();
        int get_height();
        int tab();
        char get_clr_default();
        int get_options(int opt);
        bool set_option(int option, int val);
        void refresh(int options);
    protected:
        int max_width;
        int max_height;
        int width;
        int height;
        int pos_x;
        int pos_y;
        int cursor_x;
        int cursor_y;
        char cursor_clr;
};
