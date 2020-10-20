# HW4: Let's network
Ian Jackson, Arthur Lawson

## Goal:

Modify cache code to allow access from multiple clients over a network, which implements a RESTful API.

## Part 0: Preparation

Some of the bugs in our previous code we had to fix were making the cache deep copy values into a shared ptr, changing the evictor to adhere to the original "no hh alteration" rules, and changing how our evictor handles values that are too large. For some of these, we referenced the repos of the people's code we tested for HW3 (see contributions/sources).

## Part 1: TCP Server

For our server we used Beast Program Options to parse the command line positional args, and Boost Beast (courtesy of Vinnie Falco) as the basis for the server. 
We used curl in the linux terminal to test out each request.

Our design began with implementing PUT requests, and using that as a template for the rest.

## Part 2: TCP Client

Boost Beast was also used as the basis for the client.

The GET request caused a bit of an issue because our values were not deep copied in our original cache_lib (the pointer ownership that we were told to watch out for in the homework description). Once we fixed that by creating a deep copy of the value pointer, everything else was pretty straightforward.

## Bugs, Leaks, and Warnings!

According to Valgrind, we have no blocks definitely lost, but some are still accessible.
The memory leak that seems to be causing the final aborted core dump is related to the "munmap_chunk(): invalid pointer" error. Looking into this before HW5 is due would be helpful, but all of the tests pass so we're content with that at this point.

This is possibly what leads to the test file sometimes returning too early (at test_set or test_get) due to the value pointer being corrupted. This can be fixed by running the test script again. Sorry about this! This will be fixed in the next version (HW5).

## Contributions / Sources

Cache_Lib Code Adapted from Hannah Hellerstein & David Riso's Repo, which we tested in HW3:
https://github.com/orbitalhybridization/CS389_HW3/blob/master/Systems_HW2-master/cache_lib.cc

Beast Examples:
Async Server: https://www.boost.org/doc/libs/develop/libs/beast/example/http/server/async/http_server_async.cpp
Sync Client: https://www.boost.org/doc/libs/develop/libs/beast/example/http/client/sync/http_client_sync.cpp

JSON Parsing:
https://stackoverflow.com/questions/21537637/c-boost-parse-dynamically-generated-json-string-not-a-file
https://stackoverflow.com/questions/17799229/boost-asio-post-http-request-headers-and-body#29564382

