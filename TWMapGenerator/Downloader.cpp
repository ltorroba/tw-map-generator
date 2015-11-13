//
//  Downloader.cpp
//  TWMapGenerator
//
//  Created by Lucas Torroba on 8/25/15.
//  Copyright (c) 2015 Lucas Torroba. All rights reserved.
//

#include <sstream>
#include <string>
#include <curl/curl.h>
#include <vector>
#include <unordered_map>

#include "rapidjson/document.h"
#include "Tribe.h"
#include "Player.h"
#include "Village.h"
#include "Utilities.h"
#include "Map.h"

#include "Downloader.h"

using namespace std;

string Downloader::generate_continent_json_url(int continent, int world, string server, long timestamp) {
    ostringstream url;
    
    url << "http://" << server << world << ".tribalwarsmap.com/" <<
    server << "/continent/" << continent << "?ms=" << timestamp;
    
    return url.str();
}

string Downloader::generate_continent_json_url(int continent, int world, string server) {
    return generate_continent_json_url(continent, world, server, time(nullptr));
}

size_t Downloader::write_callback (void *contents, size_t size, size_t nmemb, void *userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string Downloader::download_string(string url) {
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

bool Downloader::url_exists(string url) {
    string data;
    CURL *curl;
    CURLcode res;
    string read_buffer;
    
    string search_string = "<Code>AccessDenied</Code>";
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
        res = curl_easy_perform(curl);
        
        if(res != CURLE_OK)
            return false;
        else if(read_buffer.find(search_string) != string::npos)
            return false;
        
        curl_easy_cleanup(curl);
    }
    
    return true;
}

void Downloader::update_tribe_map(string json, unordered_map<int, Tribe*> *tribe_map) {
    rapidjson::Document document;
    document.Parse<0>(json.c_str());
    
    rapidjson::Value& tribe_data_object = document["tribedata"];
    
    for(rapidjson::Value::MemberIterator itr = tribe_data_object.MemberBegin(); itr != tribe_data_object.MemberEnd(); ++itr) {
        rapidjson::Value& arr = itr->value;
        
        Tribe*& ptr = (*tribe_map)[atoi(itr->name.GetString())];
        
        if(ptr == 0) {
            unsigned long int points = arr[3].GetUint();
            unsigned long int members = arr[1].GetUint();
            string tag = arr[2].GetString();
            unsigned long int total_points = arr[0u].GetUint();
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

void Downloader::update_player_map (string json, unordered_map<int, Tribe*> *tribe_map, unordered_map<int, Player*> *player_map) {
    rapidjson::Document document;
    document.Parse<0>(json.c_str());
    
    rapidjson::Value& player_data_object = document["playerdata"];
    
    for (rapidjson::Value::MemberIterator itr = player_data_object.MemberBegin(); itr != player_data_object.MemberEnd(); ++itr) {
        rapidjson::Value& arr = itr->value;
        
        Player*& ptr = (*player_map)[atoi(itr->name.GetString())];
        
        if(ptr == 0) {
            unsigned long int points = arr[0u].GetUint();
            Tribe* tribe = Utilities::get_tribe_by_id(arr[1].GetUint(), tribe_map);
            string username = arr[2].GetString();
            unsigned long int villages = arr[3].GetUint();
            // 4 unecessary variables
            unsigned long int points_rank = arr[8].GetUint();
            unsigned long int oda = arr[9].GetUint();
            unsigned long int odd = arr[10].GetUint();
            unsigned long int oda_rank = arr[11].GetUint();
            unsigned long int odd_rank = arr[12].GetUint();
            
            Player* p = new Player(points, tribe, username, villages, points_rank, oda, odd, oda_rank, odd_rank);
            ptr = p;
            if(tribe != nullptr)
                tribe->add_player(p);
        }
    }
}

vector<string> &Downloader::split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
    
    return elems;
}

vector<string> Downloader::split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

struct Coordinate {
    unsigned long int x;
    unsigned long int y;
};

Coordinate Downloader::parse_coordinate(string s) {
    vector<string> arr = split(s, '-');
    
    assert(arr.size() == 3);
    
    Coordinate c;
    c.x = atoi(arr[1].c_str());
    c.y = atoi(arr[2].c_str());
    
    return c;
}

void Downloader::update_village_map(string json, unordered_map<int, Tribe*> *tribe_map, unordered_map<int, Player*> *player_map, unordered_map<int, Village*> *village_map) {
    rapidjson::Document document;
    document.Parse<0>(json.c_str());
    
    rapidjson::Value& village_data_object = document["villagedata"];
    
    for(rapidjson::Value::MemberIterator itr = village_data_object.MemberBegin(); itr != village_data_object.MemberEnd(); ++itr) {
        rapidjson::Value& arr = itr->value;
        
        Village*& ptr = (*village_map)[arr[0u].GetUint()];
        
        if(ptr == 0) {
            Coordinate c = Downloader::parse_coordinate(itr->name.GetString());
            
            unsigned long int x = c.x;
            unsigned long int y = c.y;
            Player *owner = Utilities::get_player_by_id(arr[1].GetUint(), player_map);
            unsigned long int points = arr[2].GetUint();
            string name = arr[3].GetString();
            
            Village* v = new Village(x, y, owner, points, name);
            ptr = v;
            if(owner != nullptr)
                owner->add_village(v);
        }
    }
}

void Downloader::update_family_list(string json, std::vector<Family*> *families, unordered_map<int, Tribe*> *tribe_map) {
    rapidjson::Document document;
    document.Parse<0>(json.c_str());
    
    rapidjson::Value& families_data_object = document["families"];
    
    for(rapidjson::Value::MemberIterator itr = families_data_object.MemberBegin(); itr != families_data_object.MemberEnd(); ++itr) {
        rapidjson::Value& arr = itr->value;
        
        Family *f = new Family(itr->name.GetString());
        
        for(int i = 0; i < arr.Size(); i++) {
            Tribe*& ptr = (*tribe_map)[atoi(arr[i].GetString())];
            
            if(ptr != NULL) {
                f->tribes.push_back(ptr);
                f->points += ptr->get_total_points();
                f->villages += Utilities::get_village_count(ptr);
                f->members += ptr->get_members();
            }
        }
        
        if(f->tribes.size() > 0)
            families->push_back(f);
    }
}