//
//  Utilities.cpp
//  TWMapGenerator
//
//  Created by Lucas Torroba on 8/23/15.
//  Copyright (c) 2015 Lucas Torroba. All rights reserved.
//

#include <unordered_map>
#include <vector>
#include <algorithm>

#include "Utilities.h"

#include "Tribe.h"
#include "Player.h"
#include "Village.h"

using namespace std;

struct Local_Tribe {
    Tribe* tribe;
    int points;
    
    Local_Tribe(Tribe* t) {
        tribe = t;
    }
};

vector<Tribe*> Utilities::get_top_tribes(int number, unordered_map<int, Tribe*> *tribe_map) {
    vector<Local_Tribe*> list;
    
    for (auto pair : *tribe_map) {
        unsigned long int rank = (pair.second)->get_points_rank();
        
        if(rank <= number) {
            list.push_back(new Local_Tribe(pair.second));
            list[list.size() - 1]->points = pair.second->get_points();
        }
    }
    
    sort(list.begin(), list.end(), is_tribe_better);
    
    vector<Tribe*> final;
    
    for (auto lt : list) {
        final.push_back(lt->tribe);
    }
    
    return final;
}

bool Utilities::is_tribe_better(Local_Tribe *i, Local_Tribe *j) {
     return i->points > j->points;
}

struct Local_Player {
    Player* player;
    int points;
    
    Local_Player(Player* p) {
        player = p;
    }
};

bool Utilities::is_player_better(Local_Player *i, Local_Player *j) {
    return i->points > j->points;
}

vector<Tribe*> Utilities::get_local_top_tribes(int number, unordered_map<int, Tribe*> *tribe_map) {
    vector<Local_Tribe*> list;
    
    // Convert map to vector
    for(auto pair : *tribe_map)
        list.push_back(new Local_Tribe(pair.second));
    
    // Accumulate local tribe points into struct
    for(auto lt : list) {
        for(auto p : lt->tribe->get_players()) {
            int accumulator = 0;
            
            for(auto v : p->get_villages()) {
                accumulator += v->get_points();
            }
            
            lt->points = accumulator;
        }
    }
    
    // Sort struct descending
    sort(list.begin(), list.end(), is_tribe_better);
    
    // Prune entries
    vector<Tribe*> pruned_list;
    
    for(int i = 0; i < number; i++)
        if(i < list.size())
            pruned_list.push_back(list[i]->tribe);
        else
            break;    
    
    return pruned_list;
}

std::vector<Tribe*> Utilities::get_local_top_tribes(int number, std::vector<Tribe*> *tribes) {
    vector<Local_Tribe*> list;
    
    // Convert map to vector
    for(auto t : *tribes)
        list.push_back(new Local_Tribe(t));
    
    // Accumulate local tribe points into struct
    for(auto lt : list) {
        for(auto p : lt->tribe->get_players()) {
            int accumulator = 0;
            
            for(auto v : p->get_villages()) {
                accumulator += v->get_points();
            }
            
            lt->points = accumulator;
        }
    }
    
    // Sort struct descending
    sort(list.begin(), list.end(), is_tribe_better);
    
    // Prune entries
    vector<Tribe*> pruned_list;
    
    for(int i = 0; i < number; i++)
        if(i < list.size())
            pruned_list.push_back(list[i]->tribe);
        else
            break;
    
    return pruned_list;
}

std::vector<Player*> Utilities::get_local_top_players(int number, std::unordered_map<int, Player*> *player_map) {
    vector<Local_Player*> list;
    
    // Convert map to vector
    for(auto pair : *player_map)
        list.push_back(new Local_Player(pair.second));
    
    // Accumulate local player points into struct
    for(auto lp : list) {
            int accumulator = 0;
            
            for(auto v : lp->player->get_villages()) {
                accumulator += v->get_points();
            }
            
            lp->points = accumulator;
    }
    
    // Sort struct descending
    sort(list.begin(), list.end(), is_player_better);
    
    // Prune entries
    vector<Player*> pruned_list;
    
    for(int i = 0; i < number; i++)
        if(i < list.size())
            pruned_list.push_back(list[i]->player);
        else
            break;
    
    return pruned_list;
}

std::vector<Player*> Utilities::get_local_top_players(int number, std::vector<Player*> *players) {
    vector<Local_Player*> list;
    
    // Convert map to vector
    for(auto p : *players)
        list.push_back(new Local_Player(p));
    
    // Accumulate local player points into struct
    for(auto lp : list) {
        int accumulator = 0;
        
        for(auto v : lp->player->get_villages()) {
            accumulator += v->get_points();
        }
        
        lp->points = accumulator;
    }
    
    // Sort struct descending
    sort(list.begin(), list.end(), is_player_better);
    
    // Prune entries
    vector<Player*> pruned_list;
    
    for(int i = 0; i < number; i++)
        if(i < list.size())
            pruned_list.push_back(list[i]->player);
        else
            break;
    
    return pruned_list;
}

vector<Player*> Utilities::get_top_players(int number, unordered_map<int, Player*> *player_map) {
    vector<Player*> map(number);
    
    for (auto pair : *player_map) {
        unsigned long int rank = (pair.second)->get_points_rank();
        
        if(rank <= number) {
            map[rank-1] = pair.second;
        }
    }
    
    return map;
}

vector<Player*> Utilities::get_top_players_oda(int number, unordered_map<int, Player*> *player_map) {
    vector<Player*> map(number);
    
    for (auto pair : *player_map) {
        unsigned long int rank = (pair.second)->get_oda_rank();
        
        if(rank <= number) {
            map[rank-1] = pair.second;
        }
    }
    
    return map;
}

vector<Player*> Utilities::get_top_players_odd(int number, unordered_map<int, Player*> *player_map) {
    vector<Player*> map(number);
    
    for (auto pair : *player_map) {
        unsigned long int rank = (pair.second)->get_odd_rank();
        
        if(rank <= number) {
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

int Utilities::get_tribe_id_by_name(std::string name, std::unordered_map<int, Tribe*> *tribe_map) {
    for (auto pair : *tribe_map)
        if (pair.second->get_name() == name)
            return pair.first;
    
    return -1;
}

int Utilities::get_tribe_id_by_tag(std::string tag, std::unordered_map<int, Tribe*> *tribe_map) {
    for(auto pair : *tribe_map)
        if(pair.second->get_tag() == tag)
            return pair.first;
    
    return -1;
}

Tribe* Utilities::get_tribe_by_id (unsigned long int id, unordered_map<int, Tribe*> *tribe_map) {
    // No tribe - nullptr
    if(id == 0)
        return nullptr;
    
    Tribe*& ptr = (*tribe_map)[id];
    
    // Accessing a non-existent key creates is automatically. Remove it.
    if(ptr == 0) {
        tribe_map->erase(id);
        return nullptr;
    }
    
    return ptr;
}


Player* Utilities::get_player_by_id(unsigned long int id, unordered_map<int, Player*> *player_map) {
    // No owner - nullptr
    if(id == 0)
        return nullptr;
    
    Player*& ptr = (*player_map)[id];
    
    if(ptr == 0) {
        player_map->erase(id);
        return nullptr;
    }
    
    return ptr;
}

unsigned long Utilities::get_village_count(Tribe* tribe) {
    unsigned long res = 0;
    
    for (auto p : tribe->get_players()) {
        res += p->get_num_villages();
    }
    
    return res;
}