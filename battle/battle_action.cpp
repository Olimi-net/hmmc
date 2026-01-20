#include "battle_action.h"



battle_action::battle_action(hmmc_callback* battle, hmmc_callback* field){
    this->run = new battle_animation(battle, field); // animation resolve in battle
    this->event = field;
}

battle_action::~battle_action(){
    delete this->run;
}

bool battle_action::action(int at, hmmc_queue_item* one, hmmc_queue_item* two){
    if(one->is_hero()){
        //todo calc at
        //at = read_magic_book(one);
        if(at == UNIT_FLAG_RANGER){
            this->run->init(BA_HERO_RANGE, at, one, two, BATTLE_RANGER);
            return true;
        }else if(at == UNIT_FLAG_MELEE){
            this->run->init(BA_HERO_ATTACK, at, one, two, BATTLE_MELEE);
            return true;
        }else if(at == UNIT_FLAG_MAGIC){
            if(one->fac == two->fac){
                this->run->init(BA_POSITIVE_MAGIC, at, one, two, BATTLE_INTERACTION);
            }else{
                this->run->init(BA_NEGATIVE_MAGIC, at, one, two, BATTLE_MAGIC);
            }
            return true;
        }
        return false;
    }

    if(one->is_city_archer){
        this->run->init(BA_RANGE_ARROW, at, one, two, BATTLE_RANGER);
        return true;
    }

    if(at < 0){
        if(one->unit->is_machine()) return false;
        //todo show animation morale low
        this->run->init(BA_LOW_MORALE, at, one, NULL, BATTLE_NEXT);
        return true;
    }

    if(one->fac == two->fac){ //all friendly action into group
        this->run->init(BA_POSITIVE_MAGIC, at, one, two, BATTLE_INTERACTION);
        return true;
    }

    if(two->unit->is_dead()){
        if((at & UNIT_ATTACK_RESURRECT) != UNIT_ATTACK_RESURRECT) return false;
        if(one->group != two->group){
            one->group->add_unit(two->unit);
            two->group->rem_unit(two->id);
            two->group = one->group;
            two->fac = one->fac;
            //this->field->update_faction(two->id, two->fac);
        }
        this->run->init(BA_POSITIVE_MAGIC, at, one, two, BATTLE_INTERACTION);
        return true;
    }

    if(at & UNIT_FLAG_MELEE){
        this->run->init(BA_MOVE_ATTACK, at, one, two, BATTLE_MELEE);
        return true;
    }

    if(at & UNIT_FLAG_RANGER){
        this->run->init(BA_RANGE_ARROW, at, one, two, BATTLE_RANGER);
        return true;
    }

    if(at & UNIT_FLAG_MAGIC){
        if(((at & UNIT_HAS_MAGIC)==UNIT_HAS_DRAIN_LIFE) && two->unit->nonliving()){
            this->run->init(BA_MOVE_ATTACK, at & UNIT_FLAG_MELEE, one, two, BATTLE_MELEE);
        }else{
            this->run->init(BA_NEGATIVE_MAGIC, at, one, two, BATTLE_MAGIC);
        }
        return true;
    }
    return false;
}

void battle_action::end_action(int flag, int at, hmmc_queue_item* one, hmmc_queue_item* two){
    if(flag == 0) return; //aborted action
    if(one == nullptr || two == nullptr) return;

    if(flag & BATTLE_ASSIGN){
        if(one->assign){
            int f = flag ^ BATTLE_ASSIGN;
            this->end_action(f, at, one, two);
        }
        return;
    }else if(flag & BATTLE_RETALATION){
        // if(this->pulse->e2 & UNIT_FLAG_NO_RETALATION) return;
        if(at > 0){
            int f = flag ^ BATTLE_RETALATION;
            this->end_action(f, at, one, two);
            one->unit->down_retaliated();
        }
        return;
    }

    if(flag == BATTLE_NEXT){
        //move queue
        return;
    }else if(flag == BATTLE_INTERACTION){
        one->interaction(at, two);
        return;
    }else if(flag == BATTLE_MELEE){
        melee_action(at, one, two);
        return;
    }else if(flag == BATTLE_MAGIC){
        magic_action(at, one, two);
        return;
    }else if(flag == BATTLE_RANGER){
        range_action(at, one, two);
        return;
    }else if(flag == BATTLE_EFFECT){
        if(at & UNIT_AFTER_MUTILATE){
            two->unit->set_debaf(UNIT_AFTER_MUTILATE);
        }
        if ((at & UNIT_AFTER_ROOT) == UNIT_AFTER_ROOT){
            two->unit->set_debaf(UNIT_AFTER_ROOT);
        }
        return;
    }
}

/*
 * A = (heroAttack + unitAttack) x (attack is range and near enemy units then penalty ? 0.5 : 1)
 * D = heroDefense + unitDefense
 * DMGb = Base damage
 * I1 = 0.05 x (A ≥ D ? A-D : 0);
 * I2  = 0.10, 0.22, 0.40 for basic, advanced, expert
 * I3 = 0.03 x heroLevel x levelUnits
 * I4  = 1.0 for lucky strikes or 0.5 for unluck
 * I5 = I2 × hero level for specialty
 * I6 = 0.01 × jousting step
 * I6 = 0.01 × (sorcery ? heroLevel : 0);
 * R1 = 0.025 × (D ≥ A ? D - A : 0)
 * R2 = 0.05, 0.10, 0.15 for basic, advanced, expert
 * R3 = 0.05 × R2 × hero level for specialty
 *
 * DMG = DMGb × (1 + I1 + I2 + I3 + I4 + I5) × (1 - R1) × (1 - R2 - R3)
 */

int battle_action::get_damage(int at, hmmc_queue_item* one, hmmc_queue_item* two, float I6){
    int A = one->group->get_attack(one->id);
    int D = two->group->get_defense(two->id);
    int min = one->dmg_min();
    int max = one->dmg_max();
    //int luck = (at & UNIT_FLAG_LUCK) ? 1 : (at & UNIT_FLAG_UNLUCK) ? -1 : 0;

    float iA = one->group->index_attack(one->id);
    float iD = two->group->index_defense(two->id);
    //return calc_damage(A, D, min, max, luck, I6, iA, iD);

    float I1 = (A > D) ? 0.05f * (A-D) : 0;
	//float I4 = (luck < 0) ? 0.5f : (luck > 0 ? 1 : 0);
	float I4 = (at & UNIT_FLAG_UNLUCK) ? 0.5f : (at & UNIT_FLAG_LUCK) ? 1 : 0;
	float R1 = (D > A) ? 0.025f * (D - A) : 0;
	float res = (I1 + I4 + iA + I6) * (1 - R1) * iD;

	float DMG_min = min * res * 100;
	float DMG_max = max * res * 100;

	int dn = (int) DMG_min;
	int dx = (int) DMG_max;
	int dmg = math_rand_dmg(dn, dx) * 0.01f;

	if(dmg < 1) return 1;
	return dmg;
}

void battle_action::range_action(int at, hmmc_queue_item* one, hmmc_queue_item* two){
    if(at < 1) return;
    float I6 = one->get_archery();
    /* todo
        UNIT_FLAG_ROUND;
        UNIT_FLAG_NO_RETALATION;
        UNIT_FLAG_RAGE;
    */
    int dmg = get_damage(at, one, two, I6);
    int dead_count = two->set_damage(dmg);

    if(one->is_city_archer) return;

    if(at & UNIT_ATTACK_RAGE){
        int mn = two->unit->get_level() + (two->unit->is_big() ? 1 : 0);
        int rage = (dmg > 0) ? 1 : -1;
        rage += dead_count * mn;

        one->unit->up_rage(rage, dead_count * mn);
    }

    one->group->remove_shoots(one->id, 1);
}

void battle_action::magic_action(int at, hmmc_queue_item* one, hmmc_queue_item* two){
    int dmg = this->get_damage(at, one, two, one->group->get_sorcery());
    if(at == UNIT_ATTACK_DRAIN_LIFE){
        two->set_damage(dmg);
        one->unit->heal(dmg >> 1);
    }else if(at == UNIT_ATTACK_FIREBALL){
        hmmc_event he = hmmc_event(AC_FIRE);
        he.p.state = AC_DAMAGE;
        he.p.value = dmg;
        this->event->on_callback(&he, two->id);
        //this->event->on_event(, );
        /*
        pair p = this->field->get_pos(two->id);
        if(p.state == two->id){
            this->fair_round(dmg, p.value, p.subval, UNIT_ATTACK_FIREBALL);
        }*/
    }else if(at == UNIT_ATTACK_LIGHTNING){
        two->set_damage(dmg);
        if(!two->unit->nonliving() && math_rand_sum_2_4(-1, 0, 1, 2) == 3){
            hmmc_event he = hmmc_event(AC_STUN);
            this->event->on_callback(&he, two->id); // this->queue->stun(two);
        }
    }else if(one->is_hero()){
        //todo get hero magic
        two->set_damage(dmg);
    }
    one->group->remove_shoots(one->id, 1);
}

void battle_action::melee_action(int at, hmmc_queue_item* one, hmmc_queue_item* two){
    int dmg = 0;
    bool rangers = at & (UNIT_FLAG_RANGER | UNIT_FLAG_MAGIC);
    if(at == 0 || rangers){
        if(at & UNIT_ATTACK_NO_MELEE_PENALTY){
            dmg = get_damage(at, one, two, 0);
        }else{
            dmg = get_damage(at, one, two, 0) >> 1;
        }
        two->set_damage(dmg);
        return;
    }
    float I6 = 0;
    int count_step = 0; // todo one->step;

    if((at & UNIT_FLAG_AFTER)==0){
        if (at & UNIT_ATTACK_JOUSTING) I6 += count_step * one->group->get_difficult();
    }
    if (at & UNIT_ATTACK_RAGE) I6 += one->unit->get_rage() * one->group->get_difficult();

    dmg = get_damage(at, one, two, I6);
    if(at & UNIT_ATTACK_ROUND){
        //this->round_attack(dmg, one->id, UNIT_ATTACK_ROUND, 3-one->fac, one->unit->is_big());
        hmmc_event he = hmmc_event(AC_ROUND);
        he.p.state = AC_DAMAGE;
        he.p.value = dmg;
        this->event->on_callback(&he, one->id);
        return;
    }

    {
        int dead_count = two->set_damage(dmg);
        if(at & UNIT_ATTACK_RAGE){
            int mn = two->unit->get_level() + (two->unit->is_big() ? 1 : 0);

            int rage = (dmg > 0) ? 1 : -1;
            rage += dead_count * mn;
            one->unit->up_rage(rage, dead_count * mn);
        }
    }
}

bool battle_action::after_action(int at, hmmc_queue_item* one, hmmc_queue_item* two){
    if(at < 0) return false;
    if(one->unit->is_dead()) return false;

    if(at & UNIT_FLAG_MELEE){
        this->run->init(BA_STAND_ATTACK, at, one, two, BATTLE_MELEE);
        return true;
    }

    if(at & UNIT_FLAG_RANGER){
        this->run->init(BA_RANGE_ARROW, at, one, two, BATTLE_RANGER);
        return true;
    }
    return false;
}



bool battle_action::assign_or_retalation(int at, hmmc_queue_item* one, hmmc_queue_item* two, int flag){
    if(at < 0) return false;
    //todo return: melee range or magic
    //юнит ближнего боя не перемещается, если он рядом - отвечает
    //юнит дальнего боя отвечает если может стрелять
    //герой отвечает не зависимо не от чего
    //мораль не учитывается,
    // бонусы и эффекты: обычный урон юнита с учётом ярости и удачи, круговая атака, корни
    // не используется: возврат, разбег, двойная атака, калеченье
    //
    if(at & UNIT_FLAG_MELEE){
        this->run->init(BA_STAND_ATTACK, at, one, two, BATTLE_MELEE | flag);
        return true;
    }else if(at & UNIT_FLAG_RANGER){
        this->run->init(BA_RANGE_ARROW, at, one, two, BATTLE_RANGER | flag);
        return true;
    }else if(at & UNIT_FLAG_MAGIC){ //any magic
        if (((at & UNIT_HAS_MAGIC)==UNIT_HAS_DRAIN_LIFE) && two->unit->nonliving())
            this->run->init(BA_STAND_ATTACK, at | UNIT_FLAG_MELEE, one, two, BATTLE_MELEE | flag);
        else
            this->run->init(BA_NEGATIVE_MAGIC, at, one, two, BATTLE_MAGIC | flag);
        return true;
    }
    return false;
}

int battle_action::one_id(){
    if(this->run->is_end()) return 0;
    hmmc_queue_item* q = this->run->get_one();
    if(q == NULL) return 0;
    return q->id;
}

void battle_action::anime(int anim, int at, hmmc_queue_item* one, hmmc_queue_item* two, int flag){
    this->run->init(anim, at, one, two, flag);
}

bool battle_action::on_tick(){ return this->run->on_tick(); }
int battle_action::get_flag(){ return this->run->get_flag(); }
