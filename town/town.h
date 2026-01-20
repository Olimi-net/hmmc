#pragma once
#include "../modules/group.h"

struct city_args{
    int u_type;
    int pos_l;
    int pos_r;
    bool to_left;
    int count;
    bool last;
};

class town{
    public:
        town();
        ~town();
    city_args try_move(int a, group* garrison, group* army);
};
