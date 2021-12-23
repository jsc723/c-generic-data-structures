#ifndef JS_HASHSET_H
#define JS_HASHSET_H
/*
	Version 1.0
	By Sicheng Jiang
	An HashSet implemented in C.
	2021/12/22
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jscobj2.h"
#include "JSHashTableCommon.h"

#define USE_JSCOBJ

#define JSHashSet(K) pJSHashSet##K
#define NewJSHashSet(K, hash, cmp) NewJSHashSet##K(hash, cmp)
#define NewJSHashSetFull(K, hash, cmp, freeKey) NewJSHashSetFull##K(hash, cmp, freeKey)
#define JSHashSetEntry(K) pJSHashSetNode##K

#define Declare_JSHashSet(K)\
JS__DefCompare(K)\
__JS_DecCHashSetNode(K)\
__JS_DecCHashSet(K) \
__JS_DecHashSetPut(K) \
__JS_DecHashSetResize(K) \
__JS_DecHashSetContainsKey(K) \
__JS_DecHashSetRemove(K) \
__JS_DecHashSetClear(K) \
__JS_DecHashSetFree(K) \
__JS_DecNewHashSet(K)

#define Define_JSHashSet(K)\
__JS_DefCHashSetNode(K)\
__JS_DefHashSetPut(K) \
__JS_DefHashSetResize(K) \
__JS_DefHashSetContainsKey(K) \
__JS_DefHashSetRemove(K) \
__JS_DefHashSetClear(K) \
__JS_DefHashSetFree(K) \
__JS_DefNewHashSet(K)


#define __JS_DecCHashSetNode(K) \
typedef struct JSHashSetNode##K *JSHashSetEntry(K);\
typedef struct JSHashSetNode##K {\
	K key;\
	JSHashSetEntry(K) _nextInBucket;\
}JSHashSetNode##K;\
struct JSHashSetNode##K;\
JSHashSetEntry(K) _NewJSHashSetNode##K(K k);

#define __JS_DefCHashSetNode(K)\
JSHashSetEntry(K) _NewJSHashSetNode##K(K k) {\
	JSHashSetEntry(K) node = (JSHashSetEntry(K))malloc(sizeof(JSHashSetNode##K));\
	node->key = k;\
	node->_nextInBucket = NULL;\
	return node;\
}

#define JSHashSetForEach(K, _map_, entry) \
for(int (_i_) = 0; (_i_) < (_map_)->length; (_i_)++) \
for(JSHashSetEntry(K) entry = (_map_)->buckets[(_i_)]; (entry); (entry) = (entry)->_nextInBucket)

#define __JS_DecCHashSet(K)\
typedef unsigned int(*JSHashFunc##K)(K k);\
Define_Class(JSHashSet##K)\
	JSHashSetEntry(K) *buckets;\
	JSHashSetEntry(K) _cachedItem;\
	size_t length; /*num of buckets*/\
	size_t size; /*num of items*/\
	JSHashFunc##K h;\
	JSCompare(K) compare;\
	void (*freeKey)(K key);\
With_Methods(JSHashSet##K)\
	void (*put)(K k);\
	int  (*containsKey)(K k);\
	void    (*remove)(K k);\
	void (*clear)();\
	void(*free)();\
End_Class(JSHashSet##K)



#define __JS_DecHashSetPut(K)\
void __JS_HashSetPut##K(K k);

#define __JS_DefHashSetPut(K)\
void __JS_HashSetPut##K(K k) {\
	/****/getSelf(JSHashSet##K)/****/\
	if (((double)self->size) / self->length > JS_CAL_LOAD_FACTOR) {\
		__JS_HashSetResize##K(self->length * 2);\
	}\
	int b = self->h(k) % self->length;\
	JSHashSetEntry(K) p = self->buckets[b];\
	JSCompare(K) cmp = self->compare;\
	while (p) {\
		if (cmp(p->key, k) == 0) {\
			return;\
		}\
		p = p->_nextInBucket;\
	}\
	/*add to the head of bucket*/\
	JSHashSetEntry(K) node = _NewJSHashSetNode##K(k);\
	node->_nextInBucket = self->buckets[b];\
	self->buckets[b] = node;\
	/*update size*/\
	self->size++;\
}

#define __JS_DecHashSetResize(K) \
void __JS_HashSetResize##K(int len);

#define __JS_DefHashSetResize(K)\
void __JS_HashSetResize##K(int len) {\
	/****/getSelf(JSHashSet##K)/****/\
	int b;\
	JSHashSetEntry(K) *newBuckets = (JSHashSetEntry(K) *)calloc(len, sizeof(JSHashSetEntry(K) *));\
    JSHashSetEntry(K) oldNext, p;\
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

#define __JS_DecHashSetContainsKey(K)\
int __JS_HashSetContainsKey##K(K k);

#define __JS_DefHashSetContainsKey(K)\
int __JS_HashSetContainsKey##K(K k) {\
	/****/getSelf(JSHashSet##K)/****/\
		int b = self->h(k) % self->length;\
	JSHashSetEntry(K) p = self->buckets[b];\
	JSCompare(K) cmp = self->compare;\
	while (p) {\
		if (cmp(p->key, k) == 0) {\
			self->_cachedItem = p;\
			return 1;\
		}\
		p = p->_nextInBucket;\
	}\
	return 0;\
}

#define __JS_DecHashSetRemove(K)\
void __JS_HashSetRemove##K(K k);

#define __JS_DefHashSetRemove(K)\
void __JS_HashSetRemove##K(K k) {\
	/****/getSelf(JSHashSet##K)/****/\
	int b = self->h(k) % self->length;\
	JSHashSetEntry(K) p = self->buckets[b], q;\
	JSCompare(K) cmp = self->compare;\
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
#define __JS_DecHashSetClear(K)\
void __JS_HashSetClear##K();

#define __JS_DefHashSetClear(K)\
void __JS_HashSetClear##K() {\
	/****/getSelf(JSHashSet##K)/****/\
	JSHashSetEntry(K) p = NULL, oldNext = NULL;\
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
	__JS_HashSetResize##K(JS_INI_BUCKETS);\
}


#define __JS_DecHashSetFree(K)\
void __JS_HashSetFree##K();

#define __JS_DefHashSetFree(K)\
void __JS_HashSetFree##K() {\
	/****/getSelf(JSHashSet##K)/****/\
	methodof(self)->clear();\
	free(self->buckets);\
	free(self);\
}
#define __JS_DecNewHashSet(K) \
pJSHashSet##K NewJSHashSet##K(JSHashFunc##K hash, JSCompare(K) compare);\
pJSHashSet##K NewJSHashSetFull##K(JSHashFunc##K hash, JSCompare(K) compare, void (*freeKey)(K key));

#define __JS_DefNewHashSet(K)\
Install_Methods(JSHashSet##K) \
__JS_HashSetPut##K,\
__JS_HashSetContainsKey##K,\
__JS_HashSetRemove##K,\
__JS_HashSetClear##K,\
__JS_HashSetFree##K \
End_Install()\
pJSHashSet##K NewJSHashSet##K(JSHashFunc##K hash, JSCompare(K) compare) {\
	return NewJSHashSetFull##K(hash, compare, NULL);\
}\
pJSHashSet##K NewJSHashSetFull##K(JSHashFunc##K hash, JSCompare(K) compare, void (*freeKey)(K key)) {\
	/****/Alloc_Instance(map, JSHashSet##K)/****/\
	map->buckets = (JSHashSetEntry(K) *)calloc(JS_INI_BUCKETS, sizeof(JSHashSetEntry(K)));\
	map->_cachedItem = NULL;\
	map->length = JS_INI_BUCKETS;\
	map->size = 0;\
	map->h = hash;\
	map->compare = compare;\
	map->freeKey = freeKey;\
	return map;\
}

#endif
