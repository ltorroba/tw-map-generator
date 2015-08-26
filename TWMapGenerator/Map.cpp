//
//  Map.cpp
//  TWMapGenerator
//
//  Created by Lucas Torroba on 8/23/15.
//  Copyright (c) 2015 Lucas Torroba. All rights reserved.
//

#include <iostream>
#include "cairo.h"
#include <sstream>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <ctime>

#include "Utilities.h"
#include "Map.h"
#include "Tribe.h"
#include "Player.h"

using namespace std;

struct RGB {
    double R;
    double G;
    double B;
    
    RGB (double r, double g, double b) {
        R = r;
        G = g;
        B = b;
    }
    
    RGB (int r, int g, int b) {
        R = (double)r / 255.0;
        G = (double)g / 255.0;
        B = (double)b / 255.0;
    }
};

vector<RGB> Map::get_palette() {
    vector<RGB> palette;
    
    palette.push_back(RGB(255, 255, 0)); // Yellow
    palette.push_back(RGB(255, 153, 0)); // Orange
    palette.push_back(RGB(255, 0, 0)); // Red
    palette.push_back(RGB(255, 0, 153)); // Magenta
    palette.push_back(RGB(153, 0, 255)); // Purple
    palette.push_back(RGB(0, 0, 255)); // Blue
    palette.push_back(RGB(0, 153, 255)); // Light blue
    palette.push_back(RGB(0, 255, 255)); // Cyan
    palette.push_back(RGB(233, 165, 255)); // Pink
    palette.push_back(RGB(153, 255, 0)); // Lime
    
    return palette;
}

void Map::set_palette (int id, cairo_t *cr) {
    vector<RGB> palette = get_palette();
    cairo_set_source_rgba(cr, palette[id].R, palette[id].G, palette[id].B, 1.0);
}

void Map::set_palette (int id, double alpha, cairo_t *cr) {
    vector<RGB> palette = get_palette();
    cairo_set_source_rgba(cr, palette[id].R, palette[id].G, palette[id].B, alpha);
}

void Map::draw_grid(cairo_t *cr) {
    // Adjust pen for lines
    cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.8);
    cairo_set_line_width(cr, 1.0);
    
    // Draw verical lines
    for(double k = 0.1; k <= 0.9; k += 0.1) {
        cairo_move_to(cr, 1000.0*k, 0.0);
        cairo_line_to(cr, 1000.0*k, 1000.0);
    }
    
    cairo_stroke(cr);
    
    // Draw horizontal lines
    for (double k = 0.1; k <= 0.9; k += 0.1) {
        cairo_move_to(cr, 0.0, 1000.0*k);
        cairo_line_to(cr, 1000.0, 1000.0*k);
    }
    
    // Draw continent numbers
    cairo_select_font_face(cr, "helvetica", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, 16.0);
    
    for(int x = 0; x <= 9; x += 1) {
        for (int y = 0; y <= 9; y += 1) {
            cairo_move_to(cr, 100.0 * x + 3, 100.0 * (y + 1) - 3);
            
            ostringstream ss;
            
            int continent = y*10 + x;
            
            ss << (continent < 10 ? "0" : "") << continent;
            
            cairo_show_text(cr, ss.str().c_str());
        }
    }
    
    cairo_stroke(cr);
}

void Map::draw_background(cairo_t *cr) {
    cairo_set_source_rgb(cr, 0.0, 96.0/256.0, 0.0);
    cairo_rectangle(cr, 0.0, 0.0, 1000, 1000);
    cairo_fill(cr);
}

string Map::pretty_number(unsigned long int n) {
    ostringstream ss;
    ss << n;
    
    string temp = ss.str();
    ss.str("");
    
    for(int i = temp.length() - 1; i >= 0; i -= 3) {
        if(i - 3 >= 0)
            ss << temp[i] << temp[i-1] << temp[i-2] << ",";
        else if (i - 2 == 0)
            ss << temp[i] << temp[i-1] << temp[i-2];
        else if (i - 1 == 0)
            ss << temp[i] << temp[i-1];
        else
            ss << temp [i];
    }
    
    string pretty = ss.str();
    reverse(pretty.begin(), pretty.end());
    
    return pretty;
}

string Map::pretty_date() {
    ostringstream ss;
    
    auto t = time(nullptr);
    auto tm = localtime(&t);
    
    ss << put_time(tm, "%A, %B %e, %Y");
    
    return ss.str();
}

void Map::draw_sidebar_base(cairo_t *cr, string server, int world) {
    // Readjust canvas
    cairo_scale(cr, 0.5, 0.5);
    cairo_translate(cr, -600.0, 0.0);
    
    // Draw background
    cairo_set_source_rgb(cr, 33.0/256.0, 33.0/256.0, 33.0/256.0);
    cairo_rectangle(cr, 0.0, 0.0, 600.0, 2000.0);
    cairo_fill(cr);
    
    ostringstream ss;
    
    // Draw signature
    cairo_set_source_rgb(cr, 157.0/256.0, 157.0/256.0, 157.0/256.0);
    
    ss.str("");
    ss << "MAP BY ZESME";
    
    cairo_set_font_size (cr, 40.0);
    cairo_move_to(cr, 30.0, 1940.0);
    cairo_show_text(cr, ss.str().c_str());
    
    cairo_select_font_face(cr, "helvetica", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    
    ss.str("");
    ss << "www.zes.me";
    
    cairo_set_font_size(cr, 30.0);
    cairo_move_to(cr, 30.0, 1970.0);
    cairo_show_text(cr, ss.str().c_str());
}

void Map::draw_sidebar_top_tribes(cairo_t *cr, std::string server, int world, std::vector<Tribe*> tribes) {
    draw_sidebar_base(cr, server, world);
    
    // Draw title
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    
    cairo_select_font_face(cr, "helvetica", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, 72.0);
    
    ostringstream ss;
    ss << "TOP 10 TRIBES";
    
    cairo_move_to(cr, 30.0, 100.0);
    cairo_show_text(cr, ss.str().c_str());
    
    // Draw subtitle
    cairo_set_source_rgb(cr, 157.0/256.0, 157.0/256.0, 157.0/256.0);
    cairo_set_font_size (cr, 60.0);
    
    ss.str("");
    ss << server << world;
    
    cairo_move_to(cr, 30.0, 170.0);
    cairo_show_text(cr, ss.str().c_str());
    
    // Draw time
    cairo_set_source_rgb(cr, 200.0/256.0, 200.0/256.0, 200.0/256.0);
    cairo_set_font_size (cr, 30.0);
    
    string time = pretty_date();
    
    transform(time.begin(), time.end(), time.begin(), ::toupper);
    
    cairo_move_to(cr, 30.0, 210.0);
    cairo_show_text(cr, time.c_str());
    
    // Start drawing stats
    int color = 0;
    
    for(auto t : tribes) {
        set_palette(color, cr);
        
        double base = 230.0 + 150.0*color;
        
        // Tag
        cairo_set_font_size(cr, 50.0);
        cairo_move_to(cr, 30.0, base + 50.0);
        cairo_show_text(cr, t->get_tag().c_str());
        
        // Points (top 40 players)
        cairo_set_font_size(cr, 30.0);
        cairo_move_to(cr, 30.0, base + 80.0);
        
        ss.str("");
        ss << pretty_number(t->get_points()).c_str() << " points";
        
        cairo_show_text(cr, ss.str().c_str());
        
        // Village count
        cairo_set_font_size(cr, 30.0);
        cairo_move_to(cr, 30.0, base + 110.0);
        
        ss.str("");
        ss << pretty_number(Utilities::get_village_count(t)).c_str() << " villages";
        
        cairo_show_text(cr, ss.str().c_str());
        
        // Member count
        cairo_set_font_size(cr, 30.0);
        cairo_move_to(cr, 30.0, base + 140.0);
        
        ss.str("");
        ss << pretty_number(t->get_members()).c_str() << " members";
        
        cairo_show_text(cr, ss.str().c_str());
        
        color++;
    }
}

void Map::generate_top_tribes_map(string file, unordered_map<int, Tribe*> *tribe_map,
                                  unordered_map<int, Player*> *player_map, unordered_map<int, Village*> *village_map,
                                  string server, int world) {
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 2600, 2000);
    cairo_t *cr = cairo_create (surface);
    
    cairo_translate(cr, 600.0, 0.0);
    cairo_scale(cr, 2.0, 2.0);
    
    // Draw background
    draw_background(cr);
    
    // Draw villages
    vector<Tribe*> tribes = Utilities::get_top_tribes(10, tribe_map);
    
    int color = 0;
    
    for(auto t : tribes) {
        vector<Player*> players = t->get_players();
        
        cairo_set_line_width(cr, 1.0);
        
        for(auto p : players) {
            for(auto v : p->get_villages()) {
                int x = v->get_x();
                int y = v->get_y();
                
                set_palette(color, cr);
                cairo_rectangle(cr, x-1, y-1, 3, 3);
                cairo_fill(cr);
                
                cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.35);
                cairo_rectangle(cr, x-1, y-1, 3, 3);
                cairo_stroke(cr);
            }
        }
        
        color++;
        cairo_fill(cr);
    }
    
    // Draw grid
    draw_grid(cr);
    
    // Draw sidebar
    draw_sidebar_top_tribes(cr, server, world, tribes);
    
    // Flush and destroy
    cairo_destroy (cr);
    cairo_surface_write_to_png (surface, file.c_str());
    cairo_surface_destroy (surface);
}



