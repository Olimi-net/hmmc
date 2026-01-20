#include "test_main.h"
#include "../legacy/drawing.h"
#include "../keyboard.h"
#include "../components/cmp_about.h"
#include "../components/cmp_menu.h"
#include "fake_map.h"
#include "../legacy/hmmc_file_mgr.h"
#include "../components/cmp_map.h"

cmp_basic* cmp;

void clear_all(){
    printf("\n\n");
}

void draw_main(int a){
    clear_all();
    cmp->on_draw(a);
    draw_validate();
}

void press_cmd(int c, int k){
    cmp->on_press(c, k);
}

void press_char(int c){
    press_cmd(c, 0);
}

void press_key(int k){
    press_cmd(0, k);
}

void  press_arrow(int c){
    press_cmd(0, c);
}

void on_tick(bool a, int anime){
    if(a){
        //user command on tick
        draw_main(anime);
    }else{
        //animation tick, unit move and etc.

        draw_main(anime);
    }
    cmp->on_tick(anime);
}

void press_button(char c){
    if('A'<=c && c<='Z')
        press_char(c | 32);
    if(32 <=c && c<127)//backspace
        press_char(c);
}

void onkeypressed(int c){
     if(((c & ARROW_KEY) == ARROW_KEY) || c == 9){
        press_arrow(c);
     }else if(c < 32 || c == 127){
        press_key(c);
     }else{
        press_button(c);
     }
}

void screen(){
    hmmc_file_mgr m = hmmc_file_mgr();
    if(m.open_file("/home/homyak/Dev/cpp/hmmc_v5/log/screen.txt", HMMC_FILE_MGR_WRITE)){
        draw_reset_cursor();
        int a = 0;
        while(a >= 0){
            m.write_char(draw_get_cur_char());
            a = draw_next_cursor();
        }
    }
}

int test_main(){

    int anime = 0;
	int c=0;
    int keep_run = 9999999;

    printf("select test:\n\tA - about\n\tB - battle\n\tC - city\n\tM - main menu\n");
    printf("\tW - world\n\t! - exit\n");
    c=getchar();
    if((c | 32) == 'a'){
        cmp = new cmp_about(nullptr);
    }else if((c | 32) == 'b'){
        cmp = new fake_map(nullptr, 1 | FAKE_MAP_BATTLE);
    }else if((c | 32) == 'c'){
        cmp = new fake_map(nullptr, 1 | FAKE_MAP_CITY);
    }else if((c | 32) == 'm'){
        cmp = new cmp_menu(nullptr);
    }else if((c | 32) == 'w'){
        hmmc_file_mgr m = hmmc_file_mgr();
        m.get_files("/home/homyak/Dev");
        while(getchar() != '!');
        return 0;

        //cmp = new cmp_map(nullptr);
    }else{
        printf("\nBye!\n");
        return 0;
    }

	do{
        keep_run--;
		c=getchar();
        anime = (anime + 1) & 15;
        if(c=='!') {
            screen();
            printf("\nBye!\n");
            break;
        }
        printf("%d\n",c);
        /*
        if (c == 10){
            on_tick(false, anime);
            continue;
        } */
        if(c==ARROW_KEY){
			c=getchar();
			press_arrow(c);
		}else{
            onkeypressed(c);
		}
		on_tick(true, anime);


	}while(keep_run > 0);
	//free_page_mgr();
	delete cmp;
	return 0;
}
