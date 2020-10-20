//LRU.hh

#pragma once

#include <string>
#include <map>
#include <algorithm>
#include "evictor.hh"

// Data type to use as keys for Cache and Evictors:
using key_type = std::string;

// Abstract base class to define evictions policies.
// It allows touching a key (on a set or get event), and request for
// eviction, which also deletes a key. There is no explicit deletion
// mechanism other than eviction.
class LRU: public Evictor {
 public:
 	std::pair<val_type,size_type> taco?;
 	std::map<key_type, int> next_up;
 	//for LRU, keep things in vect with pair of 

  // Inform evictor that a certain key has been set or get:
  void touch_key(const key_type&);

  // Request evictor for the next key to evict, and remove it from evictor.
  // If evictor doesn't know what to evict, return an empty key ("").
  key_type evict();

  void clear(){
  	next_up.clear();

	};
	void del(key_type key);