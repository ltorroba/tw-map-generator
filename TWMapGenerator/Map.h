//
//  Map.h
//  TWMapGenerator
//
//  Created by Lucas Torroba on 8/23/15.
//  Copyright (c) 2015 Lucas Torroba. All rights reserved.
//

#ifndef __TWMapGenerator__Map__
#define __TWMapGenerator__Map__

#include <ctime>
#include <stdio.h>
#include <unordered_map>
#include <vector>
#include "cairo.h"

class Village;
class Player;
class Tribe;

struct RGB;

class Map {    
public:
    static void generate_top_tribes_map(std::string filename, std::unordered_map<int, Tribe*> *tribe_map,
                                        std::unordered_map<int, Player*> *player_map, std::unordered_map<int, Village*> *village_map, std::string server, int world,
                                        long timestamp);
    static void generate_top_players_map(std::string filename, std::unordered_map<int, Player*> *player_map,
                                         std::unordered_map<int, Village*> *village_map, std::string server, int world, long timestamp);
    static void generate_top_oda_map(std::string filename, std::unordered_map<int, Player*> *player_map, std::unordered_map<int, Village*> *village_map,
                                     std::string server, int world, long timestamp);
    static void generate_top_odd_map(std::string filename, std::unordered_map<int, Player*> *player_map, std::unordered_map<int, Village*> *village_map,
                                     std::string server, int world, long timestamp);
    
    static void draw_background(cairo_t *cr);
    static void draw_grid(cairo_t *cr);
    
    static void draw_sidebar_base(cairo_t *cr, std::string server, int world);
    static void draw_sidebar_top_tribes(cairo_t *cr, std::string server, int world, long timestamp, std::vector<Tribe*> tribes);
    static void draw_sidebar_top_players(cairo_t *cr, std::string server, int world, long timestamp, std::vector<Player*> players);
    static void draw_sidebar_top_oda(cairo_t *cr, std::string server, int world, long timestamp, std::vector<Player*> players);
    static void draw_sidebar_top_odd(cairo_t *cr, std::string server, int world, long timestamp, std::vector<Player*> players);
    
    static std::vector<RGB> get_palette();
    static void set_palette (int color, cairo_t *cr);
    static void set_palette (int color, double alpha, cairo_t *cr);
    
    static std::string pretty_number(unsigned long int n);
    static std::string pretty_date();
    static std::string pretty_date(long timestamp);
};

#endif /* defined(__TWMapGenerator__Map__) */
