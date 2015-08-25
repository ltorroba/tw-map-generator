//
//  Utilities.cpp
//  TWMapGenerator
//
//  Created by Lucas Torroba on 8/23/15.
//  Copyright (c) 2015 Lucas Torroba. All rights reserved.
//

#include <unordered_map>
#include <vector>

#include "Utilities.h"

#include "Tribe.h"
#include "Player.h"
#include "Village.h"

using namespace std;

vector<Tribe*> Utilities::get_top_tribes(unordered_map<int, Tribe*> *tribe_map) {
    vector<Tribe*> map(40);
    
    for (auto pair : *tribe_map) {
        unsigned long int rank = (pair.second)->get_points_rank();
        
        if(rank <= 40) {
            map[rank-1] = pair.second;
        }
    }
    
    return map;
}

vector<Player*> Utilities::get_top_players(unordered_map<int, Player*> *player_map) {
    vector<Player*> map(40);
    
    for (auto pair : *player_map) {
        unsigned long int rank = (pair.second)->get_points_rank();
        
        if(rank <= 40) {
            map[rank-1] = pair.second;
        }
    }
    
    return map;
}

vector<Village*> Utilities::get_tribe_villages(int id, unordered_map<int, Tribe*> *tribe_map) {
    Tribe* tribe = (*tribe_map)[id];
    vector<Village*> villages;
    
    for(auto p : tribe->get_players()) {
        for(auto v : p->get_villages()) {
            villages.push_back(v);
        }
    }
    
    return villages;
    
}

int get_tribe_id_by_name(std::string name, std::unordered_map<int, Tribe*> *tribe_map) {
    for (auto pair : *tribe_map)
        if (pair.second->get_name() == name)
            return pair.first;
    
    return -1;
}

int get_tribe_id_by_tag(std::string tag, std::unordered_map<int, Tribe*> *tribe_map) {
    for(auto pair : *tribe_map)
        if(pair.second->get_tag() == tag)
            return pair.first;
    
    return -1;
}