#pragma once
#include <time.h>

struct quadra{
    int e1;
    int e2;
    int e3;
    int e4;
};

int math_village_shuffle(int seed, int num);
int math_get_time();
int math_calc_offset(int disp, int len, int pos, int size);
quadra* quadra_factory(int e1, int e2, int e3, int e4);
void quadra_free(quadra *q);
quadra math_shuffle_quadra(quadra q);
int math_min(int a, int b);
int math_max(int a, int b);
int math_rand_sum_2_4(int e1, int e2, int e3, int e4);
int math_rand_dmg(int min, int max);
int math_rand_five(int e1, int e2, int e3, int e4, int e5);
int math_parse_int(const char *c);
