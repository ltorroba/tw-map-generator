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
#include <unistd.h>

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
    
    if(!Downloader::url_exists(url)) {
        cerr << "Data for K" << continent << " " << server << world << " is unavailable." << endl;
        abort();
    }
    
    string data = Downloader::download_string(url);
    out->push_back(ContinentData(continent, data));
}

void download_world_metadata(unsigned int world, string server, vector<Family*> *out, std::unordered_map<int, Tribe*> *tribe_map, string url) {
    ostringstream temp;
    temp << url << "?s=" << server << "&w=" << world;
    
    if(!Downloader::url_exists(temp.str())) {
        cerr << "Metadata file for " << server << world << " does not exist." << endl;
        abort();
    }
    
    string data = Downloader::download_string(temp.str());
    Downloader::process_metadata(data, out, tribe_map);
}

int main(int argc, char * argv[]) {
    // Process command line arguments
    char *access_key_id = nullptr;
    char *secret_access_key = nullptr;
    string server = "";
    int world = 0;
    bool UPLOAD = false;
    bool SAVE_LOCAL = false;
    string metadata_url = "";
    string save_path = "";
    
    int c;
    while((c = getopt(argc, argv, ":k:a:s:w:m:ul::")) != -1) {
        switch(c) {
            case 'k': // AWS access key ID
                access_key_id = optarg;
                break;
            case 'a': // AWS secret access key
                secret_access_key = optarg;
                break;
            case 's': // Server to be scraped
            {
                ostringstream ss;
                ss << optarg;
                server = ss.str();
                break;
            }
            case 'w': // World to be scraped on that server
                world = atoi(optarg);
                break;
            case 'm': // Metadata file URL. More information on the format for this URL and it's response in the documentation. Example: http://maps.zes.me/metadata.php
            {
                ostringstream ss;
                ss << optarg;
                metadata_url = ss.str();
                break;
            }
            case 'u': // Upload result to AWS?
                UPLOAD = true;
                break;
            case 'l': // Save results locally to path (if no path is supplied, they will be saved at the binary's location)
                SAVE_LOCAL = true;
                save_path = string(optarg);
                break;
            case ':': // Handle lack of arguments
                if(optopt == 'k' || optopt == 'a' || optopt == 's' || optopt == 'w' || optopt == 'm') {
                    cerr << "Option -" << optopt << " requires an argument." << endl;
                    return 1;
                } else if (optopt == 'l') {
                    SAVE_LOCAL = true;
                }
                break;
            case '?': // Handle invalid flags
                cerr << "Unknown option character." << endl;
                return 1;
            default:
                abort();
        }
    }
    
    if(access_key_id == nullptr || secret_access_key == nullptr || server == "" || world <= 0) {
        cerr << "Insufficient or invalid arguments." << endl;
        return 1;
    }
    
    cout << "AWS access key ID: " << string(access_key_id) << endl;
    cout << "AWS secret access key: " << string(secret_access_key) << endl;
    cout << "Server: " << server << endl;
    cout << "World: " << world << endl;
    cout << "Metadata: " << metadata_url << endl;
    cout << "Upload flag: " << (UPLOAD ? "true" : "false") << endl;
    cout << "Save local flag: " << (SAVE_LOCAL ? "true | Location: " + save_path : "false") << endl;
    
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
        
        // Process downloaded datsa
        Downloader::update_tribe_map(data[i].json, &tribe_map);
        Downloader::update_player_map(data[i].json, &tribe_map, &player_map);
        Downloader::update_village_map(data[i].json, &tribe_map, &player_map, &village_map);
    }
    
    cout << "Downloading world metadata..." << endl;
    download_world_metadata(world, server, &families, &tribe_map, metadata_url);
    
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
    
    if(UPLOAD) {
        cout << "Uploading files to AWS..." << endl;
        
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
    }
    
    if(SAVE_LOCAL) {
        cout << "Saving images locally to " << save_path << "..." << endl;
        
        Map::save_local(data_top_tribes, save_path + "top_tribes.png");
        Map::save_local(data_top_players, save_path + "top_players.png");
        Map::save_local(data_top_oda, save_path + "top_players_oda.png");
        Map::save_local(data_top_odd, save_path + "top_players_odd.png");
        Map::save_local(data_top_families, save_path + "top_families.png");
        
        Map::save_local(data_top_tribes_hr, save_path + "top_tribes_hr.png");
        Map::save_local(data_top_players_hr, save_path + "top_players_hr.png");
        Map::save_local(data_top_oda_hr, save_path + "top_players_oda_hr.png");
        Map::save_local(data_top_odd_hr, save_path + "top_players_odd_hr.png");
        Map::save_local(data_top_families_hr, save_path + "top_families_hr.png");
    }
    
    // Destroy libcurl
    curl_global_cleanup();
    
    return 0;
}
