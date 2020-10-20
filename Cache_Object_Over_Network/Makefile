CXX=g++-7
CXXFLAGS=-Wall -Wextra -pedantic -std=c++17 -O0 -g -Iinclude/
LDFLAGS=$(CXXFLAGS)
LIBS=-pthread -lboost_program_options
OBJ=$(SRC:.cc=.o)

all:  cache_server test_cache_lib test_cache_client

cache_server: cache_server.o cache_lib.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

test_cache_lib: test_cache_lib.o cache_lib.o fifo_evictor.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

test_cache_client: test_cache_client.o cache_client.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cc %.hh
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

clean:
	rm -rf *.o test_cache_client test_cache_lib cache_server

test: all
	./test_cache_lib
	echo "test_cache_client must be run manually against a running server"

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./test_cache_lib
	valgrind --leak-check=full --show-leak-kinds=all ./test_evictors
