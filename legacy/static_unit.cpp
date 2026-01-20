#include "static_unit.h"
#include "drawing.h"

const char* static_unit_translate[] = { // ~ цвет фракции, 4 = red, 2 = green, 1=blue
    "J/~","L\\~","j|~","l|~","S/6", "s\\6", "++6", "==6","[[~", "]]~", "TT~", "I|6", "yY6",
    "YY~", "MM2","WW2", "004","@@2","**1", "--6", "\0\0\0"
};

const char* static_unit_upgrade[][5] = {
    { "Elem",   "Race", "From", "UpGold",   "."},
    //red castle 1-4
    { ".",      "H",    "00",   ".",        "."},
    { ".",      "H",    "H3",   "5",        "."}, // all 1 or 2 level humans up to Guard 5 gold
    { ".",      "M",    "00",   ".",        "."},
    { "M",      "M",    "M4",   "25",       "."}, // mage or monk up to angel 25 gold and mercury
    //stronghold 5-8
    { ".",      "H",    "",     "",         "."},
    { ".",      "H",    "H1",   "5",        "."}, // all 1 level humans up to Slinger 5 gold
    { ".",      "L"     "H3",   "12"        "."}, // all 1 or 2 level humans up to Lancer 12 gold
    { ".",      "K",    "L="   "20",        "."}, // all Light up to Knight 20 gold and wood
    //forest union 9-12
    { ".",      "",    "",     "",         "."},
    { ".",      "",    "",     "",         "."},
    { "M",      "",    "",     "",         "."},
    { ".",      "",    "",     "",         "."},
    //white tower 13-16
    { ".",      "H",    "",     "",         "."},
    { ".",      "H",    "H3",   "5",        "."}, // all 1 or 2 level humans up to warrior 5 gold
    { ".",      "M",    "",     "",         "."},
    { "M",      "R",    "R1",   "15",       "."}, // Wild-met up to rock 15 gold and mercury
    //special 17-20
    { ".",      "",     "",      "200",     "."},
    { "M",      "",     "",      "4",        "."},
    { "W",      "",     "",      "6",        "."},
    { "W",      "",     "",      "6",        "."},
    //village 21-24
    { ".",      "R",    "",      "",        "."},
    { ".",      "",     "",      "",        "."},
    { ".",      "",     "",      "",        "."},
    { ".",      "L",    "K8",    "16",      "."}, // Knight up to Wild horse 16 gold
};

const char* static_unit_text[][10] = { /* have keys with static hero */
    {"Unit",    "Lvl", "Atk", "Def",   "Dmg",   "HP", "Spe", "Gro", "Cost", "Special"},
    //red castle 1-4
    {"Bowman",  "1",    "3", "3",       "1-2",  "4",  "5",  "11",   "5",  "Ranged (8 shots)"},
    {"Guard",   "2",    "6", "7",       "3-3",  "20", "3",  "7",    "12", "Positive Morale"},
    {"Monk",    "3",    "8", "4",       "2-4",  "15", "4",  "4",    "17", "Drain life, Healing, (16 energy)"}, // 16 mana
    {"Angel",   "4",    "10", "8",      "4-7",  "35", "50",  "1",    "45", "Flying, Resurrect"}, //mana 1
    //stronghold 5-8
    {"Peasant", "1",    "2", "3",       "1-2",  "4",  "5",  "12",   "4",  "Pays tax"},
    {"Slinger", "2",    "6", "3",       "2-3",  "10", "4",  "9",    "10", "Ranged (12 shots)"}, // all 1 or 2 level humans up to Slinger 5 gold
    {"Lancer",  "3",    "8", "6",       "3-7",  "14", "6",  "5",    "20", "Mutilate, Rage"}, // all 1 or 2 level humans up to Lancer 12 gold
    {"Knight",  "4",    "9", "8",       "5-9",  "30", "20", "1",    "40", "Unlimited retaliations, Jousting"}, // lancer or Wild horse up to Knight 20 gold
    //forest union 9-12
    {"Raven",   "1",    "2", "2",       "1-2",  "3",  "50", "16",   "3",  "Flying, Strike and return"},
    {"Druid",   "2",    "5", "6",       "2-4",  "14", "4",  "8",    "13", "Lightning, Healing, (12 energy)"}, // 12 mana
    {"Crab",    "3",    "7", "8",       "4-5",  "25", "6",  "3",    "18", "Double attack,Swimming"},
    {"Ent",     "4",    "10", "11",     "7-10", "45", "3",  "1",    "35", "Root, Nonliving, Regeneration"}, // Attack all adjacent enemies + wood 1
    //white tower 13-16
    {"Fighter", "1",    "4", "5",       "1-3",  "5",  "4",  "10",   "6",  "Luck"},
    {"Warrior", "2",    "6", "5",       "1-4",  "16", "6",  "8",    "11", "Double attack"},
    {"Mage",    "3",    "6", "5",       "3-5",  "15", "6",  "4",    "16", "Fireball, No melee penalty, (16 energy)"}, // 16 mana
    {"Rock",     "4",    "7", "7",       "6-12", "40", "50",  "1",  "30", "Flying, Luck"}, // + 1 mercury
    //special 17-20
    {"Hero", "", "", "",                "",     "",   "",   "",     "100", "Hero"},
    {"First aid tent", "","1","10",     "",     "50", "",   "1",    "60", "Heal 50 HP friendly units"}, // mana 8 + 1 mercury
    {"Wagon of ammunition", "", "1", "10", "",  "50", "",   "1",    "50", "Unlimited shots for units"}, // + 1 wood
    {"Ballista", "",    "9", "10",      "14-16","50", "",   "1",    "70", "Ranged (24 shots)"}, // + 1 wood
    //village 21-24
    {"Wild-met",  "1",  "2", "2",       "1-2",  "3",  "4",  "8",    "7",  "No enemy retaliation"},
    {"Barbarian", "2",  "7", "4",       "1-5",  "16", "6",  "5",    "14", "Rage"},
    {"Elf",       "3",  "5", "3",       "1-7",  "12",  "5",  "3",   "19", "Double shooting, Ranged (20 shots),Range retaliations"},
    {"Wild horse","4",  "7", "5",       "3-7",  "28", "30", "1",    "28", "Strike and return, Unlimited retaliations"},
};

const int static_unit_args[][5] = {
    /* version: 10 feb
       1 : morale group  &16 = some human &32 = bird &64 = horse &128 = wizards

       2 : shots count if = ranged or = magic: healing or Resurrect or fire ball or Drain life or lightning
    */

     /* version: 11 from 10 feb
       3: attack &1 = ranged &2 = No enemy retaliation &4 = Jousting &8 = Drain life &16 = Rage
            &32 = Healing & 64 = Resurrect &128 = Round attack &256 = lightning &384 = (128|256) fire ball

       4: after &1 = Strike and return &2 = Double attack &4 = Double shooting &8 = Regeneration &16 = Mutilate
            &32 = Root

       5: effects &1 = Flying &2 = Swimming &4 = Positive Morale &8 = Nonliving (ignore drain life and heal)
            &16 = Unlimited retaliations &32 = Range retaliations &64 = Luck &128 = No melee penalty
    */

    {1, 2, 3, 4, 5 },
    //red castle 1-4
    {17, 8, 1, 0, 0 },    {17, 0, 0, 0, 4 },    {129, 16, 40, 0, 0 },    {1, 1, 64, 0, 1 },
    //stronghold 5-8
    {18, 0, 0, 0, 0 },    {18, 12, 1, 0, 0 },   {18, 0, 16, 16, 0 },   {66, 0, 4, 0, 16 },
    //forest union 9-12
    {36, 0, 0, 1, 1 },    {132, 12, 288, 0, 0 },  {4, 0, 0, 2, 2 },     {4, 0, 0, 40, 8 },
    //white tower 13-16
    {24, 0, 0, 0, 64 },   {24, 0, 0, 2, 0 },    {136, 16, 384, 0, 128 },  {40, 0, 0, 0, 65 },
    //special 17-20
    {0, 0, 0, 0, 0 },     {0, 8, 32, 0, 0 },    {0, 0, 0, 0, 0 },     {0, 24, 1, 0, 0 },
    //village 21-24
    {36, 0, 2, 0, 0 },    {4, 0, 16, 0, 0 },     {4, 20, 1, 4, 32 },   {68, 0, 0, 1, 16 },
};

const char* static_unit_red_castle[][6] = {
    // archer 1 level
    {"S {) ",	    "S {) ",  " S{) ",	" (} s",	" (} s",	" (}s ",},
	{"+-[]\\", 	    "+-[]\\", "<=[]v",	"/[]-+",	"/[]-+", 	"v[]=>",},
	{"s JL ",	    "s jl ",  " sJL ",	" JL S",	" jl S", 	" JLS ",},
    // guard 2 level
	{"| {) " ,	    "| {) ", "\\ {) ",	" (} |",	" (} |", 	" (} /",},
	{"!/[()",	    "!/[()", " V[()",	"()]\\!",	"()]\\!",	"()]V ",},
	{"  JL ",	    "  jl ", "  JL ",	" JL  ",	" jl  ",	" JL  ",},
	// monk 3 level
    {"0 {\\ ",	    "0 {\\ ","0 {\\*",	 " /} 0",	" /} 0",	"*/} 0",},
	{"IV[]\\",	    "IV[]\\","IV[]V", 	 "/[]VI",	"/[]VI",	"V[]VI",},
	{"I JL ",	    "I jl ", "I JL ",	 " JL I",	" jl I",    " JL I",},
};
const char* static_unit_stronghold[][6] = {
    // peasant 1 level
    {"V {) ",	    "V {) ", "  {) ",	" (} V",	" (} V",	" (}  ",},
	{"IV[]\\",	    "IV[]\\",">-[]-",	"/[]VI",	"/[]VI",	"-[]-<",},
	{"I JL ",	    "I jl ", "  JL ",	" JL I",	" jl I",	" JL  ",},
    // slinger 2 level
    {"y {) ",	    "y {) ", "y {)/",	" (} y",	" (} y",	"\\(} y",},
	{" V[]\\",	    " V[]\\"," V[] ",	"/[]V ",	"/[]V ",	" []V ",},
	{"  JL ",	    "  jl ", "  JL ",	" JL  ",	" jl  ",	" JL  ",},
	// pikeman 3 level
    {"I {) ",	    "I {) ", "  {) ",	" (} I",	" (} I",	" (}  ",},
	{"IV[]\\",	    "IV[]\\","<-[]-",	"/[]VI",	"/[]VI",	"-[]->",},
	{"V JL ",	    "V jl ", "  JL ",	" JL V",	" jl V",	" JL  ",},
};
const char* static_unit_forest_union[][6] = {
    // raven 1 level
    {"   //",	    "  || ", "  || ",	"\\\\   ",  " ||  ",	" ||  ",},
	{"<@()<",	    "<@()<", "<@()<",	">()@>",	">()@>",	">()@>",},
	{"   \\\\",	    "  || ", "  || ",	"//   ",	" ||  ",	" ||  ",},
	// druid 2 level
    {"y {) ",	    "y {) ", "y {)@",	" (} y",	" (} y",	"@(} y",},
	{"IV[]\\",	    "IV[]\\","IV[]V",	"/[]VI",	"/[]VI",	"V[]VI",},
	{"I JL ",	    "I jl ", "I JL ",	" JL I",	" jl I",	" JL I",},
    // ent 4 level
    {"\\WYW/",  "\\WYW/", "YMYMY",  "\\WYW/",   "\\WYW/",   "YMYMY",},
    {"/III\\",  "/III\\", "\\III/",   "/III\\", "/III\\",   "\\III/",},
    {"_/n\\_",  "//^\\\\","_/n\\_", "_/^\\_",   "//^\\\\",  "_/^\\_",},
};
const char* static_unit_white_tower[][6] = {
    // swordsman 1 level
    {"| {) " ,	    "| {) ", "\\ {) ",	" (} |",	" (} |", 	" (} /",},
	{"!/[]\\",	    "!/[]\\"," V[]\\",	"/[]\\!",	"/[]\\!",	"/[]V ",},
	{"  JL ",	    "  jl ", "  JL ",	" JL  ",	" jl  ",	" JL  ",},
    // warrior 2 level
	{"| YY " ,	    "| YY ", "\\ YY ",	" YY |",	" YY |", 	" YY /",},
	{"!/TT\\",	    "!/TT\\"," VTT\\",	"/TT\\!",	"/TT\\!",	"/TTV ",},
	{"  JL ",	    "  jl ", "  JL ",	" JL  ",	" jl  ",	" JL  ",},
	// mage 3 level
    {"@ {) ",	    "@ {) ", "0 {) ",	" (} @",	" (} @",	" (} 0",},
	{"IV[]\\",	    "IV[]\\","IV[]\\", 	"/[]VI",	"/[]VI",	"/[]VI",},
	{"I JL ",	    "I jl ", "I JL ",	" JL I",	" jl I",	" JL I",},
};

const char* static_unit_wild[][6] = {

    // птенец птицы рок 1 лвл
    {"<@ //",	"<@ //", "<@ V ",	"\\\\_@>","\\\\_@>",    " V_@>" },
	{" (V) ",	" (V) ", " (v) ",	" (V) ",	" (V) ",	" (v) " },
	{" / \\ ",  " | | "," / \\ ",	" / \\ ",	" | | ",	" / \\ "},
    // варвар 2 лвл
	{"[H]pq",	"[H]pq", "   pq",	"pq[H]",    "pq[H]",    "pq   " },
	{" |V[]",	" |V[]", " |/[]",	"[]V| ",	"[]V| ",	"[]\\| "},
	{"   JL",   "   jl", "[H]JL",	"JL   ",	"jl   ",	"JL[H]" },
    //elf 3 лвл
	{"S {Y ",	 "S {Y ",  " S{Y ",	" Y} s",	" Y} s",	" Y}s ",},
	{"+-[]\\", "+-[]\\", "<=[]v",	"/[]-+",	"/[]-+", 	"v[]=>",},
	{"s JL ",	 "s jl ",  " sJL ",	" JL S",	" jl S", 	" JLS ",},
};


const char* static_big_unit_translate[] = {
    "OO~", "==6", "##~", "JJ~", "LL~", "<<7", ">>7", "@@~", "\0\0\0"
};

const char* static_big_units[][6] = {
    //hero
    { "     {)   ",	 "     {)   ",      "",	  "   (}     ",	"   (}     ",""},
	{" //\\_J)_  ",	" //\\_J)_  ",	    "", "  _(L_/\\\\ ",	"  _(L_/\\\\ ",""},
	{"   /|J /\\ ",	"   /\\J |\\ ",	    "", " /\\ L|\\   ",	" /| L/\\   ",""},
	//forest union crab 3 level ~3
    {" ()o__o() ",	"(_)o__o(_)",	    "", " ()o__o() ",	"(_)o__o(_)",""},
	{"  \\/__\\/  ","  V/__\\V  ",	    "", "  \\/__\\/  ",	"  V/__\\V  ",""},
	{"  /|\\/|\\  ","  /|\\/|\\  ",	    "", "  /|\\/|\\  ",	"  /|\\/|\\  ",""},
	//white tower roc 4 level ~6
    {"//\\\\V//\\\\ "," _  V  _  ",	    "", " //\\\\V//\\\\","  _  V  _ ",""},
	{"   | |    "	, "//\\| |/\\\\ ",  "","    | |   ",	 " //\\| |/\\\\",""},
	{"  _/^\\_   ",	  "  _/^\\_   ",    "","   _/^\\_  ",	 "   _/^\\_  ",""},
	//stronghold Sperman 4 level ~9
    { " <===q=== ",	 "<====q==  ",  "",	" ===p===> ",	  "  ==p====>",""},
	{" //\\_J)_  ",	" //\\_J)_  ",  "", "  _(L_/\\\\ ", "  _(L_/\\\\ ",""},
	{"   /|J /\\ ",	"   /\\J |\\ ",	  "", " /\\ L|\\   ", " /| L/\\   ",""},
	//red castle angel 4 level ~12
    {" | {)/\\\\  "," \\ {)//\\  ",	    "","  //\\(} | ",	"  /\\\\(} / ",""},
	{" !/[]//\\\\ ","  V[]///\\ ",	    ""," //\\\\[]\\! "," /\\\\\\[]V  ",""},
	{"   ||     ", "   ||     ",	    "","     ||   ",	"     ||   ",""},

	/* tent 15 */
	{"  _/()\\_  ",	 "  _/()\\_  ","", "  _/()\\_  ",	"  _/()\\_  ",""},
	{" (  __  ) ",	 " (  /\\  ) ","", " (  __  ) ",	" (  /\\  ) ",""},
	{" /_/  \\_\\ ", " /_/  \\_\\ ",""," /_/  \\_\\ ",	" /_/  \\_\\ ",""},
     /* vagon 18 */
	{"  ______  ",	"  ______  ",   "","  ______  ",	"  ______  ",""},
	{" [__##__] ",	" [_#__#_] ",	""," [__##__] ",	" [_#__#_] ",""},
	{" (O)==(O) ",	" (O)==(O) ",	""," (O)==(O) ",	" (O)==(O) ",""},
	/* ballista 21*/
	{"          ",	"          ",	"","          ",	"          ",""},
	{" @======= ",	"@=======  ",	""," =======@ ",	"  =======@",""},
	{" (O)==(O) ",	"  (@)==(@)",	""," (O)==(O) ",	"(@)==(@)  ",""},

	/* wild horse 4 level */
	{" //\\___   ",	" //\\___   ",	    "","   ___/\\\\ ","   ___/\\\\ ",""},
	{"   ( _ )\\ ",	"   ( _ )\\ ",	    ""," /( _ )   ",	" /( _ )   ",""},
	{"   /| /\\  ",	"   /\\ |\\  ",	    "","  /\\ |\\   ",	"  /| /\\   ",""},
};

const char* static_unit_dead[][6] = {
    {"          ",      "          ",   "          ",   "          ",   "     ",    "     "},
    {"  _/()\\_  ",     " ___##___ ",   " (O)==(O) ",   "  ______  ",   " ___ ",    "/III\\"},
    {" /_/\\/\\_\\ ",   "/(O)==(O)\\",  " =======@ ",   " /______\\ ",  "/___\\",   "//^\\\\"},
};

bool static_unit_is_huge(int v){
    if(v == HERO) return true;
    if(v == (FOREST_UNION|UNIT|LEVEL_3)) return true;
    if(v == (FOREST_UNION|UNIT|LEVEL_4)) return false;

    if (static_unit_is_machine(v)) return true;
    if((v & (15|UNIT)) == (LEVEL_4|UNIT)) return true;
    return 0;
}

int static_unit_is_machine(int v){
    if((v & BALLISTA) == BALLISTA) return BALLISTA;
    if((v & TENT) == TENT) return TENT;
    if((v & WAGON) == WAGON) return WAGON;
    return 0;
}

int static_draw_unit(const char* row){
    for(int i=0;i<10;i++){
        char c = row[i];
        if(c == 0) return i;
        for(int j=0;j<100;j++){
            char t = static_unit_translate[j][0];
            if (t == 0){
                draw_char(c, '0');
                break;
            }
            if (t == c){ //todo replace color ~ to fraction color
                draw_char(static_unit_translate[j][1], static_unit_translate[j][2]);
                break;
            }
        }
    }

    return 10;
}

int static_draw_big_unit(const char* row){
    for(int i=0;i<10;i++){
        char c = row[i];
        if(c == 0) return i;
        for(int j=0;j<100;j++){
            char t = static_big_unit_translate[j][0];
            if (t == 0){
                draw_char(c, '0');
                break;
            }
            if (t == c){ //todo replace color ~ to fraction color
                draw_char(static_big_unit_translate[j][1], static_big_unit_translate[j][2]);
                break;
            }
        }
    }

    return 10;
}

const char* static_unit_get_big(const char** row, int anim){
    int a = anim;
    if(anim == AN_ATACK_L) a = AN_MOVE_L;
    else if(anim == AN_ATACK_R) a = AN_MOVE_R;
    return row[a];
}

const char* static_unit_get_row(int unit, int row, int anim){
    if(anim == AN_DEAD){
        if (unit == TENT) return (static_unit_dead[row][0]);
        if (unit == WAGON) return (static_unit_dead[row][1]);
        if (unit == BALLISTA) return (static_unit_dead[row][2]);
        if(static_unit_is_huge(unit)) return (static_unit_dead[row][3]);
        if (unit == (FOREST_UNION|UNIT|LEVEL_4)) return (static_unit_dead[row][5]); //ent
        return (static_unit_dead[row][4]);
    }

    if (unit == (RED_CASTLE|UNIT|LEVEL_1)) return (static_unit_red_castle[row][anim]);
    if (unit == (RED_CASTLE|UNIT|LEVEL_2)) return (static_unit_red_castle[row + 3][anim]);
    if (unit == (RED_CASTLE|UNIT|LEVEL_3)) return (static_unit_red_castle[row + 6][anim]);
    if (unit == (RED_CASTLE|UNIT|LEVEL_4)) return static_unit_get_big(static_big_units[row + 12], anim);

    if (unit == (STRONHOLD|UNIT|LEVEL_1)) return (static_unit_stronghold[row][anim]);
    if (unit == (STRONHOLD|UNIT|LEVEL_2)) return (static_unit_stronghold[row + 3][anim]);
    if (unit == (STRONHOLD|UNIT|LEVEL_3)) return (static_unit_stronghold[row + 6][anim]);
    if (unit == (STRONHOLD|UNIT|LEVEL_4)) return static_unit_get_big(static_big_units[row + 9], anim);

    if (unit == (FOREST_UNION|UNIT|LEVEL_1)) return (static_unit_forest_union[row][anim]);
    if (unit == (FOREST_UNION|UNIT|LEVEL_2)) return (static_unit_forest_union[row + 3][anim]);
    if (unit == (FOREST_UNION|UNIT|LEVEL_3)) return static_unit_get_big(static_big_units[row + 3], anim);
    if (unit == (FOREST_UNION|UNIT|LEVEL_4)) return (static_unit_forest_union[row + 6][anim]);

    if (unit == (WHITE_TOWER|UNIT|LEVEL_1)) return (static_unit_white_tower[row][anim]);
    if (unit == (WHITE_TOWER|UNIT|LEVEL_2)) return (static_unit_white_tower[row + 3][anim]);
    if (unit == (WHITE_TOWER|UNIT|LEVEL_3)) return (static_unit_white_tower[row + 6][anim]);
    if (unit == (WHITE_TOWER|UNIT|LEVEL_4)) return static_unit_get_big(static_big_units[row + 6], anim);

    if (unit == (VILLAGE|UNIT|LEVEL_1)) return (static_unit_wild[row][anim]);
    if (unit == (VILLAGE|UNIT|LEVEL_2)) return (static_unit_wild[row + 3][anim]);
    if (unit == (VILLAGE|UNIT|LEVEL_3)) return (static_unit_wild[row + 6][anim]);
    if (unit == (VILLAGE|UNIT|LEVEL_4)) return static_unit_get_big(static_big_units[row + 24], anim);

    if (unit == HERO) return static_unit_get_big(static_big_units[0 + row], anim);
    if (unit == TENT) return static_unit_get_big(static_big_units[15 + row], anim);
    if (unit == WAGON) return static_unit_get_big(static_big_units[18 + row], anim);
    if (unit == BALLISTA) return static_unit_get_big(static_big_units[21 + row], anim);

    int id = static_unit_get_id(unit);
    if(id > 0 && id < 25 && (unit & TEXT)){
        if(row < 10) return (static_unit_text[id][row]);
        if(row < 15) return (static_unit_upgrade[id][row-10]);
    }
    return "";
}

// c = constant
int static_draw(int unit, int row, int anim){
    if(anim == AN_DEAD){
        return static_draw_unit(static_unit_get_row(unit, row, anim));
    }
    if(unit & TEXT){
        return draw_text(static_unit_get_row(unit, row, anim), anim);
    }
    if(static_unit_is_huge(unit)){
        return static_draw_big_unit(static_unit_get_row(unit, row, anim));
    }
    return static_draw_unit(static_unit_get_row(unit, row, anim));
}

int static_unit_get_value(int unit_id, int val){
    if(unit_id < 1 || unit_id > 24) return 0;

    if(val > 9){
        return static_unit_args[unit_id][val - 10];
    }

    int n = 0;
    for(int i=0;i<3;i++){
        char c = static_unit_text[unit_id][val][i];
        if(c == 0) return n;
        n *= 10;
        n += c - '0';
    }

    return 0;
}

pair* static_unit_get_damage(int unit_id){
    int f = 0;
    int s = 0;
    int r = 0;
    if(unit_id > 0 && unit_id < 25){
        for(int i=0;i<5;i++){
            char c = static_unit_text[unit_id][4][i];
            if(c == 0) {
                return pair_factory(f, s, 0);
            }
            if(c == '-'){
                r++;
                continue;
            }
            if(r == 0){
                f *= 10;
                f += c - '0';
            }else{
                s *= 10;
                s += c - '0';
            }
        }
    }
    return pair_factory(f, s, 0);
}

int static_unit_get_city_archer(int city){
    if(city & RED_CASTLE) return RED_CASTLE | UNIT | LEVEL_1;
    if(city & STRONHOLD) return STRONHOLD | UNIT | LEVEL_2;
    if(city & FOREST_UNION) return FOREST_UNION | UNIT | LEVEL_2;
    if(city & WHITE_TOWER) return WHITE_TOWER | UNIT | LEVEL_3;
    return VILLAGE | UNIT | LEVEL_3;
}

int static_unit_growl(int unit_type){ return static_unit_get_value(static_unit_get_id(unit_type), 7); }

int static_unit_get_id(int unit_type){
    int n = 0;
    if(unit_type & LEVEL_4) n = 4;
    if(unit_type & LEVEL_3) n = 3;
    if(unit_type & LEVEL_2) n = 2;
    if(unit_type & LEVEL_1) n = 1;
    if(n == 0) return 0;
    if(unit_type & SPECIAL) return n + 16;
    if(unit_type &  VILLAGE) return n + 20;
    if(unit_type & STRONHOLD) return n + 4;
    if(unit_type & RED_CASTLE) return n + 0;
    if(unit_type & WHITE_TOWER) return n + 12;
    if(unit_type & FOREST_UNION) return n + 8;
    return 0;
}

void static_unit_test(int anime){
    int a = (anime > AN_ATACK_R) ? AN_DEAD : anime;
    for(int i = 0; i <6; i++){
        int t = VILLAGE << i;
        t |= UNIT;
        for(int r=0;r<3;r++){
            int total = 0;
            for(int j = LEVEL_1; j<= LEVEL_4; j++){
                total += static_draw(t | j, r, a);
                draw_char(':', 0);
            }
            draw_int(total);
            next_row();
        }

        {
            t |= TEXT;
            int total = 0;
            for(int j = LEVEL_1; j<= LEVEL_4; j++){
                total += static_draw(t | j, 0, 0);
                draw_char(':', 0);
            }
        }
        next_row();
    }
}

