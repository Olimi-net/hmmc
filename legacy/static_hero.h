#pragma once

const int STATIC_HERO_COUNT = 24;
const int STATIC_HERO_FREE = 17;

/* depricated using from player_args
int static_hero_from_city(int city, int indx); */

int static_hero_get_unit_type(int hero, int indx);
int static_hero_get_unit_count(int hero, int indx);
int static_hero_draw_name_align(int id, int clr, int x, int y, int w, int align);
int static_hero_draw_name(int id, int clr);
int static_hero_count_name(int id);
