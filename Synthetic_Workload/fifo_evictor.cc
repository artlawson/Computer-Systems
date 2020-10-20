#include <vector>
#include <algorithm>
#include "fifo_evictor.hh"
#include <iostream>



  // Inform evictor that a certain key has been set or get:
   void FIFO::touch_key(const key_type& key){
  		auto it = std::find(next_up.begin(), next_up.end(), key);
  		if (it == next_up.end()){
  		next_up.push_back(key);
  	}
  	
  }

  // Request evictor for the next key to evict, and remove it from evictor.
  // If evictor doesn't know what to evict, return an empty key ("").
    key_type FIFO::evict(){
  	if (next_up.empty()){
  		return "";
  	}
  	else {
      key_type temp = next_up[0];
      next_up.erase(next_up.begin());
      return temp;
    }
    


    }
    void FIFO::clear(){
      next_up.clear();
  }
    void FIFO::del(key_type key){
      auto it = std::find(next_up.begin(), next_up.end(), key);
      next_up.erase(it);}
