# FastCache
A Simple and lightweight LRUCache

Building
========
	There is no separate build process needed! Just drop in the header file (FastCache.h) in your project and you are to go.
	The only requirement is a C++ 11 compliant compiler, in my case it's Visual Studio 2012, any other compiler will also do the job.
	
Usage
=====
	...
	FastCache::LRUCache cache(3);
	cache.set( "KEY HERE", (unsigned char*)"DATA HERE", strlen("DATA HERE") );
	cache.get( "KEY HERE" );
	
	/*Only in debug mode -- This will dump all of the cache's data to stdout*/
	cache.dumpData();
	
Pros
====
	* Lightweight and fast
	* Zero memory fragmentation
	* Allocation and De-Allocation happen only once during construction and destruction
	
Cons
====
	* Rigid in nature
	* May consume memory unnecessarily when cache is empty or not fully used
	
How it works
============
	* During compilation the block size is decided and data is stored in an unsigned char buffer
	* At runtime, during initialization of class object the blocks are dynamically allocated on heap
	* Then pointer manipulation is used to simulate the working of a doubly-linked list, without actually allocating or deallocating memory