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

class Player {
private:
    unsigned long int id;
    unsigned long int points;
    unsigned long int tribe_id;
    std::string username;
    unsigned long int villages;
    unsigned long int points_rank;
    unsigned long int oda;
    unsigned long int odd;
    unsigned long int oda_rank;
    unsigned long int odd_rank;
    
public:
    Player();
    Player(unsigned long int id, unsigned long int points, unsigned long int tribe_id,
           std::string username, unsigned long int villages, unsigned long int points_rank,
           unsigned long int oda, unsigned long int odd, unsigned long int oda_rank,
           unsigned long int odd_rank);
    ~Player();
    
    unsigned long int get_id() const { return id; }
    unsigned long int get_points() const { return points; }
    unsigned long int get_tribe_id() const { return tribe_id; }
    std::string get_username() const { return username; }
    unsigned long int get_villages() const { return villages; }
    unsigned long int get_points_rank() const { return points_rank; }
    unsigned long int get_oda() const { return oda; }
    unsigned long int get_odd() const { return odd; }
    unsigned long int get_oda_rank() const { return oda_rank; }
    unsigned long int get_odd_rank() const { return odd_rank; }
    
    void set_id (unsigned long int val) { this->id = val; }
    void set_points (unsigned long int val) { this->points = val; }
    void set_tribe_id (unsigned long int val) { this->tribe_id = val; }
    void set_username (std::string val) { this->username = val; }
    void set_villages (unsigned long int val) { this->villages = val; }
    void set_points_rank (unsigned long int val) { this->points_rank = val; }
    void set_oda (unsigned long int val) { this->oda = val; }
    void set_odd (unsigned long int val) { this->odd = val; }
    void set_oda_rank (unsigned long int val) { this->oda_rank = val; }
    void set_odd_rank (unsigned long int val) { this->odd_rank = val; }
    
};

#endif
