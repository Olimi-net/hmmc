#include "pair.h"

pair* pair_factory(int value, int subval, int state){
    pair *p = (pair*) malloc(sizeof(pair));
    p->value = value;
    p->subval = subval;
    p->state = state;
    return p;
};

void pair_free(pair *p){
    free(p);
};
