#ifndef JS_HASHTABLE_H
#define JS_HASHTABLE_H
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
#include "JSHashTableCommon.h"

#define USE_JSCOBJ

#define JSHashTable(K) pJSHashTable##K
#define NewJSHashTable(K, hash, cmp) NewJSHashTable##K(hash, cmp)
#define NewJSHashTableFull(K, hash, cmp, freeKey) NewJSHashTableFull##K(hash, cmp, freeKey);
#define JSHashTableEntry(K) pJSHashTableNode##K

#define Declare_JSHashTable(K)\
typedef int (*Compare##K)(const K t1, const K t2);\
__JS_DecCHashTableNode(K)\
__JS_DecCHashTable(K) \
__JS_DecHashTablePut(K) \
__JS_DecHashTableResize(K) \
__JS_DecHashTableContainsKey(K) \
__JS_DecHashTableRemove(K) \
__JS_DecHashTableClear(K) \
__JS_DecHashTableFree(K) \
__JS_DecNewHashTable(K)

#define Define_JSHashTable(K)\
__JS_DefCHashTableNode(K)\
__JS_DefHashTablePut(K) \
__JS_DefHashTableResize(K) \
__JS_DefHashTableContainsKey(K) \
__JS_DefHashTableRemove(K) \
__JS_DefHashTableClear(K) \
__JS_DefHashTableFree(K) \
__JS_DefNewHashTable(K)


#define __JS_DecCHashTableNode(K) \
typedef struct JSHashTableNode##K *JSHashTableEntry(K);\
typedef struct JSHashTableNode##K {\
	K key;\
	JSHashTableEntry(K) _nextInBucket;\
}JSHashTableNode##K;\
struct JSHashTableNode##K;\
JSHashTableEntry(K) _NewJSHashTableNode##K(K k);

#define __JS_DefCHashTableNode(K)\
JSHashTableEntry(K) _NewJSHashTableNode##K(K k) {\
	JSHashTableEntry(K) node = (JSHashTableEntry(K))malloc(sizeof(JSHashTableNode##K));\
	node->key = k;\
	node->_nextInBucket = NULL;\
	return node;\
}

#define JSHashTableForEach(K, _map_, entry) \
for(int (_i_) = 0; (_i_) < (_map_)->length; (_i_)++) \
for(JSHashTableEntry(K) entry = (_map_)->buckets[(_i_)]; (entry); (entry) = (entry)->_nextInBucket)

#define __JS_DecCHashTable(K)\
typedef unsigned int(*JSHashFunc##K)(K k);\
Define_Class(JSHashTable##K)\
	JSHashTableEntry(K) *buckets;\
	JSHashTableEntry(K) _cachedItem;\
	size_t length; /*num of buckets*/\
	size_t size; /*num of items*/\
	JSHashFunc##K h;\
	Compare##K compare;\
	void (*freeKey)(K key);\
With_Methods(JSHashTable##K)\
	void (*put)(K k);\
	int  (*containsKey)(K k);\
	void    (*remove)(K k);\
	void (*clear)();\
	void(*free)();\
End_Class(JSHashTable##K)



#define __JS_DecHashTablePut(K)\
void __JS_HashTablePut##K(K k);

#define __JS_DefHashTablePut(K)\
void __JS_HashTablePut##K(K k) {\
	/****/getSelf(JSHashTable##K)/****/\
	if (((double)self->size) / self->length > JS_CAL_LOAD_FACTOR) {\
		__JS_HashTableResize##K(self->length * 2);\
	}\
	int b = self->h(k) % self->length;\
	JSHashTableEntry(K) p = self->buckets[b];\
	Compare##K cmp = self->compare;\
	while (p) {\
		if (cmp(p->key, k) == 0) {\
			return;\
		}\
		p = p->_nextInBucket;\
	}\
	/*add to the head of bucket*/\
	JSHashTableEntry(K) node = _NewJSHashTableNode##K(k);\
	node->_nextInBucket = self->buckets[b];\
	self->buckets[b] = node;\
	/*update size*/\
	self->size++;\
}

#define __JS_DecHashTableResize(K) \
void __JS_HashTableResize##K(int len);

#define __JS_DefHashTableResize(K)\
void __JS_HashTableResize##K(int len) {\
	/****/getSelf(JSHashTable##K)/****/\
	int b;\
	JSHashTableEntry(K) *newBuckets = (JSHashTableEntry(K) *)calloc(len, sizeof(JSHashTableEntry(K) *));\
    JSHashTableEntry(K) oldNext, p;\
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

#define __JS_DecHashTableContainsKey(K)\
int __JS_HashTableContainsKey##K(K k);

#define __JS_DefHashTableContainsKey(K)\
int __JS_HashTableContainsKey##K(K k) {\
	/****/getSelf(JSHashTable##K)/****/\
		int b = self->h(k) % self->length;\
	JSHashTableEntry(K) p = self->buckets[b];\
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

#define __JS_DecHashTableRemove(K)\
void __JS_HashTableRemove##K(K k);

#define __JS_DefHashTableRemove(K)\
void __JS_HashTableRemove##K(K k) {\
	/****/getSelf(JSHashTable##K)/****/\
	int b = self->h(k) % self->length;\
	JSHashTableEntry(K) p = self->buckets[b], q;\
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
#define __JS_DecHashTableClear(K)\
void __JS_HashTableClear##K();

#define __JS_DefHashTableClear(K)\
void __JS_HashTableClear##K() {\
	/****/getSelf(JSHashTable##K)/****/\
	JSHashTableEntry(K) p = NULL, oldNext = NULL;\
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
	__JS_HashTableResize##K(JS_INI_BUCKETS);\
}


#define __JS_DecHashTableFree(K)\
void __JS_HashTableFree##K();

#define __JS_DefHashTableFree(K)\
void __JS_HashTableFree##K() {\
	/****/getSelf(JSHashTable##K)/****/\
	m(self)->clear();\
	free(self->buckets);\
	free(self);\
}
#define __JS_DecNewHashTable(K) \
pJSHashTable##K NewJSHashTable##K(JSHashFunc##K hash, Compare##K compare);\
pJSHashTable##K NewJSHashTableFull##K(JSHashFunc##K hash, Compare##K compare, void (*freeKey)(K key));

#define __JS_DefNewHashTable(K)\
Install_Methods(JSHashTable##K) \
__JS_HashTablePut##K,\
__JS_HashTableContainsKey##K,\
__JS_HashTableRemove##K,\
__JS_HashTableClear##K,\
__JS_HashTableFree##K \
End_Install()\
pJSHashTable##K NewJSHashTable##K(JSHashFunc##K hash, Compare##K compare) {\
	return NewJSHashTableFull##K(hash, compare, NULL);\
}\
pJSHashTable##K NewJSHashTableFull##K(JSHashFunc##K hash, Compare##K compare, void (*freeKey)(K key)) {\
	/****/Alloc_Instance(map, JSHashTable##K)/****/\
	map->buckets = (JSHashTableEntry(K) *)calloc(JS_INI_BUCKETS, sizeof(JSHashTableEntry(K)));\
	map->_cachedItem = NULL;\
	map->length = JS_INI_BUCKETS;\
	map->size = 0;\
	map->h = hash;\
	map->compare = compare;\
	map->freeKey = freeKey;\
	return map;\
}

#endif
