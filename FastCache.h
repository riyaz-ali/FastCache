#ifndef FAST_CACHE_H
#define FAST_CACHE_H

#include <iostream>
#include <string>
#include <exception>
#include <map>

using namespace std;

/* The user must define a block size or else the default will be used*/
#ifndef FAST_CACHE_BLOCK_SIZE
#define FAST_CACHE_BLOCK_SIZE	(102400) //100kb default size
#endif	//FAST_CACHE_BLOCK_SIZE

#if FAST_CACHE_BLOCK_SIZE < (50)	//lesser than 50bytes
#error Block size too low. Constituting overhead will be much larger! 
#elif FAST_CACHE_BLOCK_SIZE > (1024*1024)	//Greater than 1mb
#error Block size too large.
#endif


namespace FastCache {
	
	//forward declaration
	class CacheBlock;
	class LRUCache;

	/*define custom types*/
	typedef unsigned long HASH;
	typedef pair<HASH,CacheBlock*> key_value_pair;
	typedef map<HASH,CacheBlock*>  cache_map;
	typedef unsigned char BYTE;

		/*
		 * @class	CacheBlock
		 * @brief	low level storage container
		 */
		class CacheBlock {
			HASH _key;
			BYTE* _storage[FAST_CACHE_BLOCK_SIZE];
			CacheBlock* prev;
			CacheBlock* next;
			
			//[INTERNAL]
			void Clear(){
				memset(_storage, 0, FAST_CACHE_BLOCK_SIZE);
				_key = 0;
			}

		public:
			//CTOR
			CacheBlock() :
				prev(nullptr),
				next(nullptr) {
					Clear();
			}
			//DTOR
			~CacheBlock(){
				/*No operation here since no dynamically allocated memory is present!*/
			}


			//Data manipulation
			int data(HASH k, const BYTE* _d,int len = FAST_CACHE_BLOCK_SIZE) {
				Clear();
				if(len > FAST_CACHE_BLOCK_SIZE)
				{
					cerr << "Length of source greater than block size! Possible loss of data" << endl;
					len = FAST_CACHE_BLOCK_SIZE;	//truncate max length
				}
				memcpy(_storage, _d, len);
				_key = k;
				return 0;
			}
			const void* data(){
				return _storage;
			}
			HASH key(){
				return _key;
			}


			//Relation (link) manipulation
			CacheBlock*	unlink(){
				CacheBlock* temp = next;
				if(next)
					next->Prev(prev);
				if(prev)
					prev->Next(next);
				next = prev = nullptr;
				return temp;
			}
			CacheBlock*	Next()					{return next;}
			void		Next(CacheBlock* nxt)	{next = nxt;}
			CacheBlock* Prev()					{return prev;}
			void		Prev(CacheBlock* prv)	{prev = prv;}
		};	//CacheBlock


		/*
		 * @class	CacheListManager	--creates a fixed size list
		 * @brief	Responsible to manage the doubly-linked list (via pointer manipulation)
		 * @data	stores arbitary data, i.e., no single particular type
		 */
		class LRUCache {
			CacheBlock* _alloc_;		//points to begining of allocation region --should not be altered!
			int	_size;					//size of cache

			/*manage list with these pointers*/
			CacheBlock* _head;
			CacheBlock* _tail;
			int _inUse;

			/*key-value map*/
			cache_map _store;

			//[INTERNAL]
			CacheBlock* pushToHead(CacheBlock*);
			HASH hash(const char*);	//djb2 hash algorithm	--why? It is fast and collision rate is significantly low

		public:
			//CTOR
			LRUCache(int sz) {
				//needed to catch exception from new operator
				try {
					_inUse = 0;
					_size = sz;
					_alloc_ = new CacheBlock[_size];
					_tail = _alloc_;
					_head = nullptr;
				}catch(bad_alloc& bd){
					throw bd;
				}
			}

			//DTOR
			~LRUCache() {
				delete[] _alloc_;
			}


			//cache interface routines
			void set(const char*, const unsigned char*, int);
			const void* get(const char*);

			//[DEBUG ONLY]
		#ifdef _DEBUG
			void dumpData();
		#else
			void dumpData() {}
		#endif

		};	//CacheListManager

};	//FastCache

#endif	//FAST_CACHE_H