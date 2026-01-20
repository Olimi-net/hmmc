#include "cmp_mgr.h"
#include "cmp_menu.h"

cmp_mgr::cmp_mgr(cmp_basic* parent):cmp_basic(parent){
    cur_page = 0;
    this->pages.push_back(this);
}

cmp_mgr::~cmp_mgr(){
    crop_pages(0);
    this->pages.clear();
}

void cmp_mgr::crop_pages(int max){
    for(int i = this->pages.size() - 1; i > max; i--){
        cmp_basic *b = this->pages[i];
        this->pages.pop_back();
        delete b;
    }
}

int cmp_mgr::on_event(cmp_basic *s, hmmc_event *e){
    return 0;
}

void cmp_mgr::on_draw(int anime) {
    this->get_page()->on_draw(anime);
}
int cmp_mgr::on_press(int c, int k){
    this->get_page()->on_press(c, k);
    return 0;
}
int cmp_mgr::on_tick(int anime){
    this->get_page()->on_tick(anime);
    return 0;
}

cmp_basic* cmp_mgr::get_page(){
    if(this->cur_page == 0){
        this->cur_page = 1;
        this->pages.push_back(new cmp_menu(this));
    }
    int psize = this->pages.size();
    if(this->cur_page >= psize)
        this->cur_page = psize - 1;

    if(this->cur_page != psize - 1)
        crop_pages(this->cur_page);

    return this->pages[this->cur_page];
}
