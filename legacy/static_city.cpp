#include "static_city.h"
#include "drawing.h"
#include "static_game.h"
#include "static_city_forest_union.h"
#include "static_city_red_castle.h"
#include "static_city_stronghold.h"
#include "static_city_white_tower.h"

/*
max 3+ level = 127 | without LEVEL 4 and UNIQ BUILD
max 2+ level = 31 | without LEVEL 3 and LIBRARY
max 1 level = 15 | without LEVEL 2

max 3 level = 255 | without LEVEL 4 and WELL
max 3+ level = 127 | without LEVEL 4 and UNIQ
max 2 level = 95 | without LEVEL 3
max 2+ level = 31 | without LEVEL 3 and LIBRARY
max 1 level = 15 | without LEVEL 2

max 3 level = 255 | without LEVEL 4
max 3+ level = 127 | without LEVEL 4 and UNIQ
max 2+ level = 31 | without LEVEL 3 and LIBRARY
max 1 level = 15 | without LEVEL 2 and WELL

max 3 level = 255 | without LEVEL 4
max 2- level = 127 | without LEVEL 3 with UNIQ
max 2+ level = 31 | without LIBRARY and UNIQ
max 1 level = 15 | without level 2
*/

// minimum 50 gold room for city // city minimum gold 10 + 15 Great hall + 10 Treasure

const char* static_city_names[20]{ "STRONHOLD", "FOREST UNION", "RED CASTLE",  "WHITE TOWER",
    "LABORATORY", "LABORATORY", "HABITATION", "HABITATION", "SAWMILL", "SAWMILL", "SHACK", "INN",
    "BARRACKS", "BASTION", "MACHINERY", "ARMORY", "MARKET", "LYCEUM", "WATER MILL", "WATER MILL" };

const char* static_city_rooms[6]{ "WGTR", "HIAMBCLWD", "HIGABMLCD", "HIMABCLDT", "HMIABCLRD", "RrPpGgFIEKWNMLTt" };

const char* static_buildings[9]{
    "RANDOM__$CITY$", "USER_1__$CITY$", "USER_2__$CITY$", "USER_3__$CITY$", "USER_4__$CITY$", "THE_STRONGHOLD","THEFORESTUNION","THE_RED_CASTLE", "POSTWHITETOWER"
};

// total cost {'stronghold': (800, 75, 10), 'red castle': (790, 75, 14), 'white tower': (840, 81, 10), 'forest union': (680, 59, 20)}
// result = {x: (sum([i[0] for i in city[x]]), sum([i[1] for i in city[x]]), sum([i[2] for i in city[x]])) for x in city }

const char* static_village[][3] ={
    {"        ",
	 "        ",
	 "VILLAGES"}, // 0

	{"        ",
	 " /\\  )( ",
	 "|[]|(__)"},

	{"        ",
	" )(  /\\ ",
	"(__)|[]|"}, //лаборатория 2 A - 2 + (0|1) >> R r

	{"        ",
	"     /\\ ",
	"i==i|[]|"},

	{"        ",
	" /\\     ",
	"|[]|i==i"}, // крестьяне 4 B 2-1 >> P p

	{"        ",
	"==>> /\\ ",
	"<<==|[]|"},

	{"        ",
	" /\\<<==",
	"|[]|==>>"}, // sawmill 6 A - 4 + (0|1) >> G g

	{"   /\\   ",
	"Y /[]\\ Y",
	"|/[][]\\|"}, //raven shack 7  B 4 >> F

	{"  /\\/\\  ",
    " |[][]| ",
    " |[][]| "}, // tavern 8  A 8 >> I

	{"   ()   ",
	"   /\\   ",
	" _|[]|_ "}, // barracks 9 B 8 >> E

    {"    P   ",
    " __/\\__ ",
    "/__[]__\\"}, // bastion 10 B 16 >> K

	{"        ",
	"___/\\___",
	"|[][][]|"}, // machinery 11 B 32 >> W


	{"  /\\/\\  ",
	" |[][]| ",
	"[=]  [=]"}, // armory 12 B 64 >> N

	{" /\\  /\\ ",
	"/[]\\/[]\\",
	"|[]||[]|"}, // market 13 A 16 >> M

	{"  /[]\\  ",
	" |[][]| ",
	"  \\[]/  "}, // lyceum 14 A 32 >> L

    {"        ",
	" /\\ ~ ~ ",
	"|[]|(*)~"},

	{"        ",
	" ~ ~ /\\ ",
	"~(*)|[]|"}, // мельница 16 A 64 + (0|1) >> T t
};
//struct: city, build_level, availible units(4 levels), magic_book, links_for_heroes
/** X - unique build for city */
int static_city_get_room(int city, int room){
    if(city == VILLAGE){
        if(room > 0 && room < 16)
            return static_city_rooms[5][room-1];
        if(room >= '1' && room <= '9')
            return static_city_rooms[5][room-'1'];
        for(int i = 0; i<16;i++){
            if(static_city_rooms[5][i]==room) return i + 1;
        }
        return 0;
    }

    int c = 0;
    if(city == STRONHOLD) c = 1;
    else if(city == FOREST_UNION) c = 2;
    else if(city == RED_CASTLE) c = 3;
    else if(city == WHITE_TOWER) c = 4;
    if(c == 0) return 0;
    if(room > 0 && room < 10)
        return static_city_rooms[c][room-1];
    if(room >= '1' && room <= '9')
        return static_city_rooms[c][room-'1'];
    int n = (room == 'X') ? static_city_rooms[0][c-1] : room;
    for(int i = 0; i<9;i++){
        if(static_city_rooms[c][i]==n) return i + 1;
    }
    return 0;
}

const char* static_city_get_title(int city, int level){
    if(city == RED_CASTLE){
        return static_city_red_castle_title(level);
    }else if(city == STRONHOLD){
        return static_city_stronghold_title(level);
    }else if(city == FOREST_UNION){
        return static_city_forest_union_title(level);
    }else if(city == WHITE_TOWER){
        return static_city_white_tower_title(level);
    }else if(city == VILLAGE && level > 0 && level < 17){
        return static_city_names[level + 4];
    }
    return "";
}

void static_city_draw_town(int city, int level, int row){
    if(city == RED_CASTLE){
        static_city_red_castle_draw(level, row);
    }else if(city == STRONHOLD){
        static_city_stronghold_draw(level, row);
    }else if(city == FOREST_UNION){
        static_city_forest_union_draw(level, row);
    }else if(city == WHITE_TOWER){
        static_city_white_tower_draw(level, row);
    }else if(city == VILLAGE && level > 0 && level < 17){
        if(row >= 0 && row < 3){
            draw_text(static_village[level][row], '0');
        }else if(row > 2){
            draw_text(static_city_get_name(city, level), '0');
        }
    }
        /*
            ратуша - информация о ресурсах текущих и за неделю, количество героев и замков
            библиотека - купить заклинание 2х3 / всего заклинаний: 7 боевых (4 случайных) и 2 на карте (обязательные внизу)
            таверна - можно купить нового героя (выбор из двух) первый всегда из текущего города (или пусто если все 4 куплены) и случайный
            рынок - прилавок(stall) / stand/ loge / market / shop - купить/продать ресурсы
            особое здание - в лаборатории-ртуть/складе-дерево/сокровищница-золото ничего нельзя сделать - они приносят ресурсы раз неделю, а в боевых машинах - можно купить машину,

        */

}

int static_city_build(int city, int level, int build){
    if(city == RED_CASTLE){
        return static_city_red_castle_build(level, build);
    }else if(city == STRONHOLD){
        return static_city_stronghold_build(level, build);
    }else if(city == FOREST_UNION){
        return static_city_forest_union_build(level, build);
    }else if(city == WHITE_TOWER){
        return static_city_white_tower_build(level, build);
    }
    return level;
}

pair static_city_cost(int city, int build){
    if(city == RED_CASTLE){
        return static_city_red_castle_get_cost(build);
    }else if(city == STRONHOLD){
        return static_city_stronghold_get_cost(build);
    }else if(city == FOREST_UNION){
        return static_city_forest_union_get_cost(build);
    }else if(city == WHITE_TOWER){
        return static_city_white_tower_get_cost(build);
    }
    pair p = {0, 0, 0};
    return p;
}

/* return position, length, building */
pair* static_city_found(char* row, int len, int start_pos){
    for(int i=start_pos;i<len;i++){
        for(int j=0; j <9; j++){
            int cnt = 0;
            for(int n=0;n<14;n++){
                if(i + n >= len) break;
                if (static_buildings[j][n] == row[i + n]){
                    cnt++;
                }else{
                    break;
                }
            }
            if(cnt == 14){
                int f = j;
                if(j == 5) f = STRONHOLD;
                else if(j == 6) f = FOREST_UNION;
                else if(j == 7) f = RED_CASTLE;
                else if(j == 8) f = WHITE_TOWER;
                return pair_factory(i, cnt, CITY | f);
            }
        }

        for(int j=0; j <17; j++){
            int cnt = 0;
            for(int n=0;n<8;n++){
                if(i + n >= len) break;
                if (static_village[j][2][n] == row[i + n]){
                    cnt++;
                }else{
                    break;
                }
            }
            if(cnt == 8){
                return pair_factory(i, cnt, VILLAGE | j);
            }
        }
    }
    return NULL;
}

int static_city_growl(int city, int build){
    if(city == STRONHOLD) return static_city_stronghold_growl(build);
    if(city == WHITE_TOWER) return static_city_white_tower_growl(build);
    if(city == RED_CASTLE) return static_city_red_castle_growl(build);
    if(city == FOREST_UNION) return static_city_forest_union_growl(build);
    {
        if(build == 3 || build == 4) return  STRONHOLD | UNIT | LEVEL_1;
        if(build == 7) return FOREST_UNION | UNIT | LEVEL_1;
        if(build == 8) return HERO;
        if(build == 9) return WHITE_TOWER | UNIT | LEVEL_1;
        if(build == 10) return RED_CASTLE | UNIT | LEVEL_1;
        if(build == 11) return SPECIAL | UNIT | LEVEL_2 | LEVEL_3 | LEVEL_4;
    }
    return 0;
}

const char* static_city_get_name(int city, int build){
    if(city == STRONHOLD){ return static_city_names[0]; }
    if(city == FOREST_UNION){ return static_city_names[1]; }
    if(city == RED_CASTLE){ return static_city_names[2]; }
    if(city == WHITE_TOWER){ return static_city_names[3]; }
    if(city == VILLAGE && build > 0 && build < 17){ return static_city_names[build + 3]; }
    return "Not Found!";
}

/*
	"   /\   ","        ","  /\/\  ","        ","        ","        ",
	"  |[]|  ","     /\ "," /[][]\ "," /\     "," Y /\ Y "," )(  /\ ",
	"  |[]|  ","i==i|[]|","|[][][]|","|[]|i==i"," |/[]\| ","(__)|[]|",
	"tower\0","village\0",	"Inn","village\0","shack\0","alchemic\0",

*/
