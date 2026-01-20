#include "static_city_forest_union.h"
#include "drawing.h"

/*
const int BUILD_FU_FOREST = 256;
const int BUILD_FU_MOTE = 128;
const int BUILD_FU_LIBRARY = 64;
const int BUILD_FU_BASAR = 32;
const int BUILD_FU_DRUID = 16;
const int BUILD_FU_THE_ROOST = 8;
const int BUILD_FU_GREENHOUSE = 4;
const int BUILD_FU_LODGE = 2;
const int BUILD_FU_TOWN_HALL = 1;

max 3 level = 255 | without LEVEL 4
max 2- level = 127 | without LEVEL 3 with UNIQ
max 2+ level = 31 | without LIBRARY
max 1 level = 15 | without level 2



STOCK -> BASAR -> LIBRARY
                          \
RAVEN  ->  MOTE CRABS       +-> FOREST
                          /
TOWN_HALL -> LODGE -> DRUID

*/

const char* static_city_forest_union[][2] = {
    {"       ","       "}, // 0
    {"  /\\   ","   /\\  "}, // 1 library=0 & stock=1 | druid=0 & town-hall=1
    {" /[]\\  ","  /[]\\ "}, // 2 library=1 & stock=2 | druid=1 & town-hall=2
    {"|[][]| "," |[][]|"}, // 3 bazar=2 | lodge=2
    {"  ~    "," ~    ~"}, // 4 default=3
    {"~_~_~| "," |~_~_~"}, // 5 mote=3
    {"WW/\\WW "," WW/\\WW"}, // 6 forest=0
    {" /\\/\\  ","  /\\/\\ "}, // 7 bazar=1 | lodge=1

/*
    {"       ","       "}, //
    {"       ","       "}, //
    {"       ","       "}, //
    {"'''''' "," ''''''"}, //

    // stock  & town-hall
    {"       ","       "}, //
    {"  /\   ","   /\  "}, //
    {" /[]\  ","  /[]\ "}, //
    {"'''''' "," ''''''"}, //

    // druid  & tavern
    {"       ","       "}, //
    {" /\/\  ","  /\/\ "}, // druid - need stock
    {"|[][]| "," |[][]|"}, // stock&bazar - need town-hall
    {"'''''' "," ''''''"}, //

    //library & unknown
    {"  /\   ","   /\  "}, //
    {" /[]\  ","  /[]\ "}, //
    {"|[][]| "," |[][]|"}, // stock & bazar - lodge & town-hall
    {"'''''' "," ''''''"}, //

    {"WW/\WW "," WW/\WW"}, //           forest
    {" /[]\  ","  /[]\ "}, //       library - druid
    {"|[][]| "," |[][]|"}, // stock & bazar - lodge & town-hall
    {"~_~_~| "," |~_~_~"}, // mote

    // */
};

const char* static_city_forest_union_text[]{
    "TOWN HALL", "LODGE", "GREENHOUSE", "THE ROOST",  "DRUID COTTAGE", "BASAR", "SHRINE", "THE MOTE", "LIFE FOREST"
};

const int static_city_forest_union_cost[][3] ={
    { 70, 8, 0 }, { 50, 4, 0 }, { 70, 8, 0 }, { 40, 6, 0 }, { 50, 4, 0 }, { 50, 4, 0 },  { 50, 5, 0 }, { 200, 20, 10 }, { 100, 0, 10 }
};

pair static_city_forest_union_get_cost(int v){
    pair p = {static_city_forest_union_cost[v][0], static_city_forest_union_cost[v][1],static_city_forest_union_cost[v][2]};
    return p;
}

int static_city_forest_union_build(int level, int build){
    if(build == BUILD_FU_FOREST){
        return ((level & (BUILD_FU_LIBRARY | BUILD_FU_DRUID)) == (BUILD_FU_LIBRARY | BUILD_FU_DRUID)) ? level | build : -1;
    }

    if(build == BUILD_FU_LIBRARY){
        return (level & BUILD_FU_BASAR) ? level | build : -1;
    }

    if(build == BUILD_FU_BASAR){
        return ((level & BUILD_FU_GREENHOUSE) == BUILD_FU_GREENHOUSE) ? level | build : -1;
    }

    if(build == BUILD_FU_GREENHOUSE){
        return level | build;
    }

    if(build == BUILD_FU_DRUID){
        return ((level & BUILD_FU_LODGE) == BUILD_FU_LODGE) ? level | build : -1;
    }

    if(build == BUILD_FU_LODGE){
        return ((level & BUILD_FU_TOWN_HALL) == BUILD_FU_TOWN_HALL) ? level | build : -1;
    }

    if(build == BUILD_FU_TOWN_HALL){
        return level | build;
    }

    if(build == BUILD_FU_THE_ROOST){
        return level | build;
    }

    if(build == BUILD_FU_TOWN_HALL){
        return level | build;
    }

    if(build == BUILD_FU_MOTE){
        return ((level & (BUILD_FU_GREENHOUSE | BUILD_FU_THE_ROOST)) == (BUILD_FU_GREENHOUSE | BUILD_FU_THE_ROOST)) ? level | build : -1;
    }

    return -1;
}

void static_city_forest_union_draw_town(int level, int row){
    int left = 0;
    int right = 0;
    if(row == 3){
         left = right = (level & BUILD_FU_MOTE) ? 5 : 4;
    }else if(row == 0){
        if(level & BUILD_FU_FOREST){
            left = right = 6;
        }else{
            if(level & BUILD_FU_LIBRARY) left = 1;
            if(level & BUILD_FU_DRUID) right = 1;
        }
    }else if(row == 1){
        if(level & BUILD_FU_LIBRARY){
            left = 2;
        }else if(level & BUILD_FU_BASAR){
            left = 7;
        }else if(level & BUILD_FU_GREENHOUSE){
            left = 1;
        }

        if(level & BUILD_FU_DRUID){
            right = 2;
        }else if(level & BUILD_FU_LODGE){
            right = 7;
        }else if(level & BUILD_FU_TOWN_HALL){
            right = 1;
        }
    }else if(row==2){
        if(level & BUILD_FU_BASAR){
            left = 3;
        }else if(level & BUILD_FU_GREENHOUSE){
            left = 2;
        }
        if(level & BUILD_FU_LODGE){
            right = 3;
        }else if(level & BUILD_FU_TOWN_HALL){
            right = 2;
        }
    }

    char c;
    for (int i=0;i<7;i++){
        if(row == 1){
            if((level & BUILD_FU_THE_ROOST) && (i == 0 || i == 5)){
                draw_char('Y','6');
                continue;
            }
        }
        if(row == 2){
           if((level & BUILD_FU_THE_ROOST) && (i == 0 || i == 5)){
                draw_char('|','6');
                continue;
            }
        }
        c = static_city_forest_union[left][0][i];
        //todo translate
        draw_char(c, '7');
    }

    for (int i=0;i<7;i++){
        if(row == 1){
            if((level & BUILD_FU_THE_ROOST) && (i == 1 || i == 6)){
                draw_char('Y','6');
                continue;
            }
        }
        if(row == 2){
           if((level & BUILD_FU_THE_ROOST) && (i == 1 || i == 6)){
                draw_char('|','6');
                continue;
            }
        }

        c = static_city_forest_union[right][1][i];
        //todo translate
        draw_char(c, '7');
    }
}

void static_city_forest_union_draw(int level, int row){
    if(row < 0) return;
    if(row < 4){
        static_city_forest_union_draw_town(level, row);
        return;
    }

    if(row < 13){
        draw_text(static_city_forest_union_text[row-4], 0);
    }
}

int static_city_forest_union_growl(int level){
    int n = 0;
    if(level & BUILD_FU_THE_ROOST) n |= LEVEL_1;
    if(level & BUILD_FU_DRUID) n |= LEVEL_2;
    if(level & BUILD_FU_MOTE) n |= LEVEL_3;
    if(level & BUILD_FU_FOREST) n |= LEVEL_4;
    if (n == 0) return 0;
    return FOREST_UNION | UNIT | n;
}

const char* static_city_forest_union_title(int row){
    if(row >= 0 && row < 10)
        return static_city_forest_union_text[row];
    return "";
}
