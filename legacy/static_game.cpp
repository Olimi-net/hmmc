#include "static_game.h"



int calc_abs(int a){
    if (a == 0) return 0;
    return a < 0 ? -a : a;
}
float calc_pow(float a, int p){
    float r = 1;
    for(int i =0;i<p;i++)
        r *= a;
    return r;
}
float calc_sign(int a){
    return a < 0 ? -0.1f : 0.1f;
}
/*  acf - unit attack * count units
    cft - (unit attack + hero attack) - (unit defense + hero defense)

    236 = (acf = 100 ctf=9)
    34 = (acf = 100 ctf=-10)
*/
int calc_unit_dmg(int acf, int cft){
    return (int)(acf * calc_pow(1 + calc_sign(cft), calc_abs(cft)));
}
