#ifndef CHASHMAP_H
#define CHASHMAP_H
/*
	Version 1.0
	By Sicheng Jiang (UC Berkeley)
	An Linked Hash Map in C.
	2018/04/25
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jscobj2.h"

#define CAL_LOAD_FACTOR 2.0
#define INI_BUCKETS 8

#define HashMap(K, V) pCHashMap##K##_##V
#define NewHashMap(K, V) NewHashMap##K##_##V
#define HashMapEntry(K, V) pCHashMapNode##K##_##V

#define Declare_CHashMap(K, V)\
__DecCHashMapNode(K, V)\
__DecCHashMap(K, V) \
__DecHashMapPut(K, V) \
__DecHashMapResize(K, V) \
__DecHashMapGet(K, V) \
__DecHashMapContainsKey(K, V) \
__DecHashMapRemove(K, V) \
__DecHashMapClear(K, V) \
__DecHashMapClone(K, V) \
__DecHashMapEnumerate(K, V) \
__DecHashMapFree(K, V) \
__DecNewHashMap(K, V)

#define Define_CHashMap(K, V)\
 __DefCHashMapNode(K, V)\
 __DefDefaultEqual(HashMap, K) \
 __DefHashMapPut(K, V) \
__DefHashMapResize(K, V) \
__DefHashMapGet(K, V) \
 __DefHashMapContainsKey(K, V) \
__DefHashMapRemove(K, V) \
__DefHashMapClear(K, V) \
__DefHashMapClone(K, V) \
 __DefHashMapEnumerate(K, V) \
 __DefHashMapFree(K, V) \
__DefNewHashMap(K, V)

//#define K int
//#define V double

typedef enum CHashMapNodeType { CMNT_HEAD, CMNT_DATA } CHashMapNodeType;

#define __DecCHashMapNode(K, V) \
typedef struct CHashMapNode##K##_##V *pCHashMapNode##K##_##V;\
typedef struct CHashMapNode##K##_##V {\
	K key;\
	V value;\
	pCHashMapNode##K##_##V next, prev;\
	pCHashMapNode##K##_##V _nextInBucket;\
	CHashMapNodeType  _type;\
}CHashMapNode##K##_##V;\
struct CHashMapNode##K##_##V;\
pCHashMapNode##K##_##V _NewCHashMapNode##K##_##V(K k, V v);\
pCHashMapNode##K##_##V _NewCMapHead##K##_##V();

#define __DefCHashMapNode(K, V)\
pCHashMapNode##K##_##V _NewCHashMapNode##K##_##V(K k, V v) {\
	pCHashMapNode##K##_##V node = (pCHashMapNode##K##_##V)malloc(sizeof(CHashMapNode##K##_##V));\
	node->key = k;\
	node->value = v;\
	node->next = node->prev = node->_nextInBucket = NULL;\
	node->_type = CMNT_DATA;\
	return node;\
}\
pCHashMapNode##K##_##V _NewCMapHead##K##_##V() {\
	pCHashMapNode##K##_##V node = (pCHashMapNode##K##_##V)malloc(sizeof(CHashMapNode##K##_##V));\
	node->next = node->prev = node;\
	node->_nextInBucket = NULL;\
	node->_type = CMNT_HEAD;\
	return node;\
}

#define __DecCHashMap(K, V)\
typedef unsigned int(*HashFunc##K##_##V)(K k);\
typedef int(*KeyEquals##K##_##V)(const K *k1, const K *k2); \
Define_Class(CHashMap##K##_##V)\
	pCHashMapNode##K##_##V *buckets;\
	pCHashMapNode##K##_##V head;\
	pCHashMapNode##K##_##V _cachedItem;\
	size_t length; /*num of buckets*/\
	size_t size; /*num of items*/\
	HashFunc##K##_##V h;\
	KeyEquals##K##_##V equals;\
	void (*freeKey)(K key);\
	void (*freeValue)(V value);\
With_Methods(CHashMap##K##_##V)\
	void (*put)(K k, V v);\
	V    (*get)(K k);\
	int  (*containsKey)(K k);\
	void    (*remove)(K k);\
	void (*clear)();\
	pCHashMap##K##_##V (*clone)();\
	pCHashMapNode##K##_##V (*enumerate)();\
	void(*free)();\
End_Class(CHashMap##K##_##V)\
__DecDefaultEqual(HashMap, K)



#define __DecHashMapPut(K, V)\
void __hashMapPut##K##_##V(K k, V v);

#define __DefHashMapPut(K, V)\
void __hashMapPut##K##_##V(K k, V v) {\
	/****/getSelf(CHashMap##K##_##V)/****/\
	if (((double)self->size) / self->length > CAL_LOAD_FACTOR) {\
		__hashMapResize##K##_##V(self->length * 2);\
	}\
	int b = self->h(k) % self->length;\
	pCHashMapNode##K##_##V p = self->buckets[b];\
	KeyEquals##K##_##V eq = self->equals;\
	while (p) {\
		if (eq(&(p->key), &k)) {\
			if(self->freeValue) self->freeValue(p->value);\
			p->value = v;\
			if(self->freeKey) self->freeKey(k);\
			return;\
		}\
		p = p->_nextInBucket;\
	}\
	/*add to the head of bucket*/\
	pCHashMapNode##K##_##V node = _NewCHashMapNode##K##_##V(k, v);\
	node->_nextInBucket = self->buckets[b];\
	self->buckets[b] = node;\
	/*add to the tail of the linked list*/\
	pCHashMapNode##K##_##V tail = self->head->prev;\
	tail->next = node;\
	node->prev = tail;\
	self->head->prev = node;\
	node->next = self->head;\
	/*update size*/\
	self->size++;\
}

#define __DecHashMapResize(K, V) \
void __hashMapResize##K##_##V(int len);

#define __DefHashMapResize(K, V)\
void __hashMapResize##K##_##V(int len) {\
	/****/getSelf(CHashMap##K##_##V)/****/\
	int b;\
	pCHashMapNode##K##_##V *newBuckets = (pCHashMapNode##K##_##V *)calloc(len, sizeof(pCHashMapNode##K##_##V *));\
	pCHashMapNode##K##_##V p = self->head->next;\
	while (p != self->head) {\
		b = self->h(p->key) % len;\
		/*add to the head of the new bucket*/\
		p->_nextInBucket = newBuckets[b];\
		newBuckets[b] = p;\
		p = p->next;\
	}\
	free(self->buckets);\
	self->buckets = newBuckets;\
	self->length = len;\
}

#define __DecHashMapGet(K, V) \
V __hashMapGet##K##_##V(K k);

#define __DefHashMapGet(K, V)\
V __hashMapGet##K##_##V(K k) {\
	/****/getSelf(CHashMap##K##_##V)/****/\
	int b = self->h(k) % self->length;\
	pCHashMapNode##K##_##V p = self->buckets[b], q = self->_cachedItem;\
	KeyEquals##K##_##V eq = self->equals;\
	if (q && eq(&(q->key), &k))\
		return q->value;\
	while (p) {\
		if (eq(&(p->key), &k)) {\
			return p->value;\
		}\
		p = p->_nextInBucket;\
	}\
	printf("key does not exist.\n");\
	exit(1);\
}

#define __DecHashMapContainsKey(K, V)\
int __hashMapContainsKey##K##_##V(K k);

#define __DefHashMapContainsKey(K, V)\
int __hashMapContainsKey##K##_##V(K k) {\
	/****/getSelf(CHashMap##K##_##V)/****/\
		int b = self->h(k) % self->length;\
	pCHashMapNode##K##_##V p = self->buckets[b];\
	KeyEquals##K##_##V eq = self->equals;\
	while (p) {\
		if (eq(&(p->key), &k)) {\
			self->_cachedItem = p;\
			return 1;\
		}\
		p = p->_nextInBucket;\
	}\
	return 0;\
}

#define __DecHashMapRemove(K, V)\
void __hashMapRemove##K##_##V(K k);

#define __DefHashMapRemove(K, V)\
void __hashMapRemove##K##_##V(K k) {\
	/****/getSelf(CHashMap##K##_##V)/****/\
	int b = self->h(k) % self->length;\
	pCHashMapNode##K##_##V p = self->buckets[b], q;\
	pCHashMapNode##K##_##V prev, next;\
	KeyEquals##K##_##V eq = self->equals;\
	if (p && eq(&(p->key), &k)) {\
		self->buckets[b] = p->_nextInBucket;\
	}\
	else {\
		while(p) {\
			q = p;\
			p = p->_nextInBucket;\
			if (p && eq(&(p->key), &k)) {\
				q->_nextInBucket = p->_nextInBucket;\
				break;\
			}\
		}\
	}\
	if (p) {\
		next = p->next;\
		prev = p->prev;\
		prev->next = next;\
		next->prev = prev;\
		if (self->freeKey)\
			self->freeKey(p->key);\
		if (self->freeValue)\
			self->freeValue(p->value);\
		free(p);\
	}\
}

//clean all items, does not reset hash function and equals function
#define __DecHashMapClear(K, V)\
void __hashMapClear##K##_##V();

#define __DefHashMapClear(K, V)\
void __hashMapClear##K##_##V() {\
	/****/getSelf(CHashMap##K##_##V)/****/\
	pCHashMapNode##K##_##V head = self->head, p = head->next, q = NULL;\
	void(*freeKey)(K key) = self->freeKey;\
	void(*freeValue)(V value) = self->freeValue;\
	while (p != head) {\
		q = p;\
		p = p->next;\
		if (freeKey)\
			freeKey(q->key);\
		if (freeValue)\
			freeValue(q->value);\
		free(q);\
	}\
	head->next = head->prev = head;\
	self->size = 0;\
	self->_cachedItem = NULL;\
	__hashMapResize##K##_##V(INI_BUCKETS);\
}

//returns a shallow copy of the hash map
#define __DecHashMapClone(K, V) \
pCHashMap##K##_##V __hashMapClone##K##_##V();

#define __DefHashMapClone(K, V)\
pCHashMap##K##_##V __hashMapClone##K##_##V() {\
	/****/getSelf(CHashMap##K##_##V)/****/\
	pCHashMap##K##_##V map = NewHashMap##K##_##V(self->h);\
	pCHashMapNode##K##_##V head = self->head, p = head->next;\
	while (p != head) {\
		m(map)->put(p->key, p->value);\
		p = p->next;\
	}\
	return map;\
}

/* return the head of the circular double linked list

*****  note that every thing is READ ONLY !!!  *****

**** do not modify anything (except the values the the key-value pair)
otherwise the hash map can't work appropriately ****

	the first item is at head->next
	the last item is at head->prev
	when there is 0 item, head->next = head->prev = head;
*/
#define __DecHashMapEnumerate(K, V) \
pCHashMapNode##K##_##V __hashMapEnumerate##K##_##V();
#define __DefHashMapEnumerate(K, V)\
pCHashMapNode##K##_##V __hashMapEnumerate##K##_##V() {\
	/****/getSelf(CHashMap##K##_##V)/****/\
		return self->head;\
}

#define __DecHashMapFree(K, V)\
void __hashMapFree##K##_##V();

#define __DefHashMapFree(K, V)\
void __hashMapFree##K##_##V() {\
	/****/getSelf(CHashMap##K##_##V)/****/\
	m(self)->clear();\
	free(self->buckets);\
	free(self->head);\
	free(self);\
}
#define __DecNewHashMap(K, V) \
pCHashMap##K##_##V NewHashMap##K##_##V(HashFunc##K##_##V hash);

#define __DefNewHashMap(K, V)\
Install_Methods(CHashMap##K##_##V) \
__hashMapPut##K##_##V,\
__hashMapGet##K##_##V,\
__hashMapContainsKey##K##_##V,\
__hashMapRemove##K##_##V,\
__hashMapClear##K##_##V,\
__hashMapClone##K##_##V,\
__hashMapEnumerate##K##_##V,\
__hashMapFree##K##_##V \
End_Install()\
pCHashMap##K##_##V NewHashMap##K##_##V(HashFunc##K##_##V hash) {\
	/****/Alloc_Instance(map, CHashMap##K##_##V)/****/\
	map->buckets = (pCHashMapNode##K##_##V *)calloc(INI_BUCKETS, sizeof(pCHashMapNode##K##_##V));\
	map->head = _NewCMapHead##K##_##V();\
	map->_cachedItem = NULL;\
	map->length = INI_BUCKETS;\
	map->size = 0;\
	map->h = hash;\
	map->equals = DFT_EQUAL(HashMap, K);\
	map->freeKey = map->freeValue = NULL;\
	return map;\
}




#endif
