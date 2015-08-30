//
//  Uploader.h
//  TWMapGenerator
//
//  Created by Lucas Torroba on 8/29/15.
//  Copyright (c) 2015 Lucas Torroba. All rights reserved.
//

#ifndef __TWMapGenerator__Uploader__
#define __TWMapGenerator__Uploader__

#include <stdio.h>

class Uploader {
    static std::string azure_parse_connection_string (std::string account, std::string key);
    static std::string azure_parse_container_name (std::string server, int world);
    static std::string azure_parse_blob_path (std::string name, long timestamp);
public:
    static void azure_upload(std::string path, std::string name, std::string server, int world, unsigned long timestamp, std::string account, std::string key);
};

#endif /* defined(__TWMapGenerator__Uploader__) */
