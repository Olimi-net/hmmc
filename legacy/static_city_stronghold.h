#pragma once
#include "static_game.h"
#include "pair.h"

const int BUILD_SH_FENCE = 256;
const int BUILD_SH_WORKSHOP = 128;
const int BUILD_SH_ARCHIVE = 64;
const int BUILD_SH_LANCER = 32;
const int BUILD_SH_SLINGER = 16;
const int BUILD_SH_MARKET = 8;
const int BUILD_SH_PEASANT = 4;
const int BUILD_SH_INN = 2;
const int BUILD_SH_GREAT_HALL = 1;

int static_city_stronghold_build(int level, int build);
void static_city_stronghold_draw(int level, int row);
int static_city_stronghold_growl(int level);
pair static_city_stronghold_get_cost(int v);
const char* static_city_stronghold_title(int row);
