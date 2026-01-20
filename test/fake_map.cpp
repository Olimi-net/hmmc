#include "fake_map.h"
#include "../modules/map_const.h"
#include "../components/cmp_battle.h"
#include "../components/cmp_city.h"
#include "../legacy/static_city.h"
#include "../battle/battle_const.h"

fake_map::fake_map(cmp_basic* parent, int flags):cmp_basic(parent),hmmc_callback(){
    flag = flags;
    city_flag = WHITE_TOWER;
    res = pair_factory(3495, 260, 178);
    init(flags);
}

fake_map::~fake_map(){
    pair_free(res);
    clear();
}

int fake_map::on_event(cmp_basic *s, hmmc_event *e){
    if(e->id == 'H'){
        e->p.state = 1;
        /* 1 if  value = into and subval = out side */
        /* 2 if subval = into and value = out side */
        e->p.value = this->gl->get_hero_val_or_def(HERO_ATTR_ID, 0);
        e->p.subval = this->gr->get_hero_val_or_def(HERO_ATTR_ID, 0);
        return e->id;
    }
    return 0;
}

int fake_map::on_callback(hmmc_event *e, int a){
    if(e == nullptr) return 0;
    return 0;
}

void fake_map::battle_cells(){
    draw_select_buffer(HIGH);
    for(int j = 1; j < 7; j++){
        for(int i = 2; i < 14; i++){
            int n = ((i-3) + j) & 7;
            for(int r = 0; r < 3; r++){
                for(int k = 0; k < 5; k++){
                    if(k == r * 2){
                        set_position(i * 5 + k, j * 3 + r);
                        draw_int(n);
                    }
                }
            }
        }
    }
}

void fake_map::on_draw(int anime) {
    current_cmp->on_draw(anime);
    //if(anime == 0)
    //    battle_cells();
}

int fake_map::on_press(int c, int k){
    if(c == 'n'){
        if(city_flag == WHITE_TOWER)
            city_flag = RED_CASTLE;
        else if (city_flag == RED_CASTLE)
            city_flag = FOREST_UNION;
        else if (city_flag == FOREST_UNION)
            city_flag = STRONHOLD;
        else
            city_flag = WHITE_TOWER;

        clear();
        init(flag);
        return 0;
    }
    if(k == 10) return 0;
    return current_cmp->on_press(c, k);
}

int fake_map::on_tick(int anime){
    return current_cmp->on_tick(anime);
}

void fake_map::init(int flags){
    int f = math_get_time() & 31;
    int lih = f;

    int rih = (31 - f);

    hl = (lih == 17 || lih > 24) ? NULL : new hero(lih);
    hr = (rih == 17 || rih > 24) ? NULL : new hero(rih);
    int guid = math_get_time();
    this->gl = new group(guid++, 1, hl);
    this->gr = new group(guid++, 2, hr);

    for(int n=1;n<3;n++){
        int l1 = (lih == 17 || lih > 24) ? n * 2: lih;
        int r1 = (rih == 17 || rih > 24) ? n * 5: rih;
        gl->append_unit(static_hero_get_unit_type(l1, n), static_hero_get_unit_count(l1, n));
        gr->append_unit(static_hero_get_unit_type(r1, n), static_hero_get_unit_count(r1, n));
    }

    if(flags & FAKE_MAP_BATTLE){
        int bf = BATTLE_CITY;
        if (lih == 17) bf = BATTLE_CATACOMB;
        if (rih == 17) bf = BATTLE_TAVERN;
        this->current_cmp = new cmp_battle(this, gl, gr, 0.1f, bf);
    }else if(flags & FAKE_MAP_CITY){
        cur_city = new city(1, city_flag, BUILD_ALL, math_get_time() & 4095, 1, 1, this);
        cur_city->join_group(gl);
        int id = gr->get_hero_val_or_def(HERO_ATTR_ID, 0);
        this->current_cmp = new cmp_city(this, (id > 0) ? gr : NULL, cur_city, res);
    }
}

void fake_map::clear(){
    delete current_cmp;
    if(hl != NULL){
        delete hl;
    }
    if(hr != NULL){
        delete hr;
    }
    if(cur_city != NULL) {
        delete cur_city;
    }
    delete gl;
    delete gr;
}

/*
HLP - Show commands
ALL - No limit
L2L - Set limit build level 2
LLA - Set limit build level 1
U2L - Update cities for 2 level
ULA - Update cities for 1 level
ASN - Add stronghold n = 1..4
AFN - Add forest union n = 1..4
AWN - Add white tower n = 1..4
ARN - Add red castle n = 1..4
RST - Remove all stronghold units
RWT - Remove all white tower units
RRC - Remove all red castle units
RFU - Remove all forest union units
AGN - Add gold n = 1..9
AWN - Add wood n = 1..9
AMN - Add mercury n = 1..9
CLR - Убирает всех врагов с карты
KLL - Убить героя
LVL - герой получает уровень
INN - Показать всех доступных героев
*/


/*




          ,~-~~|                                                |~-~~,
          '~~-~|                                                |~~-~'
               |                 ________                       |
                (} \             \______/ / /|/|/           V {)
               /[]-+              |_|__| /_/ / /            |V[]\
                /\ /              |__|_|/ /|/|/             | /\
            //\(} |               |/  /|_/ / /               ()o__o()
           //\\[]\!               /__/ |/|/|/                 \/__\/
               ||                /  /|/| / /                  /|\/|\
                                /__/ / //|/
                               /__/|/|/| /
                               |__|/ /||/
                               |  ||/||/
                               |__|/||/
                               / /|/|/
                              /_/ / /
                             / /|/|/
                            / /|/|/
                           /_/ / /
                          / /|/|/
                         / /|/|/


*/
