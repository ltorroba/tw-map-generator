//
//  Uploader2.cpp
//  TWMapGenerator
//
//  Created by Lucas Torroba on 9/23/15.
//  Copyright © 2015 Lucas Torroba. All rights reserved.
//

#include "Uploader.h"
#include "Utilities.h"

#include <curl/curl.h>
#include "openssl/hmac.h"
#include "openssl/sha.h"
#include <string>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <algorithm>

using namespace std;

string Uploader::aws_parse_bucket_name (string server, int world) {
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
    blob_path << put_time(tm, "%Y/%m/%d/") << name;
    
    return blob_path.str();
}

// Credits: https://forums.aws.amazon.com/thread.jspa?threadID=23308
Uploader::Uploader(const char* aws_access_key, const char* aws_secret_key, std::string server, int world)
: AWS_ACCESS_KEY(aws_access_key), AWS_SECRET_KEY(aws_secret_key), SERVER(server), WORLD(world) { }

Uploader::~Uploader() { }

void Uploader::aws_upload(vector<char> data, std::string path) {
    string region = "us-east-1";
    string service = "s3";
    
    CURL *curl = curl_easy_init();
    if (!curl)
        cout << "Error initializing curl!" << endl;
    
    path = aws_parse_bucket_name(SERVER, WORLD) + "/" + path;
    string url = "http://twmaps.s3.amazonaws.com/" + path;
    
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    
    cout << "Calculating signature..." << endl;
    
    // :::::::::::: BUILDING CANONICAL REQUEST ::::::::::::
    string canonical_request("PUT\n"); // HTTP method
    canonical_request += "/" + path + "\n"; // Canonical URI
    canonical_request += "\n"; // Canonical query string
    
    // === CanonicalHeaders ===
    // host
    canonical_request += "host:twmaps.s3.amazonaws.com\n";
    
    // x-amz-content-sha256
    char *hash = new char[65];
    compute_sha256(data, hash);
    
    ostringstream temp_ss;
    temp_ss << "x-amz-content-sha256:" << hash << "\n";
    canonical_request += temp_ss.str();
    
    // x-amz-date
    time_t now = time(0);
    tm* gmtm = gmtime(&now);
    
    temp_ss.str("");
    temp_ss << "x-amz-date:" << put_time(gmtm, "%Y%m%d") << "T" << put_time(gmtm, "%H%M%S") << "Z" << "\n";
    canonical_request += temp_ss.str() + "\n";
    // === End CanonicalHeaders ===
    
    // SignedHeaders
    canonical_request += "host;x-amz-content-sha256;x-amz-date\n";
    
    // HashedPayload
    temp_ss.str("");
    temp_ss << hash;
    
    canonical_request += temp_ss.str();
    
    // :::::::::::: BUILDING STRING TO SIGN ::::::::::::
    string string_to_sign("AWS4-HMAC-SHA256\n");
    
    temp_ss.str("");
    temp_ss << put_time(gmtm, "%Y%m%d") << "T" << put_time(gmtm, "%H%M%S") << "Z" << "\n";
    string_to_sign += temp_ss.str();
    
    temp_ss.str("");
    temp_ss << put_time(gmtm, "%Y%m%d") << "/" << region << "/" << service << "/aws4_request" << "\n";
    string_to_sign += temp_ss.str();
    
    char canonical_request_hash[65];
    compute_sha256(canonical_request, &canonical_request_hash[0]);
    
    temp_ss.str("");
    temp_ss << canonical_request_hash;
    string_to_sign += temp_ss.str();
    
    cout << "==== CANONICAL REQUEST ====" << endl;
    cout << canonical_request << endl;
    
    cout << "==== STRING TO SIGN ====" << endl;
    cout << string_to_sign << endl;
    
    // :::::::::::: CALCULATING SIGNATURE ::::::::::::
    unsigned char date_key[32];
    unsigned char date_region_key[32];
    unsigned char date_region_service_key[32];
    unsigned char signing_key[32];
    unsigned char signature_bytes[32];
    string signature;
    
    // DateKey
    string init_key = "AWS4" + string(AWS_SECRET_KEY);
    temp_ss.str("");
    temp_ss << put_time(gmtm, "%Y%m%d");
    string base = temp_ss.str();
    compute_hmac(base, init_key, date_key);
    
    // DateRegionKey
    compute_hmac(region, date_key, date_region_key);
    
    // DateRegionServiceKey
    compute_hmac(service, date_region_key, date_region_service_key);
    
    // SigningKey
    string request_type = "aws4_request";
    compute_hmac(request_type, date_region_service_key, signing_key);
    
    // Singature
    compute_hmac(string_to_sign, signing_key, &signature_bytes[0]);    
    signature = hex_encode(signature_bytes);
    
    cout << "==== KEYS ====" << endl;
    cout << string(hex_encode(date_key)) << endl;
    cout << string(hex_encode(date_region_key)) << endl;
    cout << string(hex_encode(date_region_service_key)) << endl;
    cout << string(hex_encode(signing_key)) << endl;
    cout << signature << endl;
    
    // :::::::::::: BEGIN REQUEST ::::::::::::
    cout << "==== REQUEST ====" << endl;
    
    curl_slist *headers = NULL;
    
    temp_ss.str("");
    temp_ss << "Authorization: AWS4-HMAC-SHA256 Credential=" << AWS_ACCESS_KEY << "/" << put_time(gmtm, "%Y%m%d") << "/" << region << "/" << service << "/aws4_request" << ",";
    temp_ss << "SignedHeaders=host;x-amz-content-sha256;x-amz-date" << ",";
    temp_ss << "Signature=" << signature;
    
    headers = curl_slist_append(headers, temp_ss.str().c_str());
    headers = curl_slist_append(headers, string("x-amz-content-sha256:" + string(hash)).c_str());
    
    temp_ss.str("");
    temp_ss << "x-amz-date:" << put_time(gmtm, "%Y%m%d") << "T" << put_time(gmtm, "%H%M%S") << "Z";
    cout << temp_ss.str();
    headers = curl_slist_append(headers, temp_ss.str().c_str());
    headers = curl_slist_append(headers, string("Content-Type:image/png").c_str());
    
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, NULL);
    
    File file;
    file.data = &data[0];
    file.length = data.size();
    
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)data.size());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_READDATA, &file);
    
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    
    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK)
        cout << "Error starting file upload!" << endl;
    
    // verify result of the upload
    double total_time;
    long code;
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    
    if(code != 200)
        cout << "Error uploading file!" << endl;
    
    curl_easy_cleanup(curl);
}

// Source: http://www.askyb.com/cpp/openssl-hmac-hasing-example-in-cpp/
void Uploader::compute_hmac(const string data, const unsigned char key[32], unsigned char result[32]) {
    HMAC(EVP_sha256(), key, 32, (unsigned char*)data.c_str(), strlen(data.c_str()), result, NULL);
}

void Uploader::compute_hmac(const string data, const string key, unsigned char result[32]) {
    HMAC(EVP_sha256(), (unsigned char*)key.c_str(), strlen(key.c_str()), (unsigned char*)data.c_str(), strlen(data.c_str()), result, NULL);
}

string Uploader::hex_encode(const unsigned char hash[32]) {
    char result[65];
    
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(result + (i * 2), "%02x", hash[i]);
    
    result[64] = 0;
    
    return string(result);
}

// Source: http://stackoverflow.com/questions/7853156/calculate-sha256-of-a-file-using-openssl-libcrypto-in-c
void Uploader::compute_sha256(const vector<char> data, char result[65]) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    
    SHA256_Update(&sha256, &data[0], data.size());
    SHA256_Final(hash, &sha256);

    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(result + (i * 2), "%02x", hash[i]);
    
    result[64] = 0;
}

void Uploader::compute_sha256(const string data, char result[65]) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    
    SHA256_Update(&sha256, (unsigned char*)data.c_str(), data.size()); // Do not include terminating nullbyte
    SHA256_Final(hash, &sha256);
    
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(result + (i * 2), "%02x", hash[i]);
    
    result[64] = 0;
}

size_t Uploader::read_callback(void *buffer, size_t size, size_t nmemb, void *instream) {
    size_t curl_size = nmemb * size;
    size_t to_copy = ((File*)instream)->length < curl_size ? ((File*)instream)->length : curl_size;
    memcpy(buffer, ((File*)instream)->data, to_copy);
    
    ((File*)instream)->length -= to_copy;
    ((File*)instream)->data += to_copy;
    
    return to_copy;
}