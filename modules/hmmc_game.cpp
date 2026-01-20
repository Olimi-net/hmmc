#include "hmmc_game.h"
#include "hmmc_error.h"
#include "../legacy/static_unit.h"
#include "../legacy/hmmc_math.h"



hmmc_game::hmmc_game(hmmc_callback *e){
    guid = math_get_time();
    error_event = e;
}

hmmc_game::~hmmc_game(){
    for(int i = cities.size() - 1;i>=0;i--)
        delete cities[i];
    cities.clear();
    for(int i = heroes.size() - 1;i>=0;i--)
        delete heroes[i];
    heroes.clear();
    for(int i = groups.size() - 1;i>=0;i--)
        delete groups[i];
    groups.clear();
}

group* hmmc_game::factory_group(int fracton, int hero_id, int unit_type){
    if(hero_id < 0 || hero_id > STATIC_HERO_COUNT || hero_id == STATIC_HERO_FREE) return NULL;

    if(hero_id == 0){
        group* g = new group(guid++, fracton, NULL);
        groups.push_back(g);
        init_group_by_unit(g, unit_type);
        return g;
    }

    for(int i = groups.size() - 1;i>=0;i--){
        if(groups[i]->get_hero_val_or_def(HERO_ATTR_ID, 0) == hero_id){
            return NULL;
        }
    }

    hero* h = maybe_hero(hero_id);
    if(h == NULL){
        h = new hero(hero_id);
        heroes.push_back(h);
    }else{
        hmmc_error(ERROR_HERO_LOST);
        error_event->on_callback(NULL, ERROR_HERO_LOST);
    }

    {
        group* g = new group(guid++, fracton, h);
        groups.push_back(g);
        init_group_by_unit(g, unit_type);
        return g;
    }
}

hero* hmmc_game::maybe_hero(int hero_id){
    for(int i = heroes.size() - 1;i>=0;i--){
        if(heroes[i]->get_attr(HERO_ATTR_ID)==hero_id){
            return heroes[i];
        }
    }
    return NULL;
}

void hmmc_game::init_group_by_unit(group *g, int week_unit){
    if(week_unit < 0) return;
    if(week_unit == 0){
        int h = g->get_hero_val_or_def(HERO_ATTR_ID, 0);
        if(h > 0){
            g->append_unit(static_hero_get_unit_type(h, 1), static_hero_get_unit_count(h, 1));
            g->append_unit(static_hero_get_unit_type(h, 2), static_hero_get_unit_count(h, 2));
        }
    }else{
        g->append_unit(week_unit, static_unit_growl(week_unit));
    }
}

