#include "hmmc_event.h"


hmmc_event::hmmc_event(int id){
    this->id = id;
    p.state = 0;
    p.value = 0;
    p.subval = 0;
}

hmmc_event::~hmmc_event(){

}
