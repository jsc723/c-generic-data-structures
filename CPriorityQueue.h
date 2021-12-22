#ifndef C_PRIORITY_QUEUE_H
#define C_PRIORITY_QUEUE_H

#include "jscobj2.h"
#include "CArrayList.h"

#define USE_JSCOBJ

#define PriorityQueue(E) pCPriorityQueue##E
#define NewPriorityQueue(E) NewCPQ_##E

#define Declare_CPriorityQueue(E) \
__Dec_CPQ(E)\
__DecSwapUpDown(E)\
__Dec_CPQ_Helper(E)\
__DecCPQ_add(E) \
__DecCPQ_clear(E) \
__DecCPQ_contains(E)\
__DecCPQ_peek(E) \
__DecCPQ_poll(E) \
__DecCPQ_remove(E) \
__DecCPQ_size(E) \
__DecCPQ_free(E) \
__DecNewCPQ(E)

#define Define_CPriorityQueue(E)\
Define_CArrayList(E) \
__DefSwap(E)\
__DefUp(E)\
__DefDown(E)\
__Def_CPQ_Helper(E)\
__DefCPQ_add(E) \
__DefCPQ_clear(E) \
__DefCPQ_contains(E)\
__DefCPQ_peek(E) \
__DefCPQ_poll(E) \
__DefCPQ_remove(E) \
__DefCPQ_size(E) \
__DefCPQ_free(E) \
__DefNewCPQ(E)

//------------------------------------------

#define __Dec_CPQ(E) \
Declare_CArrayList(E)\
_Define_Optional(E)\
Define_Class(CPriorityQueue##E)\
	ArrayList(E) heap;\
With_Methods(CPriorityQueue##E)\
	void (*add)(E e);\
	void (*clear)();\
	int (*contains)(E e);\
	Optional(E) (*peek)();\
	Optional(E) (*poll)();\
	int (*remove)(E e);\
	size_t (*size)();\
	void(*free)();\
End_Class(CPriorityQueue##E)

#define __Dec_CPQ_Helper(E)\
int __CPQ__parent##E(int i);\
int __CPQ__leftChild##E(int i);\
int __CPQ__rightChild##E(int i);

#define __Def_CPQ_Helper(E)\
int __CPQ__parent##E(int i) { return (i - 1) / 2; }\
int __CPQ__leftChild##E(int i) { return i * 2 + 1; }\
int __CPQ__rightChild##E(int i) { return i * 2 + 2; }\

#define __DecSwapUpDown(E) \
void __CPQ__swap##E(E * arr, int i, int j);\
void __CPQ__up##E(int i);\
void __CPQ__down##E(int i);

#define __DefSwap(E) \
void __CPQ__swap##E(E * arr, int i, int j) {\
	E e = arr[j];\
	arr[j] = arr[i];\
	arr[i] = e;\
}
#define __DefUp(E) \
void __CPQ__up##E(int i) {\
	getSelf(CPriorityQueue##E);\
	Compare##E cmp = self->heap->compare;\
	E *arr = self->heap->arr;\
	int p = __CPQ__parent##E(i);\
	if (i == 0) return;\
	if (cmp(&(arr[p]), &(arr[i])) < 0) {\
		__CPQ__swap##E(arr, p, i);\
		m(self); __CPQ__up##E(p);\
	}\
}

#define __DefDown(E) \
void __CPQ__down##E(int i) {\
	getSelf(CPriorityQueue##E);\
	Compare##E cmp = self->heap->compare;\
	E *arr = self->heap->arr;\
	int left = __CPQ__leftChild##E(i);\
	int right = __CPQ__rightChild##E(i);\
	if (left >= self->heap->size) return;\
	if (right >= self->heap->size) {\
		if (cmp(&(arr[left]), &(arr[i])) > 0) {\
			__CPQ__swap##E(arr, left, i);\
		}\
	}\
	else {\
		int max = cmp(arr + left, arr + right) >= 0 ? left : right;\
		if (cmp(arr + max, arr + i) > 0) {\
			__CPQ__swap##E(arr, max, i);\
			m(self); __CPQ__down##E(max);\
		}\
	}\
}
#define __DecCPQ_add(E) \
void __CPQ_add##E(E e);
#define __DefCPQ_add(E) \
void __CPQ_add##E(E e) {\
	getSelf(CPriorityQueue##E);\
	m(self->heap)->pushBack(e);\
	m(self); __CPQ__up##E(self->heap->size - 1);\
}

#define __DecCPQ_clear(E)\
void __CPQ_clear##E();
#define __DefCPQ_clear(E)\
void __CPQ_clear##E() {\
	getSelf(CPriorityQueue##E);\
	m(self->heap)->free();\
	Compare##E cmp = self->heap->compare;\
	self->heap = NewArrayList(E)();\
	self->heap->compare = cmp;\
}
#define __DecCPQ_contains(E)\
int __CPQ_contains##E(E e);
#define __DefCPQ_contains(E)\
int __CPQ_contains##E(E e) {\
	getSelf(CPriorityQueue##E);\
	return m(self->heap)->indexOf(e) >= 0;\
}
#define __DecCPQ_peek(E)\
Optional(E) __CPQ_peek##E();
#define __DefCPQ_peek(E) \
Optional(E) __CPQ_peek##E() {\
		getSelf(CPriorityQueue##E); \
		Optional(E) result; \
		if (self->heap->size == 0) { \
			\
				result.valid = 0; \
				return result; \
		}\
		else {\
					result.valid = 1; \
					result.value = self->heap->arr[0]; \
					return result; \
			}\
}
#define __DecCPQ_poll(E)\
Optional(E) __CPQ_poll##E();
#define __DefCPQ_poll(E)\
Optional(E) __CPQ_poll##E() {\
	getSelf(CPriorityQueue##E);\
	Optional(E) result = m(self)->peek();\
	if (result.valid) {\
		__CPQ__swap##E(self->heap->arr, 0, self->heap->size - 1);\
		m(self->heap)->popBack();\
		m(self); __CPQ__down##E(0);\
	}\
	return result;\
}

#define __DecCPQ_remove(E)\
int __CPQ_remove##E(E e);
#define __DefCPQ_remove(E)\
int __CPQ_remove##E(E e) {\
	getSelf(CPriorityQueue##E);\
	int i, removed = 0;\
	if ((i = m(self->heap)->indexOf(e)) >= 0) {\
		__CPQ__swap##E(self->heap->arr, i, self->heap->size - 1);\
		m(self->heap)->popBack();\
		m(self); __CPQ__down##E(i);\
		removed++;\
	}\
	return removed;\
}
#define __DecCPQ_size(E)\
size_t __CPQ_size##E();
#define __DefCPQ_size(E)\
size_t __CPQ_size##E() {\
	getSelf(CPriorityQueue##E);\
	return self->heap->size;\
}

#define __DecCPQ_free(E)\
void __CPQ_free##E();
#define __DefCPQ_free(E)\
void __CPQ_free##E() {\
	getSelf(CPriorityQueue##E);\
	m(self->heap)->free();\
	free(self);\
}

#define __DecNewCPQ(E) \
pCPriorityQueue##E NewCPQ_##E(Compare##E cmp);
#define __DefNewCPQ(E) \
Install_Methods(CPriorityQueue##E)\
__CPQ_add##E,\
__CPQ_clear##E,\
__CPQ_contains##E,\
__CPQ_peek##E,\
__CPQ_poll##E,\
__CPQ_remove##E,\
__CPQ_size##E, \
__CPQ_free##E \
End_Install()\
pCPriorityQueue##E NewCPQ_##E(Compare##E cmp) {\
	Alloc_Instance(pq, CPriorityQueue##E);\
	pq->heap = NewArrayList(E)();\
	pq->heap->compare = cmp;\
	return pq;\
}

#endif
