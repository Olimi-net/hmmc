#include "cmp_dialog.h"

const char* static_dialog_buttons[] = { " Yes ", " OK  ", " No  ", "Close", "One", "Two", "Three", "Four" };

const int BNTS_CNT = 8;

cmp_dialog::cmp_dialog(cmp_basic *parent):cmp_basic(parent){
    this->parent = parent;
    set_title("");
    max_width = 0;
    buttons = 4;

    for(int i = 0; i < BNTS_CNT; i++)
        custom.push_back(static_dialog_buttons[i]);

    keys = "yonc1234";
}

cmp_dialog::~cmp_dialog(){
    rows.clear();
}


int cmp_dialog::on_event(cmp_basic *s, hmmc_event *e){
    return 0;
}

void cmp_dialog::on_draw(int anime) {
    draw_clear(HIGH);
    draw_select_buffer(HIGH);

    bool is_title = title[0] == 0 ? false : true;
    int h = 0;

    if(is_title) h = 2;
    int count = rows.size();
    h += count;
    if(buttons) h+=2;

    h+=4;
    int dh = draw_display_height();
    int ho = (dh - h) >> 1;

    int btn_count = 0;
    if(buttons){
        for(int i=0; i < BNTS_CNT; i++){
            if((1 << i) & buttons) btn_count++;
        }
        int b = btn_count * 7; //min size buttons + 2 offset
        if(max_width < b) max_width = b;
    }

    int width = max_width + 4;
    int dw = draw_display_width();
    int wo = (dw - width) >> 1;
    for(int i = 0; i < h; i++){
        set_position(wo - 1, i + ho);
        draw_as_line(' ', 0, width + 3);
        draw_char_at_position('#', 'Y', wo, i + ho);
        draw_char_at_position('#', 'Y', wo + width, i + ho);
        // draw_char_at_position('.', 'Y', wo-2, i + ho);
    }
    set_position(wo, ho);
    draw_as_line('#', 'Y', width);
    set_position(wo, ho + h - 1);
    draw_as_line('#', 'Y', width);
    set_position(wo-1, ho + h);
    draw_as_line(' ', 'Y', width);

    int t = ho + 2;
    int w2 = wo + 2;
    if(is_title){
        set_position(w2, t);

        draw_text_align(w2, max_width, title.data(), 0, 1);
        set_position(w2, t+1);
        draw_as_line('-', 0, max_width);
        t+=2;
    }

    for(int j = 0; j< count; j++){
        set_position(w2, t++);
        for(int i = 0; i < max_width; i++){
            char c = rows[j][i];
            if(c == 0){
                break;
            }
            //todo is selected
            draw_char(c, 0);
        }
    }

    if(btn_count){
        int ob = (max_width / btn_count - 5);
        int lo = (max_width - (btn_count * 5 + ob * btn_count)) >> 1;
        lo += ob >> 1;
        t++;
        for(int i=0; i < BNTS_CNT; i++){
            if((1 << i) & buttons){
                set_position(w2 + lo, t);
                std::string txt = custom[i];
                char *c = txt.data();
                draw_text(c, 0);
                lo += ob + 5;
            }
        }
    }
}
int cmp_dialog::on_press(int c, int k){
    if(k == KEY__BACKSPACE || k == KEY__ESCAPE) return -1;
    if(k == KEY__ENTER || c == ' '){
        int b = 0;
        for(int i=0; i < BNTS_CNT; i++){
            if((1 << i) & buttons) b++;
        }
        if(b == 1) return buttons;
    }

    for(int i = 0; i < BNTS_CNT; i++){
        int b = (1 << i);
        if(b & buttons){
            if(keys[i] == c) return b;
        }
    }
    return 0;
}
int cmp_dialog::on_tick(int anime){
    return 0;
}

void cmp_dialog::set_title(const char *s){
    int w = actuale_size(s);
    if(max_width < w) max_width = w;
    title = std::string(s).data();
}

void cmp_dialog::set_title(const char *str1, const char *str2){
    char buffer[128];
    for(int i = 0; i<128; i++)buffer[i]=0;
    snprintf(buffer, sizeof(buffer), "%s%s", str1, str2);
    std::string s = buffer;
    int w = s.size();
    if(max_width < w) max_width = w;
    title = std::string(s).data();
}

void cmp_dialog::set_title(const char *str1, const char *str2, const char *str3){
    char buffer[128];
    for(int i = 0; i<128; i++)buffer[i]=0;
    snprintf(buffer, sizeof(buffer), "%s%s%s", str1, str2, str3);
    std::string s = buffer;
    int w = s.size();
    if(max_width < w) max_width = w;
    title = std::string(s).data();
}

void cmp_dialog::set_row(const char *s){
    int w = actuale_size(s);
    if(max_width < w) max_width = w;
    if(max_width < w) max_width = w;
    rows.push_back(s);
}

void cmp_dialog::set_row(const char *str1, const char *str2){
    char buffer[128];
    for(int i = 0; i<128; i++)buffer[i]=0;
    snprintf(buffer, sizeof(buffer), "%s%s", str1, str2);
    std::string s = buffer;
    int w = s.size();
    if(max_width < w) max_width = w;
    rows.push_back(s);
}

void cmp_dialog::set_row(const char *str, int num){
    char buffer[128];
    for(int i = 0; i<128; i++)buffer[i]=0;
    snprintf(buffer, sizeof(buffer), "%s%d", str, num);
    std::string s = buffer;
    int w = s.size();
    if(max_width < w) max_width = w;
    rows.push_back(s);
}

void cmp_dialog::set_row(int num, const char *str){
    char buffer[128];
    for(int i = 0; i<128; i++)buffer[i]=0;
    snprintf(buffer, sizeof(buffer), "%d%s", num, str);
    std::string s = buffer;
    int w = s.size();
    if(max_width < w) max_width = w;
    rows.push_back(s);
}

void cmp_dialog::clear(){
    title="";
    rows.clear();
}

void cmp_dialog::set_buttons(int btn){
    buttons = btn;
}

void cmp_dialog::update_button(int btn, const char* text, char key){
    std::string s = text;
    for(int i=0; i < BNTS_CNT; i++){
        if((1 << i) == btn){
            custom[i] = s;
            keys[i] = key;
            return;
        }
    }
}
