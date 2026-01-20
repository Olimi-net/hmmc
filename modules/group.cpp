#include "group.h"
#include "../legacy/static_unit.h"

group::group(int id, int fraction, hero *h){
    this->current = h;
    this->fraction = fraction;
    this->group_id = id;
}
group::~group(){
    // current not delete
}

int group::get_group_id(){
    return this->group_id;
}

void group::set_position(int x, int y){
    if (this->position.value < x)
        this->step.value = 1; //R
    else if (this->position.value > x)
        this->step.value = 0; //L

    this->step.subval = (this->step.subval + 1) & 1; // next step

    this->position.value = x;
    this->position.subval = y;
}

int group::get_faction(){
    return this->fraction;
}

void group::set_faction(int f){
    this->fraction = f;
}

pair* group::get_position(){
    return &this->position;
}

int group::get_x(){
    return this->position.value;
}
int group::get_y(){
    return this->position.subval;
}

int group::can_append(int type){
    if(static_unit_is_machine(type)) return 5; //always

    for(int i = 0; i < 5; i++){
        if(this->units[i].value == type) return i;
    }
    for(int i = 0; i < 5; i++){
        if(this->units[i].value == 0) return i;
    }
    return -1;
}

bool group::append_unit(int type, int count){
    int a = can_append(type);
    if(a < 0) return false;

    this->units[a].value = type;
    if(static_unit_is_machine(type))
        this->units[a].subval = 1;
    else
        this->units[a].subval += count;
    return true;
}

int group::get_creature(){
    if(current != NULL) return HERO;

    for(int i = 0; i < 5; i++){
        if(this->units[i].value != 0){
            return this->units[i].value;
        }
    }
    return 0;
}
int group::get_animation(){
    // todo последнее направление движения
    /*
        const int AN_LEFT = 0;
        const int AN_MOVE_L = 1;
        const int AN_ATACK_L = 2;
        const int AN_RIGHT = 3;
        const int AN_MOVE_R = 4;
        const int AN_ATACK_R = 5;
    */
    if(this->step.value) return (this->step.subval) ? 4 : 3;   // r
    return this->step.subval; // AN_LEFT ^ AN_MOVE_L
}

hero* group::get_hero(){
    return this->current;
}

int group::get_hero_val_or_def(int attr, int def){
    if(this->current == NULL) return def;
    int a = this->current->get_attr(attr);
    return (a < -1) ? def : a;
}

pair* group::get_unit(int i){
    if(i >= 0 && i <= 5)
        return &units[i];
    return nullptr;
}

int group::slots_count(){
    int count = 0;
    for(int i = 0; i < 5; i++){
        if(this->units[i].value != 0){
            count++;
        }
    }
    return count;
}

void group::remove_dead(){
    for(int i = 0; i < 6; i++){
        if(this->units[i].subval < 1){
            units[i].value = 0;
            units[i].subval = 0;
            units[i].state = 0;
        }
    }
}
