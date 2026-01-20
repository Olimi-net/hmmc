#include "hmmc_unit.h"

hmmc_unit::hmmc_unit(hmmc_callback *e, pair unit, int id, int angle){
    this->event = e;
    this->id = id;
    this->angle = angle;
    if(unit.value == 0 || unit.subval == 0){
        this->state = 0;
        this->id = 0;
        return;
    }

    this->state = 1;

    int unit_id = static_unit_get_id(unit.value);
    this->unit = pair_factory(unit.value, unit.subval, unit_id); // type | count | id

    //todo get дополнительные аттрибуты
    this->admh = quadra_factory(0, 0, 0, 0);
    this->cur_prms = pair_factory(0, 0, 0);




    // level | attack | defense | hp
    this->attr = quadra_factory(static_unit_get_value(unit_id, 1),
                                static_unit_get_value(unit_id, 2),
                                static_unit_get_value(unit_id, 3),
                                static_unit_get_value(unit_id, 5));

    this->cur_unit = pair_factory(unit.subval, this->attr->e4 + this->admh->e4, 0);

    this->damage = static_unit_get_damage(unit_id);
    this->speed = pair_factory(0, static_unit_get_value(unit_id, 6), 0);

    this->huge = static_unit_is_huge(unit.value);


    // 10 : morale group  &16 = some human &32 = bird &64 = horse &128 = wizards
    this->relation = static_unit_get_value(unit_id, 10);

   /* version: 11 from 10 feb
       12: attack &1 = ranged &2 = No enemy retaliation &4 = Jousting &8 = Drain life &16 = Rage
            &32 = Healing & 64 = Resurrect &128 = Round attack &256 = lightning &384 = (128|256) fire ball

       13: after &1 = Strike and return &2 = Double attack &4 = Double shooting &8 = Regeneration &16 = Mutilate
            &32 = Root

       14: effects &1 = Flying &2 = Swimming &4 = Positive Morale &8 = Nonliving (ignore drain life and heal)
            &16 = Unlimited retaliations &32 = Range retaliations &64 = Luck &128 = No melee penalty

       11 : shots count if = ranged or = magic: healing or Resurrect or fire ball or Drain life or lightning
    */
    this->prms = quadra_factory(static_unit_get_value(unit_id, 12),
                                static_unit_get_value(unit_id, 13),
                                static_unit_get_value(unit_id, 14),
                                static_unit_get_value(unit_id, 11));
    hmmc_event he = hmmc_event(UNIT_EVENT_HERO_LEVEL);
    int hlvl = this->event-> on_callback(&he, this->id);
    he.id = UNIT_EVENT_UNIT_SPECIALITY;
    this->spec = this->event->on_callback(&he, this->id) == this->get_type();
    this->index_attack = this->calc_attack(hlvl, this->spec);
    this->index_defense = this->calc_defense(hlvl, this->spec);
    this->index_positive = (this->spec) ? 0.01f * hlvl : 0;
    this->refresh_retalation();
}

hmmc_unit::~hmmc_unit(){
    pair_free(this->unit);  // type | count | id
    quadra_free(this->attr); // level | attack | defense | hp
    pair_free(this->damage); // min - max
    pair_free(this->speed); // current | max
    quadra_free(this->prms);
    quadra_free(this->admh);
    pair_free(this->cur_unit);
}

//it is absolute position
pair hmmc_unit::get_pos(){

    pair p = {this->x.value, this->y.value, this->get_angle()};
    return p;
}
void hmmc_unit::set_pos(int x, int y){
    this->x.state = this->x.value = this->x.subval = x;
    this->y.state = this->y.value = this->y.subval = y;
}

void hmmc_unit::refresh_retalation(){
    // if((this->prms->e3 & 16) == 0) // &16 = Unlimited retaliations
    this->cur_prms->value = 1; //todo if have more 1 retalation
}

void hmmc_unit::set_huge() { this->huge = true; }
void hmmc_unit::set_hp(int hp) { this->admh->e3 = hp; cur_unit->subval = hp; }
bool hmmc_unit::is_big(){return this->huge;}
int hmmc_unit::get_id()     {return this->id;}
int hmmc_unit::get_morale() {return this->relation;}
int hmmc_unit::get_type()   {return this->unit->value; }
bool hmmc_unit::is_dead()   {return this->cur_unit->state == 1 || this->cur_unit->value < 1; }
int hmmc_unit::get_level()  {return this->attr->e1; }
bool hmmc_unit::is_ranger() {return (this->prms->e1 & 1) == 1; }
bool hmmc_unit::is_mage()   {return (this->prms->e1 & 264) != 0; }
int hmmc_unit::max_hp()     {return this->admh->e3 + this->attr->e4; }
int hmmc_unit::cur_hp()     {return this->cur_unit->subval; }
int hmmc_unit::max_speed()  {return this->speed->subval; }
int hmmc_unit::cur_speed()  {return this->speed->value; }
bool hmmc_unit::is_fly()    {return (this->prms->e3 & 1) == 1; }
bool hmmc_unit::can_swim()  {return (this->prms->e3 & 2) == 2; }
int hmmc_unit::get_angle()  {return this->is_dead() ? AN_DEAD : this->angle; }

void hmmc_unit::refresh(){
    int spd = max_speed(); //todo + speed bonus
    if(this->speed->state > 0){
        this->speed->state--;
        if(this->is_fly()){
            spd = 99;
        }else if(this->is_big()){
            spd <<= 1;
        }else{
            spd = 1;
        }
    }
    this->speed->value = spd;

    refresh_retalation();
}

int hmmc_unit::on_call_event(int e, int a){
    hmmc_event he = hmmc_event(e);
    return this->event->on_callback(&he, a);
}

int hmmc_unit::get_type_attack(){

    if(this->get_type() == WAGON) return -1;

    if(this->get_type() == TENT){
        //todo if can resurrect // UNIT_ATTACK_RESURRECT
        return (this->prms->e4 < 1) ? -1 : UNIT_ATTACK_HEAL;
    }
    if(this->get_type() == BALLISTA){
        //todo if fire ballista // UNIT_ATTACK_FIREBALL
        return (this->prms->e4 < 1) ? -1 : UNIT_FLAG_RANGER;
    }

    int at = 0;

    if(this->is_ranger()){
        at = UNIT_FLAG_RANGER;
        if(this->prms->e3 & 128) at |= UNIT_ATTACK_NO_MELEE_PENALTY;
        if (on_call_event(UNIT_EVENT_NEAR_ENEMY, this->id)){
            at |= UNIT_FLAG_MELEE;
        }else if(this->prms->e4 < 1 && on_call_event(UNIT_EVENT_WAGON, this->id) == 0){ //shots is low
            at |= UNIT_FLAG_MELEE;
        }else{
            if(this->prms->e1 & 8) at |= UNIT_ATTACK_RAGE;
            if(this->prms->e1 & 128) at |= UNIT_ATTACK_ROUND;
        }
    }else{
        if(this->is_mage()){
            at = UNIT_FLAG_MAGIC;
            if(this->prms->e3 & 128) at |= UNIT_ATTACK_NO_MELEE_PENALTY;
            if(this->prms->e4 < 1){ //mana is low
                at |= UNIT_FLAG_MELEE;
            }else{
                int magic = this->prms->e1 & 392;
                if(magic == 8) at |= UNIT_ATTACK_DRAIN_LIFE;
                if(magic == 256) at |= UNIT_ATTACK_LIGHTNING;
                if(magic == 384) at |= UNIT_ATTACK_FIREBALL;
            }
        }else{ // melee here
            at |= UNIT_FLAG_MELEE;
            if(this->prms->e1 & 128) at |= UNIT_ATTACK_ROUND;
            if(this->prms->e1 & 4) at |= UNIT_ATTACK_JOUSTING;
            if(this->prms->e1 & 8) at |= UNIT_ATTACK_RAGE;
        }

        if(this->prms->e4 > 0){ //has mana
        // buff and debuff and support
            if(this->prms->e1 & 32) at |= UNIT_ATTACK_HEAL;
            if(this->prms->e1 & 64) at |= UNIT_ATTACK_RESURRECT;
        }
    }

    if(this->prms->e1 & 2) at |= UNIT_FLAG_NO_RETALATION;
    return at;
}

// -2 = less morale | ≥ 0  for melee - show flags
int hmmc_unit::before_step(){
    int morale = (this->nonliving()) ? 0 : this->on_call_event(UNIT_EVENT_MORALE, this->id);
    if(morale < 0){
        if(this->prms->e3 & 4)
            morale = 1; // positive moral
        else
            return -2;
    }
    int at = this->get_type_attack();
    if(at < 1) return at;
    if(morale > 2) at |= UNIT_ATTACK_MORALE;
    return at | this->calc_luck();
}



int hmmc_unit::after_step(){
    if(this->get_type() == WAGON) return -1;
    if(this->get_type() == TENT){
        //todo if can resurrect // UNIT_ATTACK_RESURRECT
        //return (this->prms->e4 < 1) ? -1 : UNIT_ATTACK_HEAL;
        return -1;
    }
    if(this->get_type() == BALLISTA){
        //todo if fire ballista // UNIT_ATTACK_FIREBALL
        //return (this->prms->e4 < 1) ? -1 : UNIT_ATTACK_RANGER;
        return -1;
    }


    bool rage = false;
    //todo calculate rage for double attack

    int at = UNIT_FLAG_AFTER;


    if(this->is_ranger()){
        if((this->prms->e2 & 4) == 0 && !rage) return -1;
        if (on_call_event(UNIT_EVENT_NEAR_ENEMY, this->id)){
            return -1;
        }else if(this->prms->e4 < 1 && on_call_event(UNIT_EVENT_WAGON, this->id) == 0){ //shots is low
            return -1;
        }else{
            if(this->prms->e1 & 8) at |= UNIT_ATTACK_RAGE;
            if(this->prms->e1 & 128) at |= UNIT_ATTACK_ROUND;
            if(this->prms->e1 & 2) at |= UNIT_FLAG_NO_RETALATION;
            at |= UNIT_FLAG_RANGER;
        }
        return at;
    }
    if(this->is_mage()){ return -1; }

    //after &1 = Strike and return &2 = Double attack &4 = Double shooting &8 = Regeneration &16 = Mutilate &32 = Root
    if(this->prms->e2 & 2) at |= UNIT_FLAG_MELEE;
    if(rage) at |= UNIT_FLAG_MELEE; // rage as double attack
    if(this->prms->e1 & 128) at |= UNIT_ATTACK_ROUND;
    if(this->prms->e1 & 2) at |= UNIT_FLAG_NO_RETALATION;
    if(this->prms->e1 & 8) at |= UNIT_ATTACK_RAGE;
    if(this->prms->e2 & 1) at |= UNIT_AFTER_RETURNS;
    if(this->prms->e2 & 8) at |= UNIT_AFTER_REGENERATION;
    if(this->prms->e2 & 16) at |= UNIT_AFTER_MUTILATE;
    if(this->prms->e2 & 32) at |= UNIT_AFTER_ROOT;
    at |= this->calc_luck();
    return at;
}

int hmmc_unit::calc_luck(){
    int luck = math_rand_sum_2_4(2, 1, (this->prms->e3 & 64) ? 1 : 0, -1);
    luck += (on_call_event(UNIT_EVENT_HERO_LUCK, this->id) ? 1 : 0);

    if (luck < 0){
        return UNIT_FLAG_UNLUCK;
    }else if(luck > 2){
        return UNIT_FLAG_LUCK;
    }
    return 0;
}

float hmmc_unit::calc_attack(int hLvl, bool spec){
    int uLvl = this->get_level();
	float I2 = on_call_event(UNIT_EVENT_ATTACK_DIFICULT, this->id) * 0.01f;
	float I3 = 0.03f * uLvl * (hLvl > 1 ? hLvl : 1);
	float I5 = (spec) ? I2 * hLvl : 0;
	return 1 + I2 + I3 + I5;
}

float hmmc_unit::calc_defense(int hLvl, bool spec){
	float R2 = on_call_event(UNIT_EVENT_DEFENSE_DIFICULT, this->id) * 0.01f;
	float R3 = (spec) ? 0.05f * R2 * hLvl : 0;
	return 1 - R2 - R3;
}

int hmmc_unit::calc_assign(bool enemy_near){
    int at = this->get_type_attack();
    if(enemy_near){
        if(this->is_ranger()) return at | UNIT_FLAG_MELEE;
        return at;
    }


    if(at & UNIT_FLAG_MELEE)
        return 0;

    if(enemy_near){
        if(this->is_ranger()){
            return UNIT_FLAG_RANGER | UNIT_FLAG_MELEE | ((this->prms->e3 & 128) ? UNIT_ATTACK_NO_MELEE_PENALTY : 0);
        }

        if(this->prms->e1 & 264){ // &8 = Drain life &256 = lightning &384 = (128|256) fire ball
            return (this->prms->e3 & 128) ? 0 : -1;
        }

        int melee = this->calc_luck();
        if(this->prms->e1 & 128) melee |= UNIT_ATTACK_ROUND;
        if(this->prms->e1 & 8) melee |= UNIT_ATTACK_RAGE;
        return melee;
    }

    if((this->prms->e1 & 1) && (this->prms->e3 & 32)){ //&1 = ranged  && &32 = Range retaliations
        int shoots = this->prms->e4;
        if(shoots == 0) shoots = on_call_event(UNIT_EVENT_WAGON, this->id);
        int near = on_call_event(UNIT_EVENT_NEAR_ENEMY, this->id);
        if(shoots && near == 0){
            int flag = UNIT_FLAG_RANGER | this->calc_luck();
            if(this->prms->e1 & 8) flag |= UNIT_ATTACK_RAGE;
            return flag;
        }
        return -2;
    }

    if(this->prms->e1 & 264){ // &8 = Drain life &256 = lightning &384 = (128|256) fire ball
        if ((this->prms->e1 & 392) == 256 && this->prms->e4 > 0){
            return UNIT_ATTACK_LIGHTNING;
        }
    }
    return -2;
}

int hmmc_unit::calc_retaliation(bool enemy_near){
    int count = (this->prms->e3 & 16) ? 1 : this->cur_prms->value; // &16 = Unlimited retaliations
    if(count < 1) return -2;

    if(enemy_near){
        return this->calc_assign(enemy_near) | UNIT_FLAG_MELEE;
    }

    if(this->is_ranger() || this->is_mage()){
        if(this->prms->e3 & 32){ // &32 = Range retaliations
            int at = this->calc_assign(enemy_near);
            if(at & UNIT_FLAG_MELEE) return -2;
            return at;
        }
    }
    return -2;
}

void hmmc_unit::rem_shoots(int count){
    this->prms->e4 -= count;
    if (this->prms->e4 < 0)
        this->prms->e4 = 0;
}

int hmmc_unit::cur_attack()    {return this->attr->e2 +  this->admh->e1; }
int hmmc_unit::cur_defense()   {return this->attr->e3 + this->admh->e2; }
int hmmc_unit::cur_min()       {return this->damage->value; }
int hmmc_unit::cur_max()       {return this->damage->subval + this->admh->e3; }
float hmmc_unit::get_index_attack()  {return this->index_attack; }
float hmmc_unit::get_index_defense() {return this->index_defense; }
bool hmmc_unit::nonliving() { return (this->prms->e3 & 8) == 8 || this->is_machine(); }
int hmmc_unit::get_count()  { return this->cur_unit->value; }

int hmmc_unit::set_damage(int dmg){
    int hp_one = this->admh->e3 + this->attr->e4;
    int dead = dmg / hp_one;
    int d2 = dmg - hp_one * dead;
    if(d2 > this->cur_unit->subval){
        dead++;
        this->cur_unit->subval = hp_one + this->cur_unit->subval - d2;
    }else{
        this->cur_unit->subval -= d2;
    }
    this->cur_unit->value -= dead;
    if(this->cur_unit->value < 0) {
        this->cur_unit->value = 0;
        this->cur_unit->subval = 0;
        this->cur_unit->state = 1;
    }

    if(this->is_dead()){
        on_call_event(UNIT_EVENT_DEAD, this->id);
    }

    return  dead;
}

void hmmc_unit::heal(int val){
    int hp_one = this->max_hp();
    this->cur_unit->subval += val;
    if(this->cur_unit->subval > hp_one){
        this->cur_unit->subval = hp_one;
    }
    //todo resurrect here
}

void hmmc_unit::up_rage(int val, int dead){
    this->cur_prms->subval += val;
    this->cur_prms->state = dead;
    if(this->cur_prms->subval > this->cur_prms->state)
        this->cur_prms->subval = this->cur_prms->state;
}

int hmmc_unit::get_rage(){ return this->cur_prms->subval; }
bool hmmc_unit::range_retaliation(){ return (this->prms->e3 & 32) != 0; }
int hmmc_unit::get_heal(){ return (this->get_type() == TENT) ? 50 : this->cur_unit->value; }
float hmmc_unit::get_index_positive(){ return this->index_positive; }
bool hmmc_unit::is_machine() { return this->get_type() == TENT || this->get_type() == BALLISTA || this->get_type() == WAGON; }

void hmmc_unit::down_retaliated(){
    if(this->prms->e3 & 16) return; //  = Unlimited retaliations
    this->cur_prms->value--;
}

int hmmc_unit::get_fast(){
    //3 - reserved
    int a = 0;
    if(this->huge) a |= 4;
    //todo fly swim any?
    return a;
}

void hmmc_unit::set_debaf(int arg){
    ///todo debaf for units: UNIT_AFTER_MUTILATE or UNIT_AFTER_ROOT
}

pair hmmc_unit::get_coord(){
    pair p = {this->x.value / 5, this->y.value / 3, 0};
    return p;
}

int hmmc_unit::move(int x, int y){
    if(this->x.value == x && this->y.value == y){
        step_low(1);
        return 0;
    }

    ///todo поворачивать юнита к врагу
    ///сейчас поворот только при движении
    this->next_angle(this->x.value - x);

    if(this->speed->value < 1){
        return -1;
    }


    if(this->is_fly()){
        ///todo более прямой путь для летающих юнитов
    }

    int mx = 0;
    if(this->x.value > x){
        mx = -1;
    }else if(this->x.value < x){
        mx = 1;
    }
    int my = 0;
    if(this->y.value > y){
        my = -1;
    }else if(this->y.value < y){
        my = 1;
    }

    this->x.value += mx;
    this->y.value += my;
    return 1;
}

int hmmc_unit::step_low(int val){
    this->speed->value--;
    if(this->speed->value < 1){
        this->speed->value = 0;
        return 0;
    }
    return this->speed->value;
}

void hmmc_unit::next_angle(int a){
    int n = this->get_angle();
    if(n == AN_DEAD) return;

    int na = -1;

    if(a > 0){
        na = AN_LEFT;
    }
    if(a < 0){
        na = AN_RIGHT;
    }

    if(na < 0 || na == AN_LEFT){
        if(n == AN_MOVE_L || n == AN_ATACK_L){
            this->angle = AN_LEFT;
            return;
        }
        if(n == AN_LEFT){
            this->angle = AN_MOVE_L;
            return;
        }
        if(na == AN_LEFT){
            this->angle = AN_LEFT;
            return;
        }
    }

    if(na < 0 || na == AN_RIGHT){
        if(n == AN_RIGHT){
            this->angle = AN_MOVE_R;
            return;
        }
        if(n == AN_MOVE_R || n == AN_ATACK_R){
            this->angle = AN_RIGHT;
            return;
        }
        if(na == AN_RIGHT){
            this->angle = AN_RIGHT;
            return;
        }
    }
}
