#include "evictor.hh"
#include "fifo_evictor.hh"
#include <queue>

void FifoEvictor::touch_key(const key_type& key)
{
    FifoEvictor::queue_.push(key);
}
const key_type FifoEvictor::evict()
{
    if (!FifoEvictor::queue_.empty())
    {
        key_type key = FifoEvictor::queue_.front();
        FifoEvictor::queue_.pop();
        return key;
    }
    else
    {
        return "";
    }
    
}


