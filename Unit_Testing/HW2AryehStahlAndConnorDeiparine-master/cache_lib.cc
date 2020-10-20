#include "cache.hh"
#include <unordered_map>
#include <string>
#include <iostream>
#include <cstring>
#include "fifo_evictor.hh"
#include "lru_evictor.hh"



class Cache::Impl {
public:
    size_type maxmem_;
    float max_load_factor_;
    Evictor* evictor_;
    hash_func hasher_;

    size_type space_used_ = 0;
    std::unordered_map <key_type, Cache::val_type, hash_func> cache_;

    //using list initialization which allows us to easy initialization of the unordered map especially
    Impl(size_type maxmem, float max_load_factor, Evictor* evictor, hash_func hasher):  maxmem_{maxmem}, max_load_factor_{max_load_factor},
        evictor_{evictor}, hasher_{hasher}, cache_{maxmem, hasher} {};

};

Cache::Cache(size_type maxmem, float max_load_factor, Evictor* evictor, hash_func hasher) 
    : pImpl_(new Impl(maxmem, max_load_factor, evictor, hasher)) {
    	//set the max load factor (unordered map functions make this part easy)
        pImpl_->cache_.max_load_factor(max_load_factor);

}

Cache::~Cache() {
	//delete each value when the cache is destroyed
	reset();
}

bool Cache::del(key_type key){
	//check to see if the key is actually in the map
    if (pImpl_->cache_.count(key) > 0){
        pImpl_->space_used_ -= strlen(pImpl_->cache_[key])+1;
     	//delete the value so we don't have memory leaks
        val_type valToDelete = pImpl_->cache_[key];
        pImpl_->cache_.erase(key);
        delete[] valToDelete;
        return true;
    }
    return false;
}

void Cache::set(key_type key, val_type val, Cache::size_type size){
    key_type keyCopy = key;
    byte_type* valCopy = new byte_type[size];
    strcpy(valCopy, val);
 	//just filling until we reach capacity if there's no evictor
    if(pImpl_->evictor_ == nullptr) {
        if (pImpl_->space_used_ + size <= pImpl_->maxmem_) {
            pImpl_->space_used_ += size;
            pImpl_->cache_.insert_or_assign(keyCopy, valCopy);
        }
        //deleting the copy if we don't have any room in the cache (a failure to insert)
        delete[] valCopy;
    }

    else{
    	//If we have an eviction policy and exceed maxmem, we evict and delete from the cache
    	//then insert the new value
        while(pImpl_->space_used_ + size > pImpl_->maxmem_){
	        key_type delkey = pImpl_->evictor_->evict();
	        del(delkey); 
	    }
	    //try to insert (insert_or_assign returns true as the second comp of a pair if the insertion succeeded)
        if(pImpl_->cache_.insert_or_assign(keyCopy, valCopy).second){
        	pImpl_->space_used_ += size;


        }
        //delete if we fail to insert
        else{
        	delete[] valCopy;
        }

        //always touch 
        pImpl_->evictor_->touch_key(key);
    }

}

Cache::val_type Cache::get(key_type key, size_type& val_size) const{
    if(pImpl_->cache_.count(key) == 0){
        return nullptr;
    }
    val_size = strlen(pImpl_->cache_[key])+1;
    pImpl_->evictor_->touch_key(key);
    return pImpl_->cache_[key];
}



Cache::size_type Cache::space_used() const {
    return pImpl_->space_used_;
}

void Cache::reset(){
	//need to make sure everything is deleted when we remove from the unordered map
	std::unordered_map<key_type, Cache::val_type, hash_func>::iterator it = pImpl_->cache_.begin();
    while(it != pImpl_->cache_.end()) {
        delete[] it->second;
        it++;
    }
    pImpl_->cache_.clear();
    pImpl_->space_used_= 0;

}


