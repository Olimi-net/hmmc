#include "battle_step.h"

battle_step::battle_step(){
    attacker = defender = 0;
    at = -2;
    state = 0;
}

battle_step::~battle_step(){

}

bool battle_step::is_at(int f){
    return (at & f) == f;
}

