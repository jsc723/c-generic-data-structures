#ifndef CHASHTABLE_H
#define CHASHTABLE_H
/*
	Version 1.0
	By Sicheng Jiang
	An HashTable implemented in C.
	2021/12/22
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jscobj2.h"

#define USE_JSCOBJ

#define CAL_LOAD_FACTOR 2.0
#define INI_BUCKETS 8

#define HashTable(K) pCHashTable##K
#define NewHashTable(K, hash, cmp) NewHashTable##K(hash, cmp)
#define HashTableEntry(K) pCHashTableNode##K

#define Declare_CHashTable(K)\
__DefCompare(K)\
__DecCHashTableNode(K)\
__DecCHashTable(K) \
__DecHashTablePut(K) \
__DecHashTableResize(K) \
__DecHashTableContainsKey(K) \
__DecHashTableRemove(K) \
__DecHashTableClear(K) \
__DecHashTableFree(K) \
__DecNewHashTable(K)

#define Define_CHashTable(K)\
 __DefCHashTableNode(K)\
 __DefHashTablePut(K) \
__DefHashTableResize(K) \
 __DefHashTableContainsKey(K) \
__DefHashTableRemove(K) \
__DefHashTableClear(K) \
 __DefHashTableFree(K) \
__DefNewHashTable(K)


#define __DecCHashTableNode(K) \
typedef struct CHashTableNode##K *pCHashTableNode##K;\
typedef struct CHashTableNode##K {\
	K key;\
	pCHashTableNode##K _nextInBucket;\
}CHashTableNode##K;\
struct CHashTableNode##K;\
pCHashTableNode##K _NewCHashTableNode##K(K k);

#define __DefCHashTableNode(K)\
pCHashTableNode##K _NewCHashTableNode##K(K k) {\
	pCHashTableNode##K node = (pCHashTableNode##K)malloc(sizeof(CHashTableNode##K));\
	node->key = k;\
	node->_nextInBucket = NULL;\
	return node;\
}

#define HashTableForEach(K, _map_, entry) \
for(int (_i_) = 0; (_i_) < (_map_)->length; (_i_)++) \
for(pCHashTableNode##K entry = (_map_)->buckets[(_i_)]; (entry); (entry) = (entry)->_nextInBucket)

#define __DecCHashTable(K)\
typedef unsigned int(*HashFunc##K)(K k);\
Define_Class(CHashTable##K)\
	pCHashTableNode##K *buckets;\
	pCHashTableNode##K _cachedItem;\
	size_t length; /*num of buckets*/\
	size_t size; /*num of items*/\
	HashFunc##K h;\
	Compare##K compare;\
	void (*freeKey)(K key);\
With_Methods(CHashTable##K)\
	void (*put)(K k);\
	int  (*containsKey)(K k);\
	void    (*remove)(K k);\
	void (*clear)();\
	void(*free)();\
End_Class(CHashTable##K)



#define __DecHashTablePut(K)\
void __HashTablePut##K(K k);

#define __DefHashTablePut(K)\
void __HashTablePut##K(K k) {\
	/****/getSelf(CHashTable##K)/****/\
	if (((double)self->size) / self->length > CAL_LOAD_FACTOR) {\
		__HashTableResize##K(self->length * 2);\
	}\
	int b = self->h(k) % self->length;\
	pCHashTableNode##K p = self->buckets[b];\
	Compare##K cmp = self->compare;\
	while (p) {\
		if (cmp(p->key, k) == 0) {\
			if(self->freeKey) self->freeKey(k);\
			return;\
		}\
		p = p->_nextInBucket;\
	}\
	/*add to the head of bucket*/\
	pCHashTableNode##K node = _NewCHashTableNode##K(k);\
	node->_nextInBucket = self->buckets[b];\
	self->buckets[b] = node;\
	/*update size*/\
	self->size++;\
}

#define __DecHashTableResize(K) \
void __HashTableResize##K(int len);

#define __DefHashTableResize(K)\
void __HashTableResize##K(int len) {\
	/****/getSelf(CHashTable##K)/****/\
	int b;\
	pCHashTableNode##K *newBuckets = (pCHashTableNode##K *)calloc(len, sizeof(pCHashTableNode##K *));\
    pCHashTableNode##K oldNext, p;\
	for(int i = 0; i < self->length; i++) \
    for(p = self->buckets[i]; p; p = oldNext) {\
		b = self->h(p->key) % len;\
        oldNext = p->_nextInBucket;\
		/*add to the head of the new bucket*/\
		p->_nextInBucket = newBuckets[b];\
		newBuckets[b] = p;\
	}\
	free(self->buckets);\
	self->buckets = newBuckets;\
	self->length = len;\
}

#define __DecHashTableContainsKey(K)\
int __HashTableContainsKey##K(K k);

#define __DefHashTableContainsKey(K)\
int __HashTableContainsKey##K(K k) {\
	/****/getSelf(CHashTable##K)/****/\
		int b = self->h(k) % self->length;\
	pCHashTableNode##K p = self->buckets[b];\
	Compare##K cmp = self->compare;\
	while (p) {\
		if (cmp(p->key, k) == 0) {\
			self->_cachedItem = p;\
			return 1;\
		}\
		p = p->_nextInBucket;\
	}\
	return 0;\
}

#define __DecHashTableRemove(K)\
void __HashTableRemove##K(K k);

#define __DefHashTableRemove(K)\
void __HashTableRemove##K(K k) {\
	/****/getSelf(CHashTable##K)/****/\
	int b = self->h(k) % self->length;\
	pCHashTableNode##K p = self->buckets[b], q;\
	Compare##K cmp = self->compare;\
	if (p && cmp(p->key, k) == 0) {\
		self->buckets[b] = p->_nextInBucket;\
	}\
	else {\
		while(p) {\
			q = p;\
			p = p->_nextInBucket;\
			if (p && cmp(p->key, k) == 0) {\
				q->_nextInBucket = p->_nextInBucket;\
				break;\
			}\
		}\
	}\
	if (p) {\
		if (self->freeKey)\
			self->freeKey(p->key);\
		free(p);\
	}\
}

//clean all items, does not reset hash function and equals function
#define __DecHashTableClear(K)\
void __HashTableClear##K();

#define __DefHashTableClear(K)\
void __HashTableClear##K() {\
	/****/getSelf(CHashTable##K)/****/\
	pCHashTableNode##K p = NULL, oldNext = NULL;\
	void(*freeKey)(K key) = self->freeKey;\
	for(int i = 0; i < self->length; i++) \
    for(p = self->buckets[i]; p; p = oldNext) {\
		oldNext = p->_nextInBucket;\
		if (freeKey)\
			freeKey(p->key);\
		free(p);\
	}\
	self->size = 0;\
	self->_cachedItem = NULL;\
	__HashTableResize##K(INI_BUCKETS);\
}


#define __DecHashTableFree(K)\
void __HashTableFree##K();

#define __DefHashTableFree(K)\
void __HashTableFree##K() {\
	/****/getSelf(CHashTable##K)/****/\
	m(self)->clear();\
	free(self->buckets);\
	free(self);\
}
#define __DecNewHashTable(K) \
pCHashTable##K NewHashTable##K(HashFunc##K hash, Compare##K compare);

#define __DefNewHashTable(K)\
Install_Methods(CHashTable##K) \
__HashTablePut##K,\
__HashTableContainsKey##K,\
__HashTableRemove##K,\
__HashTableClear##K,\
__HashTableFree##K \
End_Install()\
pCHashTable##K NewHashTable##K(HashFunc##K hash, Compare##K compare) {\
	/****/Alloc_Instance(map, CHashTable##K)/****/\
	map->buckets = (pCHashTableNode##K *)calloc(INI_BUCKETS, sizeof(pCHashTableNode##K));\
	map->_cachedItem = NULL;\
	map->length = INI_BUCKETS;\
	map->size = 0;\
	map->h = hash;\
	map->compare = compare;\
	map->freeKey = NULL;\
	return map;\
}

#endif
