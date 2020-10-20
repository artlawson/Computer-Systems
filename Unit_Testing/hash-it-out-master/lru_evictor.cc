#include "evictor.hh"
#include "lru_evictor.hh"

//https://www.interviewcake.com/concept/java/lru-cache

void LRUEvictor::touch_key(const key_type& key)
{
    auto ll_loc = LRUEvictor::address_dict_.find(key);
    // if the key is already in the dict
    if (ll_loc != LRUEvictor::address_dict_.end())
    {
        // move the key to the front of the linked list
        // update the dict to hold the new location in linked list
        auto iterator_pos = ll_loc->second; 
        if (iterator_pos != LRUEvictor::linked_list_.begin())
        {
            LRUEvictor::linked_list_.splice(LRUEvictor::linked_list_.begin(), LRUEvictor::linked_list_, iterator_pos);
            LRUEvictor::address_dict_[key] = LRUEvictor::linked_list_.begin();
        }
    }
    else
    {
        LRUEvictor::linked_list_.push_front(key);
        LRUEvictor::address_dict_.emplace(key, LRUEvictor::linked_list_.begin());
    }
}
const key_type LRUEvictor::evict()
{
    auto key = LRUEvictor::linked_list_.back();
    if(LRUEvictor::linked_list_.empty())
    {
        return "";
    }
    else
    {
        auto it = LRUEvictor::linked_list_.end();
        --it;
        auto key = *it;
        LRUEvictor::linked_list_.erase(it);   
        LRUEvictor::address_dict_.erase(key);
        return key;
    }
    
}


