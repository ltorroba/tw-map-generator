//
//  Map.h
//  TWMapGenerator
//
//  Created by Lucas Torroba on 8/23/15.
//  Copyright (c) 2015 Lucas Torroba. All rights reserved.
//

#ifndef __TWMapGenerator__Map__
#define __TWMapGenerator__Map__

#include <stdio.h>
#include <unordered_map>
#include <vector>

class Village;
class Player;
class Tribe;

class Map {
    
public:
    static void generate_map(std::string filename, std::unordered_map<int, Tribe*> *tribe_map,
                             std::unordered_map<int, Player*> *player_map, std::unordered_map<int, Village*> *village_map);    
};

#endif /* defined(__TWMapGenerator__Map__) */
