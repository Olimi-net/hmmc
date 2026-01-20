#pragma once

const int SPECIAL = 512;

const int VILLAGE = 256;

const int STRONHOLD = 8192;
const int RED_CASTLE = 4096;
const int WHITE_TOWER = 2048;
const int FOREST_UNION = 1024;
// city always hugest city-build
const int ALL_CITIES = FOREST_UNION | WHITE_TOWER | RED_CASTLE | STRONHOLD;


const int CITY = 128;
const int UNIT = 64;
const int TEXT = 32;

const int LEVEL_1 = 1;
const int LEVEL_2 = 2;
const int LEVEL_3 = 4;
const int LEVEL_4 = 8;

const int HERO = SPECIAL | UNIT | LEVEL_1;
const int TENT = SPECIAL | UNIT | LEVEL_2;
const int WAGON = SPECIAL | UNIT | LEVEL_3;
const int BALLISTA = SPECIAL | UNIT | LEVEL_4;
const int MACHINE = TENT | WAGON | BALLISTA;

int calc_unit_dmg(int acf, int cft);
