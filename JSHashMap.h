#ifndef JS_HashMap_H
#define JS_HashMap_H
/*
	Version 1.0
	By Sicheng Jiang
	An HashMap implemented in C.
	2021/12/22
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jscobj2.h"
#include "JSHashTableCommon.h"

#define USE_JSCOBJ

#define JSHashMap(K, V) pJSHashMap##K##V
#define NewJSHashMap(K, V, hash, cmp) NewJSHashMap##K##V(hash, cmp)
#define NewJSHashMapFull(K, V, hash, cmp, freeKey) NewJSHashMapFull##K##V(hash, cmp, freeKey);
#define JSHashMapEntry(K, V) pJSHashMapNode##K

#define Declare_JSHashMap(K, V)\
typedef int (*Compare##K)(const K t1, const K t2);\
__JS_DecCHashMapNode(K, V)\
__JS_DecCHashMap(K, V) \
__JS_DecHashMapPut(K, V) \
__JS_DecHashMapResize(K, V) \
__JS_DecHashMapGet(K, V) \
__JS_DecHashMapContainsKey(K, V) \
__JS_DecHashMapRemove(K, V) \
__JS_DecHashMapClear(K, V) \
__JS_DecHashMapFree(K, V) \
__JS_DecNewHashMap(K, V)

#define Define_JSHashMap(K, V)\
__JS_DefCHashMapNode(K, V)\
__JS_DefHashMapPut(K, V) \
__JS_DefHashMapResize(K, V) \
__JS_DefHashMapGet(K, V) \
__JS_DefHashMapContainsKey(K, V) \
__JS_DefHashMapRemove(K, V) \
__JS_DefHashMapClear(K, V) \
__JS_DefHashMapFree(K, V) \
__JS_DefNewHashMap(K, V)


#define __JS_DecCHashMapNode(K, V) \
typedef struct JSHashMapNode##K##V *pJSHashMapNode##K##V;\
typedef struct JSHashMapNode##K##V {\
	K key;\
    V value;\
	pJSHashMapNode##K##V _nextInBucket;\
}JSHashMapNode##K##V;\
struct JSHashMapNode##K##V;\
pJSHashMapNode##K##V _NewJSHashMapNode##K##V(K k, V v);

#define __JS_DefCHashMapNode(K, V)\
pJSHashMapNode##K##V _NewJSHashMapNode##K##V(K k, V v) {\
	pJSHashMapNode##K##V node = (pJSHashMapNode##K##V)malloc(sizeof(JSHashMapNode##K##V));\
	node->key = k;\
    node->value =  v;\
	node->_nextInBucket = NULL;\
	return node;\
}

#define JSHashMapForEach(K, V, _map_, entry) \
for(int (_i_) = 0; (_i_) < (_map_)->length; (_i_)++) \
for(pJSHashMapNode##K##V entry = (_map_)->buckets[(_i_)]; (entry); (entry) = (entry)->_nextInBucket)

#define __JS_DecCHashMap(K, V)\
typedef unsigned int(*JSHashFunc##K)(K k);\
Define_Class(JSHashMap##K##V)\
	pJSHashMapNode##K##V *buckets;\
	pJSHashMapNode##K##V _cachedItem;\
	size_t length; /*num of buckets*/\
	size_t size; /*num of items*/\
	JSHashFunc##K h;\
	Compare##K compare;\
	void (*freeKey)(K key);\
    void (*freeValue)(V value);\
With_Methods(JSHashMap##K##V)\
	void (*put)(K k, V v);\
    V (*get)(K k);\
	int  (*containsKey)(K k);\
	void (*remove)(K k);\
	void (*clear)();\
	void(*free)();\
End_Class(JSHashMap##K##V)



#define __JS_DecHashMapPut(K, V)\
void __JS_HashMapPut##K##V(K k, V v);

#define __JS_DefHashMapPut(K, V)\
void __JS_HashMapPut##K##V(K k, V v) {\
	/****/getSelf(JSHashMap##K##V)/****/\
	if (((double)self->size) / self->length > JS_CAL_LOAD_FACTOR) {\
		__JS_HashMapResize##K##V(self->length * 2);\
	}\
	int b = self->h(k) % self->length;\
	pJSHashMapNode##K##V p = self->buckets[b];\
	Compare##K cmp = self->compare;\
	while (p) {\
		if (cmp(p->key, k) == 0) {\
            if(self->freeValue) self->freeValue(p->value);\
			p->value = v;\
			return;\
		}\
		p = p->_nextInBucket;\
	}\
	/*add to the head of bucket*/\
	pJSHashMapNode##K##V node = _NewJSHashMapNode##K##V(k, v);\
	node->_nextInBucket = self->buckets[b];\
	self->buckets[b] = node;\
	/*update size*/\
	self->size++;\
}

#define __JS_DecHashMapResize(K, V) \
void __JS_HashMapResize##K##V(int len);

#define __JS_DefHashMapResize(K, V)\
void __JS_HashMapResize##K##V(int len) {\
	/****/getSelf(JSHashMap##K##V)/****/\
	int b;\
	pJSHashMapNode##K##V *newBuckets = (pJSHashMapNode##K##V *)calloc(len, sizeof(pJSHashMapNode##K##V *));\
    pJSHashMapNode##K##V oldNext, p;\
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

#define __JS_DecHashMapGet(K, V) \
V __JS_HashMapGet##K##V(K k);

#define __JS_DefHashMapGet(K, V)\
V __JS_HashMapGet##K##V(K k) {\
	/****/getSelf(JSHashMap##K##V)/****/\
	int b = self->h(k) % self->length;\
	pJSHashMapNode##K##V p = self->buckets[b], q = self->_cachedItem;\
	Compare##K cmp = self->compare;\
	if (q && cmp(q->key, k) == 0)\
		return q->value;\
	while (p) {\
		if (cmp(p->key, k) == 0) {\
			return p->value;\
		}\
		p = p->_nextInBucket;\
	}\
	printf("key does not exist.\n");\
	exit(1);\
}

#define __JS_DecHashMapContainsKey(K, V)\
int __JS_HashMapContainsKey##K##V(K k);

#define __JS_DefHashMapContainsKey(K, V)\
int __JS_HashMapContainsKey##K##V(K k) {\
	/****/getSelf(JSHashMap##K##V)/****/\
	int b = self->h(k) % self->length;\
	pJSHashMapNode##K##V p = self->buckets[b];\
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

#define __JS_DecHashMapRemove(K, V)\
void __JS_HashMapRemove##K##V(K k);

#define __JS_DefHashMapRemove(K, V)\
void __JS_HashMapRemove##K##V(K k) {\
	/****/getSelf(JSHashMap##K##V)/****/\
	int b = self->h(k) % self->length;\
	pJSHashMapNode##K##V p = self->buckets[b], q;\
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
        if (self->freeValue)\
            self->freeValue(p->value);\
		free(p);\
	}\
}

//clean all items, does not reset hash function and equals function
#define __JS_DecHashMapClear(K, V)\
void __JS_HashMapClear##K##V();

#define __JS_DefHashMapClear(K, V)\
void __JS_HashMapClear##K##V() {\
	/****/getSelf(JSHashMap##K##V)/****/\
	pJSHashMapNode##K##V p = NULL, oldNext = NULL;\
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
    this;__JS_HashMapResize##K##V(JS_INI_BUCKETS);\
}


#define __JS_DecHashMapFree(K, V)\
void __JS_HashMapFree##K##V();

#define __JS_DefHashMapFree(K, V)\
void __JS_HashMapFree##K##V() {\
	/****/getSelf(JSHashMap##K##V)/****/\
	m(self)->clear();\
	free(self->buckets);\
	free(self);\
}
#define __JS_DecNewHashMap(K, V) \
pJSHashMap##K##V NewJSHashMap##K##V(JSHashFunc##K hash, Compare##K compare);\
pJSHashMap##K##V NewJSHashMapFull##K##V(JSHashFunc##K hash, Compare##K compare, void (*freeKey)(K key), void (*freeValue)(V value));

#define __JS_DefNewHashMap(K, V)\
Install_Methods(JSHashMap##K##V) \
__JS_HashMapPut##K##V,\
__JS_HashMapGet##K##V,\
__JS_HashMapContainsKey##K##V,\
__JS_HashMapRemove##K##V,\
__JS_HashMapClear##K##V,\
__JS_HashMapFree##K##V \
End_Install()\
pJSHashMap##K##V NewJSHashMap##K##V(JSHashFunc##K hash, Compare##K compare) {\
	return NewJSHashMapFull##K##V(hash, compare, NULL, NULL);\
}\
pJSHashMap##K##V NewJSHashMapFull##K##V(JSHashFunc##K hash, Compare##K compare, void (*freeKey)(K key), void (*freeValue)(V value)) {\
	/****/Alloc_Instance(map, JSHashMap##K##V)/****/\
	map->buckets = (pJSHashMapNode##K##V *)calloc(JS_INI_BUCKETS, sizeof(pJSHashMapNode##K##V));\
	map->_cachedItem = NULL;\
	map->length = JS_INI_BUCKETS;\
	map->size = 0;\
	map->h = hash;\
	map->compare = compare;\
	map->freeKey = freeKey;\
    map->freeValue = freeValue;\
	return map;\
}

#endif