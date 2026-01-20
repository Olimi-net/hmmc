#pragma once
#ifdef _TEST_
    #include <stdio.h>
#endif
#ifdef _LINUX_
    #include <ncurses.h>
	#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif
#ifdef _WIN32_
    #include <curses.h>
#endif
#include "draw_buffer.h"

static const int DRAWING_ALFA = 16;
//static const int DB_OPT_RESET = 256; // 511 remove all options
static const int DRAWING_ZIP = 512;

static const int DRAWING_OPTION = 8;
static const int BACK = 4;
static const int HIGH = 2;
static const int REGULAR = 1;

class draw_pack{
    public:
        draw_pack(int size, char clr);
        ~draw_pack();
        std::vector<char> echo;
        void draw_swap(int a);
        draw_buffer* buf(int v);
        void next_row();
        void set_options(int val, int arg);
        int draw_info(int v);
        void clear(int lvl);
        char draw_get_cur_char();
        void draw_validate();
        void reset_cursor();
        int next_cursor();
        int display_height();
        int display_width();
        draw_buffer* get_lvl();
        void set_lvl(int lvl);
        bool add_char(char c, int clr);
    private:
        int lvl;
        int is_alfa;
        draw_buffer* regular;
        draw_buffer* high;
        draw_buffer* back;
        void zip(int f, int t);
        int echo_cursor;
        int cx;
        int cy;
        bool on_buf;
        pair* terminal_size;
        pair* display_size;

};

/** default args: size = - 1; clr = 0; */
void init_drawing(int size, char clr);
void free_drawing();

int draw_int(int num);
/** draw number by align 0 = on left, 1 = center, 2 = on right  */
int draw_int_align(int x, int width, int num, int clr, int align);
bool draw_char(char c, int clr);
void next_row();
/** arg = 1-left, 2-right, 3-center, 4-block, 8-opacity, 16-row */
void draw_align(int arg);
void draw_clear(int lvl);
void draw_validate();
void draw_select_buffer(int lvl);
int draw_display_height();
int draw_display_width();

/** draw element to end row */
void draw_end(char c, int clr);

/** draw element amount turn */
int draw_as_line(char chr, int clr, int amount);
int next_col();
int draw_selected_line(const char* row, int slct, int pos);
void set_position(int x, int y);
void draw_tabs(int count, const char** text, int slct);
int draw_text(const char* row, char clr);
/** align: 0 = on left, 1 = center, 2 = on right */
void draw_text_align(int x, int width, const char* row, char clr, int align);

/* deprecated */
void draw_text_position(int start_pos, int end_pos, int rows_count, int row_pos, const char *text);
int get_position_x();
int get_position_y();
void draw_options(int opt, int val);
int draw_char_at_position(char c, char clr, int x, int y);
void draw_echo(int c);
int draw_info(int v);
int actuale_size(const char *s);

void draw_reset_cursor();
int draw_next_cursor();
char draw_get_cur_char();
