#pragma once
#include "evictor.hh"
#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>

//Header file for the lru evictor extra credit

class LRU : public Evictor {
private:
	std::vector<key_type> touchedKeys;
public:
    void touch_key(const key_type& key) override;
    const key_type evict() override;
    
};