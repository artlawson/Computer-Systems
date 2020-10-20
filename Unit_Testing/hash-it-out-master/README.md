# hash-it-out
## Kai Pinckard & Max Rosenberg
### NOTE changes for hw3 were made to this repo accidentally. Please refer to this version for grading -> https://github.com/kai-pinckard/hash-it-out/tree/8f7d8fa5efc49b4c546ae3eeda57ffa9f6f70bb1
### Background
Hash it out is an implementation of a generic look aside cache. It provides key-value storage for items that are difficult or slow to compute. If the cache has the value corresponding to a key requested by the client, then the cache will return the corresponding value. Otherwise it returns a nullptr. 
### 1. Basic Cache Operations
The cache supports the basic operations specified in cache.hh, namely creating a cache object, setting values into the cache, getting values out of the cache, deleting values from the cache, getting the space used, and resetting the cache.
### 2. Testing
In order to verify the functionality of the cache and the FIFO and LRU evictors, a number of tests were written. These tests are automatically run when executing the included makefile. In the event that the makefile runs successfully, the cache has successfully compiled and passed all tests. 
### 3. Performance
The cache provides O(1) asymptotic constant time. In order to do this the cache uses a hash table, created out of standard library vectors. 
### 4. Collision Resolution
To resolve collisions, We implemented separate chaining. As a result, each index of the hash table can simply add other items that hash to the same index on to the end. To identify the correct value when multiple values are stored at the same index the original keys are stored along with the values in these cache objects, enabling the cache to be sure that it has found the exact object item it is looking for by matching the keys. Although the cost of a hash look up potentially involves searching through an entire vector of values, if the hash function does a good job of spreading the items between the indices on average. Furthermore, when the hash table is over populated--when its load factor is too high--it is automatically resized to twice its current size, ensuring asymptotic constant time. The primary advantages of separate chaining over open addressing, and the reasons why separate chaining was implemented for this cache instead of open addressing are that separate chaining is easier to implement and that allows max load factors larger than one. 

### 5. Dynamic Resizing
The cache is implemented such that if a cache object is inserted and the size after insertion exceeds the cache's ```max_load_factor_``` member, the cache doubles in size. 

### 6. Eviction Policy
If an object is to be inserted into the cache and the insertion would not cause a resize, but would exceed ```max_mem_```, the cache will evict an object according to the evictor passed into the cache (or not evict if that is the eviction policy, or if there is no evictor implemented). We created two evictor classes, a FIFO Evictor class and a LRU Evictor class, with corresponding body and header files.  The FIFO Evictor class implements a First In First Out eviction policy where the first object touched by the evictor is the first thing to be evicted. This is implemented with a queue data structure.

### 7. LRU
The LRU Evictor class implements a Least Recently Used eviction policy where the least recently touched object is the first thing to be evicted. In order to adhere to the same resource constraints (O(1) time and O(n) memory as the cache, the LRU evictor was implemented with a linked list and a hash map holding iterators pointing into the linked list. Used in conjunction, these two data structures made it possible to find items in the linked list in O(1) time and to then move them to the head of the linked list in O(1) time as well. With these capabilities, it was possible to provide O(1) performance for both the touch_key method and the evict method. 
