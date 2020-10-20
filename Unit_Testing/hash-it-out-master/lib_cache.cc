//https://www.hackerearth.com/practice/data-structures/hash-tables/basics-of-hash-tables/tutorial/
#include "cache.hh"
#include <iostream>
#include <vector>
#include <memory>

class Cache::Impl{
 private:
    // Controls the initial size of the hash map in terms of buckets.
    const unsigned DEFAULT_CAPACITY = 2;
    using triple_t = std::tuple<key_type, size_type, val_type>;
    using ptr_t = std::unique_ptr<triple_t>;
    using bucket_t = std::vector<triple_t>;
    using cache_t = std::vector<bucket_t>;
    size_type maxmem_;
    cache_t dict_;
    Evictor* evictor_;
    hash_func hasher_;
    float max_load_factor_;
    size_type space_used_ = 0;
    unsigned num_elements_ = 0;

 public:
Impl()
{
     
}
  // Create a new cache object with the following parameters:
  // maxmem: The maximum allowance for storage used by values.
  // max_load_factor: Maximum allowed ratio between buckets and table rows.
  // evictor: Eviction policy implementation (if nullptr, no evictions occur
  // and new insertions fail after maxmem has been exceeded).
  // hasher: Hash function to use on the keys. Defaults to C++'s std::hash.
  Impl(size_type maxmem, float max_load_factor,
        Evictor* evictor,
        hash_func hasher)
        : maxmem_(maxmem), dict_(DEFAULT_CAPACITY), evictor_(evictor), hasher_(hasher), max_load_factor_(max_load_factor)
    {
        if (hasher_ == nullptr)
        {
            hasher_ = [](key_type key){
                unsigned hash = 0;
                for (unsigned i = 0; i < key.length(); i++)
                {
                    hash += i*key[i];
                }
                return hash;
            };
        }
    }

  ~Impl()
  {
  }

  // Disallow cache copies, to simplify memory management.
  Impl(const Cache&) = delete;
  Impl& operator=(const Cache&) = delete;

  // Add a <key, value> pair to the cache.
  // If key already exists, it will overwrite the old value.
  // Both the key and the value are to be deep-copied (not just pointer copied).
  // If maxmem capacity is exceeded, enough values will be removed
  // from the cache to accomodate the new value. If unable, the new value
  // isn't inserted to the cache.
  void set(key_type key, val_type val, size_type size)
  {
    if (get_load_factor() > max_load_factor_){
        resize();
    }
    if ((space_used_ + size) <= maxmem_) {
        unsigned index = hasher_(key) % dict_.size();
        triple_t trip(key,size,val);
        (dict_[index]).push_back(trip);
        if (evictor_){
            evictor_->touch_key(key);
        }
        num_elements_ += 1;
        space_used_ += size;
        
    }
    else
    {
        if (evictor_)
        {
            auto key_to_remove = evictor_->evict();
            bool removed = del(key_to_remove);
            if (removed)
            {
                set(key, val, size);
            }
        }
    }
  }

  // Retrieve a pointer to the value associated with key in the cache,
  // or nullptr if not found.
  // Sets the actual size of the returned value (in bytes) in val_size.
  val_type get(const key_type& key, size_type& val_size) const
  {
    unsigned index = hasher_(key) % dict_.size();
    bucket_t current_bucket = dict_[index];

    for (auto it = current_bucket.begin(); it != current_bucket.end(); it++)
    {
        if (std::get<0>(*it) == key)
        {
            if (evictor_)
            {
                evictor_->touch_key(key);
            }
            val_type stored_val = std::get<2>(*it);
            val_size = std::string(stored_val).length()+1;
            return stored_val;
        }
    }
    return nullptr;
  }

  // Delete an object from the cache, if it's still there
  bool del(key_type key)
  {
      unsigned index = hasher_(key) % dict_.size();
      bucket_t* current_bucket = &dict_[index];
      for (auto it = current_bucket->begin(); it != current_bucket->end(); it++)
      {
          if (std::get<0>(*it) == key)
          {
              auto item_size = std::get<1>(*it);
              current_bucket->erase(it);
              num_elements_ -= 1;
              space_used_ -= item_size;
              return true;
          }
      }
      return false;
  }

  // increase the size of the hash map when the load factor gets
  // too high. 
  void resize()
  {
    unsigned new_size = 2*dict_.size();
    cache_t temp_dict(new_size);

    for (auto bucket: dict_) 
    {
        for (auto & cur_trip : bucket)
        {  
           unsigned new_index = hasher_(std::get<0>(cur_trip)) % dict_.size();
            temp_dict[new_index].push_back(cur_trip);
        }
    }

    dict_ = temp_dict;

  }

  double get_load_factor()
  {
      return static_cast<double>(num_elements_/dict_.size());
  }

  // Compute the total amount of memory used up by all cache values (not keys)
  size_type space_used() const
  {
      return space_used_;
  }

  // Delete all data from the cache
  void reset()
  {
      for (unsigned i = 0; i < dict_.size(); i++)
      {
          bucket_t buck(1);
          dict_[i] = buck;
      }
  }

};


Cache::Cache(size_type maxmem,
        float max_load_factor,
        Evictor* evictor,
        hash_func hasher)
        : pImpl_(new Impl(maxmem, max_load_factor, evictor, hasher))
        {

        }

  // Add a <key, value> pair to the cache.
  // If key already exists, it will overwrite the old value.
  // Both the key and the value are to be deep-copied (not just pointer copied).
  // If maxmem capacity is exceeded, enough values will be removed
  // from the cache to accomodate the new value. If unable, the new value
  // isn't inserted to the cache.
  void Cache::set(key_type key, val_type val, size_type size)
  {
      pImpl_->set(key, val, size);
  }

Cache::val_type Cache::get(key_type key, size_type& val_size) const
  {
      return pImpl_->get(key, val_size);
  }
  // Delete an object from the cache, if it's still there
  bool Cache::del(key_type key)
  {
    return pImpl_->del(key);
  }

  // Compute the total amount of memory used up by all cache values (not keys)
  Cache::size_type Cache::space_used() const
  {
      return pImpl_->space_used();
  }

  // Delete all data from the cache
  void Cache::reset()
  {
      pImpl_->reset();
  }

  Cache::~Cache(){}