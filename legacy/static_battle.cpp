#include "static_battle.h"
#include "drawing.h"

const int STATIC_BATTLE_START_X = 22;

const char* static_battle[] = {
    "/___", // повторяемая линия верх стены = 22

    //row=1 ступени = 23
    " /    /",
    "/____// /",
    "|_|__|_// /",
    "|__|___|_// /",
    "|_|__|___|_/ /",
    "|__|___|___|/",

    // row=7 кирпич стены
    "|___",


    //row=8 боковая стена - начало от половины стены - связано с генератором
    "/_/ / / /",
    "/ /|/|/|/",

    //row=10
    " /_/_/_/_/", // angle down wall

    // row=11 1/2 киpпичa cтeны
    "|_",
    // row=12
    "_",

    //row=13 keg
    " ,___________, ",
    " |__:_____:__| ", // x2
    //row=15 broken keg
    " ,___    ____, ",
    " |__:\\  /_:__| ",
    " |__:_L/__:__| ",
    //row=17 keg stamp
    " |_| |___| |_| ",
    "   |_|   |_|   ",

    //row=20 hexagon floor
    " | +",
    " / \\",

    // row=22 wood floor
    "/__/. /. /  ",
    "/. /  /__/. ",
    "/  /__/. /. ",

    // row=25 arrows
    ">>-->",
    "<--<<",
    "====@",
    "@====",
    "--=<@",
    "@>=--",
    "-<O",
    "O>-",
    "c[]",

    //row=34 lightning 18 lines
    "               |            ",
    "              / \\           ",
    "             /\\  \\          ",
    "            / |   \\         ",
    "           |   \\   |        ",
    "          /    |  / \\       ",
    "         /    / \\    \\      ",
    "        /    /  |   / \\     ",
    "       /\\   |  / \\ |   \\    ",
    "      /  |  |     / \\  |    ",
    "     /   /   \\   /    / \\   ",
    "    /   /    /  /\\      |   ",
    "   |   /    |  |  \\      \\  ",
    "   |  |    /       |      \\ ",
    "  / \\  \\  /\\      / \\     | ",
    " |     /    \\    /   |    | ",
    " /    |      |      / \\    \\",
    "|    /       |     /   \\   |",

    //row=52 luck low
    "    __    __    ",
    "   /._|  |_.\\   ",
    "  /./      \\.\\  ",
    " |.|        |.| ",
    " |.|        |.| ",
    "[___]      [___]",

    //row=58 luck
    " ___     ___ ",
    "[   ]   [   ]",
    " |.|     |.| ",
    " |.|     |.| ",
    "  \\.\\___/./  ",
    "   \\_____/   ",

    //row=64
    "     /@>    ",
    "\\\\\\  ||  ///",
    " \\\\\\/''\\/// ",
    "  \\\\\\''///  ",
    "    //\\\\    ",
    "   ///\\\\\\   ",

    //row=70
    "     /@>    ",
    "     ||     ",
    "   //''\\\\   ",
    "  //\\''/\\\\  ",
    " // //\\\\ \\\\ ",
    "// ///\\\\\\ \\\\",

    //row=76 stone wall gate back
    "/_/ / /",
    "/|/|/",
    "/ / /",
    "/|/",
    "| /",

    // row 81 stone wall close gate back
    "||/",  // fragment x2
    "|/",     // before last fragment
    " /",

    // row 84 stone wall open gate back
    "_|/______",
     "/______/", // last fragment x3

    // row 86 stone wall high gate
        "/\\_/\\_/\\",
        "\\______/",
          "|_|__|",
          "|__|_|",
          "|/  /|",
          "/__/ |",
         "/  /|/|",
        "/__/ / /",
       "/__/|/|/",
       "|__|/ /",
       "|  ||/",
       "|__|/|",
       "/ /|/|",
      "/_/ / /",

    // row 100 stone wall chapter
    "/ /|/|/",
    "/_/ / /",
    "/ /|/|/",

    // row 103 catapult
    "+---(*)-----\\@/",
    "[#]// \\\\",
    "  //   \\\\",

    //row 106 catapult attack
    "      @  \\",
    "       __/",
    "      /",
    "     /",
    "  (*)",
    " //T\\\\",
    "//[#]\\\\",

    //row 113 broken tower
     "/\\",
    "\\_\\",
      "|_|",
      "|__\\_",

    //row 116
    "",
    "",
    "",
    "",
    "",
};

/*

                                      /|||/
                               WW WW WW||/
                                \\||//||/
                                 |||| |/
                                _//\\_/
                               / ~ ~ /
                              /~ ~ ~/
                             / ~ ~ /
                            /~ ~ ~/
                           / ~ ~ /            +---(*)-----\@/
                          /~ ~ ~/             [#]// \\
                         / ~ ~ /                //   \\


        ________
  ___(*)        \@/
 [#]// \\
   //   \\                       @  \
                                  __/
                                 /
                                /
                             (*)        \@/
                            //T\\
                           //[#]\\
*/

const char* static_battle_translate[][1] = {
    { ".\7D" },
};

void static_battle_draw_catapult(int state, int ox, int oy, int clr){
    if(state & 2) draw_select_buffer(HIGH);
    if(state & 1){
        for(int i = 0; i < 7; i++){
            set_position(ox + 2, oy + i - 4);
            draw_text(static_battle[106 + i], clr);
        }
        return;
    }
    for(int i = 0; i < 3; i++){
        set_position(ox, oy + i);
        draw_text(static_battle[103 + i], clr);
    }
}

/** wall: 1=close gate, 2=stone wall, 4=destroyed tower, 8=broken wall*/
void static_battle_draw_town(int wall, int ox, int oy, int clr){
    //todo translate color
    bool tower_is_destroyed = (wall & 4);
    bool gate_is_close = (wall & 1);
    if(wall & 2){ // stone wall
        draw_select_buffer(BACK);
        int x = 35 + ox;
        int y = 0; // = 0
        int k = 0;
        int r = 0;
        int xo = 0;
        for(int i = 3; i<21;i++){
            if(i == 0  || i == 3) { k = 100; r = 0; }
            else if(i == 6) { k = 76; r = 0; }
            else if(i == 7) { xo = 2; }
            else if(i == 9) { xo = 4; }
            else if(i == 11) { r = 0; k = (gate_is_close) ? 81 : 84; }
            else if(i == 12 && gate_is_close) { xo = 4; r = 0; k = 81; }
            else if(i == 12) { xo = 6; r = 0; k = 85; }
            else if(i == 13 && gate_is_close) { xo=5; r = 0; k = 82; }
            else if(i == 13) { r = 0; k = 85; }
            else if(i == 14) { r = 0; k = (gate_is_close) ? 83 : 85; }
            else if(i == 15) { xo = 0; continue; }
            // todo broken wall
            else if(i == 16 ) { k = 100; r = 0; }
            else if(i == 19 || i == 22) { k = 101; r = 0; }
            set_position(x - i + xo, y + i);
            draw_text(static_battle[k + r], clr);
            r++;
        }

        draw_select_buffer(HIGH);
        x = 26 + ox;
        y = 2 +oy;


        for(int i = 0; i<15;i++){
            if(i == 0){
                set_position(x - 3, y + i);
            }else if(i == 1){
                set_position(x - 3, y + i);
            }else if(i > 8 && i < 12){
                set_position(x - 5, y + i);
            }else if(i > 11){
                set_position(x - i + 7, y + i);
            }else if(i < 5){
                set_position(x - 2, y + i);
            }else{
                set_position(x - i + 3, y + i);
            }

            int f = 86;
            if(i < 4 && tower_is_destroyed){ f = 113; }

            draw_text(static_battle[f + i], clr);
        }
        draw_select_buffer(BACK);
    }
}

int static_battle_draw_line(int r, int x, int y, int clr, int tr){
    int count = 0;
    int color = clr;
    for(int i = 0; i<100;i++){
        color = clr;
        char c = static_battle[r][i];
        if(c == 0) return count;
        if(x + i < 0) continue;
        if(tr){
            //todo translate
            //todo color
        }
        count += draw_char_at_position(c, color, x+i, y);
    }
    return count;
}

int static_battle_row_count(int r){
    int count = 0;
    for(int i = 0; i<16;i++){
        char c = static_battle[r][i];
        if(c == 0) return count;
        count++;
    }
    return 16;
}

int static_battle_draw_end(int r, int x, int y, int clr, int tr){
    if(y < 0 || y >= draw_display_height() || x >= draw_display_width()) return 0;
    int x1 = abs(x);
    int size = static_battle_row_count(r);
    if(x < 0) x1 = 0 - (x1 % size);
    int count = 0;
    for(int i = 0; i<100;i++){
        int amount = static_battle_draw_line(r, x1 + i * size, y, clr, tr);
        count += amount;
        if(amount == 0) return count;
    }
    return count;
}

int static_battle_draw_arrow(int i, int x, int y){
    int row = 0;
    int clr = '0';
    if(i < 1) return 0;
    if(i == 1){ row = 26; clr = 'Y'; }
    else if(i == 9){ row = 25; clr = 'Y'; }
    else if(i == 3){ row = 28; clr = 'W'; }
    else if(i == 11){ row = 27; clr = 'W'; }
    else if(i == 4){ row = 30; clr = 'R'; }
    else if(i == 12){ row = 29; clr = 'R'; }
    else if(i == 2){ row = 32; clr = 'D'; }
    else if(i == 10){ row = 31; clr = 'D'; }
    else if(i == 5 || i == 13){ row = 33; clr = 'B'; }
    else return 0;
    return static_battle_draw_line(row, x, y, clr, 0);
}

void static_battle_draw_effects(int x, int y, int v){
    draw_options(DRAWING_OPTION | HIGH, DRAWING_ZIP | REGULAR);
    draw_options(DB_OPT_CLR | REGULAR | BACK, 'D');
    draw_options(HIGH | DB_OPT_RESET, 0);
    draw_options(HIGH | DRAWING_OPTION, DRAWING_ALFA);
    draw_select_buffer(HIGH);
    draw_clear(HIGH);

    int size = (v == 1) ? 18 : 6;
    int row = 28 + v * 6;
    for(int i = 0; i < size; i++){
         static_battle_draw_line(row + i, x, y + i, 'W', 0);
    }
}

//flag: 1=top wall; 2=step; 4=down wall; 8=side wall; 16=wood floor; 32=hexagon floor
void static_battle_draw_basement(int x, int y, int flag){
    int wall_clr = 'R';

    if(flag & 16){ // draw wood floor
        int xo = x-1;
        int n = 0;
        for(int j = 0;j<18;j++){
            static_battle_draw_end(22 + n, xo--, y+j+2, wall_clr, 0);
            n++;
            if(n > 2) n = 0;
        }
    }

    if(flag & 32){ // draw hexagon floor
        int xo = x-1;
        for(int j = 0;j<18;j++){
            static_battle_draw_end(20 | (j&1), xo--, y+j+2, wall_clr, 0);
        }
    }

    if(x > 0 && (flag & 8)){ // draw side wall
        int xo = x-3;
        for(int j = 0;j<18;j++){
            int n = j & 1;
            static_battle_draw_line(8 | n, xo--, y+j-1, wall_clr, 0);
            if(xo + 9 < 0) break; // 9=wall
        }
    }

    if(flag & 1){ // draw top wall
        static_battle_draw_end(12, x-1, y-3, wall_clr, 0);
        static_battle_draw_end(0, x-2, y-2, wall_clr, 0);
        int size = (flag & 2) ? 4 : 5;
        int u = (flag & 8) ? 2 : -2;
        for(int j = 0;j<size;j++){
            int n = (j & 1) ? 0 : u;
            static_battle_draw_end(7, x+n, y+j-1, wall_clr, 0);

            if(flag & 8){
                if((j & 1)==0){
                    static_battle_draw_line(11, x, y+j-1, wall_clr, 0);
                }
            }else{
                if(j & 1){
                    static_battle_draw_line(11, x-2, y+j-1, wall_clr, 0);
                }
            }
        }
    }

    if(flag & 2){ // draw step
        for(int j = 0;j<6;j++){
            static_battle_draw_line(1+j, x-1, y+j-1, wall_clr, 1);
            if(j==0 && (flag & 4)){
                //todo draw char / in angle
            }
        }
    }

    if(flag & 4){ // down wall
        int q = x - STATIC_BATTLE_START_X; // angle down wall
        int offset = 0;
        int j = y + 19;
        if(flag & 8){
            static_battle_draw_line(10, q, j-2, wall_clr, 0);
            offset = static_battle_row_count(10);
        }
        static_battle_draw_end(12, q + offset, j-2, wall_clr, 0);
        static_battle_draw_end(0, q, j-1, wall_clr, 0);


        for(int i=0;i<4;i++){
            static_battle_draw_end(7, (i & 1)? q+2 : q, j+i, wall_clr, 0);
            if(i & 1){
                static_battle_draw_line(11, q, j+i, wall_clr, 0);
            }

        }
    }
}
