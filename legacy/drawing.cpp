#include "drawing.h"
#include <sys/ioctl.h>
draw_pack* dr_pack;

//maybe private
int get_color(int c){
	if(c>=0 && c<=7) return c;
	if(c>='0'&& c<='7') return c-'0';
	int n=c&95;
	if(n=='R') return 4; // red
	if(n=='G') return 2; // green
	if(n=='B') return 1; // blue
	if(n=='Y') return 6; // yellow
	if(n=='A') return 3; // aqua ~ all water in game
	if(n=='M') return 5; // magenta / violet
	if(n=='W') return 7; // white
	if(n=='D') return 0; // gray | dark
	return 0;
}

#ifdef _TEST_
//maybe private
void draw_simbol(char c, char clr){
	if (c == '\n') { printf("%c", '\n'); return;}
	if (c<32){ printf("%c", ' '); return;}
	printf("%c", c);
}
#endif
#ifndef _TEST_
//maybe private
void draw_simbol(char c, char clr){
	if (c == '\n') { printw("%c", '\n'); return;}
	if (c<32){ printw("%c", ' '); return;}
    int color = get_color(clr);
    if(color == 0){
        attron(COLOR_PAIR(7) | A_BOLD | A_DIM);
		printw("%c", c);
		attroff(A_BOLD | A_DIM);
    }else if(color == 7 || color == 6){
		attron(COLOR_PAIR(color) | A_BOLD);
		printw("%c", c);
		attroff(A_BOLD);
	}else{
		attron(COLOR_PAIR(color) | A_BOLD | A_DIM);
		printw("%c", c);
		attroff(A_BOLD | A_DIM);
	}
}
#endif

const int DISPLAY_DEF_LEN = 60;
const int DISPLAY_MIN_LEN = 40;
const int DISPLAY_MAX_LEN = 120;

const int DISPLAY_DEF_ROW = 24; // min + hero + inciative
const int DISPLAY_MIN_ROW = 18; //4 units + 1 + machine
const int DISPLAY_MAX_ROW = 30; //

draw_pack::draw_pack(int size, char clr){
    struct winsize ws;
    int dw = 0;
    int ds = 0;
    int dh = 0;
	if (ioctl(0, TIOCGWINSZ, &ws) < 0) {
        dw = DISPLAY_DEF_LEN;
        dh = DISPLAY_DEF_ROW;
        ds = -1;
    } else {
        dw = ws.ws_col;
        if (dw < DISPLAY_MIN_LEN) {
            dw = DISPLAY_MIN_LEN;
            ds |= 1;
        }
        if (dw > DISPLAY_MAX_LEN){
            dw = DISPLAY_MAX_LEN;
            ds |= 2;
        }
        dh = ws.ws_row;
        if (dh < DISPLAY_MIN_ROW){
            dh = DISPLAY_MIN_ROW;
            ds |= 4;
        }
        if (dh > DISPLAY_MAX_ROW){
            dh = DISPLAY_MAX_ROW;
            ds |= 8;
        }
    }

    /* min: w:12 h:6 nor w:16 h:8 max: w:19 h:9*/
    char c = ('0' <= clr and clr <= '9') ? clr : '0';
    int w = 0;
    int h = 0;
    if(size < 0){
        w = dw;
        h = dh;
    }else{
        w = (12 + (size & 7)) * 5; //max = 19
        if(w > dw) w = dw;
        h = (6 + ((size >> 3) & 3)) * 3; // max = 9
        if(h > dh) h = dh;
    }


    this->terminal_size = pair_factory(dw, dh, ds);
    this->display_size = pair_factory(w, h, size);
    this->lvl = REGULAR;
    this->regular = new draw_buffer(w, h, c);
    this->high = new draw_buffer(w, h, c);
    this->back = new draw_buffer(w, h, c);
    this->is_alfa = 0;
}

void init_drawing(int size, char clr){
    dr_pack = new draw_pack(size, clr);
}

draw_pack::~draw_pack(){
    delete this->regular;
    delete this->high;
    delete this->back;
    pair_free(this->terminal_size);
    pair_free(this->display_size);
    this->echo.clear();
}

void free_drawing(){
    delete dr_pack;
}

/** draw number by align 0 = on left, 1 = center, 2 = on right  */
int draw_int_align(int x, int width, int num, int clr, int align){
	int t[5];
	for(int j=0; j<5;j++) t[j] = 0;
	int v = num;
	int i = 0;

	if(v < 0) v = -v;

	if(v >= 1000){
        v /= 1000;
        t[i] = 'k';
        if(v >= 1000){
            v /= 1000;
            t[i] = 'm';
        }
        i++;
	}

	if(v > 99){
        t[i++] = (v % 10) + '0';
        v = v / 10;
        t[i++] = (v % 10) + '0';
        v = v / 10;
        t[i++] = v + '0';
	}else if (v > 9){
        t[i++] = (v % 10) + '0';
        v = v / 10;
        t[i++] = v + '0';
	}else{
        t[i++] = v + '0';
	}

	if(num < 0){
        t[i++] = '-';
	}

    int of = 0;
    if(align == 1) of = (width - i) >> 1; // center
    if(align == 2) of = (width - i); //right
    set_position(x + of, get_position_y());

    for(int j=i-1; j>=0;j--)
        draw_char(t[j], clr);
    return i;
}

int draw_int(int num){
    return draw_int_align(get_position_x(), 6, num, 0, 0);
}

void draw_echo(int c){
    if(c == '\n'){
        dr_pack->echo.clear();
    }else if(c < 32){
        int v = abs(c);
        int n = 0;
        if (v > 999) {  n = v / 1000; v -= n * 1000; dr_pack->echo.push_back(n + '0'); }
        if (v > 99) { n = v / 100; v -= n * 100; dr_pack->echo.push_back(n + '0'); }
        if (v > 9) { n = v / 10; v -= n * 10; dr_pack->echo.push_back(n + '0'); }
        dr_pack->echo.push_back(v + '0');
    }else{
        dr_pack->echo.push_back(c);
    }
}

void draw_clear(int lvl){ dr_pack->clear(lvl); }

void draw_pack::clear(int lvl){
    if(lvl & 1) this->regular->clear(lvl);
    if(lvl & 2) this->high->clear(lvl);
    if(lvl & 4) this->back->clear(lvl);
}

//maybe private
draw_buffer* draw_pack::get_lvl(){ return this->buf(this->lvl); }

draw_buffer* get_lvl() { return dr_pack->get_lvl(); }

bool draw_pack::add_char(char c, int clr){
    if(on_buf){
        echo.push_back(c);
        return true;
    }
    return get_lvl()->add_char(c, clr);
}

bool draw_char(char c, int clr){
    if(c == '\n'){
        next_row();
        return true;
    }
    if(c == '\t'){
        next_col();
        return true;
    }
    return dr_pack->add_char(c, clr);
}

void next_row(){
    dr_pack->next_row();
}

/** arg = 1-left, 2-right, 3-center, 4-block, 8-opacity, 16-row */
void draw_align(int arg){
    get_lvl()->to_align(arg);
}

void draw_select_buffer(int lvl){ dr_pack->set_lvl(lvl); }

void draw_pack::set_lvl(int level){ this->lvl = level; }

void draw_reset_cursor(){ dr_pack->reset_cursor(); }

void draw_pack::reset_cursor(){
    if(this->on_buf){
        echo_cursor = 0;
        return;
    }
    this->cx = 0;
    this->cy = 0;
}

char draw_get_cur_char(){ return dr_pack->draw_get_cur_char(); }

char draw_pack::draw_get_cur_char(){
    if(this->on_buf){
        int size =  this->echo.size();
        if(echo_cursor < size)
            return this->echo[echo_cursor];
        return '\0';
    }

    int i = this->cx;
    int j = this->cy;
    if(i >= this->display_size->value){ return '\n'; }
    char c = this->high->get_char(i, j);
    while(1){
        if(c > ((this->is_alfa & HIGH)? 32 : 31)) break;
        c = this->regular->get_char(i, j);
        if(c > ((this->is_alfa & REGULAR)? 32 : 31)) break;
        c = this->back->get_char(i, j);
        break;
    }
    if(c == '\n') return '\n';
    if(c < 32) return 32;
    return c;
}

int draw_next_cursor(){ return dr_pack->next_cursor(); }

int draw_pack::next_cursor(){
    if(this->on_buf){
        echo_cursor++;
        int size = this->echo.size();
        if(echo_cursor < size)
            return echo_cursor;
        return -1;
    }

    this->cx++;
    // cx equals width send \n
    if(this->cx > display_width()){
        this->cx = 0;
        this->cy++;
    }
    if(cy >= display_height()) return -1;
    return cy * display_width() + cx;
}

void draw_validate(){ dr_pack->draw_validate(); }

void draw_pack::draw_validate(){
    int rows = display_height();
    int len = display_width();
    char lclr = this->regular->get_clr_default();
    char hclr = this->high->get_clr_default();
    char bclr = this->back->get_clr_default();

    int alfa_high = (this->is_alfa & HIGH)? 32 : 31;
    int alfa_low = (this->is_alfa & REGULAR)? 32 : 31;

    for (int j=0;j<rows;j++){
        for (int i=0;i<len;i++){
            int s = this->echo.size();
            if(j==0 && i < s){
                draw_simbol(this->echo[i], 0);
                continue;
            }

            char c = this->high->get_char(i, j);
            if(c > alfa_high){
                char clr = this->high->get_clr(i, j);
                if (clr>='0') hclr = clr;
                draw_simbol(c, hclr);
                continue;
            }
            c = this->regular->get_char(i, j);
            if(c > alfa_low){
                char clr = this->regular->get_clr(i, j);
                if (clr>='0') lclr = clr;
                draw_simbol(c, lclr);
            }else{
                char clr = this->back->get_clr(i, j);
                if (clr>='0') bclr = clr;
                draw_simbol(this->back->get_char(i, j), bclr);
            }
        }
        draw_simbol('\n', 0);
    }
}

int draw_pack::display_height(){ return this->display_size->subval; }
int draw_pack::display_width(){ return this->display_size->value; }
int draw_display_height(){ return dr_pack->display_height(); }
int draw_display_width(){ return dr_pack->display_width(); }



void draw_end(char c, int clr){
    get_lvl()->to_last(c, clr);
}

int draw_as_line(char chr, int clr, int amount){
    if(amount < 1) return 0;
    draw_buffer *p = get_lvl();
    int s = p->get_width();
    if(amount < s) s = amount;
    for(int j=0; j < s; j++)
        if(!p->add_char(chr, clr)) return j+1;
    return s;
}

int next_col(){
    return get_lvl()->tab();
}

int draw_selected_line(const char* row, int slct, int pos){
    char c;
	char clr='D';
	char sclr='Y';
	for(int j=0;j<100;j++){
        c = row[j];
        if(c=='\0') return 0;
        if(c=='\n') return 1;
        if(c=='>'){
            if(slct){
                if(!draw_char(c,sclr))break;
            }else{
                if(!draw_char(' ',clr))break;
            }
            continue;
        }
        if(c=='<'){
            if(slct){
                if(!draw_char(c, sclr))break;
            }else{
                if(!draw_char(' ',clr))break;
            }
            continue;
        }
        if(j==0){
            clr=c;
            continue;
        }
        if (j == pos){
            if(!draw_char(c, sclr))break;
        }else{
            if(!draw_char(c,clr))break;
        }
	}
	return -1;
}

int get_position_x(){
    return get_lvl()->get_cursor_x();
}
int get_position_y(){
    return get_lvl()->get_cursor_y();
}

void draw_pack::draw_swap(int a){
    if(a == (HIGH | REGULAR)){
        draw_buffer* b = dr_pack->high;
        dr_pack->high = dr_pack->regular;
        dr_pack->regular = b;
    }
    if(a == (BACK | REGULAR)){
        draw_buffer* b = dr_pack->back;
        dr_pack->back = dr_pack->regular;
        dr_pack->regular = b;
    }

    if(a == (BACK | HIGH)){
        draw_buffer* b = dr_pack->back;
        dr_pack->back = dr_pack->high;
        dr_pack->high = b;
    }
}

draw_buffer* draw_pack::buf(int v){
    if(v == HIGH)
        return dr_pack->high;
    if(v == BACK)
        return dr_pack->back;
    return dr_pack->regular;
}

void draw_pack::next_row(){
    if(on_buf){
        echo.push_back('\n');
        return;
    }
    get_lvl()->next_row();
}


void draw_pack::zip(int f, int t){
    draw_buffer *bf = this->buf(f);
    draw_buffer *bt = this->buf(t);
    if(bf == bt) return;
    int rows = this->display_size->subval;
    int len = this->display_size->value;
    int opacity = (this->is_alfa & f) ? 33 : 32;
    for (int j=0;j<rows;j++){
        for (int i=0;i<len;i++){
            char c = bf->get_char(i, j);
            int clr = bf->get_clr(i, j);
            bf->set_char_and_clr(i, j, 0, 0);
            if(c < opacity) continue;
            bt->set_simbol(i, j, c, clr);
        }
    }
}

int draw_pack::draw_info(int v){
    if(v == 7) return this->lvl;
    if(v & DRAWING_OPTION){
        int res = 0;
        if(v & HIGH){
            if(this->is_alfa & HIGH) res |= DRAWING_ALFA;
        }
        if(v & REGULAR){
            if(this->is_alfa & REGULAR) res |= DRAWING_ALFA;
        }
        if(v & BACK){
            if(this->is_alfa & BACK) res |= DRAWING_ALFA;
        }
        return res;
    }

    {
        int res = 0;
        if(v & HIGH){
            if(this->is_alfa & HIGH) res |= this->high->get_options(v);
        }
        if(v & REGULAR){
            if(this->is_alfa & REGULAR) res |= this->regular->get_options(v);
        }
        if(v & BACK){
            if(this->is_alfa & BACK) res |= this->back->get_options(v);
        }
        return res;
    }
    return -1;
}

int draw_info(int v){
    return dr_pack->draw_info(v);
}

void draw_pack::set_options(int opt, int val){
    if(opt == DRAWING_OPTION){
        if(val == DB_OPT_CUT){
            on_buf = true;
            return;
        }
        if(val == DB_OPT_RESET){
            echo.clear();
            on_buf = false;
            return;
        }
    }

    if(opt & DRAWING_OPTION){
        int arg = opt & 7;
        int a = val ^ (val & 7);
        if(a == DRAWING_ZIP){
            this->zip(arg, val & 7);
            return;
        }
        if(a == DRAWING_ALFA){
            this->is_alfa |= arg; // on
            this->is_alfa ^= this->is_alfa & (val & 7); //off
            return;
        }
    }

    if(opt & HIGH) high->set_option(opt, val);
    if(opt & REGULAR) regular->set_option(opt, val);
    if(opt & BACK) back->set_option(opt, val);
}

void draw_options(int opt, int val){ dr_pack->set_options(opt, val); }

void set_position(int x, int y){ get_lvl()->set_cursor(x, y); }

void draw_tabs(int count, const char** text, int slct){
    char clr = 'D';
    for(int i=0;i<count;i++){
        draw_char('[', clr);
        draw_char(' ', clr);
        if (slct == i){
            for(int j=0; j<20;j++){
                if(text[i][j] == 0) break;
                draw_char(text[i][j], clr);
            }
        }else{
            draw_char(text[i][0], 'Y');
        }
        draw_char(' ', clr);
        if(i + 1 == count){
            draw_end('_', clr);
        }else{
            draw_char('/', clr);
        }
    }
    next_row();
    draw_end('=', clr);
    next_row();
}

int draw_text(const char* row, char clr){
    for(int i=0;i<1000;i++){
        char c = row[i];
        if(c == 0) return i;
        if(!draw_char(c, clr))return i;
    }
    return get_position_x();
}

/** draw text by align 0 = on left, 1 = center, 2 = on right */
void draw_text_align(int x, int width, const char* row, char clr, int align){
    int count = 0;
    int max_width = draw_display_width();
    for(int i=0;i<max_width;i++){
        if(row[i] == 0){
            count = i;
            break;
        }
    }
    int of = 0;
    if(align == 1) of = (width - count) >> 1; // center
    if(align == 2) of = (width - count); //right
    set_position(x + of, get_position_y());
    for(int i=0;i<count;i++){
        draw_char(row[i], clr);
    }
}

/* deprecated */
void draw_text_position(int start_pos, int end_pos, int rows_count, int row_pos, const char *text){
    int rows = row_pos;
    int index = 0;
    int len = end_pos - start_pos;
    while(rows > 0){
        len--;
        char c = text[index];
        index++;
        if(len < 1){
            rows--;
            if(rows <= 0) break;
            len = end_pos - start_pos;
        }

        if(c == '\0') return;
        if(c == '\n'){
            len = 0;
            continue;
        }
        if(c == '\t'){
            len -= 5;
            continue;
        }
    }
    rows = rows_count;
    len = end_pos - start_pos;
    get_lvl()->set_cursor(start_pos, get_lvl()->get_cursor_y());
    while(rows > 0){
        len--;
        char c = text[index];
        index++;
        if(len < 1){
            next_row();
            rows--;
            if(rows <= 0) return;
            len = end_pos - start_pos;
            get_lvl()->set_cursor(start_pos, get_lvl()->get_cursor_y());
        }

        if(c == '\0') return;
        if(c == '\n'){
            len = 0;
            continue;
        }
        if(c == '\t'){
            len = (end_pos - next_col());
            continue;
        }
        draw_char(c, 'D'); ///todo if(!draw_char(c, 'D')) what next step ? break?
    }
}

int draw_char_at_position(char c, char clr, int x, int y){
    if(x < 0 || y < 0) return 0;
    if(x < draw_display_width() && y < draw_display_height()){
        get_lvl()->set_simbol(x, y, c, clr);
        return 1;
    }
    return 0;
}

int actuale_size(const char *s){
    for(int i = 0; i < 100; i++){
        if(s[i] == 0){
            return i;
            break;
        }
    }
    return 0;
}
