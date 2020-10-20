# Hash it out


## Authors

##### Aryeh Stahl and Connor Deiparine


## Implementation

We used a standard library unordered map as our hash table. This made many of the parts rather simple. For part 3, we simply passed the hasher to the unordered map when we initialized it. Thus, if a Cache is created with a custom hash function, the unordered map will use it to store the key-value pairs. 

For part 4, the standard library unordered map already deals with collisions. Reviewing the documentation reveals that it uses buckets and stores keys that hash to the same value in the same bucket. This is collision chaining.

For part 5, we just passed the max_load_factor into the unordered map with the built-in method, unordered_map::max_load_factor.

We also implemented extra credit LRU eviction policy as well as the required FIFO. Both work very similarly by using vectors to store the keys that have been inserted; however, the LRU version will search and move a key to the back of the queue if it has already been inserted, otherwise it will insert it at the end of the queue. The implementation of these evictors are detailed by comments in the code. 

Valgrind gave no memory leaks once we deleted all the values stored on the heap at the appropriate places.

In general most operations on the unordered_map will be O(1), constant time, if there is something like a ton of collisions this can increase towards O(n). But overall it offers very good performance, and coupled with the many standard libary methods, makes it a natural choice for implementing a cache. 

