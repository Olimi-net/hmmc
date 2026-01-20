#include "draw_buffer.h"
#include "hmmc_math.h"

draw_buffer::draw_buffer(int len, int rows, char clr):drawing_place(len, rows, clr){
    this->buf = (char*)malloc(len * rows *  sizeof(char));
    this->clr = (char*)malloc(len * rows *  sizeof(char));
}

draw_buffer::~draw_buffer(){
    line.clear();
    free(clr);
    free(buf);
}

void draw_buffer::clear(int arg){
    if(arg & DB_OPT_RESET){
        refresh(DB_OPT_CUT | DB_OPT_CLR);
    }
    this->cursor_x = this->pos_x;
    this->cursor_y = this->pos_y;
    int size = this->max_width * this->max_height;
    for (int i=0;i<size;i++){
        this->buf[i]='\0';
        this->clr[i]='\0';
    }
    this->line.clear();
}

bool draw_buffer::add_char(char c, int clr){
    if(this->cursor_y < this->pos_y || this->cursor_x < this->pos_x) return false;
    if(this->cursor_x < this->width && this->cursor_y < this->height){
        this->buf[this->cursor_y * this->max_width + this->cursor_x] = c;
        this->clr[this->cursor_y * this->max_width + this->cursor_x] = clr;
        this->cursor_x++;
        return true;
    }
    return false;
}

void draw_buffer::next_row(){
    this->cursor_y++;
    this->cursor_x=pos_x;
}

pair draw_buffer::calc_center_row(int y, bool alfa){
    pair p = {0, 0, 0};
    int left = -1;
    int j = y * this->max_width;
    int n = alfa ? 32 : 31;
    for(int x=pos_x;x < this->width; x++){
        if(this->buf[j + x] > n){
            left = x - pos_x;
            break;
        }
    }
    if(left < 0) return p;
    int right = -1;
    for(int x=pos_x;x < this->width; x++){
        int u = this->width-1 - x;
        if(this->buf[j + u] > n){
            right = x - pos_x;
            break;
        }
    }
    p.value = left;
    p.subval = right;
    p.state =  left + right;
    return p;
}

void draw_buffer::cut_row_to_line(int y){
    int x = y * this->max_width;
    this->line.clear();
    for(int i=pos_x;i<this->width;i++){
        pair p = {this->buf[x + i], this->clr[x + i], 0};
        line.push_back(p);
        this->buf[x + i] = 0;
        this->clr[x + i] = 0;
    }
}

void draw_buffer::copy_line(int x, int y){
    int count = line.size();
    int pos = 0;
    int j = y * this->max_width;

    if(x < 0) {
        pos = -x;
        x = 0;
    }

    for(int i = x; i < this->width; i++){
        if(pos >= count) break;
        pair p = line[pos];
        this->buf[j + i + pos_x] = p.value;
        this->clr[j + i + pos_x] = p.subval;
        pos++;
    }
}

void draw_buffer::align_row(int y, int align, int left, int right){
    if(align == 3){
        int of = (left + right) >> 1;
        if(of < 1) return;
        cut_row_to_line(y);
        copy_line(of - left, y);
        return;
    }

    if(align == 1){
        if(left < 1) return;
        cut_row_to_line(y);
        copy_line(-left, y);
        return;
    }

    if(align == 2){
        if(right < 1) return;
        cut_row_to_line(y);
        copy_line(right, y);
    }
}

void draw_buffer::to_align(int arg){
    int w = this->width;
    int h = this->height;
    bool alfa = (arg & 8);
    int align = arg & 3;

    if(arg & 16){
        int y = cursor_y;
        pair p = calc_center_row(y, alfa);
        if(p.state){
            align_row(y, align, p.value, p.subval);
        }
        this->line.clear();
        return;
    }

    if(arg & 4){
        int left = w-1;
        int right = w-1;
        for(int y=pos_y;y < h;y++){
            pair p = calc_center_row(y, alfa);
            if(p.state){
                left = math_min(left, p.value);
                right = math_min(right, p.subval);
            }
        }
        for(int y=pos_y;y<h;y++){
            align_row(y, align, left, right);
        }
        this->line.clear();
        return;
    }

    for(int y=pos_y;y < h;y++){
        pair p = calc_center_row(y, alfa);
        if(p.state){
            align_row(y, align, p.value, p.subval);
        }
    }
    this->line.clear();
}

char draw_buffer::get_char(int x, int y){
    return this->buf[y * this->max_width + x];
}

bool draw_buffer::is_pos(int x, int y){
    if(y < this->pos_y || x < this->pos_x) return false;
    return (x < this->width && y < this->height);
}

void draw_buffer::set_char_and_clr(int x, int y, char chr, char clr){
    this->buf[y * this->max_width + x] = chr;
    this->clr[y * this->max_width + x] = clr;
}

char draw_buffer::get_clr(int x, int y){
    if(this->cursor_clr) return this->cursor_clr;
    return this->clr[y * this->max_width + x];
}



void draw_buffer::set_simbol(int x, int y, char chr, char clr){
    if(this->cursor_clr != 0){
        if(this->cursor_clr == 'T'){
            if(chr < 33){
                set_char_and_clr(x, y, 0, 0);

            }else{
                set_char_and_clr(x, y, chr, clr);
            }
            return;
        }else{
            set_char_and_clr(x, y, chr, this->cursor_clr);
        }
        return;
    }
    set_char_and_clr(x, y, chr, clr);
}



void draw_buffer::to_last(char c, int clr){
    int w = this->width;
    for(int i=0;i<w;i++)
        if(!this->add_char(c, clr)) break;
}



char* draw_buffer::get_data(bool is_clr){
    if(is_clr) return this->clr;
    return this->buf;
}
