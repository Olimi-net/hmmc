#pragma once
const int BTL_UNIT = 32;
const int BTL_UNIT_CATAPULT = BTL_UNIT | 1; // LEVEL_1;
const int BTL_UNIT_GATE = BTL_UNIT | 2; // LEVEL_2;
const int BTL_UNIT_WALL = BTL_UNIT | 4; // LEVEL_3;
const int BTL_UNIT_ARCHER = BTL_UNIT | 8; // LEVEL_4;

const int BATTLE_ATTACK = 1;
const int BATTLE_DEFENCE = 2;

/* reserved for dungeon and city
const int BATTLE_CITY = 4;
const int BATTLE_SHIP = 8;
*/

const int BATTLE_CITY = 16;
const int BATTLE_CITY_FOREST_UNION = 20;

const int BATTLE_DANGEON = 32;
const int BATTLE_SHIP = BATTLE_DANGEON;

const int BATTLE_TAVERN = 36;
const int BATTLE_CATACOMB = 40;

const int BATTLE_PLACE = BATTLE_CATACOMB | BATTLE_TAVERN | BATTLE_SHIP | BATTLE_CITY;

