#ifndef CARRAYLIST_H
#define CARRAYLIST_H
/*
	Version 1.0
	By Sicheng Jiang
	An ArrayList in C.
	2018/04/19
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jscobj2.h"

#define INI_CAP 8

//Define a pointer to an ArrayList<T> instance
#define ArrayList(T) p##T##Array

//constructor
#define NewArrayList(T) New##T##Arr


#define Declare_CArrayList(T) \
__DecArr(T) \
__DecPushBack(T) \
__DecPushAll(T) \
__DecExpend(T)\
__DecInsert(T) \
__DecPopBack(T)\
__DecPop(T) \
__DecIndexOf(T)\
__DecSubList(T)\
__DecSort(T) \
__DecMap(T) \
__DecReduce(T)\
__DecFree(T)\
__DecNewArrFuc(T)


#define Define_CArrayList(T) \
__DefDefaultEqual(ArrayList, T) \
__DefPushBack(T) \
__DefPushAll(T) \
__DefExpend(T)\
__DefInsert(T) \
__DefPopBack(T)\
__DefPop(T) \
__DefIndexOf(T)\
__DefSubList(T)\
__DefSort(T) \
__DefMap(T) \
__DefReduce(T)\
__DefFree(T)\
__DefNewArrFuc(T)

/* TODO
unique
*/




//-------------------------------------------- 
//define the ArrayList class
#define __DecArr(T) \
typedef int (*Compare##T)(const T *t1, const T *t2);\
typedef int (*Equal##T)(const T *t1, const T *t2);\
typedef void (*F_Map##T)(T *t);\
typedef void (*F_Reduce##T)(T *t1, T *t2);\
Define_Class(T##Array) \
	T *arr; \
	int size; \
	int _capacity; \
	Compare##T compare; \
	Equal##T equals;\
	void (*freeItem)(T item);\
With_Methods(T##Array) \
	void (*pushBack)(T item);\
	void (*pushAll)(T *items, int n);\
	void (*expend)(p##T##Array otherList);\
	void (*insert)(T item, int index);\
	T (*popBack)();\
	T (*popAt)(int index);\
	int (*indexOf)(T item); \
	p##T##Array (*subList)(int start, int end); \
	void (*sort)();\
	void (*map)(F_Map##T map_func);\
	void (*reduce)(F_Reduce##T reduce_func, T *acc);\
	void (*free)();\
End_Class(T##Array) \
__DecDefaultEqual(ArrayList, T)



//define the constructor
#define __DecNewArrFuc(T) \
p##T##Array New##T##Arr();

#define __DefNewArrFucAux(T) \
p##T##Array New##T##Arr()\
{\
	/****/Alloc_Instance(list, T##Array)/****/\
	list->__vftb__ = &__vftb__##T##Array; \
	list->arr = (T *)malloc(sizeof(T) * INI_CAP + 1);\
	if(list->arr == NULL) return NULL;\
	memset(list->arr, 0, sizeof(T));\
	list->size = 0;\
	list->_capacity = INI_CAP; \
	list->compare = (Compare##T) DFT_CMP;\
	list->equals = DFT_EQUAL(ArrayList, T);\
	list->freeItem = NULL;\
	return list;\
}


#define __DefNewArrFuc(T) \
Install_Methods(T##Array) \
	__pushBack##T, \
	__pushAll##T, \
	__expend##T, \
	__insert##T, \
	__popBack##T, \
	__pop##T##At, \
	__indexOf##T, \
	__subList##T, \
	__mergeSort##T, \
	__map_##T, \
	__reduce_##T, \
	__free_##T \
End_Install() \
__DefNewArrFucAux(T)

//append item at the end 
#define __DecPushBack(T)\
void __pushBack##T(T thing);

#define __DefPushBack(T)\
void __pushBack##T(T thing)\
{\
	/****/getSelf(T##Array)/****/\
	m(self)->insert(thing, self->size);\
}

#define __DecPushAll(T) \
void __pushAll##T(T *things, int n);

#define __DefPushAll(T) \
void __pushAll##T(T *things, int n)\
{\
	/****/getSelf(T##Array)/****/\
	int i=0;\
	for(i=0;i<n;i++)\
		this->pushBack(things[i]);\
}

//append all items in another arraylist
#define __DecExpend(T)\
void __expend##T(T##Array *things);
#define __DefExpend(T)\
void __expend##T(T##Array *things)\
{\
	/****/getSelf(T##Array)/****/\
	int i=0;\
	for(i=0;i<things->size;i++)\
		this->pushBack(things->arr[i]);\
}

#define __DecInsert(T)\
void __insert##T(T thing, int index);
#define __DefInsert(T)\
void __insert##T(T thing, int index) {\
	/****/getSelf(T##Array)/****/\
	int i;\
	if(index > self->size) {\
		printf("index out of boundary!"); \
	}\
	if(self->size >= self->_capacity) {\
		self->_capacity *= 2;\
		self->arr = (T *)realloc(self->arr, sizeof(T) * self->_capacity + 1);\
	}\
	for(i = self->size; i > index; i--)\
		self->arr[i] = self->arr[i - 1];\
	self->arr[index] = thing;\
	self->size++;\
	memset(self->arr + self->size, 0, sizeof(T));\
}

#define __DecPopBack(T) \
T __popBack##T();
#define __DefPopBack(T) \
T __popBack##T() {\
	/****/getSelf(T##Array)/****/\
	return this->popAt(self->size - 1);\
}

//pop the item at index
#define __DecPop(T)\
T __pop##T##At(int index);
#define __DefPop(T)\
T __pop##T##At(int index)\
{\
	/****/getSelf(T##Array)/****/\
	int i;\
	if(self->size <= index) {\
		printf("index out of boundary!");\
		exit(101);\
	}\
	T p = self->arr[index];\
	for(i = index+1;i<self->size;i++)\
		self->arr[i-1] = self->arr[i];\
	self->size--;\
	memset(self->arr + self->size, 0, sizeof(T));\
	return p;\
}

#define __DecIndexOf(T) \
int __indexOf##T(T item);
#define __DefIndexOf(T) \
int __indexOf##T(T item) {\
	/****/getSelf(T##Array)/****/\
	int i; \
	for (i = 0; i < self->size; i++) \
		if (self->compare(&(self->arr[i]), &item) == 0) \
			return i;\
	return -1;\
}

//find sub-list [start, end), non-destructive
//Note: each item in the sublist is a shallow copy of the original one.
#define __DecSubList(T) \
p##T##Array __subList##T(int start, int end);
#define __DefSubList(T) \
p##T##Array __subList##T(int start, int end) {\
	/****/getSelf(T##Array)/****/\
	p##T##Array sub = New##T##Arr(); \
	int i;\
	if(end == -1) end = self->size;\
	for (i = start; i < end && i < self->size; i++) {\
		m(sub)->pushBack(self->arr[i]);\
	}\
	return sub;\
}

//merge sort arraylist
#define __DecSort(T) \
void __mergeHelper##T(const int l, const int mid, const int r, T *L, T *R);\
void __merge_sort##T(int l, int r, T *L, T *R);\
void __mergeSort##T();
#define __DefSort(T) \
void __mergeHelper##T(const int l, const int mid, const int r, T *L, T *R)\
{\
	/****/getSelf(T##Array)/****/\
	printf("merge [%d %d %d]\n", l, mid, r);\
	T *arr = self->arr;\
	int i, j, k;\
	int n1 = mid - l + 1;\
	int n2 = r - mid;\
	/* Copy data to temp arrays L[] and R[] */\
	for (i = 0; i < n1; i++)\
		L[i] = arr[l + i];\
	for (j = 0; j < n2; j++)\
		R[j] = arr[mid + 1 + j];\
	/* Merge the temp arrays back into arr[l..r]*/\
	i = 0; j = 0; k = l; \
	while (i < n1 && j < n2)\
	{\
		if (self->compare(L + i, R + j) <= 0)\
			arr[k++] = L[i++];\
		else\
			arr[k++] = R[j++];\
	}\
	/* Copy the remaining elements of L[], if there are any */\
	while (i < n1)\
		arr[k++] = L[i++];\
	/* Copy the remaining elements of R[], if there are any */\
	while (j < n2)\
		arr[k++] = R[j++];\
}\
void __merge_sort##T(int l, int r, T *L, T *R)\
{\
	/****/getSelf(T##Array)/****/\
	printf("[%d %d]\n", l, r);\
	if (l < r)\
	{\
		/* Same as (l+r)/2, but avoids overflow for large l and h */\
		int mid = l + (r - l) / 2;\
		this; __merge_sort##T(l, mid, L, R);\
		this; __merge_sort##T(mid + 1, r, L, R); \
		m(self); __mergeHelper##T(l, mid, r, L, R);\
	}\
}\
void __mergeSort##T() {\
	/****/getSelf(T##Array)/****/\
	int n = (int)(self->size / 2) + 1;\
	T *L = (T *)malloc(sizeof(T) * n);\
	T *R = (T *)malloc(sizeof(T) * n);\
	this; __merge_sort##T(0, self->size - 1, L, R);\
	free(L); free(R); \
}

#define __DecMap(T) \
void __map_##T(F_Map##T map_func);
#define __DefMap(T) \
void __map_##T(F_Map##T map_func) {\
	/****/getSelf(T##Array)/****/\
	int i;\
	for (i = 0; i < self->size; i++) \
		map_func(self->arr + i);\
}

#define __DecReduce(T) \
void __reduce_##T(F_Reduce##T reduce_func, T *acc);
#define __DefReduce(T) \
void __reduce_##T(F_Reduce##T reduce_func, T *acc) {\
	/****/getSelf(T##Array)/****/\
	int i; \
	for(i = 0; i < self->size; i++) \
		reduce_func(acc, self->arr + i);\
}


//destructor
#define __DecFree(T) \
void __free_##T();
#define __DefFree(T) \
void __free_##T() { \
	/****/getSelf(T##Array)/****/\
	int i;\
	if (self->freeItem) {\
		for(i = 0; i < self->size; i++)\
			self->freeItem(self->arr[i]);\
	}\
	free(self->arr); \
	free(self); \
}



#endif
