#pragma once
#include <stdlib.h>   // для подключения функции malloc


struct pair{
    int value;
    int subval;
    int state;
};

pair* pair_factory(int value, int subval, int state);
void pair_free(pair *p);
