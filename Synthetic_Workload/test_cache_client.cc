// A testfile

#include "cache.hh"
#include <cassert>
#include "fifo_evictor.hh"
#include <iostream>
#include <cstring>


// no interdependencies
//have 5-12 tests for cache & evictor each

//recquire instead of assert


using byte_type = char;
using val_type = const byte_type*;   // Values for K-V pairs
using size_type = uint32_t;         // Internal indexing to K-V elements


bool test_set(Cache* test_cache, val_type test_val) {
	//cache
	std::cout << "\n-----------------------\nRunning test_set function..." << std::endl;
	test_cache->set("Chilombo",test_val,2);
	size_type sz;
	auto out_val = test_cache->get("Chilombo",sz);
	assert(*out_val == *test_val and "test_cache");
	return true;
}

bool test_get(Cache* test_cache, val_type test_value){
	//cache
	std::cout << "\n-----------------------\nRunning test_get function..." << std::endl;
	test_cache->set("Aiko",test_value,2);
	size_type sz;
	auto out_val = test_cache->get("Aiko",sz);
	assert(*out_val == *test_value and "test_get");
	return true;
}

bool test_no_key(Cache* test_cache) {
	std::cout << "\n-----------------------\nRunning test_no_key function..." << std::endl;
	size_type sz;
	assert(test_cache->get("Taco",sz) == nullptr and "test_no_key");
	return true;
}

bool test_del(Cache* test_cache,val_type test_value) {
	std::cout << "\n-----------------------\nRunning test_del function..." << std::endl;
	test_cache->set("FuriKuri",test_value,2);
	auto out_val = test_cache->del("FuriKuri");
	assert(out_val == true and "test_del");
	return true;
}

bool test_del_no_key(Cache* test_cache) {
	std::cout << "\n-----------------------\nRunning test_del_no_key function..." << std::endl;
	auto out_val = test_cache->del("Mamimi");
	assert(out_val == false and "test_del_no_key");
	return true;
}

bool test_space_used(Cache* test_cache,val_type test_val1,val_type test_val2,val_type test_val3) {
	std::cout << "\n-----------------------\nRunning test_space_used function..." << std::endl;
	test_cache->set("Toe",test_val1,1);
	test_cache->set("Uhh",test_val2,1);
	test_cache->set("Jesus",test_val3,1);
	assert(test_cache->space_used() == 3 and "test_spaced_used");
	return true;
}

bool test_reset(Cache* test_cache, val_type test_val1, val_type test_val2) {
	std::cout << "\n-----------------------\nRunning test_reset function..." << std::endl;
	test_cache->set("Porter",test_val1,2);
	test_cache->set("Shorter",test_val2,3);
	test_cache->reset();
	assert(test_cache->space_used() == 0 and "test_reset");
	return true;
}

std::string& SSS (const char* s)
{
    return *(new std::string(s));
}


//Run all test functions
int main() {
	
	//init evictor-less cache to test
	Cache* test_cache = new Cache("127.0.0.1","6969");
	val_type value1 = "R";
	val_type value2 = "U";
	val_type value3 = "M";

	//First test
	if (test_set(test_cache,value1)) {
		
		std::cout << "Set function test passed!\n" << std::endl;
	}
	test_cache->reset(); 

	//Second test
	if (test_get(test_cache,value1)) {
		std::cout << "\nGet function test passed!\n" << std::endl;
	}
	test_cache->reset();

	// Third test
	if (test_no_key(test_cache)) {
		std::cout << "\nNo key test passed!\n" << std::endl;
	}
	test_cache->reset();
	
	// Fourth test
	if (test_del(test_cache,value2)) {
		std::cout << "\nDel function test passed!\n" << std::endl;
	}
	test_cache->reset();

	// Fifth test
	if (test_del_no_key(test_cache)) {
		std::cout << "\nDel no key function test passed!\n" << std::endl;
	}
	test_cache->reset();

	// Sixth test
	if (test_space_used(test_cache,value1,value2,value3)) {
		std::cout << "\nSpace used function test passed!\n" << std::endl;
	}
	test_cache->reset();

	// Seventh test
	if (test_reset(test_cache,value1,value2)) {
		std::cout << "\nReset function test passed!\n" << std::endl;
	}
	test_cache->reset();

	std::cout << "\n-----------------------\nAll tests passed! Fuck yeah!!\n-----------------------\n" << std::endl;
	std::cout << "Here are some memory leaks as a bonus <3" << std::endl;
	delete test_cache;
	delete value1;
	delete value2;
	delete value3;

	return 1;
}
