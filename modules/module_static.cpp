#include "module_static.h"
#include "../legacy/static_unit.h"
#include "../legacy/hmmc_math.h"
#include "../legacy/book_of_magic.h"
#include "../legacy/drawing.h"

module_static::module_static(){

}

module_static::~module_static(){

}


pair module_static::unit_cost(int unit_type){
    pair p = {0 , 0, 0};
    p.value = math_parse_int(static_unit_get_row(unit_type|TEXT, 8, 0));
    const char* elem = static_unit_get_row(unit_type|TEXT, 10, 0);

    int count = (static_unit_is_machine(unit_type))
        ? math_parse_int(static_unit_get_row(unit_type|TEXT, 13, 0)) : 1;

    if(elem[0] == 'W') p.subval = count;
    if(elem[0] == 'M') p.state = count;
    return p;
}

const char* module_static::unit_text(int unit_type, int row){
     return static_unit_get_row(unit_type | TEXT, row, 0);
}

void module_static::draw_book(int magic, int x, int y, int cost, int mana, bool is_double){
    for(int j = 0; j<4;j++){
        set_position(x, y + j);
        book_of_magic_draw(magic, j, (j & 1) ? cost : mana, is_double);
    }
}

void module_static::draw_unit(int unit_type, int x, int y, int angle){
    for(int j = 0; j<3;j++){
        set_position(x, y + j);
        //todo color unit
        draw_text(static_unit_get_row(unit_type, j, angle), 0);
    }
}
