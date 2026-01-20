#pragma once
#include "static_game.h"
#include "pair.h"

const int BUILD_WT_ROCK=256;
const int BUILD_WT_LABORATORY=128;
const int BUILD_WT_ATHENEUM=64;
const int BUILD_WT_MAGE=32;
const int BUILD_WT_WALL=16;
const int BUILD_WT_FIGHTER=8;
const int BUILD_WT_TAVERN=4;
const int BUILD_WT_SHOP=2;
const int BUILD_WT_GRAND_HALL=1;

void static_city_white_tower_draw(int level, int row);
int static_city_white_tower_build(int level, int build);
int static_city_white_tower_growl(int level);
pair static_city_white_tower_get_cost(int v);
const char* static_city_white_tower_title(int row);
