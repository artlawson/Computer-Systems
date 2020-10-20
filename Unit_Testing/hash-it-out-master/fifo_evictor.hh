#pragma once

#include <string>
#include <queue>

class FifoEvictor : public Evictor
{
  private:
  using key_type = std::string;
  using queue_type = std::queue<key_type>;
  queue_type queue_;

  public:
  FifoEvictor() : queue_()
  {

  }

  FifoEvictor(const FifoEvictor&) = delete;  // noncopiable
  FifoEvictor& operator=(const FifoEvictor&) = delete;
  ~FifoEvictor()
  {

  }
  // Inform evictor that a certain key has been set or get:
  void touch_key(const key_type&);
  // Request evictor for the next key to evict, and remove it from evictor.
  // If evictor doesn't know what to evict, return an empty key ("").
  const key_type evict();


};
