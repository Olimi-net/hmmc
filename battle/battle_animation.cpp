#include "battle_animation.h"
#include "battle_const.h"

battle_animation::battle_animation(hmmc_callback* e, hmmc_callback* field){
    this->event=e;
    this->state = 0;
    this->id = 0;
    this->field = field;
}

battle_animation::~battle_animation(){

}

int battle_animation::get_id() { return this->id; }

void battle_animation::drop(){
    hmmc_event e = hmmc_event(AM_DROP);
    this->field->on_callback(&e, AM_DROP);
}

void battle_animation::init(int anim, int at, hmmc_queue_item* one, hmmc_queue_item* two, int flag){
    this->id++;
    this->one = one;
    this->two = two;
    this->state = anim;
    this->flag = flag;
    this->unit = 0;
    this->at = at;

    /* depricated only test luck
    if((this->at & (UNIT_FLAG_LUCK | UNIT_FLAG_UNLUCK))==0){
        int n = this->id & 3;
        if(n < 3){
            this->at |= n << 8;
        }
    } */

    this->luck = 0;
    // HERO_ATTACK, MOVE_ATTACK, STAND_ATTACK - BATTLE_MELEE
    // RANGE_ARROW - BATTLE_RANGER
    // NEGATIVE_MAGIC - BATTLE_MAGIC
    // POSITIVE_MAGIC - BATTLE_INTERACTION
    // MOVE_HOME - MOVE_HOME
    // LOW_MORALE - BATTLE_NEXT
}

hmmc_queue_item* battle_animation::get_one(){return this->one;}
hmmc_queue_item* battle_animation::get_two(){return this->two;}
bool battle_animation::is_end()     {return this->state == 0;}
int battle_animation::get_flag()    {return this->flag;}

bool battle_animation::on_tick(){
    if(this->state & BA_HERO){
        hmmc_event e = hmmc_event((this->state & BA_RANGE) ? BA_HERO_RANGE : BA_HERO_ATTACK);
        if(this->event->on_callback(&e, this->two->id)) return true;
        this->state = 0;
        return false;
    }

    if(this->luck == 0){
        if(this->at & (UNIT_FLAG_LUCK | UNIT_FLAG_UNLUCK)){
            this->luck = BA_LUCK | ((this->at & UNIT_FLAG_LUCK) ? BA_POSITIVE : BA_NEGATIVE);
        }else{
            this->luck = -1;
        }
    }

    if(this->state & BA_RANGE){
        if(two == NULL) return false;
        if(this->luck > 0){
            hmmc_event e = hmmc_event(this->luck | BA_LUCK);
            if(this->event->on_callback(&e, 0)) return true;
            this->luck = -1;
        }
        if(this->unit != this->one->id){
            if(this->one->is_city_archer && one->city_unit == BTL_UNIT_CATAPULT){
                hmmc_event e = hmmc_event(BA_OTHER_EVENT);
                this->event->on_callback(&e, BTL_UNIT_CATAPULT);
            }else{
                this->one->anim_attack();
            }
            this->unit = this->one->id;
        }
        //move arrow not 0 if move yet
        {
            hmmc_event e = hmmc_event(BA_RANGE_ARROW);
            if(this->event->on_callback(&e, 0)) return true;
        }

        //animation damage here
        this->two->anim_damage();
        this->state = 0;
        return false;
    }

    if(this->state == BA_NEGATIVE_MAGIC){
        if(this->luck > 0){
            hmmc_event e = hmmc_event(this->luck | BA_LUCK);
            if(this->event->on_callback(&e, 0)) return true;
            this->luck = -1;
        }
        if(this->unit != this->one->id){
            this->one->anim_attack();
            this->unit = this->one->id;
            return true;
        }

        {
            hmmc_event e = hmmc_event(BA_NEGATIVE_MAGIC);
            if(this->event->on_callback(&e, (this->at & UNIT_HAS_MAGIC))) return true;
        }

        this->two->anim_damage();

        if((this->at & UNIT_HAS_MAGIC)==UNIT_HAS_DRAIN_LIFE){
            this->state = BA_POSITIVE_MAGIC;
            return true;
        }
        this->state = 0;
        return false;
    }
    if(this->state == BA_POSITIVE_MAGIC){
        if(this->luck > 0){
            hmmc_event e = hmmc_event(this->luck | BA_LUCK);
            if(this->event->on_callback(&e, 0)) return true;
            this->luck = -1;
        }
        if(this->unit != this->one->id){
            this->one->anim_attack();
            this->unit = this->one->id;
            return true;
        }

        {
            hmmc_event e = hmmc_event(BA_POSITIVE_MAGIC);
            int cid = ((this->at & UNIT_HAS_MAGIC)==UNIT_HAS_DRAIN_LIFE) ? this->one->id : this->two->id;
            if(this->event->on_callback(&e, cid)) return true;
        }

        this->state = 0;
        return false;
    }

    if(this->state & BA_LOW_MORALE){
        if(this->unit != this->one->id){
            hmmc_event e = hmmc_event(BA_LOW_MORALE);
            if(this->event->on_callback(&e, 0)) return true;
        }
        this->state = 0;
        return false;
    }

    if(this->state & BA_MOVE){
        //move on step
        hmmc_event e = hmmc_event(BA_MOVE);
        int r = this->event->on_callback(&e, this->one->id);
        if(r > 0) return true;
        if(r < 0){
            //step is low
            this->flag = 0;
            this->state = 0;
            this->drop();
            return false;
        }
        if(this->state & BA_ATTACK){
            this->state ^= BA_MOVE;
        }else{
            this->state = 0;
            return false;
        }
    }

    if(this->state & BA_ATTACK){
        if(this->luck > 0){
            hmmc_event e = hmmc_event(this->luck);
            if(this->event->on_callback(&e, 0)) return true;
            this->luck = -1;
        }
        this->one->anim_attack();
        this->two->anim_damage();
        this->state = 0;
        return false;
    }

    this->state = 0;
    return false;
}
