// A testfile

#include "cache.hh"
#include "fifo_evictor.hh"
#include <cassert>
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
	test_cache->set("Chilombo",test_val,2);
	size_type sz = 0;
	auto out_val = test_cache->get("Chilombo",sz);
	assert(*(out_val) == *test_val and "test_set");
	return true;
}

bool test_get(Cache* test_cache, val_type test_value){
	//cache
	test_cache->set("Aiko",test_value,2);
	size_type sz = 0;
	auto out_val = test_cache->get("Aiko",sz);
	assert(*out_val == *test_value and "test_get");
	return true;
}

bool test_reset(Cache* test_cache, val_type test_val1, val_type test_val2) {
	//cache
	test_cache->set("Porter",test_val1,2);
	test_cache->set("Shorter",test_val2,3);

	test_cache->reset();
	assert(test_cache->space_used() == 0 and "test_reset");
	return true;
}

bool test_no_key(Cache* test_cache) {
	//cache
	size_type sz = 0;
	assert(test_cache->get("Taco",sz) == nullptr and "test_no_key");
	return true;
}

bool test_changed_key(Cache* test_cache,val_type test_val1,val_type test_val2) {
	//cache
	test_cache->set("Matrix",test_val1,2);
	test_cache->set("Matrix",test_val2,2);
	size_type sz = 0;
	auto out_val = test_cache->get("Matrix",sz);
	assert(*out_val == *test_val2 and "test_changed_key");
	return true;
}

bool test_changed_key_size(Cache* test_cache,val_type test_val1, val_type test_val2) {
	//cache
	test_cache->set("AVG",test_val1,2);
	test_cache->set("AVG",test_val2,3);
	size_type sz  = 0;
	test_cache->get("AVG",sz);
	assert(sz == 3 and "test_changed_key_size");
	return true;
}

bool test_evictor(Cache* test_cache,val_type test_val1,val_type test_val2,val_type test_val3) {
	//evictor
	test_cache->set("Macy",test_val1,3);
	test_cache->set("Eitan",test_val2,3);
	//this last value should cause us to evict because capacity = 10
	test_cache->set("Policy",test_val3,7);
	size_type sz = 0;
	assert(test_cache->get("Macy",sz) == nullptr and "test_evictor");

	return true;
}

bool test_space_used(Cache* test_cache,val_type test_val1,val_type test_val2,val_type test_val3) {
	//cache
	test_cache->set("Toe",test_val1,1);
	test_cache->set("Uhh",test_val2,4);
	test_cache->set("Jesus",test_val3,1);
	assert(test_cache->space_used() == 6 and "test_spaced_used");
	return true;
}

bool test_word_exceed(Cache* test_cache,val_type test_val){
	//evictor/cache
	test_cache->set("Macy",test_val,31);
	//this value should cause an evict because max_size is 10
	size_type sz = 0;
	assert(test_cache->get("Macy",sz) == nullptr and "cache word exceeded");
	return true;

}

bool test_evictor_twice(Cache* test_cache,val_type test_val1, val_type test_val2, val_type test_val3){
	//evictor
	test_cache->set("Macy",test_val1,3);
	test_cache->set("YessirSki", test_val2, 8);
	//this value should cause an evict because max_size is 10
	test_cache->set("Bud Light", test_val3, 3);
	//this value should cause another evict because max_size is 10

	size_type sz = 0;
	assert(test_cache->get("Macy",sz) == nullptr and "test_evictor twice");
	assert(test_cache->get("YessirSki",sz) == nullptr and "test_evictor twice");

	return true;
}

bool test_evictor_empty(Cache* test_cache,val_type test_val,FIFO* evictor){
	//evictor
	test_cache->set("Macy",test_val,3);
	test_cache -> reset();
	auto evicted_key = evictor -> evict();
	auto eval = evicted_key == "";
	//evicting here should return "" because we've reset the cache and should, therefore
	//reset the evictor
	assert((eval) and "test_evictor evictor empty");
	return true;

}

bool test_evictor_with_del(Cache* test_cache,val_type test_val1,val_type test_val2,val_type test_val3) {
	//evictor
	//make sure 2nd value is the value evicted after deletion of first value
	//tests accuracy of evict
	test_cache->set("Robert",test_val1,1);
	test_cache->set("Uhh",test_val2,4);
	test_cache->set("Jesus",test_val3,1);
	test_cache -> del("Robert");
	test_cache -> set("Otha", test_val3, 8);
	size_type sz = 0;
	assert(test_cache->get("Robert",sz) == nullptr and "test evictor after delete used");
	return true;
}

bool test_evictor_with_repeat_keys(Cache* test_cache,val_type test_val1,val_type test_val2,val_type test_val3) {
	//evictor
	//test evictor with repeat keys, make sure right key/value pair gets evicted
	test_cache->set("repeat",test_val1,1);
	test_cache->set("repeat",test_val2,8);
	test_cache->set("repeat",test_val3,2);
	size_type sz = 0;
	auto out_val = test_cache->get("repeat",sz);
	assert(*out_val == *test_val3 and "test evictor with repeat keys");
	return true;
}


//Run all test functions
int main() {
	
	//init evictor-less cache to test
	Cache* test_cache = new Cache(10);
	auto evictor = new FIFO();
	//init cache w/evictor
	Cache* test_cache_with_evictor = new Cache(10,0.75,evictor);
	val_type value1 = "R";
	val_type value2 = "U";
	val_type value3 = "M";
	

	//First test
	if (test_set(test_cache,value1)) {
		
		std::cout << "Set function test passed!" << std::endl;
	}

	test_cache->reset(); 
	
	//Second test
	if (test_get(test_cache,value1)) {
		std::cout << "Get function test passed!" << std::endl;
	}
	test_cache->reset();

	//Third test
	if (test_reset(test_cache,value1,value2)) {
		std::cout << "Reset function test passed!" << std::endl;
	}
	test_cache->reset();

	//Fourth test
	if (test_no_key(test_cache)) {
		std::cout << "No key test passed!" << std::endl;
	}
	test_cache->reset();

	//Fifth test
	if (test_changed_key(test_cache,value1,value2)) {
		std::cout << "Changed key test passed!" << std::endl;
	}
	test_cache->reset();

	//Sixth test
	if (test_changed_key_size(test_cache,value1,value2)) {
		std::cout << "Changed size test passed!" << std::endl;
	}
	test_cache->reset();

	//Seventh test
	if (test_evictor(test_cache_with_evictor,value1,value2,value3)) {
		std::cout << "Evicted key test passed!" << std::endl;
	}
	test_cache_with_evictor->reset();

	//Eighth test
	if (test_space_used(test_cache,value1,value2,value3)) {
		std::cout << "Space used function test passed!" << std::endl;
	}
	test_cache->reset();

	//9th test
	if (test_word_exceed(test_cache_with_evictor, value1)){
		std::cout<< "Cache word exceeded test passed!"<<std::endl;
	}
	test_cache_with_evictor -> reset();

	//10th test
	if (test_evictor_twice(test_cache_with_evictor, value1, value2, value3)){
		std::cout<< "Evictor twice test passed!"<<std::endl;
	}
	test_cache_with_evictor -> reset();

	//11th test
	if (test_evictor_empty(test_cache_with_evictor, value1,evictor)){
		std::cout<< "Evictor empty test passed!"<<std::endl;
	}
	test_cache_with_evictor -> reset();

	//12th test
	if (test_evictor_with_del(test_cache_with_evictor,value1, value2, value3)){
		std::cout<< "Evictor with del test passed!"<<std::endl;
	}
	test_cache_with_evictor -> reset();

	//13th test
	if(test_evictor_with_repeat_keys(test_cache_with_evictor,value1, value2, value3)){
		std::cout<< "Evictor repeat keys test passed!"<<std::endl;
	}
	test_cache_with_evictor-> reset();

	std::cout << "All tests passed! Fuck yeah!!" << std::endl;
	delete test_cache;
	delete test_cache_with_evictor;
	delete evictor;
	delete value1;
	delete value2;
	delete value3;

	return 1;
}
