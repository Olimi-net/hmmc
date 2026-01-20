#pragma once

int static_battle_draw_arrow(int i, int x, int y);
void static_battle_draw_effects(int x, int y, int v);

/** flag: 1=top wall; 2=step; 4=down wall; 8=side wall; 16=wood floor; 32=hexagon floor */
void static_battle_draw_basement(int x, int y, int flag);

/** wall: 1=close gate, 2=stone wall, 4=destroyed tower, 8=broken wall*/
void static_battle_draw_town(int wall, int ox, int oy, int clr);

void static_battle_draw_catapult(int state, int ox, int oy, int clr);
