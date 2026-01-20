#include "drawing_place.h"

drawing_place::drawing_place(int len, int rows, char clr){
    max_width = width = len;

    max_height = height = rows;
    pos_x = pos_y = cursor_x = cursor_y = 0;
    cursor_clr = clr;
}

drawing_place::~drawing_place(){

}

void drawing_place::set_cursor(int x, int y){
    this->cursor_x = x;
    this->cursor_y = y;
}

int drawing_place::get_cursor_x(){
    return this->cursor_x;
}

int drawing_place::get_cursor_y(){
    return this->cursor_y;
}

int drawing_place::get_width(){
    return this->width;
}

int drawing_place::get_height(){
    return this->max_height;
}

int drawing_place::tab(){
    for(int i = this->pos_x; i < this->width; i+=5){
        if(this->cursor_x < i){
            this->cursor_x = i;
            return this->cursor_x;
        }
    }
    this->cursor_x = this->width;
    return this->cursor_x;
}

char drawing_place::get_clr_default(){
    return this->cursor_clr;
}

int drawing_place::get_options(int opt){
    int res = 0;
    if(opt & (DB_OPT_CUT | DB_OPT_BEGIN | DB_OPT_END)){
        if(pos_x != 0 || pos_y != 0) res |= DB_OPT_CUT;
        if(this->width != this->max_width) res |= DB_OPT_CUT;
        if(this->height != this->max_height) res |= DB_OPT_CUT;
    }

    if(opt & DB_OPT_CLR){
        if(this->cursor_clr != 0) res |= DB_OPT_CLR;
    }
    return res;
}

bool drawing_place::set_option(int option, int val){
    if(val < 0) return false;

    if(option & DB_OPT_RESET){
        this->refresh(val);
        return true;
    }

    if(option & DB_OPT_CLR){
        this->cursor_clr = val;
        return true;
    }

    int op = option & (DB_OPT_CUT | DB_OPT_BEGIN | DB_OPT_END);

    if(op == DB_OPT_BEGIN){
        if(val < width){
            this->pos_x = val;
            if(cursor_x < pos_x) cursor_x = pos_x;
            return true;
        }
        return false;
    }

    if(op == DB_OPT_END){
        if(val > pos_x && val <= max_width){
            this->width = val;
            return true;
        }
        return false;
    }

    if(op == (DB_OPT_CUT | DB_OPT_BEGIN)){
        if(val < this->height){
            this->pos_y = val;
            if(cursor_y < pos_y) cursor_y = pos_y;
            return true;
        }
        return false;
    }

    if(op == (DB_OPT_CUT | DB_OPT_END)){
        if(val > pos_y && val <= this->max_height){
            this->height = val;
            return true;
        }
        return false;
    }

    return false;
}

void drawing_place::refresh(int options){
    if(options & DB_OPT_CLR){
        this->cursor_clr = 0;
    }

     int op = options & (DB_OPT_CUT | DB_OPT_BEGIN | DB_OPT_END);
     if(op == DB_OPT_CUT){
        this->pos_x = this->pos_y = 0;
        this->width = this->max_width;
        this->height = this->max_height;
        return;
     }

     if(op == DB_OPT_BEGIN){
        this->pos_x = 0;
        return;
    }

    if(op == DB_OPT_END){
        this->width = max_width;
        return;
    }

    if(op == (DB_OPT_CUT | DB_OPT_BEGIN)){
        pos_y = 0;
        return;
    }

    if(op == (DB_OPT_CUT | DB_OPT_END)){
        this->height = max_height;
        return ;
    }
}
