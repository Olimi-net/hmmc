#pragma once
#include "static_game.h"
#include "pair.h"

const int BUILD_FU_FOREST = 256;
const int BUILD_FU_MOTE = 128;
const int BUILD_FU_LIBRARY = 64;
const int BUILD_FU_BASAR = 32;
const int BUILD_FU_DRUID = 16;
const int BUILD_FU_THE_ROOST = 8;
const int BUILD_FU_GREENHOUSE = 4;
const int BUILD_FU_LODGE = 2;
const int BUILD_FU_TOWN_HALL = 1;

int static_city_forest_union_build(int level, int build);
void static_city_forest_union_draw(int level, int row);
int static_city_forest_union_growl(int level);
pair static_city_forest_union_get_cost(int v);
const char* static_city_forest_union_title(int row);
