//
//  main.cpp
//  TWMapGenerator
//
//  Created by Lucas Torroba on 8/18/15.
//  Copyright (c) 2015 Lucas Torroba. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <ctime>
#include <curl/curl.h>
#include <vector>
#include <unordered_map>
#include <thread>

#include "Tribe.h"
#include "Player.h"
#include "Village.h"
#include "Map.h"
#include "Downloader.h"

using namespace std;

struct ContinentData {
    unsigned long continent;
    string json;
    
    ContinentData (unsigned long c, string j) {
        continent = c;
        json = j;
    }
};

void download_continent_data(unsigned long continent, unsigned long world, string server, unsigned long timestamp, vector<ContinentData> *out) {
    string url = Downloader::generate_continent_json_url(continent, world, server, timestamp);
    string data = Downloader::download_string(url);
    out->push_back(ContinentData(continent, data));
}

int main(int argc, const char * argv[]) {
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Initialize variables
    unordered_map<int, Tribe*> tribe_map;
    unordered_map<int, Player*> player_map;
    unordered_map<int, Village*> village_map;
    
    vector<Tribe*> top_tribes;
    vector<Player*> top_players;
    
    // Begin asynchronous download
    unsigned long timestamp = time(nullptr);
    vector<ContinentData> data;
    vector<thread*> threads;
    
    cout << "Beginning asynchronous download..." << endl;
    
    for(int i = 0; i <= 99; i++) {
        thread* t = new thread(download_continent_data, i, 70, "en", timestamp, &data);
        threads.push_back(t);
    }
    
    cout << "Starting to join threads..." << endl;
    
    for(int i = 0; i <= 99; i++) {
        // Join continent thread
        threads[i]->join();
        
        // Process downloaded data
        Downloader::update_tribe_map(data[i].json, &tribe_map);
        cout << "Tribe map size: " << tribe_map.size() << endl;
        
        Downloader::update_player_map(data[i].json, &tribe_map, &player_map);
        cout << "Player map size: " << player_map.size() << endl;
        
        Downloader::update_village_map(data[i].json, &tribe_map, &player_map, &village_map);
        cout << "Village map size: " << village_map.size() << endl;
    }
    
    cout << "Generating image..." << endl;
    
    Map::generate_top_tribes_map("/Users/ltorroba/Desktop/top_tribes.png", &tribe_map, &player_map, &village_map);
    
    // Destroy libcurl
    curl_global_cleanup();
    
    return 0;
}
