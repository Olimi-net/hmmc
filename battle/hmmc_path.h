#pragma once
#include <vector>
#include "../legacy/pair.h"

class hmmc_path{
    public:
        hmmc_path(int w, int h);
        ~hmmc_path();
        void set_pos(int x, int y, int v);
        int get_pos(int x, int y);
        int rem_pos(int x, int y);
        void clear();
        int gen_path(pair p1, pair p2, bool is_fly, bool is_swim);
        int pop_step(int tx, int ty);
        int step_count(int tx, int ty);
        int found_value(int val);
        int width;
        int height;
        bool is_pos(int x, int y);
        void push_pull();
        int rem_pos_by_id(int id);
        int return_step();
        bool is_has_step(int tx, int ty);
    private:
        std::vector<pair*> field;
        std::vector<pair*> step_list;
        std::vector<pair*> pair_pull;
        bool can_swim;
        int max;
        pair last;
        int sub_pos(int x, int y);
        int try_set_sub_big(int x, int y, int v);
        int try_set_sub(int x, int y, int v, bool is_big);
        bool can_stay(int x, int y, int is_big);
        int calc_path(int fx, int fy, int tx, int ty);
        int path_update(int x, int y, int b);
        void append(int x, int y);
        bool path_validate(int x, int y, int c);
        void refresh();
        int get_pos_or_def(int x, int y);
        pair* get_pair(int o, int t, int s);
        int gen_step(int fx, int fy, int tx, int ty);
        bool free_place(int x, int y, int is_big);
        void close_pos(int tx, int ty);
        int gen_fly(int x1, int y1, int x2, int y2, bool is_big, bool target_big);
};
