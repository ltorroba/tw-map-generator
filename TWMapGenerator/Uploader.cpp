//
//  Uploader.cpp
//  TWMapGenerator
//
//  Created by Lucas Torroba on 8/29/15.
//  Copyright (c) 2015 Lucas Torroba. All rights reserved.
//

#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "libs3.h"

#include "Uploader.h"

using namespace std;

string Uploader::aws_parse_container_name (string server, int world) {
    ostringstream container_name;
    
    // Go lowercase - container names cannot be uppercase
    string temp_server = server;
    transform(temp_server.begin(), temp_server.end(), temp_server.begin(), ::tolower);
    
    container_name << temp_server << world;
    
    return container_name.str();
}

string Uploader::aws_parse_object_path(string name, long timestamp) {
    auto t = timestamp;
    auto tm = localtime(&t);
    
    ostringstream blob_path;
    blob_path << put_time(tm, "%Y/%m/%e/") << name;
    
    return blob_path.str();
}

void Uploader::aws_upload(string path, string name, string server, int world, unsigned long timestamp, string account, string key) {
    S3_initialize("s3", S3_INIT_ALL, "HOST");
    
    cout << "Beginning upload of " << path << endl;
    
    // Connect
    
    // Get container
    
    // If container does not exist, create it and make it blob-public
    
    cout << "Uploading to: " << aws_parse_object_path(name, timestamp) << endl;
    
    // Upload file
    
    S3_deinitialize();
    
    cout << "Finished upload." << endl;
}