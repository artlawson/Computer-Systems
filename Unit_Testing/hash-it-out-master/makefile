CC=g++
CFLAGS=-Werror -Wall -Wextra -pedantic

main:	test_cache_lib.o lib_cache.o fifo_evictor.o lru_evictor.o
	$(CC) $(CFLAGS) test_cache_lib.o lib_cache.o lru_evictor.o -o main
	./main
clean:
	rm *.o main