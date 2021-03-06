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
#include "Map.h"

struct Local_Tribe;
struct Local_Player;

class Utilities {
    static bool is_global_tribe_better(Tribe *i, Tribe *j);
    static bool is_tribe_better(Local_Tribe* i, Local_Tribe* j);
    static bool is_player_better(Local_Player* i, Local_Player* j);
    static bool is_family_better(Family *i, Family *j);
public:
    static std::vector<Tribe*> get_top_tribes(int number, std::unordered_map<int, Tribe*> *tribe_map);
    static std::vector<Player*> get_top_players(int number, std::unordered_map<int, Player*> *player_map);
    static std::vector<Player*> get_top_players_oda(int number, std::unordered_map<int, Player*> *player_map);
    static std::vector<Player*> get_top_players_odd(int number, std::unordered_map<int, Player*> *player_map);
    static std::vector<Family*> get_top_families(int number, std::vector<Family*> *families);
    
    static std::vector<Village*> get_tribe_villages(int id, std::unordered_map<int, Tribe*> *tribe_map);
    
    static std::vector<Tribe*> get_local_top_tribes(int number, std::unordered_map<int, Tribe*> *tribe_map);
    static std::vector<Tribe*> get_local_top_tribes(int number, std::vector<Tribe*> *tribes);
    
    static std::vector<Player*> get_local_top_players(int number, std::unordered_map<int, Player*> *player_map);
    static std::vector<Player*> get_local_top_players(int number, std::vector<Player*> *players);
    
    static int get_tribe_id_by_name(std::string name, std::unordered_map<int, Tribe*> *tribe_map);
    static int get_tribe_id_by_tag(std::string tag, std::unordered_map<int, Tribe*> *tribe_map);
    
    static Tribe* get_tribe_by_id (unsigned long int id, std::unordered_map<int, Tribe*> *tribe_map);
    static Player* get_player_by_id(unsigned long int id, std::unordered_map<int, Player*> *player_map);
    
    static unsigned long get_village_count(Tribe* tribe);
    
    static std::vector<Tribe*> get_filtered_tribes(int number, std::vector<Family*> *families, std::unordered_map<int, Tribe*> *tribe_map);
    
    static std::string base64_encode(char* bytes_to_encode, unsigned int in_len);
};

#endif /* defined(__TWMapGenerator__Utilities__) */
