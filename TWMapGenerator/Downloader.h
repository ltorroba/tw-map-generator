//
//  Downloader.h
//  TWMapGenerator
//
//  Created by Lucas Torroba on 8/25/15.
//  Copyright (c) 2015 Lucas Torroba. All rights reserved.
//

#ifndef __TWMapGenerator__Downloader__
#define __TWMapGenerator__Downloader__

#include <stdio.h>

#include <string>

#endif /* defined(__TWMapGenerator__Downloader__) */

struct Coordinate;

class Downloader {
    static size_t write_callback (void *contents, size_t size, size_t nmemb, void *userp);
    static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
    static std::vector<std::string> split(const std::string &s, char delim);
    static Coordinate parse_coordinate(std::string s);
public:
    static std::string generate_continent_json_url(int continent, int world, std::string server, long timestamp);
    static std::string generate_continent_json_url(int continent, int world, std::string server);
    static std::string download_string(std::string url);
    static bool url_exists(std::string url);
    
    static void update_tribe_map(std::string json, std::unordered_map<int, Tribe*> *tribe_map);
    static void update_player_map (std::string json, std::unordered_map<int, Tribe*> *tribe_map, std::unordered_map<int, Player*> *player_map);
    static void update_village_map(std::string json, std::unordered_map<int, Tribe*> *tribe_map, std::unordered_map<int, Player*> *player_map,
                                   std::unordered_map<int, Village*> *village_map);
    static void update_family_list(std::string json, std::vector<Family*> *families, std::unordered_map<int, Tribe*> *tribe_map);
};