//cache_lib.cc

#include "cache.hh"
#include <vector>
#include <iostream>
#include <map>
#include "fifo_evictor.hh"
#include <cassert>

class Cache::Impl
{

public:
    Impl(size_type maxmem,
        float max_load_factor = 0.75,
        Evictor* evictor = nullptr,
        hash_func hasher = std::hash<key_type>()) : max(maxmem),
        max_load_factor(max_load_factor), evictor(evictor), hasher(hasher)
        {storage = std::unordered_map<key_type, std::pair<Cache::val_type, size_type>, hash_func> (1, hasher);}
        

    Impl(){};

    //maybe pass it the type
    /*
    std::unordered_map<
    key_type, 
    std::pair<val_type,size_type>,
    hash_func,
    > storage(store, hasher); //stores value pointer and size of value
    */
    size_type max;

    float max_load_factor;
    Evictor* evictor;
    hash_func hasher;
    size_type tracker = 0.0;

    ~Impl() = default;

    void set(key_type key, val_type val, size_type size){
    
    byte_type *value_deepcopy = new byte_type[size]; //LEEKS
    int j =0;
	while (val[j] != '\0')
		{
			value_deepcopy[j] = val[j];
			j++;
		}
    if (size > max) {return;}
    if (evictor) {
    evictor-> touch_key(key);
    }
	while (tracker + size > max){
        if (evictor) { 
        auto temp = evictor -> evict();

        this -> del(temp);
        }
	else {
		delete[] value_deepcopy; //delete pointer created
		break;
	}
        } // if we're going over the max, call evictor based on fifo protocol
	
     //otherwise store value in cache
    if (storage.load_factor() > max_load_factor) {
        storage.rehash(int(storage.size() / 0.75)); //rehash if max_load_factor exceeded
      }
    if (storage.find(key)!= storage.end()) {
        if(evictor){evictor -> del(key);}
        size_type difference = (size - storage[key].second);
        storage[key].second += difference;
        tracker+=difference;
        storage[key].first = value_deepcopy;
    }
    else{
	storage.insert(std::make_pair(key,std::make_pair(value_deepcopy,size))); //store value paired with its size
	tracker += size;
    }
    
	
      }

     // Retrieve a pointer to the value associated with key in the cache,
     // or nullptr if not found.
     // Sets the actual size of the returned value (in bytes) in val_size.
     val_type get(key_type key, size_type& val_size) {

  	if (storage.find(key) != storage.end()){
        if (evictor){
        evictor -> touch_key(key);
        }
	    val_type ret = storage[key].first;
            val_size = storage[key].second;

	    return ret; // returns value

  	}

  	else {
           
	    val_type ret = nullptr;
            return ret;
        }
      }

     // Delete an object from the cache, if it's still there
     bool del(key_type key) {
	if (storage.find(key) != storage.end()) {

		delete[] storage[key].first;   //delete the pointer to value
		tracker -= storage[key].second; //decrement tracker by size of val
		storage.erase(storage.find(key)); //find and erase value/size pair

		return true;
        if (evictor){evictor -> del(key);}
	}
	else{ return false;}
     }

     // Compute the total amount of memory used up by all cache values (not keys)
     size_type space_used() const {
	return tracker;
     }

     // Delete all data from the cache
     void reset(){
	auto i = storage.begin();
	while (i != storage.end()) {
		del(i->first); // go through and delete each key
		i = storage.begin();
	}
	tracker = 0.0;
	storage.clear();
    if (evictor){
    evictor-> clear(); 
    }
     }

 private:     
    std::unordered_map<key_type, std::pair<val_type,size_type>,hash_func> storage; //stores value pointer and size of value

    hash_func hasherer;
};

Cache::Cache(size_type maxmem,
        float max_load_factor,
        Evictor* evictor,
        hash_func hasher)
: pImpl_(new Impl(maxmem, max_load_factor, evictor, hasher))
{}

Cache::~Cache() {} //don't set to default...will give memory error

void Cache::set(key_type key, 
        Cache::val_type val, 
        Cache::size_type size)
{
    pImpl_->set(key,val,size);
}

Cache::val_type Cache::get(key_type key, 
        Cache::size_type& val_size) const
{
    return pImpl_->get(key,val_size);
}

bool Cache::del(key_type key)
{
    return pImpl_->del(key);
}

Cache::size_type Cache::space_used() const
{
    return pImpl_->space_used();
}

void Cache::reset()
{
    pImpl_->reset();
}
