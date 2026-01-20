//apt-get install ncurses-dev //g++ main.cpp -lncurses -D _LINUX_

#include "main.h"
#include "legacy/hmmc_main.h"

int main(int argc, char* argv[]){
    int r = 0;
    hmmc_main_ncurses_init();
    //todo init from file or params
    init_drawing(-1, 0);

#ifdef _TEST_
    r = test_main();
#endif

    free_drawing();
    hmmc_main_ncurses_free();

    return r;
}
