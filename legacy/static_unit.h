#pragma once
#include "static_game.h"
#include "pair.h"

const int AN_LEFT = 0;
const int AN_MOVE_L = 1;
const int AN_ATACK_L = 2;
const int AN_RIGHT = 3;
const int AN_MOVE_R = 4;
const int AN_ATACK_R = 5;
const int AN_DEAD = 8;

const char* static_unit_get_row(int unit, int row, int anim);
int static_draw(int c, int lvl, int row);
bool static_unit_is_huge(int v);
int static_unit_is_machine(int v);
int static_unit_growl(int unit_type);
int static_unit_get_value(int unit_id, int val);
pair* static_unit_get_damage(int unit_id);
int static_unit_get_id(int unit_type);
void static_unit_test(int anime);
int static_unit_get_city_archer(int city);
