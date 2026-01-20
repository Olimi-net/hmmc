#pragma once


const int BOOK_MAGIC_COUNT = 13;
/**
    id: 0 .. < BOOK_MAGIC_COUNT;
    row: 0 .. 4;
    cost: 0 .. 9999;
    duo: double line between books pages
*/
int book_of_magic_draw(int id, int row, int cost, bool duo);
const char* book_of_title(int id, bool is_description);
