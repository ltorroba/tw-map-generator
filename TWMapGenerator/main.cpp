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

#include "Tribe.h"
#include "Player.h"
#include "Village.h"
#include "rapidjson/document.h"

using namespace std;

string generate_continent_json_url(int continent, int world, string server, long timestamp) {
    ostringstream url;
    
    url << "http://" << server << world << ".tribalwarsmap.com/" <<
    server << "/continent/" << continent << "?ms=" << timestamp;
    
    return url.str();
}

string generate_continent_json_url(int continent, int world, string server) {
    return generate_continent_json_url(continent, world, server, time(nullptr));
}

static size_t write_callback (void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string download_string(string url) {
    string data;
    CURL *curl;
    CURLcode res;
    string read_buffer;
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    
    return read_buffer;
}

void update_tribe_map(string json, unordered_map<int, Tribe*> *tribe_map) {
    rapidjson::Document document;
    document.Parse<0>(json.c_str());
    
    rapidjson::Value& tribe_data_object = document["tribedata"];
    
    for(rapidjson::Value::MemberIterator itr = tribe_data_object.MemberBegin(); itr != tribe_data_object.MemberEnd(); ++itr) {
        rapidjson::Value& arr = itr->value;
        
        Tribe*& ptr = (*tribe_map)[atoi(itr->name.GetString())];
        
        if(ptr == 0) {
            unsigned long int points = arr[0u].GetUint();
            unsigned long int members = arr[1].GetUint();
            string tag = arr[2].GetString();
            unsigned long int total_points = arr[3].GetUint();
            unsigned long int points_rank = arr[4].GetUint();
            unsigned long int oda = arr[5].GetUint();
            unsigned long int odd = arr[6].GetUint();
            unsigned long int oda_rank = arr[7].GetUint();
            unsigned long int odd_rank = arr[8].GetUint();
            string tribe_name = arr[9].GetString();
            
            ptr = new Tribe(points, members, tag, total_points, points_rank, oda, odd, oda_rank, odd_rank, tribe_name);
        }
    }
}

Tribe* get_tribe_by_id (unsigned long int id, unordered_map<int, Tribe*> *tribe_map) {
    // No tribe - nullptr
    if(id == 0)
        return nullptr;
    
    Tribe*& ptr = (*tribe_map)[id];
    
    // Accessing a non-existent key creates is automatically. Remove it.
    if(ptr == 0) {
        tribe_map->erase(id);
        return nullptr;
    }
    
    return ptr;
}

void update_player_map (string json, unordered_map<int, Tribe*> *tribe_map, unordered_map<int, Player*> *player_map) {
    rapidjson::Document document;
    document.Parse<0>(json.c_str());
    
    rapidjson::Value& player_data_object = document["playerdata"];
    
    for (rapidjson::Value::MemberIterator itr = player_data_object.MemberBegin(); itr != player_data_object.MemberEnd(); ++itr) {
        rapidjson::Value& arr = itr->value;
        
        Player*& ptr = (*player_map)[atoi(itr->name.GetString())];
        
        if(ptr == 0) {
            unsigned long int points = arr[0u].GetUint();
            Tribe* tribe = get_tribe_by_id(arr[1].GetUint(), tribe_map);
            string username = arr[2].GetString();
            unsigned long int villages = arr[3].GetUint();
            // 4 unecessary variables
            unsigned long int points_rank = arr[8].GetUint();
            unsigned long int oda = arr[9].GetUint();
            unsigned long int odd = arr[10].GetUint();
            unsigned long int oda_rank = arr[11].GetUint();
            unsigned long int odd_rank = arr[12].GetUint();
            
            Player p(points, tribe, username, villages, points_rank, oda, odd, oda_rank, odd_rank);
            ptr = &p;
        }
    }
}

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
    
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

struct Coordinate {
    unsigned long int x;
    unsigned long int y;
};

Coordinate parse_coordinate(string s) {
    vector<string> arr = split(s, '-');
    
    assert(arr.size() == 3);
    
    Coordinate c;
    c.x = atoi(arr[1].c_str());
    c.y = atoi(arr[2].c_str());
    
    return c;
}

Player* get_player_by_id(int id, unordered_map<int, Player*> *player_map) {
    // No owner - nullptr
    if(id == 0)
        return nullptr;
    
    Player*& ptr = (*player_map)[id];
    
    if(ptr == 0) {
        player_map->erase(id);
        return nullptr;
    }
    
    return ptr;
}

void update_village_map(string json, unordered_map<int, Tribe*> *tribe_map, unordered_map<int, Player*> *player_map, unordered_map<int, Village*> *village_map) {
    rapidjson::Document document;
    document.Parse<0>(json.c_str());
    
    rapidjson::Value& village_data_object = document["villagedata"];
    
    for(rapidjson::Value::MemberIterator itr = village_data_object.MemberBegin(); itr != village_data_object.MemberEnd(); ++itr) {
        rapidjson::Value& arr = itr->value;
        
        Village*& ptr = (*village_map)[arr[0u].GetUint()];
        
        /*
         unsigned long int x, unsigned long int y, Player *owner, unsigned long int points,
         std::string name
         */
        
        if(ptr == 0) {
            Coordinate c = parse_coordinate(itr->name.GetString());
            
            unsigned long int x = c.x;
            unsigned long int y = c.y;
            Player *owner = get_player_by_id(arr[1].GetUint(), player_map);
            unsigned long int points = arr[2].GetUint();
            string name = arr[3].GetString();
        }
    }
}

int main(int argc, const char * argv[]) {
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Initialize variables
    unordered_map<int, Tribe*> tribe_map;
    unordered_map<int, Player*> player_map;
    unordered_map<int, Village*> village_map;
    
    long timestamp = time(nullptr);
    
    for(int i = 0; i <= 99; i++) {
        string url = generate_continent_json_url(i, 70, "br", timestamp);
        
        cout << "Target url: " << url << endl;
        
        string data = download_string(url);
        
        update_tribe_map(data, &tribe_map);
        cout << "Tribe map size: " << tribe_map.size() << endl;
        
        update_player_map(data, &tribe_map, &player_map);
        cout << "Player map size: " << player_map.size() << endl;
        
        update_village_map(data, &tribe_map, &player_map, &village_map);
        cout << "Village map size: " << village_map.size() << endl;
    }
    
    // Destroy libcurl
    curl_global_cleanup();
    
    return 0;
}
