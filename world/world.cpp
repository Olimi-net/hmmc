#include "world.h"
#include "../legacy/static_map.h"
#include "../legacy/hmmc_file_mgr.h"
#include "../legacy/static_game.h"


world::world(int map_id, const char* scen){
    int id = shema_load(scen);
    if(map_id == 0) map_id = id;
    map_buf = load_map(map_id);
    int w = map_buf->get_width();
    int h = map_buf->get_height();
    for(int j = 0; j < h; j++){
        for(int i = 0; i < w; i++){
            pair p = city_finder(map_buf, i, j);
            if(p.state){
                i = p.value;
                j = p.subval;
                create_town(p);
            }
        }
    }
    loaded();
}

world::~world(){
    int size = cities.size();
    for (int i = size-1; i >= 0; i--){
        delete cities[i];
    }
    cities.clear();

    delete map_buf;
    delete shema;
}

int world::on_callback(hmmc_event *e, int a){
    return 0;
}

void world::create_town(pair p){
    // get town in  point

    town_info* town = shema->city_found(p);

    if(town == nullptr){
        city* c = new city(0, p.state, shema->max_build(), 0, p.value - ((p.state & ALL_CITIES) ? 14 : 8), p.subval, this);
        this->cities.push_back(c);
        return;
    }


    if(town->build == 0){
        if(town->forest != 0){
            ///todo set forest and return
            return;
        }
        city* c = new city(0, p.state, shema->max_build(), 0, p.value - ((p.state & ALL_CITIES) ? 14 : 8), p.subval, this);
        this->cities.push_back(c);
        return;
    }

    city* c = new city(town->faction, town->build, shema->max_build(), town->magic, p.value - ((town->build & ALL_CITIES) ? 14 : 8), p.subval, this);
    //c->set_name();
    //c->set_in_group()
    //c->set_out_group()
    this->cities.push_back(c);

    if(town->forest == 0) return;
    ///todo set forest
}

//return map id
int world::shema_load(const char* scen){
    hmmc_file_mgr m = hmmc_file_mgr();
    shema = new world_shema(scen);

    char buf[1024];

    if(m.open_file(scen, HMMC_FILE_MGR_READ)){
        while(true){
            int count = m.read_line(1024, buf);
            if(count == 0) break;
            shema->parse(count, buf);
        }
    }
    //todo load сценарий
    return shema->map_id();
}

void world::loaded(){
    /**
        check all cities
        init city for random and faction
        init building
        update build level

        check villages count
        init villages random
        load heroes
        load units groups
    */


    hmmc_file_mgr m = hmmc_file_mgr();
    if(m.open_file("/home/homyak/Dev/cpp/hmmc_v5/log/map.txt", HMMC_FILE_MGR_WRITE)){
        int h = map_buf->get_height();
        int w = map_buf->get_width();
        for(int j = 0; j < h; j++){
            for(int i = 0; i < w; i++){
                m.write_char(map_buf->get_char(i, j));
            }
            m.write_char('\n');
        }
    }
}
