//
//  Player.h
//  TWMapGenerator
//
//  Created by Lucas Torroba on 8/18/15.
//  Copyright (c) 2015 Lucas Torroba. All rights reserved.
//

#ifndef TWMapGenerator_Player_h
#define TWMapGenerator_Player_h

#include <string>
#include <vector>

#include "Tribe.h"

class Village;

class Player {
private:
    unsigned long int points;
    Tribe *tribe;
    std::string username;
    unsigned long int num_villages;
    unsigned long int points_rank;
    unsigned long int oda;
    unsigned long int odd;
    unsigned long int oda_rank;
    unsigned long int odd_rank;
    std::vector<Village*> villages;
    
public:
    Player();
    Player(unsigned long int points, Tribe *tribe,
           std::string username, unsigned long int villages, unsigned long int points_rank,
           unsigned long int oda, unsigned long int odd, unsigned long int oda_rank,
           unsigned long int odd_rank);
    ~Player();
    
    unsigned long int get_points() const { return points; }
    Tribe *get_tribe() const { return tribe; }
    std::string get_username() const { return username; }
    unsigned long int get_num_villages() const { return num_villages; }
    unsigned long int get_points_rank() const { return points_rank; }
    unsigned long int get_oda() const { return oda; }
    unsigned long int get_odd() const { return odd; }
    unsigned long int get_oda_rank() const { return oda_rank; }
    unsigned long int get_odd_rank() const { return odd_rank; }
    std::vector<Village*> get_villages() const { return villages; }
    
    void set_points (unsigned long int val) { this->points = val; }
    void set_tribe (Tribe *val) { this->tribe = val; }
    void set_username (std::string val) { this->username = val; }
    void set_num_villages (unsigned long int val) { this->num_villages = val; }
    void set_points_rank (unsigned long int val) { this->points_rank = val; }
    void set_oda (unsigned long int val) { this->oda = val; }
    void set_odd (unsigned long int val) { this->odd = val; }
    void set_oda_rank (unsigned long int val) { this->oda_rank = val; }
    void set_odd_rank (unsigned long int val) { this->odd_rank = val; }
    void add_village (Village* val) { this->villages.push_back(val); }
    
};

#endif
