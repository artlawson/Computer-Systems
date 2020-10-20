#include "fifo_evictor.hh"

void FIFO::touch_key(const key_type& key)  {
    //just insert at the beginning
    FIFO::touchedKeys.insert(FIFO::touchedKeys.begin(), key);
    for(long unsigned int i=0; i < FIFO::touchedKeys.size(); i++){
       std::cout << FIFO::touchedKeys.at(i) << ' ';
   }

}


const key_type FIFO::evict()  {
    if(FIFO::touchedKeys.size() > 0) {
        key_type key = FIFO::touchedKeys[FIFO::touchedKeys.size()-1];
        FIFO::touchedKeys.pop_back();
        return key;
    }
    //return empty string if we don't have anything to evict. Delete will deal with it properly since it won't be found in the map
    return "";
}


