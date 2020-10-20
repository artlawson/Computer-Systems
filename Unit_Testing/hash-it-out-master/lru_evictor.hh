#pragma once

#include <string>
#include <queue>
#include <list>
#include <unordered_map>
#include <iostream>
#include "evictor.hh"


class LRUEvictor : public Evictor
{
  private:
  using key_type = std::string;
  using list_type = std::list<key_type>;
  using dict_type = std::unordered_map<key_type, std::_List_iterator<key_type>>;
  list_type linked_list_;
  dict_type address_dict_;
  public:
  //Create a Least Recently Used Evictor
  LRUEvictor() : linked_list_(), address_dict_()
  {

  }

  LRUEvictor(const LRUEvictor&) = delete;  // noncopiable
  LRUEvictor& operator=(const LRUEvictor&) = delete;
  ~LRUEvictor()
  {

  }
  // Inform evictor that a certain key has been set or get:
  void touch_key(const key_type&);

  // Request evictor for the next key to evict, and remove it from evictor.
  // If evictor doesn't know what to evict, return an empty key ("").
  const key_type evict();


};
