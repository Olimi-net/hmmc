#include "hmmc_path.h"
#include "../legacy/drawing.h"

hmmc_path::hmmc_path(int w, int h){
    this->width = w;
    this->height = h;
    int size = w * h;
    for (int i = 0; i < size; i++){
        this->field.push_back(pair_factory(0, 0, 0));
    }
    this->max = size;
}
hmmc_path::~hmmc_path(){
    int size = this->field.size();
    for (int i = 0; i < size; i++){
        pair_free(this->field[i]);
    }
    size = this->step_list.size();
    for (int i = 0; i < size; i++){
        pair_free(this->step_list[i]);
    }
    size = this->pair_pull.size();
    for (int i = 0; i < size; i++){
        pair_free(this->pair_pull[i]);
    }

    this->field.clear();
    this->step_list.clear();
    this->pair_pull.clear();
}

bool hmmc_path::is_pos(int x, int y){
    if(x < 0 || y < 0) return false;
    return (x < this->width && y < this->height);
}

void hmmc_path::set_pos(int x, int y, int v){
    this->field[y * this->width + x]->value = v;
}
int hmmc_path::get_pos(int x, int y){
    return this->field[y * this->width + x]->value;
}
int hmmc_path::rem_pos(int x, int y){
    int v = this->field[y * this->width + x]->value;
    this->field[y * this->width + x]->value = 0;
    return v;
}

void hmmc_path::push_pull(){
    while(this->step_list.size()){
        this->pair_pull.push_back(this->step_list[this->step_list.size()-1]);
        this->step_list.pop_back();
    }
}

void hmmc_path::clear(){
    this->push_pull();
    int size = this->field.size();
    for (int i = 0; i < size; i++){
        this->field[i]->value = this->field[i]->subval = this->field[i]->state = 0;
    }
}

int hmmc_path::sub_pos(int x, int y){
    if(!is_pos(x, y)) return -2;
    return this->field[y * this->width + x]->subval;
}

int hmmc_path::try_set_sub_big(int x, int y, int v){
    if(!is_pos(x, y) || !is_pos(x + 1, y)) return -2;
    int p = sub_pos(x, y);
    if(p) return p;

    p = sub_pos(x + 1, y);
    if (p < 0 || p == 1) return p;
    this->field[y * this->width + x]->subval = v;
    return v;
}

int hmmc_path::try_set_sub(int x, int y, int v, bool is_big){
    if(is_big) return try_set_sub_big(x, y, v);

    int p = sub_pos(x, y);
    if(p) return p;
    this->field[y * this->width + x]->subval = v;
    return v;
}

//if swimming to can stay only earth
bool hmmc_path::can_stay(int x, int y, int is_big){
    if(is_big){
        if(!can_stay(x, y, 0)) return false;
        return is_pos(x + 1, y) && this->field[y * this->width + x + 1]->state == 0;
    }

    return (is_pos(x, y) && this->field[y * this->width + x]->state == 0);
}

int hmmc_path::calc_path(int fx, int fy, int tx, int ty){
    int f = get_pos(fx, fy);
    int t = get_pos(tx, ty);
    bool is_big = false;
    if(f & 1024){
        is_big = true;
        this->field[fy * this->width + fx + 1]->subval = this->max;
    }
    if(t & 1024){
        this->field[ty * this->width + tx + 1]->subval = 1;
    }
    this->field[fy * this->width + fx]->subval = this->max;
    this->field[ty * this->width + tx]->subval = 1;

    int result = 0;

    int index = 1;
    int count = 1;
    while(count){
        count = 0;
        for(int y=0;y<this->height;y++){
            for(int x=0;x<this->width;x++){
                int p = this->sub_pos(x, y);
                if (p < 2) continue;
                int p1 = p-1;
                int n = this->try_set_sub(x - 1, y, p1, is_big);
                if(n == 1){
                    if(this->can_stay(x, y, is_big)){
                        if(this->field[y * this->width + (x - 1)]->subval == 0)
                            this->field[y * this->width + (x - 1)]->subval = p1;
                        if(result) return index;
                        result = index;
                    }
                }else if(n == p1) count++;
                n = this->try_set_sub(x + 1, y, p1, is_big);
                if(n == 1){
                    if(this->can_stay(x, y, is_big)){
                        if(this->field[y * this->width + (x + 1)]->subval == 0)
                            this->field[y * this->width + (x + 1)]->subval = p1;
                        if(result) return index;
                        result = index;
                    }
                }else if(n == p1) count++;
                n = this->try_set_sub(x, y - 1, p1, is_big);
                if(n == 1){
                    if(this->can_stay(x, y, is_big)){
                        if(this->field[(y-1) * this->width + x]->subval == 0)
                            this->field[(y-1)* this->width + x]->subval = p1;
                        if(result) return index;
                        result = index;
                    }
                }else if(n == p1) count++;
                n = this->try_set_sub(x, y + 1, p1, is_big);
                if(n == 1){
                    if(this->can_stay(x, y, is_big)){
                        if(this->field[(y+1) * this->width + x]->subval == 0)
                            this->field[(y+1)* this->width + x]->subval = p1;
                        if(result) return index;
                        result = index;
                    }
                }else if(n == p1) count++;
            }
        }
        index++;
        if(index > this->max) return result;
    }
    return (result) ? result : -1;
}

int hmmc_path::path_update(int x, int y, int b){
    int n = this->sub_pos(x, y);
    if(n < 0) return -1;
    if(this->sub_pos(x - 1, y) > 1 || this->sub_pos(x, y - 1) > 1 || this->sub_pos(x, y + 1) > 1) return 1;

    if(b){
        if(this->sub_pos(x + 2, y) > 2 || this->sub_pos(x + 1, y - 1) > 2 || this->sub_pos(x + 1, y + 1) > 2) return 2;
    }else{
        if(this->sub_pos(x + 1, y) > 1) return 1;
    }

    if(x>1){ // here fix path if big unit
        if(this->sub_pos(x - 1, y) != 0) return -2;
        if(this->sub_pos(x - 2, y) > 2 || this->sub_pos(x - 2, y - 1) > 2 || this->sub_pos(x - 2, y + 1) > 2){
            this->field[y * this->width + x]->subval = 4;
            return 3;
        }
    }
    return -3;
}

pair* hmmc_path::get_pair(int o, int t, int s){
    if(this->pair_pull.size() > 0){
        pair* p = this->pair_pull[this->pair_pull.size()-1];
        this->pair_pull.pop_back();
        p->value = o;
        p->subval = t;
        p->state = s;
        return p;
    }
    return pair_factory(o, t, s);
}

void hmmc_path::append(int x, int y){
    int val = (get_pos(x, y) || (is_pos(x-1,y) && (get_pos(x-1, y) & 1024)))? 1 : 0;
    if(this->last.subval){
        if(is_pos(x+1,y) && get_pos(x+1, y)) val |= 2;
    }
    this->step_list.push_back(get_pair(this->last.value, (x << 4) | y, val));
}

int hmmc_path::pop_step(int tx, int ty){
    int n = (tx << 4) | ty;
    for(int i = this->step_list.size()-1; i >= 0; i--){
        pair* p = this->step_list[i];
        if(p->value != n || p->state > 0) continue;
        p->state = 1;
        return p->subval;
    }
    return -1;
}

int hmmc_path::step_count(int tx, int ty){
    int n = (tx << 4) | ty;
    int count = 0;
    for(int i = this->step_list.size()-1; i >= 0; i--){
        if(this->step_list[i]->value == n) count++;
    }
    return count;
}

int hmmc_path::get_pos_or_def(int x, int y){
    if(!this->is_pos(x, y)) return 0;
    return this->field[y * this->width + x]->subval;
}

bool hmmc_path::path_validate(int x, int y, int cnt){
    if(cnt < 1) return false;
    this->append(x, y);
    int n = this->field[y * this->width + x]->subval;
    int e1 = this->get_pos_or_def(x - 1, y);
	int e2 = this->get_pos_or_def(x, y - 1);
	int e3 = this->get_pos_or_def(x, y + 1);
	int e4 = this->get_pos_or_def(x + 1, y);

	if (cnt & 9){
		if(e1 > n and e1 >= e2 and e1 >= e3 and e1 >= e4)
			return path_validate(x - 1, y, cnt - 1);
		if(e2 > n and e2 >= e3 and e2 >= e4)
			return path_validate(x, y - 1, cnt - 1);
		if(e3 > n and e3 >= e4)
			return path_validate(x, y + 1, cnt - 1);
		if(e4 > n)
			return path_validate(x + 1, y, cnt - 1);
	}else{
		if(e4 > n and e4 >= e3 and e4 >= e2 and e4 >= e1)
			return path_validate(x + 1, y, cnt - 1);
		if(e3 > n and e3 >= e2 and e3 >= e1)
			return path_validate(x, y + 1, cnt - 1);
		if(e2 > n and e2 >= e1)
			return path_validate(x, y - 1, cnt - 1);
		if(e1 > n)
			return path_validate(x - 1, y, cnt - 1);
	}
    return true;
}

void hmmc_path::refresh(){
    int i = 0;
    for(int y=0;y<this->height;y++){
        for(int x=0;x<this->width;x++){
            i = y * this->width + x;
            if(this->field[i]->value != 0){
                if(this->field[i]->value & 2048){ // dead
                    this->field[i]->subval = 0;
                    if(this->field[i]->value & 1024){
                        this->field[i + 1]->subval = 0;
                        x++;
                        continue;
                    }
                }else{
                    this->field[i]->subval = -1;
                    if(this->field[i]->value & 1024){
                        this->field[i + 1]->subval = -1;
                        x++;
                        continue;
                    }
                }
            }else if(this->field[i]->state){
                if(this->field[i]->state == 1 && this->can_swim)
                    this->field[i]->subval = 0;
                else
                    this->field[i]->subval = -1;
            }else{
                this->field[i]->subval = 0;
            }
        }
    }
}

void hmmc_path::close_pos(int tx, int ty){
    int t = (tx << 4) | ty;
    int size = this->step_list.size();
    for(int i = 0; i < size; i++){
        pair* p = this->step_list[i];
        if(p->value == t && p->subval == t){
            p->state = 1;
            return;
        }
    }
}

int hmmc_path::gen_step(int fx, int fy, int tx, int ty){
    int res = this->calc_path(fx, fy, tx, ty);
    //test_draw(1);
    //test_draw(2);
    if(res < 0) return -1;
    if(this->get_pos(tx, ty) & 1024){
        this->field[ty * this->width + tx + 1]->subval = 2;
    }

    if(this->get_pos(fx, fy) & 1024){
        if(this->path_update(tx, ty, this->get_pos(tx, ty) & 1024) < 0) return -2;
    }

    if(this->path_validate(tx, ty, this->max)){
        close_pos(tx, ty);
        // test_draw(3);
        return this->step_count(tx, ty);
    }
    return -3;
}

int hmmc_path::found_value(int val){
    for(int y=0;y<this->height;y++){
        int i = y * this->width;
        for(int x=0;x<this->width;x++){
            if((this->field[i + x]->value & 1023) == val){
                return (x << 4) | y;
            }
        }
    }
    return -1;
}

bool hmmc_path::free_place(int x, int y, int is_big){
    if(is_big){
        if(!free_place(x, y, 0)) return false;
        return is_pos(x + 1, y) && this->field[y * this->width + x + 1]->state == 0 && this->field[y * this->width + x + 1]->value == 0;
    }

    return (is_pos(x, y) && this->field[y * this->width + x]->state == 0 && this->field[y * this->width + x]->value == 0);
}

bool hmmc_path::is_has_step(int tx, int ty){
    int n = (tx << 4) | ty;
    for(int i = this->step_list.size()-1; i >= 0; i--){
        if(this->step_list[i]->value == n) return true;
    }
    return false;
}

int hmmc_path::gen_fly(int x1, int y1, int x2, int y2, bool is_big, bool target_big){
    if(is_big){
        if(this->free_place(x2 - 2, y2, is_big)){
            this->append(x1, y1);
            this->append(x2 - 2, y2);
            return 2;
        }
        if(this->free_place(x2 - 1, y2 + 1, is_big)){
            this->append(x1, y1);
            this->append(x2 - 1, y2 + 1);
            return 2;
        }
        if(this->free_place(x2 - 1, y2 - 1, is_big)){
            this->append(x1, y1);
            this->append(x2 - 1, y2 - 1);
            return 2;
        }
    }

    if(this->free_place(x2 - 1, y2, is_big)){
        this->append(x1, y1);
        this->append(x2 - 1, y2);
        return 2;
    }
    if(this->free_place(x2, y2 + 1, is_big)){
        this->append(x1, y1);
        this->append(x2, y2 + 1);
        return 2;
    }
    if(this->free_place(x2, y2 - 1, is_big)){
        this->append(x1, y1);
        this->append(x2, y2 - 1);
        return 2;
    }

    if(target_big){
        if(this->free_place(x2 + 1, y2 + 1, is_big)){
            this->append(x1, y1);
            this->append(x2 + 1, y2 + 1);
            return 2;
        }
        if(this->free_place(x2 + 1, y2 - 1, is_big)){
            this->append(x1, y1);
            this->append(x2 + 1, y2 - 1);
            return 2;
        }
        if(this->free_place(x2 + 2, y2, is_big)){
            this->append(x1, y1);
            this->append(x2 + 2, y2);
            return 2;
        }
    }else{
        if(this->free_place(x2 + 1, y2, is_big)){
            this->append(x1, y1);
            this->append(x2 + 1, y2);
            return 2;
        }
    }
    return -1;
}

int hmmc_path::gen_path(pair p1, pair p2, bool is_fly, bool is_swim){
    this->can_swim = is_swim;
    this->last.value = (p2.value << 4) | p2.subval;
    this->refresh();
    bool is_big = (get_pos(p1.value, p1.subval) & 1024) ? true : false;
    this->last.subval = is_big;
    bool target_big = (get_pos(p2.value, p2.subval) & 1024) ? true : false;
    if(is_fly){
        int res = this->gen_fly(p1.value, p1.subval, p2.value, p2.subval, is_big, target_big);
        //test_draw(1);
        //test_draw(2);
        //test_draw(3);
        return res;
    }
    return this->gen_step(p1.value, p1.subval, p2.value, p2.subval);
}

int hmmc_path::rem_pos_by_id(int id){
    for(int y=0;y<this->height;y++){
        int i = y * this->width;
        for(int x=0;x<this->width;x++){
            if((this->field[i + x]->value & 1023) == id){
                this->field[i + x]->value = 0;
                return (x << 4) | y;
            }
        }
    }
    return -1;
}

int hmmc_path::return_step(){
    int size = this->step_list.size();
    for(int i = 0; i < size; i++){
        pair* p = this->step_list[i];
        if(p->state != 1) continue;
        p->state = -2;
        return p->subval;
    }
    return -1;
}

/*
void hmmc_path::test_draw(int q){
    if(q == 1){
        for(int y=0;y<this->height;y++){
            int i = y * this->width;
            for(int x=0;x<this->width;x++){
                int v = this->field[i + x]->value & 1023;
                draw_char_at_position('0' + v, q, x, y);
                if(this->field[i + x]->value & 1024){
                    draw_char_at_position('0' + v, q, x + 1, y);
                }
            }
        }
        return;
    }
    if(q == 2){
        for(int y=0;y<this->height;y++){
            int i = y * this->width;
            for(int x=0;x<this->width;x++){
                int v = this->field[i + x]->subval;
                draw_char_at_position(' ' + v, q, x, y);
            }
        }
        return;
    }

    if(q == 3){
        char c = 'a';
        for(int i = this->step_list.size()-1; i >= 0; i--){
            pair* p = this->step_list[i];
            if(p->state > 0) continue;
            int xy = p->subval;
            draw_char_at_position(c++, q, xy >> 4, xy & 15);
        }
    }
}
*/




