#include "hero.h"


hero::hero(int id){
    this->id = id;
}
hero::~hero(){

}

void hero::set_attr(int attr, int val){
    //todo

}

int hero::get_attr(int attr){
    if(attr == HERO_ATTR_ID) return this->id;
    if(attr == HERO_ATTR_LEVEL) return this->level;
    if(attr == HERO_ATTR_SPECIALITY) return this->speciality;

    if(attr == HERO_ATTR_ATTACK){
        //todo bonus for attacks
        float bonus = 1.0f;
        return (int) (this->level * bonus);
    }
    if(attr == HERO_ATTR_DEFENSE){
        //todo bonus for defense
        float bonus = 1.0f;
        return (int) (this->level * bonus);
    }

    if(attr == HERO_ATTR_INTELLECT){
        //todo bonus for intellect
        float bonus = 1.0f;
        return (int) (this->level * bonus);
    }
    if(attr == HERO_ATTR_SORCERY){
        //todo bonus for sorcery
        float bonus = 1.0f;
        return (int) (this->level * bonus);
    }

    /* todo


    */
    return -1; // if not found
}
