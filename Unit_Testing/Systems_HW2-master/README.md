# Systems\_HW2

Hannah Hellerstein and Danny Riso's work on defining a Cache object with associated hash table, eviction policy,
and O(1) retrieval performance.

## Basic Cache Operations

The Cache class used an unordered\_map to store each K-V pair. To package the value size for instant access along
with the value when calling map[key], the second unit of each pair stored in the unordered\_map is a
user-defined type called storage\_unit, which is its own pair consisting of the value and its size. Most of
the unordered\_map's methods make implementing basic cache functions fairly simple, using mostly C++ library
functions like map.clear() in Cache::reset() and map.find(key) and map.at(key) throughout the code.

## User-Passable Hash Function

unordered\_map's constructor allows for the specification of a hash function at its creation. Given the
default value of nullptr, the unordered\_map is created with the default hash function, std::hash. And
if the user declares their own hash function, it gets initialized at the cache's creation inside the
Cache constructor.

## Collision Resolution

The unordered\_map class has innate collision resolution using separate chaining. We tested for this
by checking the size of certain buckets in the created map while setting and deleting values; when
multiple keys hashed to the same bucket, the number of elements in that bucket increased rather than
another bucket gaining an element. Implementing a second layer of collision resolution over the one
innate in the STL only seemed like needless complication, and since there is no way to directly access
the buckets within the map or turn off the STL's collision resolution, we decided to trust the
collision resolution already implemented in unordered\_map.

## Dynamic Resizing

The unordered\_map class also innately handles dynamic resizing using the rehash() method. It checks
at every operation if the load factor is going to increase beyond the maximum; if it is, it creates a
new unordered\_map with more buckets, rehashes all the current elements, and places them in the new 
table. It then deallocates the old unordered\_map.

## Eviction Policy

The FIFO eviction policy was implemented with the Fifo\_Evictor class, which uses a std::queue of
key\_type values to keep track of the order of keys to evict. For every set() and get() called,
the touch\_key method is called, which adds the relevant key to the back of the queue. Then whenever
set() is called, cache\_lib checks if there is space for the desired addition, and if there isn't,
the K-V pair with the key at the front of the queue is deleted from the unordered map until the
cache has sufficient space for the new value.

## LRU
The LRU eviction policy was implemented with the LRU\_Evictor class, which uses a std::deque of
key\_type values, similar to FIFO's std::queue but with the ability to access and then delete values from
anywhere in the deque, not just the front. This allows the touch\_key method to update the position
of keys in the deque by deleting old values wherever they appear and then moving them to the back,
so that setting or getting any key moves it to the back of the eviction order regardless of when
it was added.

### The Makefile

The Makefile links the runnable program, test\_cache\_lib, with its dependent libraries, cache\_lib
and fifo\_evictor. The relevant commands are:  
(1) make		      - Compiles test\_cache\_lib and its dependents  
(2) make test		  - Runs ./test\_cache\_lib. On a successful run, the entire file will run with no output  
(3) make valgrind	- Runs valgrind ./test\_cache\_lib to check for memory leaks  
(4) make debug		- Runs the gdb debugger on test\_cache\_lib  
(5) make clean		- Tidies up \*.o and executable files

