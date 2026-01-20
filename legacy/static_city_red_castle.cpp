#include "static_city_red_castle.h"
#include "drawing.h"

/*
const int BUILD_RC_GRAND_HALL = 1;
const int BUILD_RC_RESIDENCE = 2;
const int BUILD_RC_DEALER = 4;
const int BUILD_RC_BOWMAN = 8;
const int BUILD_RC_GUARD = 16;
const int BUILD_RC_MONK = 32;
const int BUILD_RC_SANCTURE = 64;
const int BUILD_RC_ANGEL = 128;
const int BUILD_RC_TREASURE = 256;


max 3+ level = 127 | without LEVEL 4 and UNIQ BUILD
max 2+ level = 31 | without LEVEL 3 and LIBRARY
max 1 level = 15 | without LEVEL 2

DEALER -> BOWMAN -> ANGEL -> TREASURE
      \          \
      & wall     +-> GUARD
      /          /
GRAND_HALL -> RESIDENCE -> MONK -> SANCTURE


*/

const char* static_city_red_castle_text[]{
    "GRAND HALL", "RESIDENCE", "DEALER", "BOWMAN ROOM", "GUARD HALL", "MONK TEMPLE", "SANCTURE", "CATHEDRAL", "TREASURE"
};

const int static_city_red_castle_cost[][3] ={
    { 150, 15, 0 }, { 150, 15, 0 }, { 100, 10, 0 }, { 100, 10, 0 }, { 50, 5, 0 }, { 50, 5, 6 },  { 70, 5, 0 }, { 50, 5, 8 }, { 70, 5, 0 }
};

pair static_city_red_castle_get_cost(int v){
    pair p = {static_city_red_castle_cost[v][0], static_city_red_castle_cost[v][1], static_city_red_castle_cost[v][2]};
    return p;
}

const char* static_city_red_castle[][2] = {
    {"       ","       "}, // 0
    {"       ","P      "}, // 1 empty=1 residence=0
    {"      /","\\      "}, // 2 empty=2
    {"     | "," |     "}, // 3 empty=3

    {" _____/","\\_____ "}, // 4 dealer=2 great-hall=2
    {"|''''| "," |''''|"}, // 5 dealer=3 great-hall=3

    {"   ____","|___   "}, //6 bowman=1 residence=1
    {" _/___/","\\___\\_ "}, //7 bowman=2 residence=2

    {"   []  ","P []   "}, //8 treasure=0 sanctuary=0
    {"   []__","|_[]   "}, //9 angel=1  monk=1


/* red castle
       P
      /\
     |  |

       P
 _____/\_____
|    |  |    |   dealer great-hall

       P
   ____|___
 _/___/\___\_    bowman residence
|    |  |    |   dealer great-hall

       P
   ___[]___       guard
 _/___/\___\_    bowman residence
|''''|  |''''|   dealer great-hall

   []  P []      treasure sanctuary
   []_[]_[]      angel guard monk
 _/___/\___\_    bowman residence
|    |  |    |   dealer great-hall

 residence sanctuary warehouse (treasury) great-hall
*/

};

int static_city_red_castle_build(int level, int build){
    if(build == BUILD_RC_TREASURE)
        return (level & BUILD_RC_ANGEL) ? level | build: -1;
    if(build == BUILD_RC_SANCTURE)
        return (level & BUILD_RC_MONK) ? level | build: -1;
    if(build == BUILD_RC_ANGEL)
        return (level & BUILD_RC_BOWMAN) ? level | build: -1;
    if(build == BUILD_RC_MONK)
        return (level & BUILD_RC_RESIDENCE) ? level | build: -1;
    if(build == BUILD_RC_GUARD)
        return ((level & (BUILD_RC_RESIDENCE | BUILD_RC_BOWMAN)) == (BUILD_RC_RESIDENCE | BUILD_RC_BOWMAN)) ? level | build: -1;
    if(build == BUILD_RC_BOWMAN)
        return (level & BUILD_RC_DEALER) ? level | build: -1;
    if(build == BUILD_RC_RESIDENCE)
        return (level & BUILD_RC_GRAND_HALL) ? level | build: -1;
    if(build == BUILD_RC_DEALER)
        return level | build;
    if(build == BUILD_RC_GRAND_HALL)
        return level | build;

    return -1;
}

void static_city_red_castle_draw_town(int level, int row){
    int left = 0;
    int right = 0;

    if(row == 3){
        left = (level & BUILD_RC_DEALER)? 5 : 3;
        right = (level & BUILD_RC_GRAND_HALL)? 5 : 3;
    }else if(row == 2){
        if(level & BUILD_RC_BOWMAN){
            left = 7;
        }else if(level & BUILD_RC_DEALER){
            left = 4;
        }else{
            left = 2;
        }

        if(level & BUILD_RC_RESIDENCE){
            right = 7;
        }else if(level & BUILD_RC_GRAND_HALL){
            right = 4;
        }else{
            right = 2;
        }
    }else if(row == 1){
        if(level & BUILD_RC_ANGEL){
            left = 9;
        }else if(level & BUILD_RC_BOWMAN){
            left = 6;
        }else{
            left = 1;
        }

        if(level & BUILD_RC_MONK){
            right = 9;
        }else if(level & BUILD_RC_RESIDENCE){
            right = 6;
        }else{
            right = 1;
        }
    }else if(row == 0){
        left = (level & BUILD_RC_TREASURE)? 8 : 0;

        if(level & BUILD_RC_SANCTURE){
            right = 8;
        }else if(level & BUILD_RC_RESIDENCE){
            right = 1;
        }else{
            right = 0;
        }
    }

    char c;
    for (int i=0;i<7;i++){
        c = static_city_red_castle[left][0][i];
        if(i == 6 && row == 1 && (level & BUILD_RC_GUARD))
            c = '[';
        //todo translate
        draw_char(c, '7');
    }

    for (int i=0;i<7;i++){
        c = static_city_red_castle[right][1][i];
        if(i == 0 && row == 1 && (level & BUILD_RC_GUARD))
            c = ']';
        //todo translate
        draw_char(c, '7');
    }
}

const char* static_city_red_castle_title(int row){
    if(row >= 0 && row < 10)
        return static_city_red_castle_text[row];
    return "";
}

void static_city_red_castle_draw(int level, int row){
    if(row < 0) return;
    if(row < 4){
        static_city_red_castle_draw_town(level, row);
        return;
    }

    if(row < 13){
        draw_text(static_city_red_castle_text[row-4], 0);
    }
}

int static_city_red_castle_growl(int level){
    int n = 0;
    if(level & BUILD_RC_BOWMAN) n |= LEVEL_1;
    if(level & BUILD_RC_GUARD) n |= LEVEL_2;
    if(level & BUILD_RC_MONK) n |= LEVEL_3;
    if(level & BUILD_RC_ANGEL) n |= LEVEL_4;
    if (n == 0) return 0;
    return RED_CASTLE | UNIT | n;
}
