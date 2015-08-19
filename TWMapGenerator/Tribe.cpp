//
//  Tribe.cpp
//  TWMapGenerator
//
//  Created by Lucas Torroba on 8/18/15.
//  Copyright (c) 2015 Lucas Torroba. All rights reserved.
//

#include "Tribe.h"

Tribe::Tribe() { }

Tribe::Tribe(unsigned long int points, unsigned long int members, std::string tag,
             unsigned long int total_points, unsigned long int points_rank, unsigned long int oda,
             unsigned long int odd, unsigned long int oda_rank, unsigned long int odd_rank,
             std::string name) {
    this->set_points(points);
    this->set_members(members);
    this->set_tag(tag);
    this->set_total_points(total_points);
    this->set_points_rank(points_rank);
    this->set_oda(oda);
    this->set_odd(odd);
    this->set_oda_rank(oda_rank);
    this->set_odd_rank(odd_rank);
    this->set_name(name);
}

Tribe::~Tribe() { }