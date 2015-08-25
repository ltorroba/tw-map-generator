//
//  Utilities.h
//  TWMapGenerator
//
//  Created by Lucas Torroba on 8/23/15.
//  Copyright (c) 2015 Lucas Torroba. All rights reserved.
//

#ifndef __TWMapGenerator__Utilities__
#define __TWMapGenerator__Utilities__

#include <stdio.h>
#include <vector>
#include <unordered_map>

#include "Tribe.h"
#include "Player.h"
#include "Village.h"

struct Local_Tribe;
struct Local_Player;

class Utilities {
    static bool is_tribe_better(Local_Tribe* i, Local_Tribe* j);
    static bool is_player_better(Local_Player* i, Local_Player* j);
    
public:
    static std::vector<Tribe*> get_top_tribes(int number, std::unordered_map<int, Tribe*> *tribe_map);
    static std::vector<Player*> get_top_players(int number, std::unordered_map<int, Player*> *player_map);
    static std::vector<Village*> get_tribe_villages(int id, std::unordered_map<int, Tribe*> *tribe_map);
    
    static std::vector<Tribe*> get_local_top_tribes(int number, std::unordered_map<int, Tribe*> *tribe_map);
    static std::vector<Tribe*> get_local_top_tribes(int number, std::vector<Tribe*> *tribes);
    
    static std::vector<Player*> get_local_top_players(int number, std::unordered_map<int, Player*> *player_map);
    static std::vector<Player*> get_local_top_players(int number, std::vector<Player*> *players);
    
    static int get_tribe_id_by_name(std::string name, std::unordered_map<int, Tribe*> *tribe_map);
    static int get_tribe_id_by_tag(std::string tag, std::unordered_map<int, Tribe*> *tribe_map);
    
    static Tribe* get_tribe_by_id (unsigned long int id, std::unordered_map<int, Tribe*> *tribe_map);
    static Player* get_player_by_id(unsigned long int id, std::unordered_map<int, Player*> *player_map);
};

#endif /* defined(__TWMapGenerator__Utilities__) */
