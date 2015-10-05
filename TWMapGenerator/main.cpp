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
#include <algorithm>
#include <string>

#include "Tribe.h"
#include "Player.h"
#include "Village.h"
#include "Map.h"
#include "Downloader.h"
#include "Uploader.h"

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
    // TODO: Use getopt to get this, world, etc.
    const char *access_key_id = argv[1];
    const char *secret_access_key = argv[2];
    
    cout << "AWS access key ID: " << access_key_id << endl;
    cout << "AWS secret access key: " << secret_access_key << endl;
    
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Initialize variables
    unordered_map<int, Tribe*> tribe_map;
    unordered_map<int, Player*> player_map;
    unordered_map<int, Village*> village_map;
    
    vector<Tribe*> top_tribes;
    vector<Player*> top_players;
    
    // Begin asynchronous download
    vector<ContinentData> data;
    vector<thread*> threads;
    
    // Configs
    unsigned long timestamp = time(nullptr);
    int world = 70;
    string server = "br";
    
    string server_upper = server;
    transform(server_upper.begin(), server_upper.end(), server_upper.begin(), ::toupper);
    
    cout << "Beginning asynchronous download..." << endl;
    
    for(int i = 0; i <= 99; i++) {
        thread* t = new thread(download_continent_data, i, world, server, timestamp, &data);
        threads.push_back(t);
    }
    
    cout << "Starting to join threads..." << endl;
    
    for(int i = 0; i <= 99; i++) {
        // Join continent thread
        threads[i]->join();
        //delete threads[i];
        
        // Process downloaded data
        Downloader::update_tribe_map(data[i].json, &tribe_map);
        cout << "Tribe map size: " << tribe_map.size() << endl;
        
        Downloader::update_player_map(data[i].json, &tribe_map, &player_map);
        cout << "Player map size: " << player_map.size() << endl;
        
        Downloader::update_village_map(data[i].json, &tribe_map, &player_map, &village_map);
        cout << "Village map size: " << village_map.size() << endl;
    }
    
    cout << "Generating images..." << endl;
    
    // Initialize uploader
    Uploader u = Uploader(access_key_id, secret_access_key);
    
    // Generate maps
    vector<char> data_top_tribes = Map::generate_top_tribes_map("/Users/ltorroba/Desktop/top_tribes.png", &tribe_map, &player_map, &village_map, server_upper, world, timestamp);
    vector<char> data_top_players = Map::generate_top_players_map("/Users/ltorroba/Desktop/top_players.png", &player_map, &village_map, server_upper, world, timestamp);
    vector<char> data_top_oda = Map::generate_top_oda_map("/Users/ltorroba/Desktop/top_oda.png", &player_map, &village_map, server_upper, world, timestamp);
    vector<char> data_top_odd = Map::generate_top_odd_map("/Users/ltorroba/Desktop/top_odd.png", &player_map, &village_map, server_upper, world, timestamp);
    
    // Upload maps
    u.aws_upload(data_top_tribes, "top_tribes.png", server, world, timestamp);
    u.aws_upload(data_top_players, "top_players.png", server, world, timestamp);
    u.aws_upload(data_top_oda, "top_players_oda.png", server, world, timestamp);
    u.aws_upload(data_top_odd, "top_players_odd.png", server, world, timestamp);
    
    // Destroy libcurl
    curl_global_cleanup();
    
    return 0;
}
