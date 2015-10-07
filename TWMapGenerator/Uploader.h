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
#include <string>
#include <iostream>
#include <vector>

struct File {
    char *data;
    size_t length;
};

class Uploader {
    const char* AWS_ACCESS_KEY;
    const char* AWS_SECRET_KEY;
    const std::string SERVER;
    const int WORLD;
    static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream);
public:
    std::string aws_parse_bucket_name (std::string server, int world);
    std::string aws_parse_object_path (std::string name, long timestamp);
    void compute_hmac(const std::string data, const unsigned char key[32], unsigned char result[32]);
    void compute_hmac(const std::string data, const std::string key, unsigned char result[32]);
    std::string hex_encode(const unsigned char data[32]);
    void compute_sha256(const std::vector<char> data, char result[65]);
    void compute_sha256(const std::string data, char result[65]);
    Uploader(const char* aws_access_key, const char* aws_secret_key, std::string server, int world);
    ~Uploader();
    void aws_upload(std::vector<char> data, std::string name);
};

#endif /* defined(__TWMapGenerator__Uploader__) */
