#include "cmp_menu.h"
#include "cmp_about.h"
#include "cmp_campany.h"
#include "cmp_sal.h"
#include "cmp_settings.h"


const char* hmmctitle[] = {
	"6 __    __ #   _____ _____ #   _____ _____ #   _______     ",
	"6JR R  JR R#  JR    RR    R#  JR    RR    R#  J|  ____R    ",
	"6F R R_F_R R# F R RR RR RR R# F R RR RR RR R# F R R===J    ",
	"6 F R  ____ R# F R RR____RR R# F R RR____RR R# F R R       ",
	"6  F R R==JR R# F R R===JF R R# F R R===JF R R# F R R_____ ",
	"6   F R_R F R_R# F R_R    F R_R# F R_R    F R_R# F R______R",
	"6    FJ=J  FJ=J # FJ=J     FJ=J # FJ=J     FJ=J # F=======J",
	"2 *  Hour of Midnight Magic in Castle  *  BY OLIMI  *",
};

const char* translatemenu[13] = {
	"J4/","j6/","F4\\","R6\\","=4_","*1*","Z1N","T1L","S1S","X1H",//10
	"E1E","#0 ","A1C",
};

const char* hmmcarraymenu[] = {
	"21.>New game< \n", //
	"22.>Load game<\n", //
	"23.>Settings< \n", //
	"24.>About<    \n", //
	"25.>Exit<     \n", //
	"\0"
};

const char* cmd_menu[] = { "1n","2l","3s","4a","5e" };
const int menu_keys[]  = { CMP_NEW, CMP_LOAD, CMP_SETTINGS, CMP_ABOUT, CMP_EXIT };
const int cmd_size = 5;

cmp_menu::cmp_menu(cmp_basic* parent):cmp_basic(parent){
    selected_line = 0;
    is_dialog = false;
    is_page = false;
    dialog = nullptr;
    page = nullptr;
}

cmp_menu::~cmp_menu(){
    if(dialog != nullptr){
        delete dialog;
    }
    if(page != nullptr)
        delete page;
}

int cmp_menu::on_event(cmp_basic *s, hmmc_event *e){
    return 0;
}

void cmp_menu::on_draw(int anime) {
    if(is_page){
        page->on_draw(anime);
        return;
    }

    if(is_load){
        draw_clear(511);
        is_load = false;
        draw_select_buffer(BACK);
        this->draw_title();
    }

    this->draw_menu(selected_line);

    if(is_dialog){
        if(dialog == nullptr){
            dialog = new cmp_dialog(this);
            dialog->set_title("Do you want to close game?");
            dialog->set_buttons(DIALOG_BTN_YES_NO);
        }
        dialog->on_draw(anime);
    }else{
        draw_clear(HIGH);
    }
}

int cmp_menu::resolve_key(int c, int k){
    if (k == KEY__ENTER && 0 < selected_line && selected_line < cmd_size){
        return menu_keys[selected_line];
	}
    if ('1' <= c && c <= '9'){
        int n = c - '1';
        if(n < cmd_size) return menu_keys[n];
        return 0;
    }
    if ('a' <= c && c <= 'z'){
        for(int i = 0; i < cmd_size; i++){
            int n = cmd_menu[i][0] - '1';
            if(cmd_menu[i][1] == c) return menu_keys[n];
        }
    }
    return 0;
}

int cmp_menu::on_press(int c, int k){
    if(is_page){
        if(page->on_press(c, k)){
            is_page = false;
            is_load = true;
        }
        return 0;
    }

    if (k == KEY__ESCAPE){
        is_dialog = !is_dialog;
        return 0;
    }

    if(is_dialog){
        if (c == 'y'){
            return CMP_EXIT;
        }
        if(c == 'n'){
            is_dialog = 0;
        }
        return 0;
    }

    if (k == KEY__ESCAPE){
        is_dialog = CMP_EXIT;
        return 0;
    }

    if (k == ARROW_TOP){
        selected_line--;
        if(selected_line < 0) selected_line = cmd_size - 1;
        return 0;
    }
    if (k == ARROW_DOWN || k == KEY__TAB){
        selected_line++;
        if(selected_line >= cmd_size) selected_line = 0;
        return 0;
    }

    int r = resolve_key(c, k);
    if(r == CMP_EXIT){
        is_dialog  = true;
    }else if(r){
        if(page != nullptr)
            delete page;
        if(r == CMP_ABOUT){
            page = new cmp_about(this);
        }else if(r == CMP_NEW){
            page = new cmp_campany(this);
        }else if(r == CMP_LOAD){
            page = new cmp_sal(this);
        }else if(r == CMP_SETTINGS){
            page = new cmp_settings(this);
        }
        is_page = true;
    }
    return 0;

}
int cmp_menu::on_tick(int anime){
    if(is_page) return page->on_tick(anime);
    return 0;
}

void cmp_menu::transleting(char c, int clr){
	for(int k=0;k<13;k++){
		if(translatemenu[k][0]==c){
			draw_char(translatemenu[k][2],translatemenu[k][1]);
			return;
		}
	}
	draw_char(c,clr);
}

void cmp_menu::draw_title(){
    draw_select_buffer(BACK);
    set_position(0, 2);
	char c;
	char clr='0';
	for(int i=0;i<7;i++){
        clr=hmmctitle[i][0];
		for(int j=1;j<100;j++){
            c=hmmctitle[i][j];
			if(c==0){   break; }
			transleting(c, clr);
		}
        next_row();
	}
	draw_align(15);
	next_row();
	clr=hmmctitle[7][0];
	for(int j=1;j<100;j++){
        c=hmmctitle[7][j];
        if(c==0){ break; }
        transleting(c, clr);
    }
    draw_align(19);
}

void cmp_menu::draw_menu(int slct){
    draw_clear(REGULAR);
    draw_select_buffer(REGULAR);
	set_position(0, 14);
	for(int i=0;i<cmd_size;i++){
        if(draw_selected_line(hmmcarraymenu[i], slct==i, 4) < 1) break;
        next_row();
	}
    draw_align(7);
}

