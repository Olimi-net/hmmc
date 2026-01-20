#pragma once
#include "static_game.h"
#include "pair.h"

const int BUILD_RC_GRAND_HALL = 1;
const int BUILD_RC_RESIDENCE = 2;
const int BUILD_RC_DEALER = 4;
const int BUILD_RC_BOWMAN = 8;
const int BUILD_RC_GUARD = 16;
const int BUILD_RC_MONK = 32;
const int BUILD_RC_SANCTURE = 64;
const int BUILD_RC_ANGEL = 128;
const int BUILD_RC_TREASURE = 256;

int static_city_red_castle_build(int level, int build);
void static_city_red_castle_draw(int level, int row);
int static_city_red_castle_growl(int level);
pair static_city_red_castle_get_cost(int v);
const char* static_city_red_castle_title(int row);
