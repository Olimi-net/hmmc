#include "cmp_city.h"
#include "../legacy/static_game.h"
#include "../legacy/book_of_magic.h"
#include "../legacy/static_unit.h"
#include "../legacy/static_city.h"
#include "../modules/module_static.h"

cmp_city::cmp_city(cmp_basic* parent, group *gout, city* c, pair* res):cmp_basic(parent),hmmc_callback(){
    cur_city = c;
    page = 0;
    subpage = 0;
    resources = res;
    dialog = nullptr;
    is_dialog = 0;
    this->parent = parent;
    this->group_out = gout;
    this->ms = module_static();
}

cmp_city::~cmp_city(){
    if(dialog != nullptr)
        delete dialog;
}

int cmp_city::on_event(cmp_basic *s, hmmc_event *e){
    return 0;
}

int cmp_city::on_callback(hmmc_event *e, int a){
    ///if heroes->one->is_growling (a = UNIT)
    //todo increase growling
    return 0;
}

void cmp_city::draw_resources_list(){
    draw_text("Resources: ", 0);
    next_col(); draw_text("Gold: ", 'Y'); draw_int(resources->value);
    next_col(); draw_text("Wood: ", 'G'); draw_int(resources->subval);
    next_col(); draw_text("Mercury: ", 'R'); draw_int(resources->state);
    next_row();
    draw_end('_', 0);
    next_row();
}

char cmp_city::get_build(int build, int level, int max_lvl, int v){
    char access = 'x';
    int b = 1 << v;
    if(max_lvl & b){
        if(level & b){
            access = 'v';
        }else if(static_city_build(build, level, b) > 0){
            access = 'o';
        }
    }
    return access;
}

void cmp_city::draw_build_list(){
    int build = cur_city->get_city();
    int level = cur_city->get_level();
    int max_lvl = cur_city->get_max_level();

    for(int i = 0; i < 26; i++)
        draw_char_at_position('|', 0, 30, 2 + i);

    set_position(35, 3);
    draw_text("Build list", 0);

    for(int i = 0; i < 9; i++){
        set_position(35, 5 + i);
        draw_char(i + '1', 0);
        draw_text(" [", 0);
        char a = get_build(build, level, max_lvl, i);
        char clr = (a == 'v') ? 'G' : (a == 'o') ? 'Y' : 'R';
        draw_char(a, clr);
        draw_text("] ", 0);
        static_city_draw_town(build, level, i + 4);
    }

    set_position(31, 14);
    draw_end('_', 0);

    int y = 16;
    set_position(35, y);
    draw_text("Fast key", 0);
    y+=2;
    set_position(35, y++);
    draw_text("1..9 create new building", 0);
    set_position(35, y++);
    draw_text("U - recruit are units", 0);
    set_position(35, y++);
    draw_text("L - study in a library", 0);
    set_position(35, y++);
    draw_text("I - drink in a tavern", 0);
    set_position(35, y++);
    draw_text("M - visit a market", 0);
    set_position(35, y++);
    draw_text("X - open unique building", 0);
}

void cmp_city::on_draw_page_main(int anime){
    draw_city_img(anime, cur_city->get_city(), 0);

    next_row();
    next_row();
    draw_text(" Heroes into the city:", 0);
    next_row();
    next_row();

    hmmc_event e = hmmc_event('H');
    parent->on_event(this, &e);

    draw_text("  In side:", 0);
    int hid = e.p.state == 1 ? e.p.value : e.p.subval;

    if(hid > 0) static_hero_draw_name(hid, 0);
    next_row();
    draw_text(" Out side:", 0);
    hid = e.p.state == 2 ? e.p.value : e.p.subval;
    if(hid > 0) static_hero_draw_name(hid, 0);
    next_row();
    next_row();
    draw_text(" Press TAB for switch heroes", 0);
    next_row();
    draw_as_line('_', 0, 30);
    next_row();
    next_row();
    draw_text(" Q - return in the world", 0);

    draw_build_list();
}

void cmp_city::on_draw_tavern(int anime){
    int n = static_city_get_room(cur_city->get_city(), 'I');
    static_city_draw_town(cur_city->get_city(), 0, n + 3);

    hmmc_event e = hmmc_event(0);
    if(this->parent->on_event(this, &e)){

    }
    ///todo event for heroes
}

void cmp_city::on_draw_library(int anime){
    int magic = cur_city->magic;
    ///todo magic book
    int n = static_city_get_room(cur_city->get_city(), 'L');
    static_city_draw_town(cur_city->get_city(), 0, n + 3);

    int oh = 3;
    int row = 0;
    for(int i = 0; i < BOOK_MAGIC_COUNT; i++){
        int id = 1 << i;
        if(id & magic){
            int y = row >> 2;
            int x = row & 3;
            ms.draw_book(i, 2 + x * 20, y * 5 + oh, 0, 100, true);
            set_position(2 + x * 20, y * 5 + 4 + oh);

            draw_text_align(2 + x * 20, 20, book_of_title(i, false), 'W', 1);
            row++;
        }
    }
}

void cmp_city::draw_city_img(int anime, int city, int room){
    int n = static_city_get_room(city, room);
    int build = city == VILLAGE ? n : cur_city->get_level();

    draw_options(REGULAR | DB_OPT_END, 30);
    set_position(0,3);
    draw_text_align(0, 30, static_city_get_name(city, build), 0, 1);
    next_row();
    draw_options(REGULAR | DB_OPT_CUT | DB_OPT_BEGIN, 4);
    int count = 4;
    if(city == VILLAGE){
        count = 3;
        next_row();
    }

    for(int i = 0; i<count;i++){
        static_city_draw_town(city, build, i);
        next_row();
    }
    draw_align(7);

    next_row();
    if(city == VILLAGE){
        draw_text(cur_city->get_name(), 0);
    }else if(room >= 'A' && room <= 'Z'){
        static_city_draw_town(city, 0, n + 3);
    }else{
        draw_text(cur_city->get_name(), 0);
    }
    draw_align(16+3);
    draw_options(REGULAR | DB_OPT_RESET, DB_OPT_CUT);
    next_row();
    draw_as_line('_', 0, 30);
}

void cmp_city::on_draw_machinery(int anime){
    draw_city_img(anime, cur_city->get_city(), 'W');
    int hero_machine = 0;

    hmmc_event e = hmmc_event('H');
    parent->on_event(this, &e);
    group* g = get_army(4, e.p.state);
    if(g != nullptr){
        pair *u = g->get_unit(5);
        hero_machine = u->value;

        int hid = g->get_hero_val_or_def(HERO_ATTR_ID, 0);
        if(hid){
            static_hero_draw_name_align(hid, 0, 0, 12, 30, 1);
        }

        if(hero_machine)
            ms.draw_unit(hero_machine, 10, 14, AN_RIGHT);
        else{
            set_position(0, 15);
            draw_text_align(0, 30, "Hero doesn't have a machine yet", 'R', 1);
        }
    }

    for(int i = 2; i < 28; i++)
        draw_char_at_position('|', 0, 30, i);

    set_position(35, 3);
    draw_text("Buy machine:", 0);
    int h = 5;

    for(int j = 0; j < 3; j++){
        int machine = SPECIAL | UNIT | (2 << j);

        set_position(35, h++);
        static_draw(machine, 0, AN_LEFT);
        next_col();
        draw_int(j + 1); draw_text(". ", 0); draw_text(ms.unit_text(machine, 0), 0);

        set_position(35, h++);
        static_draw(machine, 1, AN_LEFT);
        next_col();
        draw_text(ms.unit_text(machine, 9), 0);

        set_position(35, h++);
        static_draw(machine, 2, AN_LEFT);
        next_col();

        if(machine == hero_machine){
             draw_text("Hero already has this machine", 0);
        }else{
            pair p = ms.unit_cost(machine);
            int money = hero_machine ? p.value >> 1 : p.value;
            draw_text("Cost: ", 0); draw_int(money); draw_text(" Gold", 0);
            if(p.subval) { draw_text(" and ", 0); draw_int(p.subval); draw_text(" Wood", 0); }
            if(p.state) { draw_text(" and ", 0); draw_int(p.subval); draw_text(" Mercury", 0); }
        }
        h++;
    }
}

void cmp_city::on_draw_manufacture(int room, int anime){
    draw_city_img(anime, cur_city->get_city(), room);

    // 10 gold => 5 wood => 3 Mercury
    next_row();next_row();
    draw_text("You have resources:", 0);
    next_row();
    next_col(); draw_text("Gold: ", 0); draw_int(resources->value);
    next_row();
    next_col(); draw_text("Wood: ", 0); draw_int(resources->subval);
    next_row();
    next_col(); draw_text("Mercury: ", 0); draw_int(resources->state);
    next_row();
    next_row();

    if(room == 'T'){
        draw_text("Buy Gold", 0);
        next_row();
        next_col();
        draw_text("1. 5 Wood => 9 Gold", 0);
        next_row();
        next_col();
        draw_text("2. 3 Mercury => 9 Gold", 0);
    }else if(room == 'G'){
        draw_text("Buy Wood", 0);
        next_row();
        next_col();
        draw_text("1. 11 Gold => 5 Wood", 0);
        next_row();
        next_col();
        draw_text("2. 4 Mercury => 6 Wood", 0);
    }else if(room == 'R'){
        draw_text("Buy Mercury", 0);
        next_row();
        next_col();
        draw_text("1. 11 Gold => 3 Mercury", 0);
        next_row();
        next_col();
        draw_text("2. 7 Wood => 4 Mercury", 0);
    }

    for(int i = 2; i < 28; i++)
        draw_char_at_position('|', 0, 30, i);

    /// todo guardian small unit for village
    // and image for city
    // forest - листья растения и etc
    /*
        /\
       /  \
      (_/\_)
         \\
    */

}

void cmp_city::on_draw_market(int anime){
    draw_city_img(anime, cur_city->get_city(), 'M');

    // 10 gold => 5 wood => 3 Mercury  /// 4620 => 4320

    next_row();next_row();
    draw_text("You have resources:", 0);
    next_row();next_row();
    next_col(); draw_text("Gold: ", 0); draw_int(resources->value);
    next_row();
    next_col(); draw_text("Wood: ", 0); draw_int(resources->subval);
    next_row();
    next_col(); draw_text("Mercury: ", 0); draw_int(resources->state);
    next_row();

    set_position(35, 3);
    draw_text("Buy resources:", 0);
    int h = 5;
    set_position(35, h++);
    if(resources->value > 11)
        draw_text("1. 12 Gold => 5 Wood", 0);
    set_position(35, h++);
    if(resources->value > 11)
        draw_text("2. 12 Gold => 3 Mercury", 0);
    set_position(35, h++);
    if(resources->subval > 5)
        draw_text("3. 6 Wood => 10 Gold", 0);
    set_position(35, h++);
    if(resources->subval > 5)
        draw_text("4. 6 Wood => 3 Mercury", 0);
    set_position(35, h++);
    if(resources->state > 2)
        draw_text("5. 3 Mercury => 8 Gold", 0);
    set_position(35, h++);
    if(resources->state > 2)
        draw_text("6. 3 Mercury => 4 Wood", 0);

    for(int i = 2; i < 28; i++)
        draw_char_at_position('|', 0, 30, i);

}

/*
A-level-1 B-level-2 C-level-3 D-level-4
Hall Market Inn Library

R-Laboratory - white tower
G-Greenhouse - forest union
W-workshop - stronghold
T-TREASURE - red castle
X - unique room

"HIGABMLCD" - forest union
"HIAMBCLWD" - stronghold
"HIMABCLDT" - red castle
"HMIABCLRD" - white tower

wall = 5 > White tower | Red castle
mote = 8 > forest union
fence = 9 > strongold
*/

bool cmp_city::is_build(char page){
    int room = static_city_get_room(cur_city->get_city() & ALL_CITIES, page);
    if(room == 0) return false;
    int level = cur_city->get_level();
    return(level & (1 << (room-1)));
}

group* cmp_city::get_army(int arg, int toogle){
    if(arg == 2){
        return toogle == 2 ? group_out : cur_city->get_garrison();
    }

    if(arg == 4){
        return toogle == 2 ? cur_city->get_garrison() : group_out;
    }
    return NULL;
}

void cmp_city::on_draw_recruting(int anime){
    int w4 = draw_display_width() >> 2;

    int a = subpage; // if hero out not found then a = 1
    hmmc_event e = hmmc_event('H');
    parent->on_event(this, &e);

    int hid = e.p.state == 2 ? e.p.value : e.p.subval;
    if(a > 1 && hid == 0) a = 1;
    if(hid == 0) a = 1;

    subpage = a;

    if(a == 0){
        on_draw_units(1, 0, e.p, anime);
        on_draw_units(8, w4-5, e.p, anime);
        on_draw_units(4, w4 * 2, e.p, anime);
        on_draw_units(2, w4 * 3, e.p, anime);
    }else if(a == 1){
        on_draw_units(1, 0, e.p, anime);
        on_draw_units(8, w4-5, e.p, anime);
        on_draw_units(2, w4 * 2, e.p, anime);
        on_draw_units(4, w4 * 3, e.p, anime);
    }else{
        on_draw_units(2, 0, e.p, anime);
        on_draw_units(16, w4, e.p, anime);
        on_draw_units(4, w4 * 2, e.p, anime);
        on_draw_units(1, w4 * 3, e.p, anime);
    }

    // 0 - buy for out hero, 1 - buy for into or garrison
    // 2 - обмен между out and into/garrison
}

/** arg = 1-buy, 2-garrison or into army, 4-out army */
void cmp_city::on_draw_units(int arg, int pos, pair heroes, int anime){
    int top  = 0;
    if(arg == 1){
        int city = cur_city->get_city();
        set_position(pos, top++);        draw_text("___   _ _  _  _", 0);
        set_position(pos, top++);        draw_text("||_)) | V | \\\\//", 0);
        set_position(pos, top++);        draw_text("||_))  \\_/   ||", 0);
        top++;
        for(int i = 0; i < 4; i++){
            int lvl = 1 << i;
            bool is_big = static_unit_is_huge(city|UNIT|lvl);
            int lo = (is_big ? 3 : 5);
            for(int n=0;n<3;n++){
                set_position(pos + lo, top++);
                static_draw(city|UNIT|lvl, n, AN_LEFT);
            }
            top++;
        }
        return;
    }

    if(arg == 8){
        top += 4;
        int city = cur_city->get_city();
        int units = static_city_growl(city, cur_city->get_level()) & 15;
        for(int i = 0; i < 4; i++){
            int lvl = 1 << i;
            if((units & lvl) == 0){
                top ++;
                set_position(pos, top++);
                draw_text("Not built yet", 0);
                top +=2;
            }else{
                set_position(pos, top++);
                draw_int(i+1); draw_text(". ", 0); draw_text(static_unit_get_row(city|TEXT|lvl, 0, 0), 0);
                set_position(pos, top++);
                draw_text("Cost: ", 0);
                    draw_text(static_unit_get_row(city|TEXT|lvl, 8, 0), 0); draw_text(" gold", 0);
                    const char* elem = static_unit_get_row(city|TEXT|lvl, 10, 0);
                    if(elem[0] == 'M') draw_text(" and mercury", 0);
                    if(elem[0] == 'W') draw_text(" and wood", 0);
                set_position(pos, top++);
                draw_text("Available: ", 0); draw_int(cur_city->get_available(city|UNIT|lvl));
                top ++;
            }
        }
        return;
    }

    if(arg == 16){
        top += 4;
        group *left = get_army(2, heroes.state);
        group *right = get_army(4, heroes.state);
        for(int i = 0; i < 5; i++){
            set_position(pos, top++);
            pair* lu = left->get_unit(i);
            if(lu != nullptr && lu->value > 0){
                draw_int(i+1);
                draw_text(" >> ", 0);
                draw_text(ms.unit_text(lu->value, 0), 0);
            }
            top++;
            set_position(pos, top++);
            pair* ru = right->get_unit(i);
            if(ru != nullptr && ru->value > 0){
                draw_text(ms.unit_text(ru->value, 0), 0);
                draw_text(" << ", 0);
                draw_int((i+6 > 9) ? 0 : i+6);
            }
        }
        return;
    }

    group *army = get_army(arg, heroes.state);
    if(army == nullptr) return;

    int hero_id = army->get_hero_val_or_def(HERO_ATTR_ID, 0);
    int angle = (arg == 4) ? AN_LEFT : AN_RIGHT;
    if(hero_id != 0){
        for(int i = 0; i < 3; i++){
            set_position(pos + 2, top++);
            static_draw(HERO, i, angle);
        }
    }else{
        for(int i = 0; i < 3; i++){
            set_position(pos + 5, top++);
            static_city_draw_town(VILLAGE, 12, i);
        }
    }
    top++;

    for(int j=0; j<5; j++){
        pair *p = army->get_unit(j); // {type, count, ???}
        if(p->value == 0) {
            top += 3;
            continue;
        }
        bool is_big = static_unit_is_huge(p->value);
        int lo = (is_big ? 3 : 5);
        for(int n=0;n<3;n++){
            set_position(pos + lo, top++);
            static_draw(p->value, n, angle);
        }
        set_position(pos, top-1);
        draw_text("(", 0); draw_int(p->subval); draw_text(")", 0);
    }
}

void cmp_city::on_draw_room(int city, int anime){

}

void cmp_city::on_draw_armory(int anime){

}

void cmp_city::on_draw_village(int anime){
    int l = cur_city->get_level();
    if(l == 15 || l == 16){ // water mill
        on_draw_manufacture('T', anime);
    }else if(l == 5 || l == 6){ //sawmill
        on_draw_manufacture('G', anime);
    }else if(l == 1 || l == 2){ //laboratory
        on_draw_manufacture('R', anime);
    }else if(l == 12){ //armory
        on_draw_armory(anime);
    }else if(l == 11){ // machinery
        on_draw_machinery(anime);
    }else if(l == 9){ // tower
        on_draw_room(WHITE_TOWER, anime);
    }else if(l == 3 || l == 4){ // village
        on_draw_room(STRONHOLD, anime);
    }else if(l == 7){ // shack
        on_draw_room(FOREST_UNION, anime);
    }else if(l == 8){ // inn
        on_draw_tavern(anime);
    }else if(l == 10){ //red post
        on_draw_room(RED_CASTLE, anime);
    }else if(l == 13){ //market
        on_draw_market(anime);
    }else if(l == 14){ //library
        on_draw_library(anime);
    }
}

void cmp_city::on_draw_city(int anime){
    if(page == 'I' && is_build(page)){
        on_draw_tavern(anime);
    }else if(page == 'L' && is_build(page)){
        on_draw_library(anime);
    }else if(page == 'M' && is_build(page)){
        draw_resources_list();
        on_draw_market(anime);
    }else if(page == 'U' || (page >= 'A' && page <= 'D')){
        on_draw_recruting(anime);
    }else if((page == 'G' || (page == 'X' &&  cur_city->get_city() == FOREST_UNION)) && is_build('G')){
        draw_resources_list();
        on_draw_manufacture('G', anime);
    }else if((page == 'T' || (page == 'X' &&  cur_city->get_city() == RED_CASTLE)) && is_build('T')){
        draw_resources_list();
        on_draw_manufacture('T', anime);
    }else if((page == 'R' || (page == 'X' &&  cur_city->get_city() == WHITE_TOWER)) && is_build('R')){
        draw_resources_list();
        on_draw_manufacture('R', anime);
    }else if((page == 'W' || (page == 'X' &&  cur_city->get_city() == STRONHOLD)) && is_build('W')){
        draw_resources_list();
        on_draw_machinery(anime);
    /*
    }else if(page == 'H'){
        on_draw_hall(anime); */
    }else{
        page = 0;
        draw_resources_list();
        on_draw_page_main(anime);
    }
}

void cmp_city::on_draw(int anime) {
    if(is_load){
        draw_clear(511);
        is_load = false;
    }else{
        if(is_dialog){
            dialog->on_draw(anime);
            return;
        }
        draw_clear(REGULAR | HIGH);
        draw_select_buffer(REGULAR);
    }
    if(cur_city->get_city() == VILLAGE){
        on_draw_village(anime);
    }else{
        on_draw_city(anime);
    }
}

bool cmp_city::enough_resources(pair p, int cost){
    if(resources->value < p.value * cost) return false;
    if(resources->subval < p.subval * cost) return false;
    if(resources->state < p.state * cost) return false;

    return true;
}

//return true if exit
bool cmp_city::buy_or_move(int a){
    if(a < 1 || a > 10) return true;
    if(dialog == nullptr)
        dialog = new cmp_dialog(this);
    dialog->clear();

    hmmc_event e = hmmc_event('H');
    parent->on_event(this, &e);

    if(subpage == 2){
        int r = 0;
        city_args q = the_town.try_move(a, get_army(2, e.p.state), get_army(4, e.p.state));
        if(q.u_type > 0) r |= 1; else return true;
        if(q.pos_l > 0 && q.pos_r > 0) r |= 2;

        if(r == 3){
            int available = q.count;
            dialog->set_title("You moving ", ms.unit_text(q.u_type, 0));
            dialog->set_row("Available: ", available);
            int btns = DIALOG_BTN_CLOSE;

            while(available > 0){
                if(q.last && available < 2) break;
                dialog->update_button(DIALOG_BTN_ONE, "One", 'o');
                btns |= DIALOG_BTN_ONE;
                break;
            }
            while(available > 9){
                if(q.last && available < 11) break;
                dialog->update_button(DIALOG_BTN_TWO, "Ten", 't');
                btns |= DIALOG_BTN_TWO;
                break;
            }

            if(available > 1){
                dialog->update_button(DIALOG_BTN_THREE, "Half", 'h');
                btns |= DIALOG_BTN_THREE;
            }

            if(available > 0 && !q.last){
                dialog->update_button(DIALOG_BTN_FOUR, "All", 'a');
                btns |= DIALOG_BTN_FOUR;
            }
            dialog->set_buttons(btns);
        }else{
            dialog->set_title("You can't move ", ms.unit_text(q.u_type, 0));
            dialog->set_row("Not enough empty slots!");
            dialog->set_buttons(DIALOG_BTN_CLOSE);
        }
        is_dialog = a+1;
    }else{
        int city = cur_city->get_city();
        int units = static_city_growl(city, cur_city->get_level()) & 15;

        int r = 0;
        if(units & (1 << a)) r |= 1; // unit is build

        group *army = NULL;
        if(subpage == 1){
            army = get_army(2, e.p.state);
        }else if(subpage == 0){
            army = get_army(4, e.p.state);
        }
        int unit_type = city|UNIT|(1 << a);
        if(army != nullptr && army->can_append(unit_type) >= 0) r |= 2;

        if(r == 3){
            dialog->set_title("Buy unit ", ms.unit_text(unit_type, 0));
            int available = cur_city->get_available(unit_type);
            dialog->set_row("Available: ", available);
            pair cost = ms.unit_cost(unit_type|TEXT);

            if(cost.value > 0) dialog->set_row("Gold: ", cost.value);
            if(cost.subval > 0) dialog->set_row("Wood: ", cost.subval);
            if(cost.state > 0)dialog->set_row("Mercury: ", cost.state);

            int btns = DIALOG_BTN_CLOSE;

            if(available > 0 && enough_resources(cost, 1)){
                dialog->update_button(DIALOG_BTN_ONE, "One", 'o');
                btns |= DIALOG_BTN_ONE;
            }
            if(available > 9 && enough_resources(cost, 10)){
                dialog->update_button(DIALOG_BTN_TWO, "Ten", 't');
                btns |= DIALOG_BTN_TWO;
            }
            if(available > 2 && enough_resources(cost, available >> 1)){
                dialog->update_button(DIALOG_BTN_THREE, "Half", 'h');
                btns |= DIALOG_BTN_THREE;
            }
            if(available > 0){
                dialog->update_button(DIALOG_BTN_FOUR, "All", 'a');
                btns |= DIALOG_BTN_FOUR;
            }
            dialog->set_buttons(btns);
        }else{
            dialog->set_title("You can't buy");
            dialog->set_row("The building for the unit is not yet built");
            dialog->set_buttons(DIALOG_BTN_CLOSE);
        }
        is_dialog = a+1;
    }
    return false;
}

void cmp_city::spend_resources(pair cost, int count){
    resources->value -= cost.value * count;
    resources->subval -= cost.subval * count;
    resources->state -= cost.state * count;
}

bool cmp_city::on_press_dialog(int r){
    if(page == 0){
        if(r == DIALOG_BTN_YES){
            int b = 1 << (is_dialog - 1);
            pair cost = static_city_cost(cur_city->get_city(), is_dialog - 1);
            spend_resources(cost, 1);
            cur_city->set_level( b | cur_city->get_level());
        }
        return true;
    }
    if(page == 'U'){
        hmmc_event e = hmmc_event('H');
        parent->on_event(this, &e);
        int count = 0;
        if(r == DIALOG_BTN_CLOSE){ is_dialog = 0; return true; }
        if(r == DIALOG_BTN_ONE) count = 1;
        if(r == DIALOG_BTN_TWO) count = 10;

        if(subpage == 2){
            group* gl = get_army(2, e.p.state);
            group* gr = get_army(4, e.p.state);
            city_args q = the_town.try_move(is_dialog - 1, gl, gr);
            if(r == DIALOG_BTN_THREE) count = q.count >> 1;
            if(r == DIALOG_BTN_FOUR) count = q.count;
            if(q.to_left){
                gl->append_unit(q.u_type, count);
                pair *un = gr->get_unit(q.pos_r - 1);
                un->subval -= count;
                gr->remove_dead();
            }else{
                gr->append_unit(q.u_type, count);
                pair *un = gl->get_unit(q.pos_l - 1);
                un->subval -= count;
                gl->remove_dead();
            }
            return buy_or_move(is_dialog - 1);
        }else{
            group* g = get_army((subpage == 1) ? 2 : 4, e.p.state);
            int lvl = 1 << (is_dialog - 1);
            int unit = cur_city->get_city() | lvl | UNIT;
            int amount = cur_city->get_available(unit);
            if(r == DIALOG_BTN_THREE) count = amount >> 1;
            if(r == DIALOG_BTN_FOUR) count = amount;
            pair cost = ms.unit_cost(unit);
            spend_resources(cost, count);
            g->append_unit(unit, count);
            cur_city->remove(unit, count);
            return buy_or_move(is_dialog - 1);
        }
    }

    return true;
}

bool cmp_city::dialog_not_enough(pair cost, int count){
    if(dialog == nullptr)
        dialog = new cmp_dialog(this);
    dialog->clear();
    dialog->set_row ("Resource requirements:");

    int low = 0;
    if(cost.value > 0){
        if(this->resources->value >= cost.value * count){
            dialog->set_row("Gold: ", cost.value * count);
        }else{
            dialog->set_row("Gold is low! Need: ", cost.value * count);
            low++;
        }
    }
    if(cost.subval > 0){
        if(this->resources->subval >= cost.subval * count){
            dialog->set_row("Wood: ", cost.subval * count);
        }else{
            dialog->set_row("Wood is low! Need: ", cost.subval * count);
            low++;
        }
    }
    if(cost.state > 0){
        if(this->resources->state >= cost.state * count){
            dialog->set_row("Mercury: ", cost.state * count);
        }else{
            dialog->set_row("Mercury is low! Need: ", cost.state * count);
            low++;
        }
    }
    return low;
}

int cmp_city::on_press(int c, int k){
    if(is_dialog){
        int r = dialog->on_press(c, k);
        if(on_press_dialog(r)){
            is_dialog = 0;
            is_load = true;
        }
        return 0;
    }

    if(page == 0){
        if(c >= '1' && c <= '9'){
            int ba = (c - '0');
            char r = get_build(cur_city->get_city(), cur_city->get_level(), cur_city->get_max_level(), ba - 1);
            if(r == 'v'){
                page = static_city_get_room(cur_city->get_city() & ALL_CITIES, c);
                if(page == 'H'){
                    if(dialog == nullptr)
                        dialog = new cmp_dialog(this);
                    dialog->clear();
                    dialog->set_title(static_city_get_title(cur_city->get_city(), 0));
                    dialog->set_row("Received this week in the city");
                    pair* info = cur_city->week_info();
                    dialog->set_row("Gold: ", info->value);
                    dialog->set_row("Wood:", info->subval);
                    dialog->set_row("Mercury:", info->state);
                    dialog->set_buttons(DIALOG_BTN_CLOSE);
                    is_dialog = 100;
                }
            }else if(r == 'o'){
                if(dialog == nullptr)
                    dialog = new cmp_dialog(this);
                dialog->clear();
                int lvl = c - '1';
                pair cost = static_city_cost(cur_city->get_city(), lvl);
                bool low = dialog_not_enough(cost, 1);
                dialog->set_title("Do you build ", static_city_get_title(cur_city->get_city(), lvl), "?");
                if(low){
                    dialog->set_buttons(DIALOG_BTN_CLOSE);
                }else{
                    dialog->set_buttons(DIALOG_BTN_YES_NO);
                }
                is_dialog = ba;
            }else{ // x

            }
        }else if(c >= 'a' && c <= 'z'){
            page = c ^ (c & 32);
        }else if(c == '0'){
            page = '0';
        }
    }else if(page == 'U'){
        if(subpage == 2 && c > '0' && c <= '9'){
            buy_or_move(c - '0');
        }else if(subpage == 2 && c == '0'){
            buy_or_move(10);
        }else if(c >= '1' && c <= '4'){
            buy_or_move(c - '1');
        }else if(k == KEY__TAB){
            subpage++;
            if(subpage > 2) subpage = 0;
        }else{
            page = 0;
        }
    }else if(page == 'M'){
        if(c >= '1' && c <= '6'){
            if(c=='1' && resources->value > 11) { resources->value -= 12; resources->subval += 5; }
            if(c=='2' && resources->value > 11) { resources->value -= 12; resources->state += 3; }
            if(c=='3' && resources->subval > 5) { resources->subval -= 6; resources->value += 10; }
            if(c=='4' && resources->subval > 5) { resources->subval -= 6; resources->state += 3; }
            if(c=='5' && resources->state > 2) { resources->state -= 3; resources->value += 8; }
            if(c=='6' && resources->state > 2) { resources->state -= 3; resources->subval += 4; }
        }else{
            page = 0;
        }
    }else if(page == 'W' || (page == 'X' &&  cur_city->get_city() == STRONHOLD)){
        int a = 0;
        if(c == '1' || c == 'f') a = TENT;
        if(c == '2' || c == 'w') a = WAGON;
        if(c == '3' || c == 'b') a = BALLISTA;
        if(a == 0){
            page = 0;
            return 0;
        }

        hmmc_event e = hmmc_event('H');
        parent->on_event(this, &e);
        group* g = get_army(4, e.p.state);
        int hm = 0;
        if(g == nullptr){
            if(dialog == nullptr)
                dialog = new cmp_dialog(this);
            dialog->clear();
            dialog->set_title("You can't buy a ", ms.unit_text(a, 0));
            dialog->set_row("You don't have a hero in the town");
            dialog->set_buttons(DIALOG_BTN_CLOSE);
            return 0;
        }else{
            pair *u = g->get_unit(5);
            hm = u->value;
        }

        if(hm == a) return 0;
        if(a){
            pair p = ms.unit_cost(a);
            int money = hm ? p.value >> 1 : p.value;
            p.value = money;
            if (enough_resources(p, 1)){
                spend_resources(p, 1);
                g->append_unit(a, 1);
            }else{
                dialog_not_enough(p, 1);
                dialog->set_title("You can't buy a ", ms.unit_text(a, 0));
                dialog->set_buttons(DIALOG_BTN_CLOSE);
                return 0;
            }
        }else{
            page = 0;
        }
    }else{
        page = 0;
    }

    if(page == 0) subpage = 0;

    return 0;
}

int cmp_city::on_tick(int anime){
    return 0;
}

/* todo пасхалочка
    создать анимацию для красного замка:
    лучник выглядывает, потом скрывается
    потом выходит и стреляет в перегородку внизу - она ломается и падает
    проходит вдоль экрана и уходит


*/
