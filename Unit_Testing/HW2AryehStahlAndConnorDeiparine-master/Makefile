CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -Werror -std=c++2a -g -O3

all: test_cache_lib

test_cache_lib: test_cache_lib.o cache_lib.o fifo_evictor.o lru_evictor.o
	$(CXX) $(CXXFLAGS) -o test_cache_lib cache_lib.o fifo_evictor.o lru_evictor.o test_cache_lib.o

test_cache_lib.o: test_cache_lib.cc cache.hh evictor.hh fifo_evictor.hh lru_evictor.hh
	$(CXX) $(CXXFLAGS) -c test_cache_lib.cc

fifo_evictor.o: fifo_evictor.cc cache.hh evictor.hh fifo_evictor.hh
	$(CXX) $(CXXFLAGS) -c fifo_evictor.cc 

lru_evictor.o: lru_evictor.cc cache.hh evictor.hh lru_evictor.hh
	$(CXX) $(CXXFLAGS) -c lru_evictor.cc 

cache_lib.o: cache_lib.cc cache.hh evictor.hh fifo_evictor.hh lru_evictor.hh
	$(CXX) $(CXXFLAGS) -c cache_lib.cc

clean:
	rm -rf *.o test_cache_lib
