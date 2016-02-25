#include <iostream>
#include "FastCache.h"

using namespace std;
using namespace FastCache;

#define CACHE_SIZE 5

char *key[] = 
{
	"ONE",
	"TWO",
	"3",
	"4",
	"FIVE5"
};

char *data[] = 
{
	"Lorem",
	"ipsum",
	"dolor",
	"amet",
	"numquam"
};

int main()
{
	LRUCache cache(CACHE_SIZE);	//a cache of pre-initialised blocks

	for(int i=0; i<CACHE_SIZE; i++)
		cache.set(key[i], (unsigned char*)data[i], strlen(data[i]));		//set data in cache

	cout << (char*)cache.get(key[0]) <<endl;		//will not print anything as key[0] does not exists in the cache
	cout << (char*)cache.get(key[4]) <<endl;		//will print numquam and push key[4] to head of cache

	cache.set(key[3], (unsigned char*)data[0], strlen(data[0]));	//will change "4"=>"amet" to "4"=>"Lorem" and push it to head

	cache.dumpData();

	return 0;
}