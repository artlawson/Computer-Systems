
#include "LRU.hh"
#include <vector>

  // Inform evictor that a certain key has been set or get:
  void LRU::touch_key(const key_type& key){
  		if (next_up.find(key) == next_up.end()){
  			next_up[key] +=1;
  		}
  		else {next_up[key] = 1;}
  }

  // Request evictor for the next key to evict, and remove it from evictor.
  // If evictor doesn't know what to evict, return an empty key ("").
  key_type LRU::evict(){
  	if (next_up.emtpy()){
  		return "";
  	}
  	else {
      auto min = *min_element(next_up.begin(), next_up.end());
      next_up.erase(next_up.find(min));
      return min;
    }


  }
  void clear(){
    next_up.clear();
  }
  void del(key_type key) {next_up.erase(next_up.find(key));}



