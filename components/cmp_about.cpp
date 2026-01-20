#include "cmp_about.h"
#include "../legacy/static_city.h"
#include "../legacy/static_game.h"
#include "../legacy/static_unit.h"
#include "../legacy/static_hero.h"
#include "../legacy/book_of_magic.h"

#include <cmath>

const int ABOUT_TAB_SIZE = 10;
const int AK_QUIT = 1;

const char* menu_help_tab_title_10[] = {
	"ABOUT GAME", "HEROES", "STRONGHOLD",	"RED CASTLE",	"WHITE TOWER", "FOREST UNION","MACHINES", "VILLAGES", "BOOK of MAGIC", "QUIT",
};

const char* about_game_13[] = {
    "Hour of Midnight Magic in Castle", "\n",
    "Found magic in castle", "Get strong warrior", "Make an union with druids", "Ride the horse",
    "Go meet adventures", "\n", "Game created by Olimi",  "Version 0.5 (c) 2025", "site: http:\\\\olimi.net\\hmmc\\","",
    "Press Q to return to the main menu",
};

cmp_about::cmp_about(cmp_basic* parent):cmp_basic(parent){
    tab_indx = 0;
    scroll_offset = 0;
}

cmp_about::~cmp_about(){

}

int cmp_about::on_event(cmp_basic *s, hmmc_event *e){
    return 0;
}

int cmp_about::on_tick(int anime){
    return 0;
}

void cmp_about::about_unit(int city){
    next_col();
    draw_text("UNITS", 0);
    next_col();
    draw_text("Description", 0);
    next_row();
    next_row();
    for (int j=(city & SPECIAL)?1:0;j<5;j++){
        int lvl = 1 << j;
        int is_big = static_unit_is_huge(city|UNIT|lvl);
        for(int row=0;row<3;row++){
            if(is_big == 0){
                next_col();
            }else{
                draw_text(" ", 0);
            }
            static_draw(city|UNIT|lvl, row, AN_LEFT);
            if ((row & 1) == 0){
                next_col(); static_draw(city|UNIT|TEXT|lvl, row == 0 ? 0 : 9, 0);
            }
            next_row();
        }
        next_row();
    }
}

void cmp_about::about_heroes(){
    for (int i=1;i<25;i++){
        static_hero_draw_name(i, 0);
        set_position(12, get_position_y());
        int u = static_hero_get_unit_type(i, 1);
        static_draw(u | TEXT, 0, 0);
        set_position(36, get_position_y());
        u = static_hero_get_unit_type(i, 2);
        static_draw(u | TEXT, 0, 0);
        next_row();
    }
}


/* todo
        Polly

    Primary unit:   First aid tent
    Support unit:   Barbarian

    Polly дочь такого и т.д.

    Polly specialization by First aid tent.
    Полли получает палатку первой помощи в начале игры
    улучшения:
        увеличивает лечение
        управление палаткой
        палатка может воскрешать
*/

void cmp_about::about_book(){
    int id = scroll_offset * ((BOOK_MAGIC_COUNT + 2) >> 3);
    for(int j = 0;j<4;j++){
        for(int i = 0; i<4;i++){
            book_of_magic_draw(id + j, i, 0, true);
            next_col();
            if(i == 1 || i == 2) draw_text(book_of_title(id + j, i == 2), 'W');
            next_row();
        }
        next_row();
    }
    draw_text("Page ", 0);
    draw_int(scroll_offset);
}

void cmp_about::on_draw(int anime){
    if(is_load){
        draw_clear(511);
        is_load = false;
    }else{
        draw_clear(REGULAR);
    }
    draw_select_buffer(REGULAR);

    draw_tabs(ABOUT_TAB_SIZE, menu_help_tab_title_10, tab_indx);
    if(tab_indx == 0){
        for(int i=0;i<12;i++){
            if(about_game_13[i][0] == 0) return;
            next_row();
            next_col();
            for(int j=0;j<100;j++){
                if(about_game_13[i][j] == 0) break;
                draw_char(about_game_13[i][j], '7');
            }
        }
        return;
    }
    // todo heroes move to info city next pages
    if(tab_indx == 1){
        about_heroes();
        return;
    }

    if(tab_indx == 2){
        about_unit(STRONHOLD);
        return;
    }

    if(tab_indx == 3){
        about_unit(RED_CASTLE);
        return;
    }

    if(tab_indx == 4){
        about_unit(WHITE_TOWER);
        return;
    }

    if(tab_indx == 5){
        about_unit(FOREST_UNION);
        return;
    }

    if(tab_indx == 6){
        about_unit(SPECIAL);
        return;
    }

    if(tab_indx == 7){
        about_unit(VILLAGE);
        return;
    }


    if(tab_indx == 8){
        about_book();
        return;
    }

    if(tab_indx == ABOUT_TAB_SIZE - 1){
        for(int j=0;j<100;j++){
            if(about_game_13[12][j] == 0) break;
            draw_char(about_game_13[12][j], 'M');
        }
    }
}

int cmp_about::on_press(int c, int k){
    if (k == ARROW_LEFT){
        tab_indx = (tab_indx < 1)? ABOUT_TAB_SIZE - 1 : tab_indx - 1;
        scroll_offset = 0;
        return 0;
    }else if(k == ARROW_RIGTH || k == KEY__TAB){
        tab_indx = (tab_indx + 1 < ABOUT_TAB_SIZE)? tab_indx + 1 : 0;
        scroll_offset = 0;
        return 0;
    }
    //todo up-down not all pages

    if (k == KEY__BACKSPACE){
        tab_indx = (tab_indx < 1)? ABOUT_TAB_SIZE - 1 : tab_indx - 1;
        scroll_offset = 0;
        return 0;
    }
    if (k == KEY__ENTER){
        if(tab_indx == ABOUT_TAB_SIZE - 1) return AK_QUIT;
        return 0;
    }

    if (c == 'q') return AK_QUIT;
    if ('0' <= c && c <= '9'){
        scroll_offset = c - '0';
        return 0;
    }

    for(int i=0;i<ABOUT_TAB_SIZE;i++){
        if (c == (menu_help_tab_title_10[i][0] | 32)){
            tab_indx = i;
            return 0;
        }
    }


    return 0;
}
