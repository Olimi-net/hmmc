#include "battle_field.h"
#include "battle_const.h"

static int battle_field_city[10]={5, 6, 7, 17, 18, 28, 29, 51, 52, 53};


battle_field::battle_field(int cols, int rows, group* gl, group* gr, float dif, hmmc_callback* e):hmmc_callback(){
    this->field = new hmmc_path(cols, rows);
    this->battle = e;
    this->left = new battle_group(gl, LEFT_GROUP, dif, this);
    this->right = new battle_group(gr, RIGHT_GROUP, dif, this);
    this->queue = new battle_queue();
    this->pulse = new battle_step();
    this->run = new battle_action(e, this);
}

battle_field::~battle_field(){
    for(int i = city_units.size() - 1; i >= 0; i--){
        delete city_units[i];
    }

    delete this->field;
    delete this->run;
    delete this->pulse;
    delete this->queue;
    delete this->left;
    delete this->right;
}

int battle_field::on_callback(hmmc_event* e, int a){
    if(e->id == AM_DROP){
        this->pulse->state = 2049;
        //animation was drop: see battle_animation->drop()
        return 0;
    }

    if(e->id == AC_FIRE){
        pair p = this->get_pos(a);
        if(p.state == a){
            this->fair_round(e->p.value, p.value, p.subval, UNIT_ATTACK_FIREBALL);
        }
        return 0;
    }
    if(e->id == AC_STUN){
        this->queue->stun(this->queue->get_item(a));
        return 0;
    }
    if(e->id == AC_ROUND){
        hmmc_queue_item* q = this->queue->get_item(a);
        this->round_attack(e->p.value, q->id, UNIT_ATTACK_ROUND, q->fac, q->unit->is_big());
        return 0;
    }

    if(e->id == GROUP_EVENT){
        if(a == GE_REVALIDATE_QUEUE){
            ///todo dead or respawn
        }
    }

    return 0;
}

bool battle_field::unit_is_valid(int flag, hmmc_unit* u){
    if(u->is_big()){
        if((flag & 2)==0) return false;
    }else{
        if((flag & 1)==0) return false;
    }

    //melee, ranger, mage

    if(u->nonliving()){
        if((flag & 4)==0) return false;
    }

    if(u->is_machine()){
        if((flag & 8)==0) return false;
    }

    return true;
}

//flag: small = 1, big = 2, nonliving = 4, machine = 8(12), hero's = 16
void battle_field::init(int flag){
    if(flag & 16){
        if(left->hero_id()) this->queue->add(new hmmc_queue_item(left, NULL, 1));
        if(right->hero_id()) this->queue->add(new hmmc_queue_item(right, NULL, 2));
    }

    int uid = 0;
    while(true){
        hmmc_unit* u = this->left->get_next_unit(uid);
        if(u == NULL) break;
        if(u->get_id() == 0) continue;
        uid = u->get_id();
        if(!this->unit_is_valid(flag, u)) continue;
        //this->set_unit(u->get_id(), u->get_coord(), u->get_type(), 1 | u->get_fast());
        this->queue->add(new hmmc_queue_item(this->left, u, 1));
    }

    uid = 0;
    while(true){
        hmmc_unit* u = this->right->get_next_unit(uid);
        if(u == NULL) break;
        if(u->get_id() == 0) continue;
        uid = u->get_id();
        if(!this->unit_is_valid(flag, u)) continue;
        //this->set_unit(u->get_id(), u->get_coord(), u->get_type(), 2 | u->get_fast());
        this->queue->add(new hmmc_queue_item(this->right, u, 2));
    }
}

void battle_field::add_city_unit(int unit_type, int x, int y){
    pair p = pair();
    p.value = unit_type;
    if(unit_type & UNIT){
        int lvl = unit_type & (LEVEL_1 | LEVEL_2 | LEVEL_3);
        p.subval = (6 - lvl) * 10;
    }else{
        p.subval = 1;
    }
    hmmc_unit* u = new hmmc_unit(this, p, 80 + city_units.size(), AN_RIGHT);
    hmmc_queue_item *q;
    if(unit_type & UNIT) {
        u->set_pos(5 * 5, -3);
        q = new hmmc_queue_item(left, u, 1);
        q->is_city_archer = true;
        q->city_unit = unit_type & (LEVEL_1 | LEVEL_2 | LEVEL_3);
        int cols = field->width;
        for(int i=0;i<10;i++){
            int n = battle_field_city[i];
            field->set_pos(n % cols, n / cols, 999);
        }
    }else if(unit_type == BTL_UNIT_CATAPULT){
        q = new hmmc_queue_item(right, u, 2);
        u->set_hp(300);
        u->set_pos(10 * 5, 5 * 3);
        q->is_city_archer = true;
        q->city_unit = unit_type;
        u->set_huge();
    }else{
        if(unit_type == BTL_UNIT_GATE) u->set_pos(4 * 5, 3 * 3);
        if(unit_type == BTL_UNIT_WALL) u->set_pos(3 * 5, 5 * 3);
        u->set_huge();
        u->set_hp(1000);
        q = new hmmc_queue_item(left, u, 1);
        q->city_unit = unit_type;
    }
    this->queue->add(q);
    city_units.push_back(u);
}

void battle_field::update_pos_for_unit(int id){
    hmmc_queue_item* q = this->get_item(id);
    pair p = q->unit->get_coord();
    this->field->rem_pos_by_id(id);
    this->set_unit(id, p, q->unit->get_type(), q->fac | q->unit->get_fast());
}

void battle_field::start_line(int offsetL, int offsetR){
    this->left->start_line(offsetL); // usually 0
    this->right->start_line(offsetR);

    // update all position
    int size = this->queue->get_unit_count();
    for(int i = 0; i < size; i++){
        hmmc_queue_item* q = this->queue->get_item_by_index(i);
        if(q->city_unit || q->is_hero()) continue;
        pair p = q->unit->get_coord();
        this->field->rem_pos_by_id(q->id);
        this->set_unit(q->id, p, q->unit->get_type(), q->fac | q->unit->get_fast());
    }
}

void battle_field::set_unit(int id, pair pos, int t, int p){
    if(!this->field->is_pos(pos.value, pos.subval)) return;
    int a = id;
    if(p & 4) a |= 1024; //big unit here
    this->field->set_pos(pos.value, pos.subval, a);
}

int battle_field::get_id_by_pos(int x, int y){
    if(!this->field->is_pos(x, y)) return 0;
    int id = this->field->get_pos(x, y);
    if(id != 0) return id & 1023;
    if(this->field->is_pos(x-1, y)){
        id = this->field->get_pos(x, y);
        if(id & 1024) return id & 1023;
    }
    return 0;
}

int battle_field::round(int x, int y, int a){
    /*  1 4 7
        2 5 8 10
        3 6 9     */

    if(a == 3)return this->get_id_by_pos(x-1, y+1);
    if(a == 6)return this->get_id_by_pos(x, y+1);
    if(a == 9)return this->get_id_by_pos(x+1, y+1);

    if(a == 2)return this->get_id_by_pos(x-1, y);
    if(a == 5)return this->get_id_by_pos(x, y);
    if(a == 8)return this->get_id_by_pos(x+1, y);

    if(a == 1)return this->get_id_by_pos(x-1, y-1);
    if(a == 4)return this->get_id_by_pos(x, y-1);
    if(a == 7)return this->get_id_by_pos(x+1, y-1);
    if(a == 10)return this->get_id_by_pos(x+2, y);
    return -2;
}

int battle_field::round_by_id(int id, int attr){
    int p = this->field->found_value(id);
    if(p < 0) return -3;
    int x = p >> 4;
    int y = p & 15;
    return this->round(x, y, attr);
}

bool battle_field::is_near(int f, int s){
    int p = this->field->found_value(f);
    if(p < 0) return false;
    int x = p >> 4;
    int y = p & 15;

    for(int i=2;i< 7; i+=2){
        if(this->round(x, y, i) == s) return true;
    }
    if(this->field->get_pos(x, y) & 1024){
        if(this->round(x, y, 9)==s) return true;
        if(this->round(x, y, 7)==s) return true;
        if(this->round(x, y, 10)==s) return true;
    }else{
        if(this->round(x, y, 8)==s) return true;
    }
    return false;
}


int battle_field::get_cols() {return this->field->width; }

int battle_field::calc_far(int f, int s){
    int pf = this->field->found_value(f);
    if(pf < 0) return 1024;
    int ps = this->field->found_value(s);
    if(ps < 0) return 1024;
    int x1 = pf >> 4;
    int y1 = pf & 15;
    int a1 =  this->field->get_pos(x1, y1) >> 10;
    int x2 = ps >> 4;
    int y2 = ps & 15;
    int a2 =  this->field->get_pos(x2, y2) >> 10;

    if (y1 == y2) return ((x1 < x2) ? abs(x2 - (x1 + a1)) : abs(x1 - (x2 + a2))) - 1;
    if (x1 <= x2 + a2 and x2 <= x1 + a1) return abs(y1 - y2) - 1;
    return abs(y1 - y2) + ((x1 < x2) ? abs(x2 - x1 - a1) : abs(x1 - x2 - a2)) - 1;
}

pair battle_field::get_pos(int id){
    int p = this->field->found_value(id);
    int x = (p < 0) ? -1 : (p >> 4);
    int y = (p < 0) ? -1 : (p & 15);
    pair e = {x, y, (p < 0) ? 0 : id};
    return e;
}

int battle_field::get_city_unit(int f){
    quadra q;
    q.e4 = 0;
    int size = city_units.size();
    for(int i = 0; i < size; i++){
        int id = city_units[i]->get_id();
        if(i == 0) q.e1 = id;
        if(i == 1) q.e2 = id;
        if(i == 2) q.e3 = id;
        if(i == 3) q.e4 = id;
    }

    quadra q_shuffle = math_shuffle_quadra(q);

    {
        hmmc_queue_item *item = this->queue->get_item(q_shuffle.e1);
        if(item != NULL && item->fac != f){
            return item->id;
        }
    }
    {
        hmmc_queue_item *item = this->queue->get_item(q_shuffle.e2);
        if(item != NULL && item->fac != f){
            return item->id;
        }
    }
    {
        hmmc_queue_item *item = this->queue->get_item(q_shuffle.e3);
        if(item != NULL && item->fac != f){
            return item->id;
        }
    }
    {
        hmmc_queue_item *item = this->queue->get_item(q_shuffle.e4);
        if(item != NULL && item->fac != f){
            return item->id;
        }
    }
    return 0;
}

int battle_field::get_simle_unit(int at, hmmc_queue_item* one){
    int heal = ((at & UNIT_ATTACK_HEAL) == UNIT_ATTACK_HEAL) ? one->group->get_heal(one->id) : 0;
    int damage = one->dmg_min();

    pair target = {0, 0, 0};
    pair subtarget = {0, 0, 0};

    pair p1 = one->unit->get_coord();

    int uid = one->id;
    for(int i = 0; i < 12; i++){
        hmmc_queue_item* q = this->next_unit(at, uid, one->fac, one->id);
        if(q == NULL) break;
        int total = 0;
        int hp = 0;
        uid = q->id;
        if(q->dmg_max() > damage) total += 1; // strong unit

        if(q->unit->is_dead()){
            if((at & UNIT_ATTACK_RESURRECT) == UNIT_ATTACK_RESURRECT) total += (q->unit->is_big() ? 5 : 3); // more units
        }else if(q->fac == one->fac){
            int qat = q->unit->get_type_attack();
            if((qat & UNIT_ATTACK_RESURRECT) == UNIT_ATTACK_RESURRECT){
                total += 1; // supporter
            }
            if((qat & UNIT_ATTACK_HEAL) == UNIT_ATTACK_HEAL){
                if(q->unit->max_hp() > q->unit->cur_hp()){
                    hp = q->unit->max_hp() - q->unit->cur_hp();
                    if(hp <= heal) total += (q->unit->is_big() ? 3 : 2); // actual heal
                }else{
                    continue;
                }
            }
        }else{
            if(q->unit->is_ranger() || q->unit->is_mage()) total += 1; //ranger
            hp = q->unit->cur_hp();
            if(damage > hp) total += (q->unit->is_big() ? 5 : 3); // kill

            if(at & UNIT_FLAG_MELEE){
                pair p2 = q->unit->get_coord();
                if(this->field->is_has_step(p2.value, p2.subval)) continue;
                int res = this->field->gen_path(p1, p2, one->unit->is_fly(), one->unit->can_swim());
                if(res < 0) continue; // path not found
                total += q->unit->max_speed() - res;
            }
        }
        if(total == 0) continue;
        if(target.value == 0){
            target.value = q->id;
            target.state = total;
        }else if(target.state <= total){
            subtarget.state = subtarget.subval;
            subtarget.subval = subtarget.value;
            subtarget.value = target.subval;
            target.subval = target.value;
            target.state = total;
        }
    }

    int n = math_rand_five(target.value, target.subval, subtarget.state, subtarget.subval, subtarget.value);
    return (n == 0) ? target.value : n;
}

int battle_field::read_magic_book(hmmc_queue_item* hero){
    //todo read magic book
    int at = UNIT_FLAG_RANGER;

    //todo
    //if(this->ai.state == one->fac) at |= UNIT_ATTACK_RESURRECT | UNIT_ATTACK_HEAL;

    return at;
}

int battle_field::get_unit_for_hero(hmmc_queue_item* hero){
    //todo учесть заклинания
    //get any enemy unit not machine
    int at = read_magic_book(hero);
    int uid = this->pulse->defender;
    for(int i = 0; i < 2; i++){
        hmmc_queue_item* q = this->next_unit(at, uid, hero->fac, this->pulse->defender);
        if(q == NULL) break;
        this->pulse->at = at;
        return q->id;
    }
    return uid;
}

bool battle_field::select_unit(){
    if(this->pulse->attacker < 1) return false;
    hmmc_queue_item* one = this->queue->get_item(this->pulse->attacker);
    int fac = one->fac;
    hmmc_queue_item* q = this->next_unit(this->pulse->at, this->pulse->defender, fac, this->pulse->attacker);
    if(q==NULL){
        this->pulse->defender = 0;
        //todo end?
        if(this->queue->is_finish()){
            //todo show finish or fail
            return false; ///todo
        }
        return false;
    }
    this->pulse->defender = q->id;

    if(this->pulse->at & UNIT_FLAG_MELEE){
        pair p2 = q->unit->get_coord();
        if(!this->field->is_has_step(p2.value, p2.subval)){
            this->field->gen_path(one->unit->get_coord(), p2, one->unit->is_fly(), one->unit->can_swim());
        }
    }
    return true; // friendly or enemy not found
}

hmmc_queue_item* battle_field::next_unit(int at, int id, int fac, int skip_id){
    int uid = id;
    for(int i = 0; i < 12; i++){
        hmmc_queue_item* q = this->queue->get_next_unit(uid);
        if(q == NULL) return NULL;
        uid = q->id;
        if(q->id == skip_id) continue;
        if(q->unit->is_dead()){
            if ((at & UNIT_ATTACK_RESURRECT)== UNIT_ATTACK_RESURRECT) return q;
            continue;
        }
        if(q->fac == fac){
            if((at & UNIT_ATTACK_HEAL) == UNIT_ATTACK_HEAL) return q;
            // todo any buff also
            continue;
        }
        if(q->fac != fac) return q;
    }
    return NULL;
}

bool battle_field::valid_action(int flag){
    if(flag == BATTLE_ACTION){
        bool res = this->run->action(this->pulse->at, this->queue->get_item(this->pulse->attacker), this->queue->get_item(this->pulse->defender));
        if(this->pulse->is_at(UNIT_ATTACK_MORALE)){
            this->queue->morale(this->pulse->attacker);
            hmmc_event he = hmmc_event(BA_UP_MORALE);
            this->battle->on_callback(&he, this->pulse->attacker);
        }
        return res;
    }
    if(flag == BATTLE_ASSIGN){
        hmmc_queue_item* two = this->queue->get_item(this->pulse->defender);
        if(two != nullptr && two->assign){
            hmmc_queue_item* one = this->queue->get_item(this->pulse->attacker);
            hmmc_queue_item* assign = this->queue->get_item(two->assign);
            int at = assign->get_assign(this->is_near(assign->id, one->id));
            return this->run->assign_or_retalation(at, assign, one, BATTLE_ASSIGN);
        }
        return false;
    }
    if(flag == BATTLE_RETALATION){
        if(this->pulse->is_at(UNIT_FLAG_NO_RETALATION)) return false;
        hmmc_queue_item* one = this->queue->get_item(this->pulse->attacker);
        if(one->is_hero()) return false;
        hmmc_queue_item* two = this->queue->get_item(this->pulse->defender);
        int at = two->unit->calc_retaliation(this->is_near(one->id, two->id));
        return this->run->assign_or_retalation(at, two, one, BATTLE_RETALATION);
    }

    if(flag == BATTLE_AFTER){
        hmmc_queue_item* one = this->queue->get_item(this->pulse->attacker);
        if(one->is_hero() || one->unit->is_dead()) {
            this->pulse->at = -2;
            return false;
        }
        this->pulse->at = one->unit->after_step();
        return this->run->after_action(this->pulse->at, one, this->queue->get_item(this->pulse->defender));
    }

    if(flag == MOVE_HOME){
        if(this->pulse->is_at(UNIT_AFTER_RETURNS)){
            //todo return
            this->run->anime(BA_MOVE, 0, this->queue->get_item(this->pulse->attacker), 0, MOVE_HOME);
            return true;
        }
        return false;
    }

    if(flag == REGENERATION){
        if(pulse->is_at(UNIT_AFTER_REGENERATION)){
            hmmc_queue_item* one = this->queue->get_item(this->pulse->attacker);
            this->run->anime(BA_POSITIVE_MAGIC, UNIT_AFTER_REGENERATION, one, one, BATTLE_INTERACTION);
            return true;
        }
        return false;
    }
    return false;
}

void battle_field::set_dmg_id(int id, int dmg, int t, int f){
    hmmc_queue_item* u = this->queue->get_item(id);
    if(u == NULL || u->is_hero() || u->unit->is_dead() || f == u->fac) return;
    u->set_damage(dmg);
}

void battle_field::round_attack(int dmg, int id, int t, int f, bool big){
    if(dmg < 1) return;
    for(int i=2;i< 7; i+=2)
        this->set_dmg_id(this->round_by_id(id, i), dmg, t, f);

    if(big){
        this->set_dmg_id(this->round_by_id(id, 9), dmg, t, f);
        this->set_dmg_id(this->round_by_id(id, 7), dmg, t, f);
        this->set_dmg_id(this->round_by_id(id, 10), dmg, t, f);
    }else{
        this->set_dmg_id(this->round_by_id(id, 8), dmg, t, f);
    }
}

void battle_field::fair_round(int dmg, int x, int y, int t){
    if(dmg < 1) return;
    //todo magic arrow
    //fire ball
    int d2 = dmg >> 1;
    if (d2 < 1) return;
    int d4 = dmg >> 2;

    for(int i = 1; i < 10; i++){
        int d = (i & 1) ? d4 : d2;
        if(i == 5) d = dmg;
        if(d < 1) continue;
        this->set_dmg_id(this->round(x, y, i), d, t, 0);
    }
}

/**
    return:
        -1 = reset pulse;
        1 = waiting;
        2 = keyboard on;
        4 = finish;
        16 = select hero;


*/
int battle_field::resolve_pulse(int user_faction){
    if(this->pulse->attacker < 1) this->pulse->state = -1;
    if(this->pulse->state == 0) { return 2; }

    if(this->pulse->state < 0){
        hmmc_queue_item* one = this->queue->pop();
        if(one->is_pass()){
            return 0;
        }
        if(!one->is_hero()) one->unit->refresh();
        this->field->push_pull();

        this->pulse->attacker = one->id;

        if(one->is_hero()){
            this->pulse->at = 0;
            this->pulse->defender = get_unit_for_hero(one);
            this->pulse->state = (user_faction & one->fac)? 0 : 4; //keyboard on or anime for ai
            return 16;
        }

        if(one->is_city_archer){
            this->pulse->at = UNIT_FLAG_RANGER;
            if(one->city_unit < BTL_UNIT_CATAPULT){
                this->pulse->defender = this->get_simle_unit(this->pulse->at, one);
            }else if(one->city_unit == BTL_UNIT_CATAPULT){ // catapult
                this->pulse->defender = this->get_city_unit(2);
            }
            this->pulse->state = 4; // anime ai
            return 0;
        }

        this->pulse->at = one->unit->before_step();
        if(this->pulse->at < -1){ //low morale
            this->run->action(this->pulse->at, one, NULL);
            this->pulse->state = 2049;
            return 0;
        }

        if(user_faction != one->fac){ // AI here
            this->pulse->defender = this->get_simle_unit(this->pulse->at, one);
            if(this->pulse->defender < 1){
                if(this->queue->is_finish()){
                    return 4;
                }
                this->pulse->at = -2;
                this->run->action(this->pulse->at, one, NULL);
                this->pulse->state = 2049;
                return 0;
            }
            this->pulse->state = 4;
            return 0;
        }else{
            this->select_unit();
            //keyboard enable
            this->pulse->state = 0;
            return 2;
        }
    }

    int more = this->pulse->state & 4092; // xor animation and end

    if(this->pulse->state & 1){
        if(this->run->on_tick())
            return 1;

        this->pulse->state++;
        return 0;
    }

    if(this->pulse->state & 2){
        this->run->end_action(this->run->get_flag(), this->pulse->at,
                              this->queue->get_item(this->pulse->attacker),
                              this->queue->get_item(this->pulse->defender));
        this->pulse->state = more << 1;
        if(this->pulse->state == 0){
            this->pulse->state = more = 8;
        }
        //end animation
        //todo resolve

        return 0;
    }

    if(more == 4){
        if(!this->valid_action(BATTLE_ACTION)){
            this->pulse->state = -1;
            return 0;
        }
        this->pulse->state |= 1;
        return 0;
    }
    if(more == 8){
        if(this->valid_action(BATTLE_ASSIGN)){
            this->pulse->state |= 1;
            return 0;
        }
        this->pulse->state = more = 16;
    }
    if(more == 16){
        if(this->valid_action(BATTLE_RETALATION)){
            this->pulse->state |= 1;
            return 0;
        }
        this->pulse->state = more = 32;
    }
    if(more == 32){
        if(this->valid_action(BATTLE_AFTER)){
            this->pulse->state |= 1;
            return 0;
        }
        this->pulse->state = more = (this->pulse->at) < 0 ? 2048 : 256;
    }

    if(more == 64){
        if(this->valid_action(BATTLE_ASSIGN)){
            this->pulse->state |= 1;
            return 0;
        }
        this->pulse->state = more = 128;
    }

    if(more == 128){
        if(this->valid_action(BATTLE_RETALATION)){
            this->pulse->state |= 1;
            return 0;
        }
        this->pulse->state = more = 256;
    }
    if(more == 256){
        this->run->end_action(BATTLE_EFFECT, this->pulse->at, this->queue->get_item(this->pulse->attacker), this->queue->get_item(this->pulse->defender));
        this->pulse->state = more = 512;
    }
    if(more == 512){
        if(this->valid_action(MOVE_HOME)){
            this->pulse->state |= 1;
            return 0;
        }
        this->pulse->state = more = 1024;
    }
    if(more == 1024){
        if(this->valid_action(REGENERATION)){
            this->pulse->state |= 1;
            return 0;
        }
        this->pulse->state = more = 2048;
    }

    if(more == 2048){
        this->pulse->state = -1;
    }

    this->pulse->state = -1;
    this->pulse->at = -2;
    this->pulse->defender = 0;
    return -1;
}

int battle_field::get_clr(int a){
    if(a == 1) return this->left->get_clr();
    if(a == 2) return this->right->get_clr();
    return 'M';
}

int battle_field::hero_id(int a){
    if(a == 1) return this->left->hero_id();
    if(a == 2) return this->right->hero_id();
    return 0;
}

hmmc_queue_item* battle_field::next_queue(int index){
    if(index < 0){
        if(this->one_id()) return this->get_item(this->one_id());
        return NULL;
    }
    return this->queue->next_queue(index);
}

hmmc_queue_item* battle_field::get_index(int i)     { return this->queue->get_item_by_index(i);}
hmmc_queue_item* battle_field::get_item(int id)     { return this->queue->get_item(id);}

int battle_field::unit_count()  { return this->queue->get_unit_count();}
int battle_field::one_id()      { return this->pulse->attacker;}
int battle_field::two_id()      { return this->pulse->defender;}
bool battle_field::key_on()     { return (this->pulse->state == 0 && this->pulse->attacker != 0); }

void battle_field::on_run(){
    if(this->pulse->state == 0 && this->pulse->attacker != 0)
        this->pulse->state = 4;
}

// 8=return; 4=is big; 2=fly; 1=move
pair battle_field::unit_move(){
    pair p = {0, 0, 0};
    hmmc_queue_item* one = this->get_item(this->pulse->attacker);
    if (one->is_city_archer) return p;

    if ((this->pulse->at & UNIT_AFTER_RETURNS) == UNIT_AFTER_RETURNS){
        int xy = this->field->return_step();
        if(xy < 0) return p;
        p.value = xy >> 4;
        p.subval = xy & 15;
        p.state = 8;
        if (one->unit->is_fly()) p.state |= 2;
        if (one->unit->is_big()) p.state |= 4;
    }else{
        pair pos = this->get_item(this->pulse->defender)->unit->get_coord();
        int xy = this->field->pop_step(pos.value, pos.subval);
        if(xy < 0) return p;
        p.value = xy >> 4;
        p.subval = xy & 15;
        p.state = 1;
        ///todo если у нас юнит плывёт - это нужно как-то отметить
        if (one->unit->is_fly()) p.state |= 2;
        if (one->unit->is_big()) p.state |= 4;
    }
    return p;
}
