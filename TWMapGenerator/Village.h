//
//  Village.h
//  TWMapGenerator
//
//  Created by Lucas Torroba on 8/18/15.
//  Copyright (c) 2015 Lucas Torroba. All rights reserved.
//

#ifndef TWMapGenerator_Village_h
#define TWMapGenerator_Village_h

#include "Player.h"
#include <string>

class Village {
private:
    unsigned long int x;
    unsigned long int y;
    Player *owner;
    unsigned long int points;
    std::string name;
    
public:
    Village();
    Village(unsigned long int x, unsigned long int y, Player *owner, unsigned long int points,
            std::string name);
    ~Village();
    
    unsigned long int get_x() const { return x; }
    unsigned long int get_y() const { return y; }
    Player *get_owner() const { return owner; }
    unsigned long int get_points() const { return points; }
    std::string get_name() const { return name; }
    
    void set_x(unsigned long int val) { this->x = val; }
    void set_y(unsigned long int val) { this->y = val; }
    void set_owner(Player *val) { this->owner = val; }
    void set_points(unsigned long int val) { this->points = val; }
    void set_name(std::string val) { this->name = val; }
};

#endif
