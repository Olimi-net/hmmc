#include "static_city_stronghold.h"
#include "drawing.h"

/*


const int BUILD_SH_FENCE = 256;
const int BUILD_SH_WORKSHOP = 128;
const int BUILD_SH_ARCHIVE = 64;
const int BUILD_SH_LANCER = 32;
const int BUILD_SH_SLINGER = 16;
const int BUILD_SH_MARKET = 8;
const int BUILD_SH_PEASANT = 4;
const int BUILD_SH_INN = 2;
const int BUILD_SH_GREAT_HALL = 1;


max 3 level = 255 | without LEVEL 4 and WELL
max 3+ level = 127 | without LEVEL 4 and UNIQ
max 2 level = 95 | without LEVEL 3
max 2+ level = 31 | without LEVEL 3 and LIBRARY
max 1 level = 15 | without LEVEL 2
*/

const char* static_city_stronghold[][2] = {
    {"       ","       "},      //0
    {"  /\\   ","   /\\  "},    //1 lancer=0  peasant=1 | city-hall=1 archive=0
    {" |[]|  ","  |[]| "},      //2 peasant=2 | city-hall=2
    {" /\\/\\  ","  /\\/\\ "},  //3 slinger=1 | inn=1
    {"|[][]| "," |[][]|"},      //4 slinger=2 | inn=2
    {" /[]\\  ","  /[]\\ "},    //5 lancer=1  | archive=1
    {"|====| "," |====|"},      //6 FENCE=3
    {" . . . "," . . . "},      //7 default=3

/*
    {"       ","       "}, //
    {"       ","       "}, //
    {"       ","       "}, //
    {"       ","       "}, //

    {"       ","       "}, //
    {"  /\   ","   /\  "}, //
    {" |[]|  ","  |[]| "}, //peasant and city-hall
    {"       ","       "}, //

    {"       ","       "}, //
    {" /\/\  ","  /\/\ "}, //
    {"|[][]| "," |[][]|"}, //peasant/slinger inn/city-hall
    {"       ","       "}, //

    {"  /\   ","   /\  "}, //
    {" /[]\  ","  /[]\ "}, // lancer  | archive
    {"|[][]| "," |[][]|"}, // peasant & slinger  | inn/city-hall
    {"|====| "," |====|"}, // FENCE

    {"  /\  /","\  /\  "}, //
    {" /[]\|[","]|/[]\ "}, // market
    {"|[][]| "," |[][]|"}, //
    {"|====| "," |====|"}, //

    {"  /\ _/","\_ /\  "}, //
    {" /[]\[]","[]/[]\ "}, // WORKSHOP & market
    {"|[][]| "," |[][]|"}, //
    {"|====| "," |====|"}, //

*/

};

const char* static_city_stronghold_text[9]{
    "GREAT HALL", "INN", "HUT of PEASANT", "MARKET", "HOUSE of SLINGER", "APARTMENT of LANCER", "ARCHIVE", "WORKSHOP", "FENCE"
};

const int static_city_stronghold_cost[][3] ={
    { 50, 5, 0 }, { 50, 5, 0 }, { 50, 5, 0 }, { 50, 5, 0 }, { 50, 5, 0 }, { 100, 10, 0 },  { 100, 10, 0 }, { 150, 10, 10 }, { 200, 20, 0 }
};

pair static_city_stronghold_get_cost(int v){
    pair p = {static_city_stronghold_cost[v][0], static_city_stronghold_cost[v][1],static_city_stronghold_cost[v][2]};
    return p;
}

/*
    MARKET -> WORKSHOP
                       /-> LANCER
    PEASANT -> SLINGER =
                       \-> FENCE
    GREAT_HALL -> INN -> ARCHIVE
*/

int static_city_stronghold_build(int level, int build){
    if(build == BUILD_SH_WORKSHOP){
        return (level & BUILD_SH_MARKET) ? level | build: -1;
    }

    if(build == BUILD_SH_MARKET){
        return level | build;
    }

    if(build == BUILD_SH_SLINGER){
        return (level & BUILD_SH_PEASANT) ? level | build: -1;
    }

    if(build == BUILD_SH_LANCER){
        return (level & BUILD_SH_SLINGER) ? level | build: -1;
    }


    if(build == BUILD_SH_INN){
        return (level & BUILD_SH_GREAT_HALL) ? level | build: -1;
    }

    if(build == BUILD_SH_ARCHIVE){
        return (level & BUILD_SH_INN) ? level | build: -1;
    }

    if(build == BUILD_SH_PEASANT){
        return level | build;
    }

    if(build == BUILD_SH_GREAT_HALL){
        return level | build;
    }

    if(build == BUILD_SH_FENCE){
        return (level & BUILD_SH_SLINGER) ? level | build: -1;
    }

    return -1;
}

void static_city_stronghold_draw_town(int level, int row){
    int left = 0;
    int right = 0;
    if(row == 3){
        if(level & BUILD_SH_FENCE){
            left = right = 6;
        }else{
            left = right = 7;
        }
    }else if(row == 0){
        if(level & BUILD_SH_LANCER){
            left = 1;
        }
        if(level & BUILD_SH_ARCHIVE){
            right = 1;
        }
    }else if(row == 1){
        if(level & BUILD_SH_LANCER){
            left = 5;
        }else if(level & BUILD_SH_SLINGER){
            left = 3;
        }else if(level & BUILD_SH_PEASANT){
            left = 1;
        }

        if(level & BUILD_SH_ARCHIVE){
            right = 5;
        }else if(level & BUILD_SH_INN){
            right = 3;
        }else if(level & BUILD_SH_GREAT_HALL){
            right = 1;
        }
    }else if(row == 2){
        if(level & BUILD_SH_SLINGER){
            left = 4;
        }else if(level & BUILD_SH_PEASANT){
            left = 2;
        }

        if(level & BUILD_SH_INN){
            right = 4;
        }else if(level & BUILD_SH_GREAT_HALL){
            right = 2;
        }
    }

    char c;
    for (int i=0;i<7;i++){
        c = static_city_stronghold[left][0][i];
        if(i > 4){
            if(row == 0){
                if(level & BUILD_SH_WORKSHOP){
                    c = (i==5)? '_' : '/';
                }else if(level & BUILD_SH_MARKET){
                    c = (i==5)? ' ' : '/';
                }
            }else if(row == 1){
                if(level & BUILD_SH_WORKSHOP){
                    c = (i==5)? '[' : ']';
                }else if(level & BUILD_SH_MARKET){
                    c = (i==5)? '|' : '[';
                }
            }
        }
        //todo translate
        draw_char(c, '7');
    }

    for (int i=0;i<7;i++){
        c = static_city_stronghold[right][1][i];
        if(i < 2){
            if(row == 0){
                if(level & BUILD_SH_WORKSHOP){
                    c = (i==0)? '\\' : '_';
                }else if(level & BUILD_SH_MARKET){
                    c = (i==0)? '\\' : ' ';
                }
            }else if(row == 1){
                if(level & BUILD_SH_WORKSHOP){
                    c = (i==0)? '[' : ']';
                }else if(level & BUILD_SH_MARKET){
                    c = (i==0)? ']' : '|';
                }
            }
        }
        //todo translate
        draw_char(c, '7');
    }
}

const char* static_city_stronghold_title(int row){
    if(row >= 0 && row < 10)
        return static_city_stronghold_text[row];
    return "";
}

void static_city_stronghold_draw(int level, int row){
    if(row < 0) return;
    if(row < 4){
        static_city_stronghold_draw_town(level, row);
        return;
    }

    if(row < 13){
        draw_text(static_city_stronghold_text[row-4], 0);
    }
}

int static_city_stronghold_growl(int level){
    int n = 0;
    if(level & BUILD_SH_PEASANT) n |= LEVEL_1;
    if(level & BUILD_SH_SLINGER) n |= LEVEL_2;
    if(level & BUILD_SH_LANCER) n |= LEVEL_3;
    if(level & BUILD_SH_FENCE) n |= LEVEL_4;
    if (n == 0) return 0;
    return STRONHOLD | UNIT | n;
}
