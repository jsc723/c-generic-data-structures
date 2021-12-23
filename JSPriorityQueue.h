#ifndef JS_PRIORITY_QUEUE_H
#define JS_PRIORITY_QUEUE_H

#include "jscobj2.h"
#include "JSArrayList.h"

#define USE_JSCOBJ

#define JSPriorityQueue(E) pJSPriorityQueue##E
#define NewJSPriorityQueue(E, cmp) NewJSCPQ_##E(cmp)

#define Declare_JSPriorityQueue(E) \
__JS_Dec_CPQ(E)\
__JS_DecSwapUpDown(E)\
__JS_Dec_CPQ_Helper(E)\
__JS_DecCPQ_add(E) \
__JS_DecCPQ_clear(E) \
__JS_DecCPQ_contains(E)\
__JS_DecCPQ_peek(E) \
__JS_DecCPQ_poll(E) \
__JS_DecCPQ_remove(E) \
__JS_DecCPQ_size(E) \
__JS_DecCPQ_free(E) \
__JS_DecNewCPQ(E)

#define Define_JSPriorityQueue(E)\
Define_JSArrayList(E) \
__JS_DefUp(E)\
__JS_DefDown(E)\
__JS_Def_CPQ_Helper(E)\
__JS_DefCPQ_add(E) \
__JS_DefCPQ_clear(E) \
__JS_DefCPQ_contains(E)\
__JS_DefCPQ_peek(E) \
__JS_DefCPQ_poll(E) \
__JS_DefCPQ_remove(E) \
__JS_DefCPQ_size(E) \
__JS_DefCPQ_free(E) \
__JS_DefNewCPQ(E)

//------------------------------------------

#define __JS_Dec_CPQ(E) \
JS__DefCompare(E)\
Declare_JSArrayList(E)\
Define_Class(JSPriorityQueue##E)\
	JSArrayList(E) heap;\
	JSCompare(E) compare;\
With_Methods(JSPriorityQueue##E)\
	void (*add)(E e);\
	void (*clear)();\
	int (*contains)(E e);\
	E (*peek)();\
	E (*poll)();\
	int (*remove)(E e);\
	size_t (*size)();\
	void(*free)();\
End_Class(JSPriorityQueue##E)

#define __JS_Dec_CPQ_Helper(E)\
int __JS_CPQ__parent##E(int i);\
int __JS_CPQ__leftChild##E(int i);\
int __JS_CPQ__rightChild##E(int i);

#define __JS_Def_CPQ_Helper(E)\
int __JS_CPQ__parent##E(int i) { return (i - 1) / 2; }\
int __JS_CPQ__leftChild##E(int i) { return i * 2 + 1; }\
int __JS_CPQ__rightChild##E(int i) { return i * 2 + 2; }

#define __JS_CPQ__swap(E, _arr_, _i_, _j_) \
do {   \
	E e = (_arr_)[(_j_)]; \
	(_arr_)[(_j_)] = (_arr_)[(_i_)]; \
	(_arr_)[(_i_)] = e; \
} while(0)

#define __JS_DecSwapUpDown(E) \
void __JS_CPQ__up##E(int i);\
void __JS_CPQ__down##E(int i);

#define __JS_DefUp(E) \
void __JS_CPQ__up##E(int i) {\
	getSelf(JSPriorityQueue##E);\
	E *arr = self->heap->arr;\
	int p = __JS_CPQ__parent##E(i);\
	if (i == 0) return;\
	if (self->compare(arr[p], arr[i]) < 0) {\
		__JS_CPQ__swap(E, arr, p, i);\
		m(self); __JS_CPQ__up##E(p);\
	}\
}

#define __JS_DefDown(E) \
void __JS_CPQ__down##E(int i) {\
	getSelf(JSPriorityQueue##E);\
	JSCompare(E) cmp = self->compare;\
	E *arr = self->heap->arr;\
	int left = __JS_CPQ__leftChild##E(i);\
	int right = __JS_CPQ__rightChild##E(i);\
	if (left >= self->heap->size) return;\
	if (right >= self->heap->size) {\
		if (cmp(arr[left], arr[i]) > 0) {\
			__JS_CPQ__swap(E, arr, left, i);\
		}\
	}\
	else {\
		int max = cmp(arr[left], arr[right]) >= 0 ? left : right;\
		if (cmp(arr[max], arr[i]) > 0) {\
			__JS_CPQ__swap(E, arr, max, i);\
			m(self); __JS_CPQ__down##E(max);\
		}\
	}\
}
#define __JS_DecCPQ_add(E) \
void __JS_CPQ_add##E(E e);
#define __JS_DefCPQ_add(E) \
void __JS_CPQ_add##E(E e) {\
	getSelf(JSPriorityQueue##E);\
	m(self->heap)->pushBack(e);\
	m(self); __JS_CPQ__up##E(self->heap->size - 1);\
}

#define __JS_DecCPQ_clear(E)\
void __JS_CPQ_clear##E();
#define __JS_DefCPQ_clear(E)\
void __JS_CPQ_clear##E() {\
	getSelf(JSPriorityQueue##E);\
	m(self->heap)->free();\
	self->heap = NewJSArrayList(E);\
}
#define __JS_DecCPQ_contains(E)\
int __JS_CPQ_contains##E(E e);
#define __JS_DefCPQ_contains(E)\
int __JS_CPQ_contains##E(E e) {\
	getSelf(JSPriorityQueue##E);\
	return m(self->heap)->indexOf(e, self->compare) >= 0;\
}

#define __JS_DecCPQ_peek(E)\
E __JS_CPQ_peek##E();
#define __JS_DefCPQ_peek(E) \
E __JS_CPQ_peek##E() {\
		getSelf(JSPriorityQueue##E); \
		return self->heap->arr[0];\
}

#define __JS_DecCPQ_poll(E)\
E __JS_CPQ_poll##E();
#define __JS_DefCPQ_poll(E)\
E __JS_CPQ_poll##E() {\
	getSelf(JSPriorityQueue##E);\
	E result = m(self)->peek();\
	__JS_CPQ__swap(E, self->heap->arr, 0, self->heap->size - 1);\
	m(self->heap)->popBack();\
	m(self); __JS_CPQ__down##E(0);\
	return result;\
}

#define __JS_DecCPQ_remove(E)\
int __JS_CPQ_remove##E(E e);
#define __JS_DefCPQ_remove(E)\
int __JS_CPQ_remove##E(E e) {\
	getSelf(JSPriorityQueue##E);\
	int i, removed = 0;\
	if ((i = m(self->heap)->indexOf(e, self->compare)) >= 0) {\
		__JS_CPQ__swap(E, self->heap->arr, i, self->heap->size - 1);\
		m(self->heap)->popBack();\
		m(self); __JS_CPQ__down##E(i);\
		removed++;\
	}\
	return removed;\
}
#define __JS_DecCPQ_size(E)\
size_t __JS_CPQ_size##E();
#define __JS_DefCPQ_size(E)\
size_t __JS_CPQ_size##E() {\
	getSelf(JSPriorityQueue##E);\
	return self->heap->size;\
}

#define __JS_DecCPQ_free(E)\
void __JS_CPQ_free##E();
#define __JS_DefCPQ_free(E)\
void __JS_CPQ_free##E() {\
	getSelf(JSPriorityQueue##E);\
	m(self->heap)->free();\
	free(self);\
}

#define __JS_DecNewCPQ(E) \
pJSPriorityQueue##E NewJSCPQ_##E(JSCompare(E) cmp);
#define __JS_DefNewCPQ(E) \
Install_Methods(JSPriorityQueue##E)\
__JS_CPQ_add##E,\
__JS_CPQ_clear##E,\
__JS_CPQ_contains##E,\
__JS_CPQ_peek##E,\
__JS_CPQ_poll##E,\
__JS_CPQ_remove##E,\
__JS_CPQ_size##E, \
__JS_CPQ_free##E \
End_Install()\
pJSPriorityQueue##E NewJSCPQ_##E(JSCompare(E) cmp) {\
	Alloc_Instance(pq, JSPriorityQueue##E);\
	pq->heap = NewJSArrayList(E);\
	pq->compare = cmp;\
	return pq;\
}

#endif
