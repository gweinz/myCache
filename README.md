# Threaded Cache in C

A multithreaded cache built from scratch in C. The queue (which I denoted capped queue) takes a capacity and so does the hash map to emulate a finite amount of storage for a cache. Both were written from scratch to demonstrate the core implementations of these ubiquitous data structures.

To compile run:
gcc -o ./exe/mycache server.c cache.c

The executable takes any pathname as an argument for the Unix connection. 

-> ./exe/mycache TEST

Will open a socket to listen for connections over TEST on the Unix domain. The python client script shows how to connect simply.

The server only accepts GET or DEL commands followed by a KEY to retrieve or invalidate keys from a Cache. At the moment this lacks the ability to store actual data beyond the keys however that will be implemented soon.
