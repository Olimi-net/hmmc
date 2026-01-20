#include "book_of_magic.h"
#include "drawing.h"

const char* book_of_magic[4][BOOK_MAGIC_COUNT] = {
    // unit support 0-3 | hero attack 4 - 6 |
    { "  ||  "," (\\/) ","  vv  "," >--> "," >--+ ","  |_  "," \\()/ ","  /\\  ","  )(  ","  [|] "," o    ","  /\\  "," \\  / "},
    { "  **  ", "  \\/  ","  ()  "," >--> ","    V ","   /  ", " ~[]~ ","  \\/  "," (__) "," \\__] "," T->X "," |[]| ","  []  "},
    { "The medal","Magic Shield","Protection Ring","Magic arrows","Crack Arrows","Lightning",
      "Summon Swamp","Summon Crystal","Healing potion", "Summon Boat","Summon units","Home Portal","Amulet of mark"},
    {
        "Increase attack for a unit",
        "Protect from ranged attacks",
        "Ring a friendly unit to be protected",
        "Hit a target",
        "Reduce ammo stock",
        "Hit a target. The target has a chance to be stunned",

        "Slow a target down",
        "Summon a large crystal to block the path",
        "Restore health to living creatures",
        "Requires water nearby. Non battle.",
        "Summons a army to assistance. Non battle.",
        "Teleport to a nearest friendly city. Non battle.",
        "It can't be studied in a library.",
    }
};

const char* book_of_title(int id, bool is_description){
    if(id >= BOOK_MAGIC_COUNT) return 0;
    int n = is_description ? 3 : 2;
    return book_of_magic[n][id];
}

int book_of_magic_draw(int id, int row, int cost, bool duo){
    if(id >= BOOK_MAGIC_COUNT || row > 3) return 0;
    int clr='W';
    int total = 0;
    int n = 0;

    if(row == 0){
        for(int j=0;j<2;j++){
            draw_char(' ', clr);
            total++;
            total+=draw_as_line('_', clr, 6);
            if(j==0 && duo){
                draw_char(' ', clr);
                total++;
            }
        }
        draw_char(' ', clr);
        total++;
        return total;
    }

    draw_char('|', clr);
    total++;

    if(row == 3){
        for(int j=0;j<2;j++){
            total+=draw_as_line('_', clr, 6);
            if(j==0){
                draw_char('|', clr);
                total++;
                if(duo){
                    draw_char('|', clr);
                    total++;
                }
            }
        }
        draw_char('|', clr);
        total++;
        return total;
    }

    n = row - 1;
    total+=draw_text(book_of_magic[n][id], '0');
    draw_char('|', clr);
    total++;

    if(duo){
        draw_char('|', clr);
        total++;
    }

    if(cost < 1)
        cost = (id < 11) ? 4 : (id - 10) * 2 + 4;

    int x = get_position_x();
    total += draw_int_align(x, 6, cost, 0, 1);
    set_position(x+6, get_position_y());
    draw_char('|', clr);
    total++;
    return total;
}

/*
    H A




*/


/*
    enchanted arrows
	 heal   medale  magic  move	 broken Swamp  summon  cost  summon teleport       Lightning amulet protection
				    arrow  unit  arrow		  crystal full  boat    home  shield            of mark ring
	|  )(  |  ||  | >--> | .    | >--+ | \()/ |  /\  |  4   |  [|] |  /\  | (\/) |  |  |_  | \  / |  vv  |
	|_(__) |  **  | >--> | T->X |    V | ~[]~ |  \/  |16/16 | \__| | |[]| |  \/ _|  |_  /  |  []  |  () _|
	 \|____|______|______|______|______|______|______|______|______|______|____|/    \|____|______|____|/

      witch's
       song
	 | p-p- | q-q- | p=d= |/(o)\ |      |
     |_-d-- | b-b- | =p=d |\___/ |     _| ===E
	  \|____|______|______|______|____|/

	 *
	 * heal = восстанавливает здоровье выбраному живому существу
	 * medale = увеличивает атаку
	 * magic arrow = магические стрелы поражают врагов
	 * move unit = перемещает выбраного юнита в нужную точку (не действует на больших существ и энтов)
	 * broken arrow = снижает количество выстрелов (действует только на стрелков)
	 * swamp = снижает следующее движение больших юнитов  в 2 раза, маленьких юнитов до 1, не действует на летающих и плавающих
	 * summon cristall = призывает на поле боя большой кристалл кварца, мешает передвижению пеших юнитов, при разрушении наносит урон всем юнитам вокруг
	 * summon boat = призывает на карте корабль, призыватель должен находиться рядом с водоёмом
	 * teleport home = перемещает призывателя в ближайший дружественный город
	 * shield = увеличивает защиту
	 *
	 * support(heal, medale, move unit, shield)
	 * attack(magic arrow, summon cristall, Swamp, broken arrow)
	 * word(summon boat, teleport home)
	 *

	 *  S	S	A	S	D		D	A	W	W	S
	 *
*/
