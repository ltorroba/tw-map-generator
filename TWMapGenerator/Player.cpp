//
//  Player.cpp
//  TWMapGenerator
//
//  Created by Lucas Torroba on 8/18/15.
//  Copyright (c) 2015 Lucas Torroba. All rights reserved.
//

#include "Player.h"

Player::Player() { }

Player::Player(unsigned long int points, Tribe *tribe,
       std::string username, unsigned long int num_villages, unsigned long int points_rank,
       unsigned long int oda, unsigned long int odd, unsigned long int oda_rank,
       unsigned long int odd_rank) {
    this->set_points(points);
    this->set_tribe(tribe);
    this->set_username(username);
    this->set_num_villages(num_villages);
    this->set_points_rank(points_rank);
    this->set_oda(oda);
    this->set_odd(odd);
    this->set_oda_rank(oda_rank);
    this->set_odd_rank(odd_rank);
}

Player::~Player() {
    // TODO: Possible memory leak here?
    //delete tribe;
    //tribe = NULL;
}