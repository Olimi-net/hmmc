#include "battle_queue.h"

battle_queue::battle_queue(){
    last_id = 0;
}

battle_queue::~battle_queue(){
    int size = this->items.size();
    for(int i = 0; i < size; i++){
        delete this->items[i];
    }
}

void battle_queue::add(hmmc_queue_item *q){
    this->items.push_back(q);
}

hmmc_queue_item* battle_queue::get_item(int id){
    int size = this->items.size();
    for(int i=0; i<size; i++){
        if(this->items[i]->id == id) return this->items[i];
    }
    return NULL;
}

void battle_queue::morale(int id){
    //сместить по шкале инициативы вперёд
}

void battle_queue::stun(hmmc_queue_item* q){
    //сместить по шкале инициативы назад
}

hmmc_queue_item* battle_queue::pop(){
    //todo fix here
    int size = this->items.size();
    if(last_id >= size) last_id = 0;
    return this->items[last_id++];
}

int battle_queue::is_finish(){
    int i = 0;
    int size = this->items.size();
    for(int i=0; i<size; i++){
        if(this->items[i]->is_hero()) continue;
        if(this->items[i]->unit->is_dead()) continue;
        if(this->items[i]->unit->is_machine()) continue;
        if(this->items[i]->city_unit) continue;
        //todo morale is low?
        i |= this->items[i]->fac;
        if(i == 3) return 0;
    }
    return (i == 3) ? 0 : (i == 0) ? -1 : i;
}

hmmc_queue_item* battle_queue::get_next_unit(int id){
    int size = this->items.size();
    bool next = (id == 0);
    for(int i=0;i<size;i++){
        if(next){
            if(this->items[i]->not_next() || this->items[i]->id == id) continue;
            return this->items[i];
        }
        next = (this->items[i]->id == id);
    }
    for(int i=0;i<size;i++){
        if(next){
            if(this->items[i]->not_next() || this->items[i]->id == id) continue;
            return this->items[i];
        }
        next = (this->items[i]->id == id);
    }
    return NULL;
}


hmmc_queue_item* battle_queue::next_queue(int indx){
    //todo fix queue
    int size = this->items.size();
    int k = (last_id + indx) % size;
    if(k < 0) return NULL;
    hmmc_queue_item* q = this->items[k];
    if(q->is_hero()) return q;
    if(q->unit->is_dead()) return NULL;
    return q;
}

hmmc_queue_item* battle_queue::get_item_by_index(int i){
    int size = this->items.size();
    if(i < size) return this->items[i];
    return NULL;
}

int battle_queue::get_unit_count(){ return this->items.size(); }
