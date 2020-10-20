
#include <vector>
#include <random>
#include <chrono>
#include "cache.hh"
#include "fifo_evictor.hh"
#include <iostream>
#include <cstring>
#include "stdlib.h"

// Define Globals
int LARGEST_VALUE_SZ = 1000;
int LARGEST_KEY_SZ = 250;
int CACHE_SIZE = 2048;
int REQ_HITS = 0;
std::vector<key_type> LIST_OF_KEYS;

using key_type = std::string;

// Item size can vary from a few bytes to
//over 100 KB, heavily skewed toward smaller items 

//key/value distribution at bottom of page 10
//value length distribution on pg 11 table 5


//want to show inverse relation between size of cache and hit/miss rates

//hw5 class

//generate workload for each request
//for initialize cache with 10 values for warm start for all methods

//consider moving chunk outside of function for speed reasons
std::default_random_engine generator;

std::string generate_value(int length){
    //generate random string of size length
    const std::string VALID_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char * cstr = new char [VALID_CHARS.length()+1];
    std::strcpy(cstr, VALID_CHARS.c_str());
    int x = sizeof(VALID_CHARS);
        std::string output;
    int n = 0;
    while (n < length) {
        output = output + (VALID_CHARS[(rand() % (x-1)) ]);
        n = n + 1;
    }
    return output;
}

//converts std::string to val_type
Cache::val_type to_val(std::string string){
    char * cstr = new char [string.length()+1];
    std::strcpy(cstr, string.c_str());
    return cstr;
}


// Creates and returns a ready-to-run cache. Yum!
Cache* warm_cache(int keys){
    //std::shared_ptr<Cache> cache_ = Cache()
    Cache* cache = new Cache("127.0.0.1","6969");
    for (int j = 0; j<keys;j++){
        key_type temp_key =generate_value(3);
        std::uniform_int_distribution<int> distribution(1, LARGEST_VALUE_SZ- 1);
        Cache::val_type temp_value = to_val(generate_value(distribution(generator)));
        LIST_OF_KEYS.push_back(temp_key);
        Cache::size_type sz = 0;
        cache->set(temp_key,temp_value,sz);
    }
    return cache;

}

//workload needs to generate random requests to cache at the ETC rates with a 80% hit rate
//key freq and val size need to be similar to the ETC workload

//for key freq, we can use the probabilities vector we started up top
//for value sz, we can maybe use the distribution we initialized up top, just gotta be sure there aren't any distribution requirements
//connect this with client to send steady stream of requests
//we should define each request as a variable, put them in a vector and randomly pick on each time
//within each one there needs to be a random selection (get/del should be out of index, set should be creating value)


//gets key for get or del request depending on probability
key_type give_up(){
    int half = LIST_OF_KEYS.size()/2;
    int which_half = rand() % 100;
    key_type key;
    if (which_half > 1) {
        key = LIST_OF_KEYS[rand() % half]; // gives us a random key in the "high probability" half
    }

    else {
        key = LIST_OF_KEYS[(rand() % half) + half]; // gives us a random key in the "low probability" half
    }
    return key;

}

//perform set rquest
void work_set(Cache* cache){
    Cache::Cache::size_type sz = 0;
    auto key = generate_value(rand() % LIST_OF_KEYS.size()+1); // generate random key
    Cache::val_type value = to_val(generate_value(rand() % LARGEST_VALUE_SZ+1)); // generate random val to pair with key
    cache->set(key,value,sz);
    LIST_OF_KEYS.push_back(key);
}
    

// Perform get request
Cache::val_type work_get(Cache* cache){
    // Determine, based on probabilities from paper, whether to use frequently-accessed keys or uncommon keys
    key_type key = give_up();
    Cache::size_type sz = 0;
    Cache::val_type val = cache->get(key, sz);
    if (val != nullptr) {
        REQ_HITS++; // if we successfully found a key, increment hit count
    }
    return val;
    
}

// Perform delete request
bool work_del(Cache* cache){
    key_type key = give_up();
    auto it = std::find(LIST_OF_KEYS.begin(), LIST_OF_KEYS.end(), key);
    if (it != LIST_OF_KEYS.end()){
        LIST_OF_KEYS.erase(it);
    }
    if (cache->del(key)){
        REQ_HITS++; // if we successfully found and deleted a key, increment hit count
        return true;
    }
    return false;
}


// Main generate request function
//   Generates requests based on ETC workload probability (GET:SET:DEL, 20:1:9)
//   Accesses keys based on probability (i.e. 50% of keys are accessed 1% of get requests)

void generate_req(int n_req){
        Cache* cache_ = warm_cache(CACHE_SIZE);

        // Loop through workload size :)
        std::vector<int> req_list(1,20);
        req_list.insert(req_list.end(), 2, 1);
        req_list.insert(req_list.end(), 3, 9);
        //set up vector for probablities of each req
        for(int i = 0;i<n_req;i++){
        
            int req = req_list[rand() % 30];

            switch(req) {

                // Case: GET Request
                case 1 :
                    work_get(cache_);
                
                // Case: SET Request
                case 2 :

                    work_set(cache_);

                // Case: DEL Request
                case 3 :
                    work_del(cache_);
            }


    }   

    double hit_rate = REQ_HITS / n_req;
}

//from hw1, (:
/*
std::vector<double> baseline_latencies(int nreq){
    std::std::vector<double> output;
    for (int j = 0; j<nreq; j++){
    //get random request
    auto t1 = std::chrono::high_resolution_clock::now();
    //call request here
    auto t2 = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    duration*=pow(10,3);
    
    output.push_back(duration);
    
    }
    return output;
}

std::pair<double,double> baseline_performance(int num){

    //if we change the vector type for baseline_lat... make sure to change this one too :)
    std::vector<double> vec = baseline_latencies(num);
    sort(vec.begin(),vec.end());
    double total;
    for (int i =0; i < num; i++){total+=vec[i];} 
    double req_per = total / num; //average of values in vector

    //perc is the 95th percentile barrier in the vector
    int perc = num * 0.95;
    double ret = vec[perc]; //ret is the 95th percentile of vec

    std::pair<double, double> output;
    output.first = ret;
    output.second = req_per;
    return output;

}

*/