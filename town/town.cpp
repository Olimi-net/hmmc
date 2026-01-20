#include "town.h"

town::town(){

}

town::~town(){

}

/// unit_type, slot left, slot right, to left, available, is last
city_args town::try_move(int a, group* garrison, group* army){
    city_args q = {0, 0, 0, false, 0, false};
    if(garrison == nullptr || army == nullptr) return q;

    int pos = -1;
    if(a > 5){ pos = a - 6;
    }else{
        pair *p = garrison->get_unit(a - 1);
        if(p == nullptr || p->subval < 1) return q;
        int available = p->subval;
        q.pos_l = a;
        q.pos_r = army->can_append(p->value) + 1;
        if(garrison->get_hero_val_or_def(HERO_ATTR_ID, 0) > 0 && garrison->slots_count() < 2) q.last = true;
        q.count = available;
        q.u_type = p->value;
        return q;
    }

    {
        pair* p = army->get_unit(pos);
        if(p == nullptr || p->subval < 1) return q;
        int available = p->subval;
        q.pos_r = pos + 1;
        q.pos_l = garrison->can_append(p->value) + 1;
        if (army->slots_count() < 2) available--;
        q.count = available;
        q.to_left = true;
        q.u_type = p->value;
        return q;
    }
}
