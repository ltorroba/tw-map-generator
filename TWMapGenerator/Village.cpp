//
//  Village.cpp
//  TWMapGenerator
//
//  Created by Lucas Torroba on 8/21/15.
//  Copyright (c) 2015 Lucas Torroba. All rights reserved.
//

#include "Village.h"

Village::Village() { }

Village::Village(unsigned long int x, unsigned long int y, Player *owner, unsigned long int points,
                 std::string name) {
    this->set_x(x);
    this->set_y(y);
    this->set_owner(owner);
    this->set_points(points);
    this->set_name(name);
}

Village::~Village() {
    // Possible memory leak
}