#include "hmmc_queue_item.h"

hmmc_queue_item::hmmc_queue_item(battle_group* g, hmmc_unit* u, int f){
    this->group = g;
    this->unit = u;
    this->id = (u == NULL) ? g->hero_id() : u->get_id();
    this->fac = f;
    this->assign = 0;
    this->is_rem = false;
    this->hero_angle = f == 1 ? AN_RIGHT : AN_LEFT;
    this->attack_anime = 0;
    this->city_unit = 0;
    this->is_city_archer = false;
}

hmmc_queue_item::~hmmc_queue_item(){

}

bool hmmc_queue_item::is_hero(){ return this->id == this->group->hero_id(); }

int hmmc_queue_item::get_assign(bool near){
    if(this->is_hero()){
        //todo
        return -2;
    }
    return this->unit->calc_assign(near);
}

void hmmc_queue_item::interaction(int at, hmmc_queue_item* q){
    this->group->interaction(at, this->id, q->id);
}

bool hmmc_queue_item::not_next(){
    if(city_unit || is_hero()) return true;
    return (this->unit->is_dead() && this->unit->nonliving());
}

int hmmc_queue_item::get_type(){
    if(this->is_hero()) return HERO;
    return this->unit->get_type();
}

void hmmc_queue_item::anim_attack(){
    //todo next animation show as attack
    attack_anime = 1;
}

void hmmc_queue_item::anim_damage(){
    attack_anime = 2;
}

int hmmc_queue_item::set_damage(int dmg){
    return this->unit->set_damage(dmg);
}

int hmmc_queue_item::get_anime(){
    int angle = this->is_hero() ? this->hero_angle : this->unit->get_angle();
    if(this->attack_anime == 2){
        this->attack_anime = 0;
        if(angle == AN_LEFT || angle == AN_ATACK_L) return AN_MOVE_L;
        if(angle == AN_RIGHT || angle == AN_ATACK_R) return AN_MOVE_R;
    }else if(this->attack_anime == 1){
        this->attack_anime = 0;
        if(angle == AN_LEFT || angle == AN_MOVE_L) return AN_ATACK_L;
        if(angle == AN_RIGHT || angle == AN_MOVE_R) return AN_ATACK_R;
    }
    return angle;
}

int hmmc_queue_item::dmg_min(){
    if(is_city_archer) return 10;
    if(this->is_hero())  return 1; // todo select damage for hero
    return this->unit->cur_min() * this->unit->get_count();
}

int hmmc_queue_item::dmg_max(){
    if(is_city_archer) return 20;
    //todo bonus from wagon for archers
    if(this->is_hero())  return 1; // todo select damage for hero
    return this->unit->cur_max() * this->unit->get_count();
}

float hmmc_queue_item::get_archery(){
    if(this->is_hero() || is_city_archer) return 0;
    return this->group->get_archery();
}

bool hmmc_queue_item::is_pass(){
    if(is_hero()) return false;
    return this->unit->is_dead();
    //todo stunned or dead
}
