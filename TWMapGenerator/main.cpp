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

using namespace std;

string generateContinentJsonUrl(int continent, int world, string server, long timestamp) {
    ostringstream url;
    
    url << "http://" << server << world << ".tribalwarsmap.com/" <<
    server << "/continent/" << continent << "?ms=" << timestamp;
    
    return url.str();
}

string generateContinentJsonUrl(int continent, int world, string server) {
    return generateContinentJsonUrl(continent, world, server, time(nullptr));
}

string downloadString(string url) {
    string data;
    CURL *curl;
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        data = curl_easy_perform(curl);        
        curl_easy_cleanup(curl);
    }
    
    return data;
}

class Village
{
    unsigned long int x;
    unsigned long int y;
    
};


int main(int argc, const char * argv[]) {
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    
    string url = generateContinentJsonUrl(0, 23, "br");
    
    cout << "Target url: " << url << endl;
    
    string data = downloadString(url);
    
    cout << data << endl;
    
    // Destroy libcurl
    curl_global_cleanup();
    
    return 0;
}
