#include "lru_evictor.hh"

void LRU::touch_key(const key_type& key) {
    int index;
    auto it = std::find(LRU::touchedKeys.begin(), LRU::touchedKeys.end(), key);
    //move the key to the back of the queue if it's already been inserted
    if (it != LRU::touchedKeys.end()) {
        index = distance(LRU::touchedKeys.begin(), it);
        LRU::touchedKeys.push_back(LRU::touchedKeys[index]);
        LRU::touchedKeys.erase(LRU::touchedKeys.begin() + index);
    }
    //otherwise insert it at the back of the queue
    else{
        LRU::touchedKeys.insert(LRU::touchedKeys.end(), key);

    }

    for(long unsigned int i=0; i < LRU::touchedKeys.size(); i++){
       std::cout << LRU::touchedKeys.at(i) << ' ';
   }

}


const key_type LRU::evict() {
    if(LRU::touchedKeys.size() > 0) {
        //evict the least recently used item which will be at the front of the queue
        key_type key = LRU::touchedKeys[0];
        LRU::touchedKeys.erase(LRU::touchedKeys.begin());
        return key;
    }
    //return empty string if we don't have anything to evict. Delete will deal with it properly since it won't be found in the map
    return "";
}


