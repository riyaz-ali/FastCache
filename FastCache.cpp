#include <iostream>
#include <string>
#include <exception>
#include <map>

using namespace std;

#include "FastCache.h"

namespace FastCache {

	/* pushToHead() */
	CacheBlock* LRUCache::pushToHead( CacheBlock* thisBlock )
	{
		if( !_head )
			_head = thisBlock;
		else
		{
			thisBlock->unlink();
			_head->Next(thisBlock);
			thisBlock->Prev(_head);
			_head = thisBlock;
		}
		return _head;
	}

	/* hash() */
	FastCache::HASH FastCache::LRUCache::hash(const char *str)
	{
		unsigned long hash = 5381;
		int c;
		while (c = *str++)
			hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
		
		return hash;
	}

	/* set() */
	void LRUCache::set(const char* key, const unsigned char* _block, int sz)
	{
		if(sz <= 0)return;
		auto hashKey = hash(key);
		auto it = _store.find(hashKey);
		
		//if key exists then bring it to head and update value
		if( it != _store.end() )
		{
			if( it->second == _tail )	//Special case treatment
				_tail = it->second->unlink();

			pushToHead(it->second);
			it->second->data(hashKey,_block,sz);
		}
		//Key does'nt exist but the cache is stil empty
		else if(_inUse < _size)
		{
			//pointer to a free block
			auto temp = &(_alloc_[ _inUse ]);
			//fill in new data
			temp->data(hashKey, _block, sz);
			//push temp to head
			pushToHead(temp);
			//mark an entry for temp in the cache_map
			_store.insert( key_value_pair(hashKey,temp) );
			//finally!
			_inUse++;
		}
		//key does'nt exists and also the cache is full	--time to drop out something!
		else
		{
			//store current tail
			auto temp = _tail;
			//Unlink tail
			_tail = temp->unlink();
			//revoke map entry
			//delete tail entry
			_store.erase( _store.find( temp->key() ) );
			//insert new
			_store.insert( key_value_pair(hashKey,temp) );
			//setup new data in temp
			temp->data(hashKey, _block,sz);
			//push temp to head
			pushToHead(temp);
		}
	}

	/* get() */
	const void* LRUCache::get(const char* key)
	{
		//Lookup for the key
		auto hashKey = hash(key);
		auto it = _store.find(hashKey);
		//if key is not found return nullptr
		if( it == _store.end() )
			return nullptr;	
		
		//if it's found then reposition the block to head and return its value
		pushToHead(it->second);
		return it->second->data();
	}

	#ifdef _DEBUG
	/* dataDump() */
	void LRUCache::dumpData()
	{
		cout << "\n----------------------------DUMPING CACHE DATA------------------------------\n";
		int i=0;
		auto temp = _head;
		while( temp )
		{
			cout << "BLOCK #" << i << "\t" << "KEY: " << temp->key() << " >> " << "DATA: " << (char*)temp->data() << endl;
			temp = temp->Prev();
			i++;
		}
		cout << "\n----------------------------END DUMPING CACHE DATA------------------------------\n";
	}
	#endif

};	//FastCache