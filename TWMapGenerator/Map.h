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

struct Family {
    std::string name;
    std::vector<Tribe*> tribes;
    unsigned long points;
    unsigned long villages;
    unsigned long members;
    
    Family (std::string n) {
        name = n;
        points = 0;
        villages = 0;
        members = 0;
    }
};

class Map {
    double WIDTH = 1000.0;
    double HEIGHT = 800.0;
    std::string FONT_FAMILY = "Open Sans";
    
    static cairo_status_t write_png_stream_to_byte_array (void *in_closure, const unsigned char *data, unsigned int length);
    
    void setup_canvas(cairo_surface_t *surface, cairo_t *cr);
    std::vector<char> finalize_canvas(cairo_surface_t *surface, cairo_t *cr);
    
    void draw_background(cairo_t *cr);
    void draw_grid(cairo_t *cr);
    
    void draw_sidebar_base(cairo_t *cr, std::string server, int world, long timestamp);
    void draw_sidebar_top_tribes(cairo_t *cr, std::string server, int world, long timestamp, std::vector<Tribe*> tribes);
    void draw_sidebar_top_players(cairo_t *cr, std::string server, int world, long timestamp, std::vector<Player*> players);
    void draw_sidebar_top_oda(cairo_t *cr, std::string server, int world, long timestamp, std::vector<Player*> players);
    void draw_sidebar_top_odd(cairo_t *cr, std::string server, int world, long timestamp, std::vector<Player*> players);
    void draw_sidebar_top_families(cairo_t *cr, std::string server, int world, long timestamp, std::vector<Family*> top_families, std::vector<Tribe*> filtered_tribes);
    
    void draw_deleted_snippet(cairo_t *cr, int base);
    
    std::vector<RGB> get_palette();
    void set_palette (int color, cairo_t *cr);
    void set_palette (int color, double alpha, cairo_t *cr);
    
    std::string pretty_number(unsigned long int n);
    std::string pretty_date();
    std::string pretty_date(long timestamp);
public:
    Map(double width, double height) : WIDTH(width), HEIGHT(height) { }
    
    std::vector<char> generate_top_tribes_map(std::unordered_map<int, Tribe*> *tribe_map, std::unordered_map<int, Player*> *player_map, std::unordered_map<int, Village*> *village_map, std::string server, int world, long timestamp);
    std::vector<char> generate_top_players_map(std::unordered_map<int, Player*> *player_map, std::unordered_map<int, Village*> *village_map, std::string server, int world, long timestamp);
    std::vector<char> generate_top_oda_map(std::unordered_map<int, Player*> *player_map, std::unordered_map<int, Village*> *village_map, std::string server, int world, long timestamp);
    std::vector<char> generate_top_odd_map(std::unordered_map<int, Player*> *player_map, std::unordered_map<int, Village*> *village_map,  std::string server, int world, long timestamp);
    std::vector<char> generate_top_families_map(std::unordered_map<int, Tribe*> *tribe_map, std::vector<Family*> *families, std::string server, int world, long timestamp);
    
    void save_local(std::string path, std::vector<char> bytes);
};

#endif /* defined(__TWMapGenerator__Map__) */
