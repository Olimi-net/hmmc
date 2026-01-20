#include "static_map.h"
#include "../modules/hmmc_error.h"

const char *static_first_map[] = {
"tuu^t q ^ ^ ^tuu p v~  ^ ^ ^ q ^ ^tuut/\\/\\u^ q ^ p:","tu^ ^ q ^ ^ ^ ^t pt v^ ^ ^ q ^ ^t t  / /  \\ \\  ^ q ^ p:",
" ||^ x || s x x x || ^ pt  ~ ~ ~x x x s x x || t / /t \\ \\ x s x p:","  t ||wwu  ^twx e || =======www t  ^t ||/ / //\\\\ \\ \\tw t  tf:",
"u||wwuu  ^twwwwwwwu  t  ^t  / / ///\\\\\\ \\ \\/\\w t  twu  :","twww x ||wwwww#u ^ ^tu  ||  x || / / ////\\\\\\\\t||w  ||u||wu  :",
"tuu#uu^twwu ^ ^ ^w========w^ ^ ^t q ^////  \\\\\\\\^uu  ^tw ^:"," ||wu ^twwu ^ ^ ^wv~ ~uu  x x || ^ q ^w ||tuu  ^tw ^:",
" twux ||wwu x x xuutuv~wwu x s xwu ||uu  x ||w x:","  ^wu  ^ ^ ^ ^ ^ww |[]|(*)v~wwwww  #wuu^wutu :",
"  ||twu  ^ ^ ^ ^ ^wwwv~uu  ^wwwwwu ^wutu :","t  ||tw  x x x x xu  #wwu v~u t^ ^ ^w ^tww xwu ||t :",
"tu tww^twwwuu^ ^v~ ~tx ^ ^ ^ ^ ^uu ^uu^w ^ t :"," ||t  ||twuu^twwuu^ ^ ^t v~ ~||uux x ^t x ||uu ^uu^w ^  || :",
"utu  ^uu#u  x ||u #wuu ^ ^ x xvv~  p x ||wuu xuuxuut xu  :","u ||t  || ^w ^wuu  ^t x ||  ^tuvv~ p ^ww  twwtqt:",
"uutu  ||tw ^w  ^ ^ ||w  ^ ^ vv p ^ww^ww ||qt:","  t  ||uu  ||w xuu  ^t xwu  x |||[]|(*)v~ ~ d xuu tuu t ||wuu  t s ||:",
"^t qtwwut xwuu #ww=========wwuu  ^uu  ||u^ ^ ^twu:","/\\ ||t qtwwu ||wuu twwwwww  t ||uutu ^ ^ ^ ||wu:",
"||u || s ||wwwwu  ^wwwwww||w ^ x x xwuu:"," ^wwwwww || ^ ^ ^t ========^ ^ ^u  ^wu|| ^ ^uutwuu:",
" ^ww ^tww  ^ ^|| ^ ^ ^v~  ^ ^ ^u  ^wuu|| ^ ^ ^wuu:"," xwu ^ ^ ^twu  ^ ^ux x x v~ x x xu  xuu #w x || ^ ||uutut:",
" tw^ ^ x || ^ ^uu  x x q ^t t v t qtuu ^wwuu  xw ^t:"," tw^ xw x ^wu  ^ q ^t |[]|(*)~ ~ t qtuu ^wwwwu  ||t ||:",
"  ||wx  putxwu  ^ s x ||uu  =====|| s ||uu xw  ^ ^wu #uu  ||t:","tw  ^ p ^w #uu  xwu^wwww^ ^ ^ ^ ^twwt:",
"tw  ^ pt ||wwww^wwwu #u ^ ^t x x ^ ^wu ||:"," ||w  x b ||wwwwuxuu  ,=====^ ^tuu  x x || twx ^twu:",
"wwwwwu ^w  tww t~ ~  ^ ^tuu  t q ^wuux ^w:","wwwww ^tw^twuutvx x ||uu  t qt ||wwu||tw:",
"wwwwuu ^ ^tu  t || ^tw(\\vt q ^uu|| s ||www||tuu:","t q ^ ^uu ^ ^t x || ^t||u || ^w \\ ~ ~ ~t q ^uutww ^ ^w  ^t:",
"t q ^ ^uu ^ ^ ||w || ^ qxu #u t ~ ~  || s xuutww ^ ^w  || ^:"," || s x xuu x xwuu x q ^uu t ~ ~ ~wwuu ||tu ^uu x xtw  x:",
"  twwwwwwu  s ^uut v~wwutu^ ^uut qtw tu:","  twwwwuu  #wwu  x______/\\ v~ ~uu #wutu||t xuut q ||/\\w^:",
"u||twwwwwwwut\\\\\\\\\\\\\\\\ v~ ~ ~wu  ^ || ^|| tw|| st/\\w||t:","/\\u ^ ^ ^ ^ ^ ^twww  tvvv~ ~w  ^tu ^utwtuu  t||wt||:",
"/\\u || ^ ^ ^ ^ ^ ^ ^ ^wuu tvvv~ ~ ~wt xu xt ||wtw||w  t  :","||uu x x x x x x ^ ^wu ___/\\vvvv~w|| ptwt ||wwu  ||  :",
" qwwwwx xw __/\\\\\\\\vvvv~ ~w  p ||wt  twuu#uut:"," qwwwwwuu ____/\\\\\\ vvvvv~w pwu ||  twwu t:",
" swu_____________wu#wut\\\\\\\\ vvvvv~ ~ ~wcw  tu ||u  _____wu ||:",
"ww__/\\\\\\\\\\\\\\V/////\\__ww____/\\ vvvvvv~ ~'\\wwwuu ^u____/\\\\V//\\___w  :",
"w________/\\ \\ vv///\\wuut\\\\\\\\vvvvvvv~/\\__wwuu  t ||t t\\\\\\\\~ ~ ~////\\__uu  :",
"________/\\\\\\\\\\\\\\\\\\ vvvvvvvvvvvvvv~///\\___wwu t  __||_/\\vv~ ///\\_______:",
"||||\\\\\\\\\\ vvvvvvvvvv~ ~ ~q vvv~ ~ ~////\\______wu____||_/\\\\\\\\\\\\vvv/////|||:",
" vvvvvvvvvvvv~ qvvvv~ ~ ///////\\__________/\\\\\\\\\\\\\\\\ vvvvv~:",
"vvvvvvvvvvvv~ ~r vvvvv~ ~ /////||||\\\\\\vvvvvv~ ~ :"," vvvvvvvvvvvvvvvvvvvvvvvvvvvv~:",
"vvvvvvvvvvvvvvvvvvvvvvvvvvvv~ :"," vvvvvvvvvvvvvvvvvvvvvvvvvvvv~:",
"vvvvvvvvvvvvvvvvvvvvvvvvvvvv~ :"," vvvvvvvvvvvvvvvvvvvvvvvvvvvv~:",
"vvvvvvvvvvvvvvvvvvvvvvvvvvvv~ :"," vvvvvvvvvvvvvvvvvvvvvvvvvvvv~:",
"vvvvvvvvvvvvvvvvvvvvvvvvvvvv~ :"," vvvvvvvvvvvvvvvvvvvvvvvvvvvv~:",
"vv~q vvvv~ ~ ~_~_~/w\\vvvvvvvvvvvvvvvvv~ :"," vvqvvvv~ ~_~/wu ^vvvvvvvvvvvvvvvv~ ~ ~:",
"vv~r vvvv~/wuu ^t~^~/\\ vvvvvvvvvvvvvv~ ~ :"," vvvvvvv~ ~/wuu  x ^~^vvvvvvvvvvvvvv~ ~ ~:",
"vvvvvvv~ ~/u t qtu  x x_~_vvvvvvvvvvvvvv~ :"," vvvvvvv~(u  t qt ~ ~ p \\ vvvvvvvvvvvvv~ ~ ~:",
"vvvvvvv~tu  || s ||~ ~ ~p  ) vvvvvvvvvvvvv~ ~ :"," vvvvvvv~twu  ^~ ~ gtvvvvvqvvvvvvv~ ~:",
"vvvvvvv~ ~t____w^wuu  tvvvvv~q vvvvvvv~ :"," vvvvvvv~ ~ /////\\uu  xwuu_tvvvvv~ rvvvvvvv~ ~:",
"vvvvvvvv~ ~tww ______t\\\\vvvvvvvvvvvvvv~ :"," vvvvvvvv~ ~t_________________/\\\\\\\\\\\\\\\\vvvvvvvvvvvvvv~ ~ ~:",
"vvvvvvvvv/////////|||||\\\\\\\\\\ vvvvvvvvvvvvvvvv:"," vvvvvvvvvvvvvvvvvvvvvvvvvvvv~:",
"vvvvvvvvvvvvvvvvvvvvvvvvvvvv~ \0"
};

/*
    a - aqua <-> white(yellow/red) / dark ;
    u - green / sunrise(yellow) / dark;
    s - green / sunset(red) / dark
*/

const char *static_map_replace[] = {
    ".abcdefghijklmnopqrs^xtuvw",
     "RANDOM__$CITY$",
     "RANDOM_0$CITY$",
     "RANDOM_1$CITY$",
     "RANDOM_2$CITY$",
     "RANDOM_3$CITY$",
     "RANDOM_4$CITY$",
     "RANDOM_5$CITY$",
     "RANDOM_6$CITY$",
     "RANDOM_7$CITY$",
     "RANDOM_8$CITY$",
     "RANDOM_9$CITY$",
     "THE_STRONGHOLD",
     "THEFORESTUNION",
     "THE_RED_CASTLE",
     "POSTWHITETOWER",
     "XXXXXXXXXXXXXX",
     "XXXXXXXX",
     "SEACRASH",
     "VILLAGES",
     "/\\ /\\",
     "|| ||",
     " /\\",
     "   ",
     "~ ~ ~ ",
     "         ",
     ""
};

const char* static_map_places[][3] = {
    { " ~ ~ ~ ~",
      "~ ~ ~ ~ ",
      " ~ ~ ~ ~" }, // water replace SEACRASH
    { " ~ ~ \\/~",
      "~ |\\ /\\ ",
      " ~ ~ ~ ~" }, // SEACRASH as village
     {" ~\\/ ~ ~",
      "~ /\\~/| ",
      " ~ ~ ~ ~" }, // mirror
};

const char* static_map_search[] = {
    "",
    "RANDOM__$CITY$",
    "RANDOM_0$CITY$",
    "RANDOM_1$CITY$",
    "RANDOM_2$CITY$",
    "RANDOM_3$CITY$",
    "RANDOM_4$CITY$",
    "RANDOM_5$CITY$",
    "RANDOM_6$CITY$",
    "RANDOM_7$CITY$",
    "RANDOM_8$CITY$",
    "RANDOM_9$CITY$",
    "THE_STRONGHOLD",
    "THEFORESTUNION",
    "THE_RED_CASTLE",
    "POSTWHITETOWER",
    "XXXXXXXXXXXXXX", //16
    "XXXXXXXX", //17
    "SEACRASH",
    "///  \\\\\\", // убежище
    "VILLAGES", //
    "|[]|(__)", //laboratory
    "(__)|[]|", //laboratory
    "i==i|[]|", //peasant
    "|[]|i==i", //peasant
    "<<==|[]|", //sawmill
    "|[]|==>>", // sawmill
    "|/[][]\\|", // raven
    " |[][]| ", // tavern
    " _|[]|_ ", // barracks 9 B 8 >> E
    "/__[]__\\", // bastion 10 B 16 >> K
	"|[][][]|", // machinery 11 B 32 >> W
	"[=]  [=]", // armory 12 B 64 >> N
	"|[]||[]|", // market 13 A 16 >> M
	"  \\[]/  ", // lyceum 14 A 32 >> L
	"|[]|(*)~",
	"~(*)|[]|", // water mill 16 A 64 + (0|1) >> T t
	"",
};


draw_buffer* unzip_map(int i){
    draw_buffer* buf = new draw_buffer(1024, 512, '\0');
    buf->clear(511);

    char clrs[10];
    char rpls[20];
    int rcnt = 0;
    int ccnt = 0;

    for(int j=0; j < 512; j++){
        for(int i=0; i < 1024; i++){
            char c = static_first_map[j][i];
            if(c == '\0') {
                j = 512;
                break;
            }
            if (c == ':'){
                buf->next_row();
                break;
            }
            rcnt = ccnt = 0;
            int u = 0;

            for(int n=1; n<40; n++){
                char f = static_map_replace[0][n];
                if (f == '\0') break;
                if (f != c) continue;
                u = 1;
                for(int k=0; k<120; k++){
                    char t = static_map_replace[n][k];
                    if(t == '\0'){
                        n = 20;
                        break;
                    }else if(t == '\n'){
                        u = 2;
                        continue;
                    }
                    if(u==1){
                        rpls[rcnt++] = t;
                    }else if(u==2){
                        clrs[ccnt++] = t;
                    }
                }
                if(u){
                    for(int r=0;r<rcnt;r++){
                        int l = (r < ccnt) ? clrs[r] : '\0';
                        if(!buf->add_char(rpls[r], l))
                            break;
                    }
                    break;
                }
            }

            if(!u){
                buf->add_char(c, '\0');
            }
        }
    }
    return buf;
}

draw_buffer* load_map(int i){
    draw_buffer* buf = unzip_map(i);

    if(buf->get_cursor_x() >= 1023 && buf->get_cursor_y() >= 511)
        return buf;
    int x = buf->get_cursor_x();
    int y = buf->get_cursor_y();
    draw_buffer* new_buf = new draw_buffer(x, y, '\0');
    new_buf->clear(511);
    for(int j=0; j < y; j++){
        for(int i=0; i < x; i++){
           if(!new_buf->add_char(buf->get_char(i, j), buf->get_clr(i, j))) break;
        }
        new_buf->next_row();
    }
    delete buf;
    return new_buf;
}

pair city_finder(draw_buffer* buf, int x, int y){
    pair p = {0, 0, 0};
    int w = buf->get_width();
    int h = buf->get_height();
    for(int q=y; q < h; q++){
        for(int j=x; j < w; j++){
            for(int k = 1; k <= 40; k++){
                for(int i = 0; i < 20; i++){
                    char c = static_map_search[k][i];
                    if(c == 0){
                        p.value = j;
                        p.subval = q;
                        p.state = k;
                        return p;
                    }
                    if(i + j >= w) break;
                    if(buf->get_char(i + j, q) != c) break;
                }
            }
        }
    }
    return p;
}




/*
the_red_castle
theforestunion
postwhitetower

the_stronghold

//todo случайные:
random_1$rfps$ // случайный город из 4х - 1 - не совпадает
random_2$rfps$ // случайный город из 4х - 2 - не совпадает
random_3$rfps$ // случайный город из 4х - 3 - не совпадает
random_4$rfps$ // случайный город из 4х - 4 - не совпадает
random__$city$ // случайный город - может совпадать


random__1$rps$ // случайный город из 3х - 1 - не совпадает
random__2$rps$ // случайный город из 3х - 2 - не совпадает
random__3$rps$ // случайный город из 3х - 3 - не совпадает

villages 14 случайные: (исключая мельницы)
    лесопилка(Л/Р) // 2
    лаборатория(Л/Р) // 2
    мастерская  // 1
    крестьяне (Л/Р) // 2
    FOREST_UNION // 1
    armory // 1
    market // 1
    library // 1
    inn // 1
    WHITE_TOWER // 1
    RED_CASTLE // 1
*/

