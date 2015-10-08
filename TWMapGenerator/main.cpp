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

void download_world_metadata(unsigned int world, string server, vector<Family*> *out, std::unordered_map<int, Tribe*> *tribe_map) {
    ostringstream temp;
    temp << "http://twmaps.s3.amazonaws.com/" << server << world << "/data";
    string data = Downloader::download_string(temp.str());
    Downloader::update_family_list(data, out, tribe_map);
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
    vector<Family*> families;
    
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
        //cout << "Tribe map size: " << tribe_map.size() << endl;
        
        Downloader::update_player_map(data[i].json, &tribe_map, &player_map);
        //cout << "Player map size: " << player_map.size() << endl;
        
        Downloader::update_village_map(data[i].json, &tribe_map, &player_map, &village_map);
        //cout << "Village map size: " << village_map.size() << endl;
    }
    
    cout << "Downloading world metadata..." << endl;
    download_world_metadata(world, server, &families, &tribe_map);
    
    cout << "Generating images..." << endl;
    
    // Generate standard res maps
    Map standard_res(900, 692);
    
    vector<char> data_top_tribes = standard_res.generate_top_tribes_map(&tribe_map, &player_map, &village_map, server_upper, world, timestamp);
    vector<char> data_top_players = standard_res.generate_top_players_map(&player_map, &village_map, server_upper, world, timestamp);
    vector<char> data_top_oda = standard_res.generate_top_oda_map(&player_map, &village_map, server_upper, world, timestamp);
    vector<char> data_top_odd = standard_res.generate_top_odd_map(&player_map, &village_map, server_upper, world, timestamp);
    vector<char> data_top_families = standard_res.generate_top_families_map(&tribe_map, &families, server_upper, world, timestamp);
    
    // Generate high res maps
    Map high_res(2600, 2000);
    
    vector<char> data_top_tribes_hr = high_res.generate_top_tribes_map(&tribe_map, &player_map, &village_map, server_upper, world, timestamp);
    vector<char> data_top_players_hr = high_res.generate_top_players_map(&player_map, &village_map, server_upper, world, timestamp);
    vector<char> data_top_oda_hr = high_res.generate_top_oda_map(&player_map, &village_map, server_upper, world, timestamp);
    vector<char> data_top_odd_hr = high_res.generate_top_odd_map(&player_map, &village_map, server_upper, world, timestamp);
    vector<char> data_top_families_hr = high_res.generate_top_families_map(&tribe_map, &families, server_upper, world, timestamp);
    
    // Initialize uploader
    Uploader u = Uploader(access_key_id, secret_access_key, server, world);
    
    // Upload maps to date folder
    u.aws_upload(data_top_tribes, u.aws_parse_object_path("top_tribes.png", timestamp));
    u.aws_upload(data_top_players, u.aws_parse_object_path("top_players.png", timestamp));
    u.aws_upload(data_top_oda, u.aws_parse_object_path("top_players_oda.png", timestamp));
    u.aws_upload(data_top_odd, u.aws_parse_object_path("top_players_odd.png", timestamp));
    u.aws_upload(data_top_families, u.aws_parse_object_path("top_families.png", timestamp));
    
    u.aws_upload(data_top_tribes_hr, u.aws_parse_object_path("top_tribes_hr.png", timestamp));
    u.aws_upload(data_top_players_hr, u.aws_parse_object_path("top_players_hr.png", timestamp));
    u.aws_upload(data_top_oda_hr, u.aws_parse_object_path("top_players_oda_hr.png", timestamp));
    u.aws_upload(data_top_odd_hr, u.aws_parse_object_path("top_players_odd_hr.png", timestamp));
    u.aws_upload(data_top_families_hr, u.aws_parse_object_path("top_families_hr.png", timestamp));
    
    // Update root maps
    u.aws_upload(data_top_tribes, "top_tribes.png");
    u.aws_upload(data_top_players, "top_players.png");
    u.aws_upload(data_top_oda, "top_players_oda.png");
    u.aws_upload(data_top_odd, "top_players_odd.png");
    u.aws_upload(data_top_families, "top_families.png");
    
    u.aws_upload(data_top_tribes_hr, "top_tribes_hr.png");
    u.aws_upload(data_top_players_hr, "top_players_hr.png");
    u.aws_upload(data_top_oda_hr, "top_players_oda_hr.png");
    u.aws_upload(data_top_odd_hr, "top_players_odd_hr.png");
    u.aws_upload(data_top_families_hr, "top_families_hr.png");
    
    // Destroy libcurl
    curl_global_cleanup();
    
    return 0;
}
