//
//  Tribe.h
//  TWMapGenerator
//
//  Created by Lucas Torroba on 8/18/15.
//  Copyright (c) 2015 Lucas Torroba. All rights reserved.
//

#ifndef TWMapGenerator_Tribe_h
#define TWMapGenerator_Tribe_h

#include <string>
#include <vector>

class Player;

class Tribe {
private:
    unsigned long int points; // top 40 players
    unsigned long int members;
    std::string tag;
    unsigned long int total_points;
    unsigned long int points_rank;
    unsigned long int oda;
    unsigned long int odd;
    unsigned long int oda_rank;
    unsigned long int odd_rank;
    std::string name;
    std::vector<Player*> players;
    
public:
    Tribe();
    Tribe(unsigned long int points, unsigned long int members, std::string tag,
          unsigned long int total_points, unsigned long int points_rank, unsigned long int oda,
          unsigned long int odd, unsigned long int oda_rank, unsigned long int odd_rank,
          std::string name);
    ~Tribe();
    
    unsigned long int get_points() const { return points; }
    unsigned long int get_members() const { return members; }
    std::string get_tag() const { return tag; }
    unsigned long int get_total_points() const { return total_points; }
    unsigned long int get_points_rank() const { return points_rank; }
    unsigned long int get_oda() const { return oda; }
    unsigned long int get_odd() const { return odd; }
    unsigned long int get_oda_rank() const { return oda_rank; }
    unsigned long int get_odd_rank() const { return odd_rank; }
    std::string get_name() const { return name; }
    std::vector<Player*> get_players() const { return players; }
    
    void set_points(unsigned long int val) { this->points = val; }
    void set_members(unsigned long int val) { this->members = val; }
    void set_tag(std::string val) { this->tag = val; }
    void set_total_points (unsigned long int val) { this->total_points = val; }
    void set_points_rank (unsigned long int val) { this->points_rank = val; }
    void set_oda (unsigned long int val) { this->oda = val; }
    void set_odd (unsigned long int val) { this->odd = val; }
    void set_oda_rank (unsigned long int val) { this->oda_rank = val; }
    void set_odd_rank (unsigned long int val) { this->odd_rank = val; }
    void set_name (std::string val) { this->name = val; }
    void add_player (Player* val) { this->players.push_back(val); }
    
};

#endif
