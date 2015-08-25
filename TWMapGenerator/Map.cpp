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

#include "Utilities.h"
#include "Map.h"
#include "Tribe.h"
#include "Player.h"


using namespace std;

void Map::generate_map(string file, unordered_map<int, Tribe*> *tribe_map,
                              unordered_map<int, Player*> *player_map, unordered_map<int, Village*> *village_map) {
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 1000, 1000);
    cairo_t *cr = cairo_create (surface);
    
    // Draw background
    cairo_set_source_rgb(cr, 0.0, 96.0/256.0, 0.0);
    cairo_rectangle(cr, 0.0, 0.0, 1000, 1000);
    cairo_fill(cr);
    
    // Draw villages
    vector<Village*> villages = Utilities::get_tribe_villages(3180, tribe_map); // 3180 = Union; 868 = |BSK|
    
    cairo_set_line_width(cr, 2.0);
    
    for(auto v : villages) {
        int x = v->get_x();
        int y = v->get_y();
        
        cairo_set_source_rgba(cr, 1.0, 0.0, 0.0, 1.0);
        cairo_rectangle(cr, x-3, y-3, 7, 7);
        cairo_fill(cr);
        
        cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.65);
        /*cairo_move_to(cr, x-2, y-2);
        cairo_line_to(cr, x-2, y+2);
        cairo_line_to(cr, x+2, y+2);
        cairo_line_to(cr, x+2, y-2);
        cairo_line_to(cr, x-2, y-2);*/
        cairo_rectangle(cr, x-2, y-2, 5, 5);
        cairo_stroke(cr);
    }
    
    cairo_fill(cr);
    
    // Adjust pen for lines
    cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.4);
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
    cairo_select_font_face(cr, "arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
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
    
    // Flush and destroy
    cairo_destroy (cr);
    cairo_surface_write_to_png (surface, file.c_str());
    cairo_surface_destroy (surface);
}



