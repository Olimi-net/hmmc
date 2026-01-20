#include "cmp_basic.h"

cmp_basic::cmp_basic(cmp_basic* parent){
    this->parent = parent;
    is_load = true;
}

cmp_basic::~cmp_basic(){

}
