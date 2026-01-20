#include "battle_args.h"
#include "battle_const.h"

battle_args::battle_args(int flag, int w, int h){
    //    this->game_args = pair_factory(0, 0, flag);
    max_width = w;
    max_height = h;
    columns = w / 5;
    type_field = flag & BATTLE_PLACE;
    type_city = flag & 15360; // ALL CITIES;
    rows = 6;

    {
        int dp = h - 18;
        if(dp < 6){
            if(dp < 1){
                top = 0;
            }else if(dp < 4){
                top = dp - (dp >> 1);
            }else{
                top = 3;
            }
        }else{
            top = dp - (dp >> 1);
        }
    }


    unit_arg = BTL_ARG_ALL; // small = 1, big = 2, nonliving = 4, machine = 8, hero's = 16
    if(type_field == BATTLE_SHIP){
        columns = 12;
    }else if(type_field == BATTLE_CITY){
        columns = 12;
    }else if(type_field == BATTLE_TAVERN){
        top+=5;
        unit_arg = BTL_ARG_SMALL;
        columns = 12;
        rows = 4;
    }else if(type_field == BATTLE_CATACOMB){
        top+=4;
        unit_arg = BTL_ARG_SMALL;
        columns = 12;
        rows = 4;
    }
    if(columns > 18){
        columns = 18;
    }
    width = columns * 5;
    height = rows * 3;

    left = (max_width - width) >> 1;
    right = left + width;
    bottom = top + rows * 3;
}

battle_args::~battle_args(){

}

void battle_args::info_left(int fr, int hero_id){
    fraction_left = fr;
    hero_left = hero_id;
}

void battle_args::info_right(int fr, int hero_id){
    fraction_right = fr;
    hero_right = hero_id;
}

int battle_args::get_start_column(bool is_right){
    if(type_field == BATTLE_SHIP) return is_right ? (columns - 1) : 1;
    return is_right ? (columns - 2) : 0;
}
