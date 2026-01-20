#pragma once
#include "pair.h"
#include "drawing_place.h"
#include <vector>

class draw_buffer:public drawing_place{
    public:
        draw_buffer(int len, int rows, char clr);
        virtual ~draw_buffer();
        void clear(int arg);
        bool add_char(char c, int clr);
        void next_row();
        void to_align(int arg);
        bool is_pos(int x, int y);
        char get_char(int x, int y);
        char get_clr(int x, int y);
        void set_char_and_clr(int x, int y, char chr, char clr);
        void set_simbol(int x, int y, char chr, char clr);
        void to_last(char c, int clr);
        char* get_data(bool is_clr);
    private:
        char* buf;
        char* clr;
        pair calc_center_row(int y, bool alfa);
        std::vector<pair> line;
        void cut_row_to_line(int y);
        void copy_line(int x, int y);
        void align_row(int y, int align, int left, int right);

};


