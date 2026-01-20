#include "battle_back.h"
#include "../legacy/static_hero.h"
#include "../legacy/drawing.h"
#include "../legacy/static_unit.h"
#include "../legacy/static_battle.h"
#include "battle_const.h"

battle_back::battle_back(battle_args *a){
    game_args = a;
    is_floor = false;
    is_load = true;

    anime_flag = 0;

    init_header(a->top, a->type_field);
    is_standard = a->width > 50 && top_img >= 0;
}

battle_back::~battle_back(){

}

void battle_back::init_header(int top, int flag){
    top_name = -1;
    top_img = -1;

    if(flag & (BATTLE_TAVERN|BATTLE_CATACOMB)) return;

    if(top > 7){
        /// todo хаотичное поле ёлочек или воды
        top_name = top - 2;
        top_img = top - 6;
    }else if(top < 3){
        top_name = top - 1 - (top >> 1);
    }else{
        top_img = 0;
        int t = (top - 3);
        if(t > 2)
            top_name = top - 2;
        else
            top_name = t > 1 ? top - 1 : -1;
    }
}

void battle_back::draw_heros_name(int offset){
    if(game_args->hero_left){
        static_hero_draw_name_align(game_args->hero_left, game_args->fraction_left,
            game_args->left + offset, top_name, 10, 0);
    }
    if(game_args->hero_right){
        static_hero_draw_name_align(game_args->hero_right, game_args->fraction_right,
            game_args->right - offset, top_name, 10, 2);
    }
}

void battle_back::draw_heros(){
    int offset = 0;
    if(top_img >= 0){
        offset = (game_args->width - 36) >> 2; // hero x 2 + book = 36
        if(game_args->hero_left){
            int left = game_args->left + offset;
            for(int i = 0; i < 3; i++){
                set_position(left, top_img + i);
                static_draw(HERO, i, AN_RIGHT);
            }
        }
        if(game_args->hero_right){
             int right = game_args->right - offset;
             for(int i = 0; i < 3; i++){
                set_position(right, top_img + i);
                static_draw(HERO, i, AN_LEFT);
            }
        }
    }
}

void battle_back::draw_standard(bool angle, int clr, int ow, int ho, int anim){
/*          |~~~~,      ,~~~~|
            |~~~~'      '~~~~|
            |                | */

    int a = anim & 7;
    if(angle){
        for(int j=0; j<3;j++){
            set_position(ow, ho + j);
            if(j == 0) draw_char(',' , clr);
            else if(j == 1) draw_char('\'' , clr);
            else set_position(ow + 5, ho + j);
            if(j < 2) {
                for(int i=0;i<4;i++){
                    if(a + j == i)
                      draw_char('-', '0');
                    else
                      draw_char('~', clr);
                }
            }
            draw_char('|', 'Y');
        }
        return;
    }

    for(int j=0; j<3;j++){
        set_position(ow, ho + j);
        draw_char('|', 'Y');
        if(j > 1) break;
        for(int i=0;i<4;i++){
            if(a + j == i)
              draw_char('-', '0');
            else
              draw_char('~', clr);
        }
        draw_char(j ? '\'' : ',' , clr);
    }
}
 /** 1=door open, 2=door close, 4=door broken, 8=catapult pull, 16=wall broken, 32=destroyed tower */
void battle_back::on_anime_flag(int flag){
    if(flag == 1){
        anime_flag |= 1;
    }else if(flag == 2){
        anime_flag ^= anime_flag & 1;
    }else{
        anime_flag |= flag;
    }
}

void battle_back::draw(int anime){
    if(is_load) draw_clear(BACK);
    draw_select_buffer(BACK);

    if(is_standard){
        draw_standard(true, game_args->fraction_left, game_args->left, top_img, anime);
        draw_standard(false, game_args->fraction_right, game_args->right - 6, top_img, anime);
    }

    ///todo рисуем один раз, перерисовываем если произошли события - поштучно, флаг / кружка и т.д.

    if(this->game_args->type_field == BATTLE_TAVERN){
        static_battle_draw_basement(0,3, is_floor ? 31 : 15);
    }else if(this->game_args->type_field == BATTLE_CATACOMB){
        static_battle_draw_basement(0,3, is_floor ? 45 : 13);
    }else if(this->game_args->type_field == BATTLE_CITY){
        if(anime_flag & 8){
            anime_flag ^= 8;
            static_battle_draw_catapult(3, game_args->right - 10, game_args->bottom - 3, 0);
        }else{
            static_battle_draw_catapult(2, game_args->right - 10, game_args->bottom - 3, 0);
        }

        /* 1=door open, 2=door close, 4=door broken, 8=catapult pull, 16=wall broken, 32=destroyed tower  */
        int a = (anime_flag & 5) ? 1 : 0;


        if(this->game_args->type_city == FOREST_UNION){
            //todo add flags for mote
        }else if(this->game_args->type_city == STRONHOLD){
            //todo add flags for wood stone
        }else{
            a |= 2;
        }

        if(anime_flag & 16) a |= 8;
        if(anime_flag & 32) a |= 4;

        /* wall: 1=close gate, 2=stone wall, 4=destroyed tower, 8=broken wall*/

        static_battle_draw_town(a, game_args->left, game_args->top - 3, 0);
        this->draw_heros();
    }else{
        this->draw_heros();
    }
}
