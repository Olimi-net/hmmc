#pragma once
#include "pair.h"
/*
union   strongold - tower - castle

lodge - inn - tavern - residence

library - archive - atheneum - sanctuary

stock - barn - storehouse - warehouse (treasury)

town-hall - city-hall -  white-hall - grand-hall
*/

// для энтов и рощи энтов и канавы с крабами(постройка) нужна живая вода , для постройки гнезда рок - нужно много древесины, для покупки нужна живая вода
// для покупки ангелов нужна живая вода, для рыцарей нужно дерево для наёма

const int BUILD_ALL = 511;
const int BUILD_2L = 31;
const int BUILD_LA = 15;

int static_city_get_room(int city, int room);
void static_city_draw_town(int city, int level, int row);
int static_city_build(int city, int level, int build);
pair* static_city_found(char* row, int len, int start_pos);
int static_city_growl(int city, int build);
const char* static_city_get_name(int city, int build);
const char* static_city_get_title(int city, int level);
pair static_city_cost(int city, int build);
