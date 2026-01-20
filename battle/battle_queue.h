#pragma once
#include "hmmc_queue_item.h"

class battle_queue{
    public:
        battle_queue();
        ~battle_queue();
        void add(hmmc_queue_item *q);
        hmmc_queue_item* get_item(int id);
        void morale(int id);
        void stun(hmmc_queue_item* q);
        hmmc_queue_item* pop();
        int is_finish();
        hmmc_queue_item* get_next_unit(int id);
        hmmc_queue_item* next_queue(int indx);
        hmmc_queue_item* get_item_by_index(int i);
        int get_unit_count();
    private:
        std::vector<hmmc_queue_item*> items;
        int last_id;
};
