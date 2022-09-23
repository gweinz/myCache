# Cache in C

Side project I built to strengthen my skills and fundamentals in lower level programming using as few libraries as possible. The following code produces a simple implementation of a multithreaded Cache built in C. The Server is in the Unix domain and takes an address to begin. The server then accepts GET or DEL requests followeed by a KEY over the connection to either retrieve an item from the Cache or adds it to the Cache if there is a miss - one can also invalidate an entry into the Cache with the DEL command. 

Client is written in Python for simplicity sake.
