#include "city.h"
#include "../legacy/static_city.h"
#include "../legacy/static_game.h"
#include "../legacy/static_unit.h"


city::city(int fraction, int builded, int max_build, int magic, int x, int y, hmmc_callback *c){
    this->position.value = x;
    this->position.subval = y;
    this->build = pair_factory(builded & ALL_CITIES, builded & BUILD_ALL, max_build);
    this->event = c;
    for(int i=0; i<5; i++){
        this->units[i].value = 0;
        this->units[i].subval = 0;
        this->units[i].state = 0;
    }
    update_growl();
    this->magic = magic;
    garrison = new group(0, fraction, NULL);
    resources = {0, 0, 0};
}
city::~city(){
    pair_free(this->build);
}

pair city::get_growl_today(int unit){
    pair p;
    p.state = unit;
    p.value = static_unit_growl(unit);
    p.subval = 0;
    int raven = FOREST_UNION | UNIT | LEVEL_1;
    if(unit == raven){
         if(this->get_city() == FOREST_UNION){
            int g = static_city_growl(this->get_city(), this->get_level());
            if(g & LEVEL_4) p.subval += p.value >> 2;
        }
    }
    //todo greenhouse приносит больше нормы, то количество энтов увеличивается
    //todo лаборатория приносит больше ртути, то количество птиц рух увеличивается
    //todo объявлена неделя лошади, то рыцари тоже увеличиваются и наоборот
    //todo если объявлена неделя птиц рух , то wild meet тоже увеличиваются и наоборот
    ///todo check hero into city with unit growling call event
    hmmc_event e = hmmc_event('G');
    int check = event->on_callback(&e, unit);
    if(check && p.value && static_unit_is_machine(unit) == 0){
        if(unit & LEVEL_4) p.subval += 1;
        if(unit & LEVEL_3) p.subval += 2;
        if(unit & LEVEL_2) p.subval += 3;
        if(unit & LEVEL_1) p.subval += 4;
    }
    return p;
}

int city::get_available(int unit){
    for(int i = 0; i<4;i++){
        if(this->units[i].state == unit)
            return this->units[i].subval;
    }
    return 0;
}

void city::update_growl(){
    int city = this->get_city();
    int g = static_city_growl(city, this->get_level());
    for(int i = 0; i < 4; i++){
        int lvl = 1 << i;
        if(g & lvl){
            bool b = (units[i].state == 0);
            units[i].state = city | UNIT | lvl;
            //growling in week
            pair p = get_growl_today(units[i].state);
            units[i].value = p.value + p.subval;
            //created
            if(b) units[i].subval = p.value;
        }else{
            units[i].state = 0;
        }
    }
}

void city::week_growl(){
    update_growl();
    for(int i = 0; i < 4; i++){
        if(units[i].state == 0) continue;
        //growling in week
        pair p = get_growl_today(units[i].state);
        units[i].value = p.value + p.subval;
        //total units
        units[i].subval += units[i].value;
    }
}

group* city::get_garrison(){
    return this->garrison;
}

char* city::get_name(){
    //todo
    return "Town unique name";
}

bool city::has_xy(int x, int y){
    return this->position.value == x && this->position.subval == y;
}

int city::get_city(){
    return this->build->value;
}

int city::get_fraction(){
    return this->garrison->get_faction();
}
void city::set_fraction(int f){
    this->garrison->set_faction(f);
}

void city::raw(){
    //found in map
}

int city::get_x(){
    return this->position.value;
}
int city::get_y(){
    return this->position.subval;
}

int city::get_level(){
    return this->build->subval;
}

void city::set_level(int b){
    this->build->subval = b;
    update_growl();
}

int city::get_max_level(){
    return this->build->state;
}

bool city::join_group(group *g){
    g->remove_dead();
    group* garrison = get_garrison();
    if(garrison->get_hero_val_or_def(HERO_ATTR_ID, 0) > 0) return false;
    for(int i = 0; i < 5; i++){
        pair* p = garrison->get_unit(i);
        if(p->value == 0) continue;
        if(!g->append_unit(p->value, 0)){
            g->remove_dead();
            return false;
        }
    }
    g->remove_dead();
    for(int i = 0; i < 5; i++){
        pair* p = garrison->get_unit(i);
        if(p->value == 0) continue;
        g->append_unit(p->value, p->subval);
    }

    delete this->garrison;
    this->garrison = g;
    return true;
}

void city::remove(int unit, int count){
    for(int i = 0; i<4;i++){
        if(this->units[i].state == unit){
            this->units[i].subval -= count;
            return;
        }
    }
}

pair* city::week_info(){
    return &resources;
}
