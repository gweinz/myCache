# Cache in C

A cache built from scratch in C. The queue (which I denoted capped queue) takes a capacity and so does the hash map to emulate a finite amount of storage for a cache. Both were written from scratch to demonstrate the core implementations of these ubiquitous data structures.

To compile run:
gcc -o ./exe/mycache server.c cache.c

The executable takes any pathname as an argument for the Unix connection. 

-> ./exe/mycache TEST

Will open a socket to listen for connections over TEST on the Unix domain. The python client script shows how to connect simply.

The server only accepts GET or DEL commands followed by a KEY to retrieve or invalidate keys from a Cache. For simplicity sake I emulated the process of storing data in the hash by assigned each new node with a random integer as its value. The capacity is set to 10 so once 10 items are in the cache any further get operations will result in an eviction that defaults to LRU.
