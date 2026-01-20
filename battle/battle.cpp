#include "battle.h"
#include "../legacy/drawing.h"
#include "../legacy/book_of_magic.h"
#include "../keyboard.h"
#include "battle_const.h"
/**

flag if have wall = BATTLE_CITY else BATTLE_ACTION

*/
battle::battle(group *attack, group * defence, float difficult, int flag):hmmc_callback(){
    this->game_args = new battle_args(flag, draw_display_width(), draw_display_height());
    game_args->info_left(attack->get_faction(), attack->get_hero_val_or_def(HERO_ATTR_ID, 0));
    game_args->info_right(defence->get_faction(), defence->get_hero_val_or_def(HERO_ATTR_ID, 0));
    ground = new battle_back(game_args);

    this->is_back = false;
    this->move_obj = quadra_factory(0, 0, 0, 0);

    this->field = new battle_field(game_args->columns, game_args->rows, attack, defence, difficult, this);
    this->field->init(game_args->unit_arg);
    this->field->start_line(game_args->get_start_column(false) , game_args->get_start_column(true));
    if(flag & BATTLE_CITY){
        this->field->add_city_unit(static_unit_get_city_archer(flag & ALL_CITIES), game_args->left, game_args->top);
        this->field->add_city_unit(BTL_UNIT_GATE, game_args->left, game_args->top); // gate
        if((flag & ALL_CITIES) != FOREST_UNION){
            this->field->add_city_unit(BTL_UNIT_WALL, game_args->left, game_args->top); // wall
        }
        this->field->add_city_unit(BTL_UNIT_CATAPULT, game_args->left, game_args->top); // catapult
    }


    this->ai.state = flag & 3;
    this->ai.value = this->ai.subval = 0;

    /**todo
        is show  initiative as text and/or as icons
        open dialog if have tactics and using tactics
        create initiative */
}

battle::~battle(){
    delete ground;
    quadra_free(this->move_obj);
    delete this->field;
    delete game_args;
}

int battle::fast_event(int e, int a){
    if(this->move_obj->e4 == e){
        this->move_obj->e3 >>= 1;
        if(this->move_obj->e3 == 0){
            this->move_obj->e4 = 0;
            return 0;
        }
    }

    this->move_obj->e4 = e;
    this->move_obj->e3 = a;
    return 1;
}

//event from battle animation
int battle::on_callback(hmmc_event *e, int a){
    if(e->id == BA_OTHER_EVENT){
        ground->on_anime_flag(8);
        return 0;
    }
    if(e->id == BA_HERO_ATTACK){
        event_value = BA_HERO_ATTACK;
        ///todo animation for hero
        return 0;
    }
    if(e->id == BA_HERO_RANGE){
        event_value = BA_HERO_RANGE;
        ///todo animation for hero
        return 0;
    }
    if(e->id == BA_RANGE_ARROW){
        event_value = BA_RANGE_ARROW;
        return this->move_arrow(BA_RANGE_ARROW);
    }
    if(e->id == BA_MOVE){
        event_value = BA_MOVE;
        int step = this->move_unit(BA_MOVE, false);
        if (step < 1){
            this->field->update_pos_for_unit(a);
        }
        return step;
    }
    if(e->id == BA_NEGATIVE_MAGIC){
        event_value = BA_NEGATIVE_MAGIC;
        if(a & UNIT_ATTACK_FIREBALL){
            if(this->move_arrow(BA_RANGE_ARROW)) return 1;
            if(this->move_obj->e4 == (UNIT_ATTACK_FIREBALL | BATTLE_EFFECT)){
                this->move_obj->e4 = 0;
                return 0;
            }else{
                pair p = this->field->get_item(this->field->two_id())->unit->get_pos();
                this->move_obj->e4 = (UNIT_ATTACK_FIREBALL | BATTLE_EFFECT);
                this->move_obj->e1 = p.value;
                this->move_obj->e2 = p.subval;
                return 1;
            }
        }else if(a & UNIT_ATTACK_LIGHTNING){
            return this->fast_event(UNIT_ATTACK_LIGHTNING | BATTLE_EFFECT, 1);
        }else if(a & UNIT_ATTACK_DRAIN_LIFE){
            ///todo animation drain life
        }
        return 0;
    }
    if(e->id == BA_POSITIVE_MAGIC){
        event_value = BA_POSITIVE_MAGIC;
        ///todo animation heal or ressurect
        return 0;
    }
    if(e->id == BA_LOW_MORALE){
        event_value = BA_LOW_MORALE;
        return this->fast_event(BA_LOW_MORALE, 1);
        ///todo animation low morale
        return 0;
    }
    if(e->id == BA_UP_MORALE){
        event_value = BA_UP_MORALE;
        return this->fast_event(BA_UP_MORALE, 1);
        ///todo animation up morale
        return 0;
    }
    if(e->id == BA_UP_LUCK){
        event_value = BA_UP_LUCK;
        return this->fast_event(BA_UP_LUCK, 1);
        ///todo animation up luck
        return 0;
    }
    if(e->id == BA_LOW_LUCK){
        event_value = BA_LOW_LUCK;
        return this->fast_event(BA_LOW_LUCK, 1);
        ///todo animation low luck
        return 0;
    }
    return 0;
}

int battle::move_unit(int v, bool r){
    if(this->move_obj->e4 != v){
        this->move_obj->e4 = v;
        this->move_obj->e3 = 0;
    }
    if(this->move_obj->e3 == 0){
        pair p = this->field->unit_move();
        if(p.state == 0){
            this->move_obj->e4 = 0;
            return 0;
        }
        this->move_obj->e3 = 1;
        this->move_obj->e3 |= p.state & 2; //fly
        this->move_obj->e1 = p.value * 5;
        this->move_obj->e2 = p.subval * 3;
    }

    {
        hmmc_queue_item* q = this->field->get_item(this->field->one_id());
        int step = q->unit->move(this->move_obj->e1, this->move_obj->e2);
        if(step > 0) return step;
        if(step < 0){
            this->move_obj->e4 = 0;
            return step;
        }

        if(r){
            this->move_obj->e4 = 0;
            return 0;
        }else{
            this->move_obj->e3 = 0;
            return this->move_unit(v, true);
        }
    }
    this->move_obj->e4 = 0;
    return 0;
}

bool battle::calc_ammo(int val){
    //готовим стрелу к полёту
        hmmc_queue_item* q1 = this->field->get_item(this->field->one_id());
        hmmc_queue_item* q2 = this->field->get_item(this->field->two_id());
        if(q1 == NULL || q2 == NULL) return false;
        pair p1 = q1->unit->get_pos();
        pair p2 = q2->unit->get_pos();
        int angle = 1; // arrow
        int speed = 20; //fast
        if (q1->unit->get_type() == BALLISTA){ angle = 3; }
        else if (q1->unit->get_type() == (STRONHOLD | UNIT | LEVEL_2)){ angle = 2; speed = 12; } // Slinger
        else if (q1->unit->get_type() == (WHITE_TOWER | UNIT | LEVEL_3)) angle = 4; // MAGE
        else if (q1->unit->get_type() == (VILLAGE | UNIT | LEVEL_2)){ angle = 5; speed = 12; } // Barbarian
        else if (q1->unit->get_type() == (VILLAGE | UNIT | LEVEL_3)){ angle = 1; speed = 20; } // elf
        else if (q1->unit->get_type() == (RED_CASTLE | UNIT | LEVEL_1)){ angle = 1; } // bowman
        else if (q1->unit->get_type() == (FOREST_UNION | UNIT | LEVEL_3)) angle = 2; // druid
        else if (q1->is_city_archer) { angle = 4; p1.subval -= 6; p1.value +=5; }
        int x1 = p1.value;
        int x2 = p2.value;
        if(p1.value > p2.value){
            if(q2->unit->is_big()){ x2 = p2.value + 5;}
        }else{
            if(q1->unit->is_big()){ x1 = p1.value + 5; }
            angle |= 8;
        }

        int distance = abs(x1 - x2) >> 1;
        int far = abs(p1.subval - p2.subval) >> 1;

        int y1 = p1.subval;

        if(speed == 20){ // fast
            if(distance > speed){
                distance = (distance + (distance >> 1)) >> 1;
                far = (far + (far >> 1)) >> 1;
                angle |= 16;
            }else{
                distance >>= 2;
                far >>= 2;
            }
        }else{ // slow
            if(this->move_obj->e3 != 0){
                x1 = this->move_obj->e1;
                y1 = this->move_obj->e2;
                distance = abs(x1 - x2) >> 1;
                far = abs(y1 - p2.subval) >> 1;
            }
            if(distance > speed * 2){
                distance >>= 2;
                far >>= 2;
                angle |= 64;
            }else if(distance > speed){
                distance = (distance + (distance >> 1)) >> 1;
                far = (far + (far >> 1)) >> 1;
                angle |= 16;
            }else{
                distance >>= 2;
                far >>= 2;
            }
        }
        this->move_obj->e1 = (p1.value > p2.value)? x1 - distance : x1 + distance;
        this->move_obj->e2 = (p1.subval > p2.subval)? y1 - far : y1 + far;
        this->move_obj->e3 = angle | val;
    return true;
}

int battle::move_arrow(int v){
    if(this->move_obj->e4 != v){
        this->move_obj->e4 = v;
        this->move_obj->e3 = 0;
    }

    if(this->move_obj->e3 == 0){
        if(this->calc_ammo(128))
            return 1;
        this->move_obj->e3 = 0;
        this->move_obj->e4 = 0;
        return 0;
    }

    if(this->move_obj->e3 & 128){
        //unit not pull yet
        this->move_obj->e3 ^= 128;
        return 1;
    }

    if(this->move_obj->e3 & 64){
        if(this->calc_ammo(0))
            return 1;
        this->move_obj->e3 = 0;
        this->move_obj->e4 = 0;
        return 0;
    }

    if(this->move_obj->e3 & 16){
        hmmc_queue_item* q2 = this->field->get_item(this->field->two_id());
        pair p2 = q2->unit->get_pos();

        int x2 = p2.value;
        if(this->move_obj->e1 > p2.value){
            // arrow fly from right to left
            if(q2->unit->is_big()){
                //сместить позицию куда прилетит на 5
                x2 = p2.value + 5;
            }
        }
        this->move_obj->e1 = (x2 + this->move_obj->e1) >> 1;
        this->move_obj->e2 = (this->move_obj->e2 + p2.subval) >> 1;
        this->move_obj->e3 ^= 16;
        return 1;
    }

    this->move_obj->e3 = 0;
    this->move_obj->e4 = 0;
    return 0;
}



/*

 /  /  /__/
/  /  /  /__
  /__/  /  /
_/  /__/  /_
/  /  /  /
__/  /__/  /


"|___|_// /", //ступень повторная + 2, начало кирпича 1
"|___|_/ / "  // завершающая ступень
"__|___|/  "  // конечная ступень


        ___
       /  /\
      /  / /|
     /  / / /
    /  / / /\
   /__/ / /\ \
  /   \/ /__\/
 |     |/\
  \_T_/ \ \
  /______\/


  ,___________,
  |___________|
  |___________|
 f|_| |___| |_|t
 '  |_|   |_|c[]

*/


/*
//todo добавить разбитый бочонок
// если бочонок разбит вешается дебаф на всех
// облит элем - понижает мораль на первом ходе - действует на всех всем гуманоидов (ангелов такая мелочь не пугает)
// опьянение -  снижает атаку и урон до конца битвы - действует на всех живых существ

// при битве в подвале таверны
// если варвар стоит рядом с кружкой эля на полу то он может её кинуть во вражеского юнита
// после метания - кружка эля должна исчезнуть

*/

/* размещение кружки на полках стелажа
|c[]  c[]|  c[]c[]| c[]    |
|========|========|========|
    5        7       2
*/

/*
void battle::draw_keg(int ow, int oh, int anim){

/ * todo fix for buffer
    low = static always

    // при выборе персонажа
    medium = подстветка и точечная анимация
    high = персонажи

    // при движении
    medium = персонажи
    high = движение персонажа и точечная анимация

* /

    // todo достать анимацию капли из статического буфера
    int is_right = anim & 8;
    int clr = 'Y';

    //todo на первый ряд используем фильтр рисования только на пробел
    for(int j = 0; j < 5; j++){
        set_position(ow, oh + j);
        // todo draw keg here
        for(int i = 1; i<14;i++) draw_char('_', clr);
    }

    if(anim & 8){
        set_position(ow + 14, oh + 3);
        draw_char('t', 0);
        if(anim & 4){ // рисуем кружку
            // todo при битве в подвале таверны
            // если варвар стоит рядом с кружкой эля на полу то он может её кинуть во вражеского юнита
            // после метания - кружка эля должна исчезнуть с карты
            set_position(ow + 12, oh + 4);
            draw_char('c', 0);
            draw_char('[', 0);
            draw_char(']', 0);
        }else if(anim & 3){
            //todo  добавляем точечную анимацию капли
            // add_point_animation(ow + 14, oh+4, anim & 3, '\'', 'A');
        }
    }else{
        set_position(ow, oh + 3);
        draw_char('f', 0);
        if(anim & 3){
            //todo  добавляем точечную анимацию капли
            // add_point_animation(ow, oh+4, anim & 3, '\'', 'A');
        }

    }

    if(!is_right)draw_char('f' 0);

        for(int i = 0; i<14;i++){

        }

    if(!is_right)draw_char('t' 0);

    for(int j = 0; j < 5; g++){
        if(!is_right)draw_char( j == 3 ? 'f' : ' ', 0);
        for(int i = 0; i<14;i++){

        }

        if(is_right) draw_char( j == 3 ? 'f' : ' ', 0);


    }

} */



void battle::draw_book(int page){

    int x = game_args->left + (game_args->width >> 1) - 5;

    if(page == 0){
        set_position(x, game_args->top - 2);
        draw_text("O==8======", 0);
    }else{
        int id = 1; //todo magic from hero
        for(int i = 0; i < 3; i++){
            set_position(x, game_args->top - 3 + i);
            book_of_magic_draw(id, i, 4, game_args->width & 1);
        }
    }

        //todo current hero
        /*
        if(this->pulse->e3 == lid){
            //todo get weapon or spell
               O======[]=
                     (__)

            // O==8======
            // ========8@
            // >>======>>
        }else if(this->pulse->e3 == rid){
            //todo get weapon or spell
        }
        // */
}

void battle::draw_queue(int ow, int w, int low, int ho, int anime){
    if(low < 1) return;
    int f = (low > 4) ? 7 : low + 1;

    if(f & 1){
        set_position(ow, ho);
        draw_end('_', 0);
        ho++;
    }

    int size = 0;
    for(int j=-1;j<150;j++){
        if(size > w) break;
        hmmc_queue_item* q =  this->field->next_queue(j);
        if(q == NULL) continue;
        if(q->city_unit && !q->is_city_archer) continue;
        int clr = q->group->get_clr();
        int max = 0;
        int ih = -1;
        if (f & 4){
            int a = anime & 1;
            ih = ho;
            if(q->is_city_archer && q->city_unit == BTL_UNIT_CATAPULT){
                static_battle_draw_catapult(0, ow + size, ho, 0);
                max = 15;
            }else{
                for(int i = 0; i < 3; i++){
                    set_position(ow + size, ho + i);
                    max = static_draw(q->get_type(), i, (q->fac == 1 ? AN_RIGHT : AN_LEFT) + a);
                }
            }
        }
        int th = -1;
        if (f & 2){
            int m = 0;
            th = ho + ((f & 4) ? 3 : 0);
            set_position(ow + size, th);
            if(q->is_hero()){
                m = static_hero_draw_name(q->id, clr);
            }else if(q->is_city_archer && q->city_unit == BTL_UNIT_CATAPULT){
                m = draw_text("Catapult", clr);
            }else{
                m = static_draw(q->get_type() | TEXT, 0, 0);
            }
            if(m > max) max = m;
        }

        size += max;

        if(ih > 0){
            for(int i = 0; i < 3; i++){
                set_position(ow + size + 1, ih + i);
                draw_text("|", 0);
            }
        }
        if(th > 0){
            set_position(ow + size + 1, th);
            draw_text("|", 0);
        }

        size += 3;
    }
}

void battle::draw_field(int ow, int w, int top, int anime){
    int count = this->field->unit_count();
    for(int i=0;i<count;i++){
        hmmc_queue_item* q = this->field->get_index(i);
        if(q->is_rem || q->is_hero() || q->id == this->field->one_id() || q->id == this->field->two_id()) continue;
        pair p = q->unit->get_pos();
        for(int j=0;j<3;j++){
            set_position(ow + p.value, top + p.subval + j);
            static_draw(q->unit->get_type(), j, q->get_anime());
        }
    }
}

void battle::draw_currents_unit(int ow, int w, int top, int anime){
    for(int i=0;i<2;i++) {
        int id = (i==0) ? this->field->one_id() : this->field->two_id();
        hmmc_queue_item* q = this->field->get_item(id);
        if(q == NULL || q->is_hero()) continue;
        draw_select_buffer(q->unit->is_fly() ? HIGH : REGULAR);
        pair p = q->unit->get_pos();
        int a = q->get_anime();
        for(int j=0;j<3;j++){
            set_position(ow + p.value, top + p.subval + j);
            static_draw(q->unit->get_type(), j, a);
        }
    }
}

void battle::draw_effects(int left, int top){
    if(this->move_obj->e4 & BATTLE_EFFECT){
        if((this->move_obj->e4 & UNIT_ATTACK_LIGHTNING) && this->move_obj->e3 > 0){
            static_battle_draw_effects(0, top, 1);
            draw_align(3);
        }
        if(this->move_obj->e4 & UNIT_ATTACK_FIREBALL){
            draw_options(DRAWING_OPTION | HIGH, DRAWING_ZIP | REGULAR);
            draw_options(HIGH | DB_OPT_RESET, 0);
            draw_options(HIGH | DRAWING_OPTION, DRAWING_ALFA);
            draw_select_buffer(HIGH);
            draw_clear(HIGH);
            int x = this->move_obj->e1;
            int y = this->move_obj->e2;
            ///todo draw взрыв where (x, y) = center
        }
        return;
    }
    if(this->move_obj->e4 == BA_RANGE_ARROW && this->move_obj->e3 < 128){
        static_battle_draw_arrow(this->move_obj->e3 & 15, this->move_obj->e1 + left, this->move_obj->e2 + top + 1);
        return;
    }
    if(this->move_obj->e4 == BA_LOW_MORALE && this->move_obj->e3){
        static_battle_draw_effects(0, top, 7);
        draw_align(7);
    }
    if(this->move_obj->e4 == BA_UP_MORALE && this->move_obj->e3){
        static_battle_draw_effects(0, top, 6);
        draw_align(7);
    }
    if(this->move_obj->e4 == BA_UP_LUCK && this->move_obj->e3){
        static_battle_draw_effects(0, top, 5);
        draw_align(7);
    }
    if(this->move_obj->e4 == BA_LOW_LUCK && this->move_obj->e3){
        static_battle_draw_effects(0, top, 4);
        draw_align(7);
    }
}

void battle::draw(int anime){
    draw_clear(HIGH | REGULAR);
    ground->draw(anime);

    int ow =  game_args->left;
    int ho = game_args->top;

    int buf = (keyboard_enable()) ? HIGH : REGULAR;
    draw_select_buffer(buf);
    draw_options(buf | DB_OPT_END, game_args->width + ow);

    int bo = ho;

    draw_options(DB_OPT_CLR | HIGH | REGULAR, 'T'); // opacity = on

    this->draw_field(ow, game_args->width, bo, anime);
    //draw_select_buffer(HIGH);
    //todo только для летающих юнитов буфер в high при перемещении, остальные в low
    this->draw_currents_unit(ow, game_args->width, bo, anime);
    draw_select_buffer(HIGH);
    this->draw_effects(ow, bo);

    if(bo > 0){
        draw_select_buffer(buf);
        draw_options(DB_OPT_RESET | buf, DB_OPT_CLR);
        this->draw_queue(ow, game_args->width, bo, ho + 18, 0);
    }
    draw_options(buf | DB_OPT_RESET, DB_OPT_CUT);
    draw_echo('\n');
    draw_echo(-this->field->one_id());
    draw_echo(' ');
    draw_echo(-this->field->two_id());

    ///todo
    if(event_value){
        draw_echo(' ');
        if(event_value == BA_LOW_MORALE){
            draw_echo('L'); draw_echo('M');
        }else if(event_value == BA_RANGE_ARROW){
            draw_echo('R'); draw_echo('A');
        }else if(event_value == BA_NEGATIVE_MAGIC){
            draw_echo('@');draw_echo('@');
        }else if(event_value == BA_POSITIVE_MAGIC){
            draw_echo('+');draw_echo('+');
        }else if(event_value == BA_HERO_ATTACK){
            draw_echo('H');draw_echo('A');
        }else if(event_value == BA_HERO_RANGE){
            draw_echo('H');draw_echo('R');
        }else if(event_value == BA_UP_MORALE){
            draw_echo('U'); draw_echo('M');
        }else if(event_value == BA_UP_LUCK){
            draw_echo('U'); draw_echo('L');
        }else if(event_value == BA_LOW_LUCK){
            draw_echo('N'); draw_echo('L');
        }else if(event_value == BA_MOVE){
            draw_echo('M'); draw_echo('V');
        }
        event_value = 0;
    }
}

bool battle::keyboard_enable(){
    return this->field->key_on();
}

int battle::on_tick(int anime){
    if(this->keyboard_enable()) return 0;
    this->field->resolve_pulse(this->ai.state);
    return 0;
}

int battle::on_press(int c, int k){
    if(!this->keyboard_enable()) return 0;


    //todo battle dialog


    if(k == KEY__TAB){
        this->field->select_unit();
        return 0;
    }

    if(c == ' '){
        this->field->on_run();
        return 0;
    }

    //todo move
    return 0;
}
