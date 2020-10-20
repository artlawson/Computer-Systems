# HW2: Hash it out
Arthur Lawson, Ian Jackson

### Goal:  
Implement a general look-aside cache with a FIFO eviction policy using the interface (two header files, cache.hh and evictor.hh) provided.

## Part 1: Basic cache operations

Using the Impl class defined as a private member of the Cache class, we implemented the functions set(), get(), del(), reset(), and space_used(). This method for implementation of the member functions made it easier to account for potential changes in the interface. Thus, when the functions were called upon a Cache object, the Impl function of the same name would be invoked. 

## Part 2: Testing

For the testing section of this assignment, we went with eight standard tests conditions:
	1)  the set() function sets the correct value in the cache
	2) the get() function returns the correct value
	3) the reset() function resets the internal storage
	4) the get() function returns "" if the requested key is not found
	5) the get() function returns the correct value if a new one is placed for the same key
	6) the get() function returns a new size if a value is replaced with a new one of a different size
	7) the eviction policy works, in that, when we set() a new value that is larger than the maxmem, we delete the first-in value
	8) the space_used() function returns the correct amount of space used

## Part 3: Performance

We achieved O(1) look-up performance (on average) using an unordered map from the standard library (std::unordered_map). This data structure has an internal hash function that hashes keys to buckets and stores an std::pair of the key and value types.

## Part 4: Collision Resolution

Fortunately, the standard template library's unordered map has collision resolution -- by way of collision chaining -- built into the data structure's functionality. So, handling collisions was done by the implemented unordered map.

## Part 5: Dynamic Resizing

Dynamic resizing was achieved by setting a load factor of 0.75, and rehashing once the load factor breached that limit. The number to rehash to was calculated using [number of buckets / 0.75], since load_factor is equal to buckets / size, and we want it to equal to 0.75, the maximum.

## Part 6: Eviction Policy

The eviction policy we initially implemented is a FIFO (first-in, first-out)-style evictor. The Evictor class utilized member functions touch_key(), which added that key to the queue of items ready to be evicted if need be, and evict(), which deleted the first-in item in the list. We implemented these functions with a subclass, FIFO, which utilized an internal storage in the form of an std::vector.

## Part 7: LRU

The LRU was implemented with an ordered map that has the inputted keys as keys and the number of accesses as values, each time get is called,
the number of accesses increases by one. We returned the lowest value for deciding which thing needs to be evicted next. We ran pretty close to the dealine,
so we did not get to test LRU, but given another hour, we would probably be gravy.

## Bugs, Leaks, and Warnings!

- After running Valgrind on the completed code, we found a 20-byte leak somewhere, and traced it down to the test_cache_lib.cc file, but the leak ceased after more looking around.

- We got all of our tests to work. Most things turned out to be small bugs, per usual. We had an assertion fail at the very end, but we could not find it . We assume
it was apart of our debugging process.
