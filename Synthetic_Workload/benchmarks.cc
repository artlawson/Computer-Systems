// Benchmarking HW4 <3

#include "cache.hh"
#include "fifo_evictor.hh"
#include "stdlib.h"
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <cstring>

// Define Globals
int LARGEST_VALUE_SZ = 1000;
int LARGEST_KEY_SZ = 250;
double REQ_HITS = 0;
int REQ_MISSES = 0;
std::vector<key_type> COMMON_KEYS;
std::vector<key_type> RARE_KEYS;
std::default_random_engine generator;
int WARM_CACHE_DIVIDER = 3;

// User-Defined Types
using key_type = std::string;

// Generate random string of size length
std::string generate_value(int length){
    const std::string VALID_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGIJKLMNOPQRSTUVWXYZ0123456789"; //all letters except H
	std::uniform_int_distribution<int> distribution(1,VALID_CHARS.length()-1); // generate random keys
    char * cstr = new char[VALID_CHARS.length()+1];
    std::strcpy(cstr, VALID_CHARS.c_str());
    std::string output;
    int n = 0;
    while (n < length) {
        output = output + (VALID_CHARS[(distribution(generator))]);
        //output = output + (VALID_CHARS[(rand() % (x-1))]);
        n = n + 1;
    }
	delete cstr;
    return output;
}

//converts std::string to val_type
Cache::val_type to_val(std::string string){
    char * cstr = new char [string.length()+1];
    std::strcpy(cstr, string.c_str());
    return cstr;
}


// Creates and returns a ready-to-run cache. Yum!
void warm_cache(int keys, Cache* cache){
    keys = keys / WARM_CACHE_DIVIDER;

    for (int j = 0; j<keys;j++){
		std::uniform_int_distribution<int> key_distribution(1,LARGEST_KEY_SZ-1); // generate random keys
        key_type temp_key = generate_value(key_distribution(generator));
        std::uniform_int_distribution<int> val_distribution(1, LARGEST_VALUE_SZ-1); // generate random values
        Cache::val_type temp_value = to_val(generate_value(val_distribution(generator)));
		if (j % 2 == 0) { // for half of keys, put in rare keys
		    RARE_KEYS.push_back(temp_key);
		}
	
		else { // otherwise place in common keys
			COMMON_KEYS.push_back(temp_key);
		}
	
		// add new pair to cache
		Cache::size_type sz = 0;
		cache->set(temp_key,temp_value,sz);
		
	}
}

//gets key for get request depending on probability
key_type give_up_get(){
    int which_pool = (rand() % 100) + 1;
    key_type key;
    if (which_pool > 1) {
        key = COMMON_KEYS[rand() % COMMON_KEYS.size()]; // gives us a random key in the "high probability" half
    }

    else {
        key = RARE_KEYS[rand() % RARE_KEYS.size()]; // gives us a random key in the "low probability" half
    }

    return key;
}

//gets key for del request randomly
key_type give_up_del(){

    
    int which_pool = rand() % 2;

	key_type key;

    if (which_pool == 1){
        
        key = RARE_KEYS[rand() % RARE_KEYS.size()];
    }
	else if (which_pool == 0){
		key = COMMON_KEYS[rand() % COMMON_KEYS.size()];
	}


	

    return key;
}

//perform set rquest
double work_set(Cache* cache){
    Cache::Cache::size_type sz = 0;
    key_type key = generate_value((rand() % LARGEST_KEY_SZ) + 1); // generate random key
    Cache::val_type value = to_val(generate_value((rand() % LARGEST_VALUE_SZ) + 1)); // generate random val to pair with key
    int which_pool = (rand() % 1); // add to pools at random
	if (which_pool == 0){
    	COMMON_KEYS.push_back(key);
	}

	else {
		RARE_KEYS.push_back(key);
	}
    auto t1 = std::chrono::high_resolution_clock::now();
    cache->set(key,value,sz); //when we record time of each, we can take time around this function and return that instead of void :)
    auto t2 = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    duration*=pow(10,3);
    return duration;
}
    

// Perform get request
double work_get(Cache* cache){
    // Determine, based on probabilities from paper, whether to use frequently-accessed keys or uncommon keys
    key_type key = give_up_get();
    Cache::size_type sz = 0;
    auto t1 = std::chrono::high_resolution_clock::now();
    cache->get(key, sz);    
    auto t2 = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    duration*=pow(10,-3);
    Cache::val_type val = cache->get(key, sz);

    if( val == nullptr){
        auto it1 = std::find(COMMON_KEYS.begin(), COMMON_KEYS.end(), key); // check both pools for the key
        auto it2 = std::find(RARE_KEYS.begin(), RARE_KEYS.end(), key);
        if (it1 != COMMON_KEYS.end()){
        COMMON_KEYS.erase(it1);

    }

    else if (it2 != RARE_KEYS.end()) {
        RARE_KEYS.erase(it2);
    }
        REQ_MISSES++;
        return duration;

    }

    if (val != nullptr || *val == 'H') { //'H' is only letter not in valid character list, used to detect json bug
    REQ_HITS++; // if we successfully found a key, increment hit count
    }
    return duration;
    
}

// Perform delete request
double work_del(Cache* cache){
    key_type key = give_up_del();
    auto t1 = std::chrono::high_resolution_clock::now();
    cache->del(key); //when we record time of each, we can take time around this function and return that instead of void :)
    auto t2 = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    duration*=pow(10,-3);
    
    return duration;
}

std::string& SSS (const char* s)
{
    return *(new std::string(s));
}

// Main generate request function
//   Generates requests based on ETC workload probability (GET:SET:DEL, 20:1:9)
//   Accesses keys based on probability (i.e. 50% of keys are accessed 1% of get requests)

// Run Workload and measure hit rate >:)
double generate(int n_req, Cache* cache){ //number of requests generated 
    double n_gets = 0;
    double time = 0;

	//int n_req = 5000;

	
   // std::cout << "Beep... Beep... Beeeeeep! Cache Warm!" << std::endl;
    // Loop through workload size :)
    std::vector<int> req_list;
    req_list.insert(req_list.end(),20,1);
    req_list.insert(req_list.end(), 1, 2);
    req_list.insert(req_list.end(), 9, 3);

    //set up vector for probablities of each req
    for(int i = 0;i<n_req;i++){

    
        int req = req_list[rand() % 30];
        //req =1;
    
        if (req == 3){
            time = work_del(cache);
        }
        else if(req ==2){
            time = work_set(cache);
        }
        else{ 
            time = work_get(cache);
            n_gets++;
        }
        

	}   
	return time;
	//double false_misses = cache->get_false_misses(); // add false misses into the mix ;) quick fixes hehe	
    //double false_misses = 21.0;
	//double hit_rate = REQ_HITS / n_gets;
	//std::cout << hit_rate << " percent is the hit_rate" std::endl;
}

//int main(){return 1;}