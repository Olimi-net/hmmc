#include "battle_group.h"
#include "../legacy/static_unit.h"

//https://pro-prof.com/forums/topic/multiple-inheritance-cplusplus

battle_group::battle_group(group *g, int side, float user_lvl, hmmc_callback* e):hmmc_callback(){
    this->event = e;
    this->parent = g;
    this->difficult = user_lvl;
    this->offset = (side == LEFT_GROUP) ? 1 : 0; //side L | R выравнивание по правому краю или по левому
    int an = (side == LEFT_GROUP) ? AN_RIGHT : AN_LEFT;
    for (int i = 0; i < 6; i++){
        this->units.push_back(new hmmc_unit(this, g->units[i], side + i, an));
    }
}

battle_group::~battle_group(){
    // this->parent = NULL; //not delete
    int size = units.size();
    for (int i = size-1; i >= 0; i--){
        delete units[i];
    }
    units.clear();
}

int battle_group::on_callback(hmmc_event *e, int a){
    if(e->id == UNIT_EVENT_MORALE){
        int total = calc_morale(a);
        return math_rand_sum_2_4(total < 0 ? -1 : 2, 1, 0, total > 3 ? 0 : -1);
    }
    if(e->id == UNIT_EVENT_WAGON){
        int size = this->units.size();
        for(int i=0;i<size;i++){
            if(this->units[i]->get_id() == 0) continue;
            if(this->units[i]->get_type() == WAGON){
                return this->units[i]->is_dead() ? 0 : this->units[i]->get_id();
            }
        }
        return 0;
    }
    if(e->id == UNIT_EVENT_HERO_LEVEL){ return this->parent->get_hero_val_or_def(HERO_ATTR_LEVEL, 0); }
    if(e->id == UNIT_EVENT_UNIT_SPECIALITY) {return this->parent->get_hero_val_or_def(HERO_ATTR_SPECIALITY, 0);}

    //I2  = 0.10, 0.22, 0.40 for basic, advanced, expert  x 100
    if(e->id == UNIT_EVENT_ATTACK_DIFICULT) { return (int)(this->difficult * 100); }

    // R2 = 0.05, 0.10, 0.15 for basic, advanced, expert x 100
    if(e->id == UNIT_EVENT_DEFENSE_DIFICULT){
        if(this->difficult < 0.2f) return 5;
        if(this->difficult > 0.3f) return 15;

        return 10;
    }

    if(e->id == UNIT_EVENT_DEAD){
        hmmc_event he = hmmc_event(GROUP_EVENT);
        this->event->on_callback(&he, GE_REVALIDATE_QUEUE);
        return 0;
    }
    return 0;
}

void battle_group::start_line(int x){
    int size = this->units.size();
    for(int i=0;i<size;i++){
        int a = this->units[i]->is_big() ? 0 : this->offset;
        this->units[i]->set_pos((a + x) * 5, i * 3);
    }
}

int battle_group::calc_morale(int id){
    hmmc_unit* u = this->get_unit_by_id(id);
    if(u == NULL) return 0;
    int morale = u->get_morale();
    int total = this->parent->get_hero_val_or_def(HERO_ATTR_LIDER, 0);  //todo лидерство героя поднимает первое значение морали
    int size = this->units.size();
    for(int i=0;i<size;i++){
        if(this->units[i]->get_id() == id) continue;
        int m = this->units[i]->get_morale();
        if(m == 0) continue;
        total += (m & morale) ? 1 : -1;
    }
    return total;
}

int battle_group::get_after(int id){
    hmmc_unit* u = this->get_unit_by_id(id);
    if(u != NULL) return u->after_step();
    return -2;
}

hmmc_unit* battle_group::get_unit_by_id(int id){
    int size = this->units.size();
    for(int i=0;i<size;i++)
        if(this->units[i]->get_id() == id)
            return this->units[i];
    return NULL;
}

int battle_group::is_life(int id){
    if(id == this->parent->get_hero_val_or_def(HERO_ATTR_ID, 0)) return id;
    hmmc_unit* u = this->get_unit_by_id(id);
    if(u != NULL) return u->is_dead() ? 0 : id;
    return 0;
}

int battle_group::is_id(int id){
    if(id == this->parent->get_hero_val_or_def(HERO_ATTR_ID, 0)) return id;
    hmmc_unit* u = this->get_unit_by_id(id);
    return (u != NULL) ? id : 0;
}

int battle_group::hero_id(){
    return this->parent->get_hero_val_or_def(HERO_ATTR_ID, 0);
}

void battle_group::remove_shoots(int id, int count){
    if(id == this->hero_id()){
        //todo remove mana from hero
        return;
    }

    hmmc_unit* u = this->get_unit_by_id(id);
    hmmc_event he = hmmc_event(UNIT_EVENT_WAGON);
    if(u->is_ranger() && this->on_callback(&he, 0)) return;
    u->rem_shoots(count);
}

int battle_group::get_attack(int id){
    hmmc_unit* u = this->get_unit_by_id(id);
    if(u == NULL) return 0;
    return u->cur_attack() + this->parent->get_hero_val_or_def(HERO_ATTR_ATTACK, 0);
}

int battle_group::get_defense(int id){
    hmmc_unit* u = this->get_unit_by_id(id);
    if(u == NULL) return 0;
    return u->cur_defense() + this->parent->get_hero_val_or_def(HERO_ATTR_DEFENSE, 0);
}

float battle_group::index_attack(int id){
    hmmc_unit* u = this->get_unit_by_id(id);
    if(u == NULL) return 0;
    return u->get_index_attack();
}

float battle_group::index_defense(int id){
    hmmc_unit* u = this->get_unit_by_id(id);
    if(u == NULL) return 0;
    return u->get_index_defense();
}

int battle_group::nonliving(int id){
    hmmc_unit* u = this->get_unit_by_id(id);
    if(u == NULL) return 0;
    return (u->nonliving()) ? id : 0;
}

float battle_group::get_sorcery(){
    if(this->parent->get_hero_val_or_def(HERO_ATTR_SORCERY, 0))
        return this->parent->get_hero_val_or_def(HERO_ATTR_LEVEL, 0) * this->difficult;
    return 0;
}

int battle_group::get_ballistic(){
    if(this->parent->get_hero_val_or_def(HERO_ATTR_BALLISTIC, 0))
        return this->parent->get_hero_val_or_def(HERO_ATTR_LEVEL, 0);
    return 0;
}

float battle_group::get_archery(){
    if(this->parent->get_hero_val_or_def(HERO_ATTR_ARCHERY, 0))
        return this->parent->get_hero_val_or_def(HERO_ATTR_LEVEL, 0) * this->difficult;
    return 0;
}

void battle_group::heal(int id, int val){
    hmmc_unit* u = this->get_unit_by_id(id);
    if(u == NULL) return;
    u->heal(val);
}

void battle_group::up_rage(int id, int val, int dead){
    hmmc_unit* u = this->get_unit_by_id(id);
    if(u == NULL) return;
    u->up_rage(val, dead);
}

float battle_group::get_difficult(){ return this->difficult; }

bool battle_group::is_shoot_retaliation(int id){
    hmmc_unit* u = this->get_unit_by_id(id);
    if(u == NULL) return false;
    return u->range_retaliation();
}

void battle_group::down_retaliated(int id){
    hmmc_unit* u = this->get_unit_by_id(id);
    if(u == NULL) return;
    u->down_retaliated();
}

int battle_group::get_heal(int id){
    hmmc_unit* u = this->get_unit_by_id(id);
    if(u != NULL) return u->get_heal();
    return this->parent->get_hero_val_or_def(HERO_SPELL_HEAL, 0);
}

int battle_group::get_clr(){
    //todo color from user faction
    return this->parent->get_hero_val_or_def(HERO_COLOR, '0');
}

float battle_group::get_positive(int id){
    hmmc_unit* u = this->get_unit_by_id(id);
    if(u != NULL) return u->get_index_positive();
    return this->hero_spell;
}

bool battle_group::is_machine(int id){
    hmmc_unit* u = this->get_unit_by_id(id);
    if(u != NULL) return u->is_machine();
    return false;
}

void battle_group::interaction(int at, int one, int two){
    hmmc_unit* utwo = this->get_unit_by_id(two);
    if(utwo == NULL) return;

    if(at & UNIT_FLAG_AFTER){
        if((at & UNIT_AFTER_REGENERATION)== UNIT_AFTER_REGENERATION){
            if(one == two){ //regeneration
                //todo calc regenaration
                float I6 = get_positive(one);
                int pos = this->calculate_positive(at, utwo->get_count(), I6);
                utwo->heal(pos);
            }
        }
        return;
    }
    if(utwo->nonliving()){
        return;
    }

    if((at & UNIT_ATTACK_HEAL) == UNIT_ATTACK_HEAL){
        if(utwo->is_dead()) return;
        float I6 = get_positive(one);
        if(!this->is_machine(one)) I6 += this->get_sorcery();
        int pos = this->calculate_positive(at, this->get_heal(one), I6);
        utwo->heal(pos);
        this->remove_shoots(one, 1);
    }else if((at & UNIT_ATTACK_RESURRECT) == UNIT_ATTACK_RESURRECT){
        // todo may be resurrect dead or

        this->remove_shoots(one, 1);
    }

/* version: 11 from 10 feb
       12: attack: &32 = Healing & 64 = Resurrect

       13: after:

       14: effects: &8 = Nonliving (ignore drain life and heal) &64 = Luck

       11 : shots count mana: healing or Resurrect
    */



    hmmc_unit* uone = this->get_unit_by_id(one);
    if(uone != NULL){
        //todo unit

    }
    //todo hero here
}

hmmc_unit* battle_group::get_next_unit(int id){
    int size = this->units.size();
    bool next = (id == 0);
    for(int i=0;i<size;i++){
        if(next){
            if(this->units[i] == NULL || this->units[i]->get_id() == 0) continue;
            return this->units[i];
        }
        next = (this->units[i]->get_id() == id);
    }
    return NULL;
}


int battle_group::calculate_positive(int at, int val, float I6){
    float I4 = (at & UNIT_FLAG_LUCK) ? 1 : (at & UNIT_FLAG_UNLUCK) ? 0.5f : 0;
    float res = val * (I4 + I6);
    return (int) res;
}

int battle_group::get_assign(int id){
    //todo защита юнита героем
    return 0;
}

void battle_group::add_unit(hmmc_unit* unit){
    this->units.push_back(unit);
}

void battle_group::rem_unit(int id){
    int size = this->units.size();
    for(int i=0;i<size;i++){
        if(this->units[i]->get_id() == id){
            this->units.erase(this->units.begin() + i);
            return;
        }
    }
}
