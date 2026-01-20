#include "static_city_white_tower.h"
#include "drawing.h"

/*
const int BUILD_WT_ROCK=256;
const int BUILD_WT_LABORATORY=128;
const int BUILD_WT_ATHENEUM=64;
const int BUILD_WT_MAGE=32;
const int BUILD_WT_WALL=16;
const int BUILD_WT_FIGHTER=8;
const int BUILD_WT_TAVERN=4;
const int BUILD_WT_SHOP=2;
const int BUILD_WT_GRAND_HALL=1;

max 3 level = 255 | without LEVEL 4
max 3+ level = 127 | without LEVEL 4 and UNIQ
max 2+ level = 31 | without LEVEL 3 and LIBRARY
max 1 level = 15 | without LEVEL 2 and WALL
*/



const char* static_city_white_tower[][2] = {
    {"       ","       "}, // 0
    {"      (",")      "}, // 1
    {"      /","\\      "}, // 2
    {"     | "," |     "}, // 3

    {"   /\\ /","\\ /\\   "}, // 4 storehouse=2 | grand-hall=2
    {"  |[]| "," |[]|  "}, // 5 storehouse=3 | grand-hall=3

    {" /\\/\\ /","\\ /\\/\\ "}, // 6 fighter=2 | tavern=2
    {"|[][]| "," |[][]|"}, // 7 fighter=3 | tavern=3

    {"  /\\  (","   /\\  "}, // 8 laboratory=1 |  mage=0
    {" |[]| /",") |[]| "}, // 9 laboratory=2 |  mage=1


    {" WWWW (","  /()\\ "}, // 10 rock=1 | atheneum=0
    {" \\[]/ /",") \\[]/ "}, // 11 rock=2 | atheneum=1


    {"|TTTT| "," |TTTT|"}, // 12 warrior&wall=3

};

const char* static_city_white_tower_text[9]{
    "WHITE HALL", "SHOP",  "TAVERN", "HALL of FIGHTER", "WALL",  "AUDITORIUM of MAGE", "ATHENEUM", "LABORATORY", "NEST of ROCK"
};

const int static_city_white_tower_cost[][3] ={
    { 50, 6, 0 }, { 50, 6, 0 }, { 50, 6, 0 }, { 50, 6, 0 }, { 100, 10, 0 }, { 120, 12, 0 },  { 120, 12, 0 }, { 120, 8, 0 }, { 180, 15, 10 }
};

pair static_city_white_tower_get_cost(int v){
    pair p = {static_city_white_tower_cost[v][0], static_city_white_tower_cost[v][1], static_city_white_tower_cost[v][2]};
    return p;
}

/*
    GRAND_HALL -> TAVERN -> MAGE -> ATHENEUM
                         \
                          +->  WELL
                         /
    SHOP ->      FIGHTER -> LABORATORY -> ROCK

*/


int static_city_white_tower_build(int level, int build){
    if(build == BUILD_WT_ATHENEUM){
        return (level & BUILD_WT_MAGE) ? level | build : -1;
    }

    if(build == BUILD_WT_MAGE){
        return (level & BUILD_WT_TAVERN) ? level | build : -1;
    }

    if(build == BUILD_WT_ROCK){
        return (level & BUILD_WT_LABORATORY) ? level | build : -1;
    }

    if(build == BUILD_WT_LABORATORY){
        return (level & BUILD_WT_FIGHTER) ? level | build : -1;
    }

    if(build == BUILD_WT_FIGHTER){
        return (level & BUILD_WT_SHOP) ? level | build : -1;
    }

    if(build == BUILD_WT_TAVERN){
        return (level & BUILD_WT_GRAND_HALL) ? level | build : -1;
    }

    if(build == BUILD_WT_WALL){
        return ((level & (BUILD_WT_TAVERN|BUILD_WT_FIGHTER)) == (BUILD_WT_TAVERN|BUILD_WT_FIGHTER)) ? level | build : -1;
    }

    if(build == BUILD_WT_SHOP){
        return level | build;
    }

    if(build == BUILD_WT_GRAND_HALL){
        return level | build;
    }

    return -1;
}

void static_city_white_tower_draw_town(int level, int row){
    int left = row;
    int right = row;
    if(row == 3){
        if(level & BUILD_WT_WALL){
            left = right = 12;
        }else{
            if(level & BUILD_WT_FIGHTER){
                left = 7;
            }else if(level & BUILD_WT_SHOP){
                left = 5;
            }

            if(level & BUILD_WT_TAVERN){
                right = 7;
            }else if(level & BUILD_WT_GRAND_HALL){
                right = 5;
            }
        }
    }else if(row == 0){
        if(level & BUILD_WT_ATHENEUM){
            right = 10;
        }else if(level & BUILD_WT_MAGE){
            right = 8;
        }
    }else if(row == 1){
        if(level & BUILD_WT_ROCK){
            left = 10;
        }else if(level & BUILD_WT_LABORATORY){
            left = 8;
        }
        if(level & BUILD_WT_ATHENEUM){
            right = 11;
        }else if(level & BUILD_WT_MAGE){
            right = 9;
        }
    }else if(row == 2){
        if(level & BUILD_WT_ROCK){
            left = 11;
        }else if(level & BUILD_WT_LABORATORY){
            left = 9;
        }else if(level & BUILD_WT_FIGHTER){
            left = 6;
        }else if(level & BUILD_WT_SHOP){
            left = 4;
        }

        if(level & BUILD_WT_TAVERN){
            right = 6;
        }else if(level & BUILD_WT_GRAND_HALL){
            right = 4;
        }
    }

    char c;
    for (int i=0;i<7;i++){
        c = static_city_white_tower[left][0][i];
        //todo translate
        draw_char(c, '7');
    }

    for (int i=0;i<7;i++){
        c = static_city_white_tower[right][1][i];
        //todo translate
        draw_char(c, '7');
    }
}

void static_city_white_tower_draw(int level, int row){
    if(row < 0) return;
    if(row < 4){
        static_city_white_tower_draw_town(level, row);
        return;
    }

    if(row < 13){
        draw_text(static_city_white_tower_text[row-4], 0);
    }
}

/* white tower

      ()
      /\
     |  |


      ()
   /\ /\ /\
  |[]|  |[]|     storehouse | grand-hall


      ()
 /\/\ /\ /\/\
|[][]|  |[][]|   fighter storehouse | grand-hall tavern



  /\  ()
 |[]| /\ /\/\    laboratory
|[][]|  |[][]|   fighter storehouse | grand-hall tavern


          /\
 WWWW () |[]|    rock |  mage
 \[]/ /\ /\/\    laboratory |
|[][]|  |[][]|   fighter storehouse | grand-hall tavern


          /\
 WWWW () |[]|    rock |  mage
 \[]/ /\ /\/\    laboratory |
|[][]|  |[][]|   fighter storehouse | grand-hall tavern


         /()\       atheneum
 WWWW () \[]/   rock | mage
 \[]/ /\ /\/\   laboratory
|TTTT|  |TTTT|  fighter storehouse | warrior&well | grand-hall tavern

         _()_
 WWWW () [][]   rock | mage atheneum
 \[]/ /\ /\/\   laboratory
|TTTT|  |TTTT|  fighter storehouse | warrior&well | grand-hall tavern


         _/\_       atheneum
 WWWW () [][]  rock | mage
 \[]/ /\ /\/\   laboratory
|TTTT|  |TTTT|  fighter storehouse | warrior&well | grand-hall tavern

*/

int static_city_white_tower_growl(int level){
    int n = 0;
    if(level & BUILD_WT_FIGHTER) n |= LEVEL_1;
    if(level & BUILD_WT_WALL) n |= LEVEL_2;
    if(level & BUILD_WT_MAGE) n |= LEVEL_3;
    if(level & BUILD_WT_ROCK) n |= LEVEL_4;
    if (n == 0) return 0;
    return WHITE_TOWER | UNIT | n;
}

const char* static_city_white_tower_title(int row){
    if(row >= 0 && row < 10)
        return static_city_white_tower_text[row];
    return "";
}
