#include "hmmc_main.h"


void hmmc_main_ncurses_init(){
#ifndef _TEST_
	initscr();
	noecho();
	curs_set(false); //hide cursor
	cbreak();
	keypad(stdscr, false);
	halfdelay(1);
    start_color();
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(4, COLOR_RED, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_BLACK);
#endif // _TEST_
}


void hmmc_main_ncurses_free(){
#ifndef _TEST_
    curs_set(true); // show cursor
	endwin();
#endif // _TEST_
}
