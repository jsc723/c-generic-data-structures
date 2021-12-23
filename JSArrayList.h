#ifndef JS_ARRAYLIST_H
#define JS_ARRAYLIST_H
/*
	Version 1.0
	By Sicheng Jiang
	An ArrayList in C.
	2018/04/19
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "sglib.h"
#include "jscobj2.h"

#define JS_ARR_INI_CAP 8

//Define a pointer to an ArrayList<T> instance
#define JSArrayList(T) pJS##T##Array

//constructor
#define NewJSArrayList(T) NewJS##T##Arr()


#define Declare_JSArrayList(T) \
__JS_DecArr(T) \
__JS_DecArrPushBack(T) \
__JS_DecArrPushAll(T) \
__JS_DecArrExpend(T)\
__JS_DecArrInsert(T) \
__JS_DecArrPopBack(T)\
__JS_DecArrPop(T) \
__JS_DecArrIndexOf(T) \
__JS_DecArrSubList(T)\
__JS_DecArrSort(T) \
__JS_DecArrStableSort(T) \
__JS_DecArrMap(T) \
__JS_DecArrReduce(T)\
__JS_DecArrFree(T)\
__JS_DecArrNewArrFuc(T)


#define Define_JSArrayList(T) \
__JS_DefArrPushBack(T) \
__JS_DefArrPushAll(T) \
__JS_DefArrExpend(T)\
__JS_DefArrInsert(T) \
__JS_DefArrPopBack(T)\
__JS_DefArrPop(T) \
__JS_DefArrIndexOf(T) \
__JS_DefArrSubList(T)\
__JS_DefArrSort(T) \
__JS_DefArrStableSort(T) \
__JS_DefArrMap(T) \
__JS_DefArrReduce(T)\
__JS_DefArrFree(T)\
__JS_DefArrNewArrFuc(T)

/* TODO
unique
*/




//-------------------------------------------- 
//define the ArrayList class
#define __JS_DecArr(T) \
__DefCompare(T)\
typedef void (*F_Map##T)(T *t);\
typedef void (*F_Reduce##T)(T *t1, T *t2);\
Define_Class(JS##T##Array) \
	T *arr; \
	int size; \
	int _capacity; \
	void (*freeItem)(T item);\
With_Methods(JS##T##Array) \
	void (*pushBack)(T item);\
	void (*pushAll)(T *items, int n);\
	void (*expend)(pJS##T##Array otherList);\
	void (*insert)(T item, int index);\
	T (*popBack)();\
	T (*popAt)(int index);\
	int (*indexOf)(T item, Compare##T compare); \
	pJS##T##Array (*subList)(int start, int end); \
	void (*sort)(Compare##T compare);\
	void (*stableSort)(Compare##T compare);\
	void (*map)(F_Map##T map_func);\
	void (*reduce)(F_Reduce##T reduce_func, T *acc);\
	void (*free)();\
End_Class(JS##T##Array)



//define the constructor
#define __JS_DecArrNewArrFuc(T) \
pJS##T##Array NewJS##T##Arr();

#define __JS_DefArrNewArrFucAux(T) \
pJS##T##Array NewJS##T##Arr()\
{\
	/****/Alloc_Instance(list, JS##T##Array)/****/\
	list->__vftb__ = &__vftb__JS##T##Array; \
	list->arr = (T *)malloc(sizeof(T) * JS_ARR_INI_CAP + 1);\
	if(list->arr == NULL) return NULL;\
	memset(list->arr, 0, sizeof(T));\
	list->size = 0;\
	list->_capacity = JS_ARR_INI_CAP; \
	list->freeItem = NULL;\
	return list;\
}


#define __JS_DefArrNewArrFuc(T) \
Install_Methods(JS##T##Array) \
	__js_arr_pushBack##T, \
	__js_arr_pushAll##T, \
	__js_arr_expend##T, \
	__js_arr_insert##T, \
	__js_arr_popBack##T, \
	__js_arr_pop##T##At, \
	__js_arr_indexOf##T, \
	__js_arr_subList##T, \
	__js_arr_quickSort##T, \
	__js_arr_mergeSort##T, \
	__js_arr_map_##T, \
	__js_arr_reduce_##T, \
	__js_arr_free_##T \
End_Install() \
__JS_DefArrNewArrFucAux(T)

//append item at the end 
#define __JS_DecArrPushBack(T)\
void __js_arr_pushBack##T(T thing);

#define __JS_DefArrPushBack(T)\
void __js_arr_pushBack##T(T thing)\
{\
	/****/getSelf(JS##T##Array)/****/\
	m(self)->insert(thing, self->size);\
}

#define __JS_DecArrPushAll(T) \
void __js_arr_pushAll##T(T *things, int n);

#define __JS_DefArrPushAll(T) \
void __js_arr_pushAll##T(T *things, int n)\
{\
	/****/getSelf(JS##T##Array)/****/\
	int i=0;\
	for(i=0;i<n;i++)\
		this->pushBack(things[i]);\
}

//append all items in another arraylist
#define __JS_DecArrExpend(T)\
void __js_arr_expend##T(JS##T##Array *things);
#define __JS_DefArrExpend(T)\
void __js_arr_expend##T(JS##T##Array *things)\
{\
	/****/getSelf(JS##T##Array)/****/\
	int i=0;\
	for(i=0;i<things->size;i++)\
		this->pushBack(things->arr[i]);\
}

#define __JS_DecArrInsert(T)\
void __js_arr_insert##T(T thing, int index);
#define __JS_DefArrInsert(T)\
void __js_arr_insert##T(T thing, int index) {\
	/****/getSelf(JS##T##Array)/****/\
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

#define __JS_DecArrPopBack(T) \
T __js_arr_popBack##T();
#define __JS_DefArrPopBack(T) \
T __js_arr_popBack##T() {\
	/****/getSelf(JS##T##Array)/****/\
	return this->popAt(self->size - 1);\
}

//pop the item at index
#define __JS_DecArrPop(T)\
T __js_arr_pop##T##At(int index);
#define __JS_DefArrPop(T)\
T __js_arr_pop##T##At(int index)\
{\
	/****/getSelf(JS##T##Array)/****/\
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

#define __JS_DecArrIndexOf(T) \
int __js_arr_indexOf##T(T item, Compare##T compare);
#define __JS_DefArrIndexOf(T) \
int __js_arr_indexOf##T(T item, Compare##T compare) {\
	/****/getSelf(JS##T##Array)/****/\
	int i; \
	for (i = 0; i < self->size; i++) \
		if (compare(self->arr[i], item) == 0) \
			return i;\
	return -1;\
}

//find sub-list [start, end), non-destructive
//Note: each item in the sublist is a shallow copy of the original one.
#define __JS_DecArrSubList(T) \
pJS##T##Array __js_arr_subList##T(int start, int end);
#define __JS_DefArrSubList(T) \
pJS##T##Array __js_arr_subList##T(int start, int end) {\
	/****/getSelf(JS##T##Array)/****/\
	pJS##T##Array sub = NewJS##T##Arr(); \
	int i;\
	if(end == -1) end = self->size;\
	for (i = start; i < end && i < self->size; i++) {\
		m(sub)->pushBack(self->arr[i]);\
	}\
	return sub;\
}

#define __JS_DecArrSort(T) \
void __js_arr_quickSort##T(Compare##T compare);

#define __JS_DefArrSort(T) \
void __js_arr_quickSort##T(Compare##T compare) {\
	/****/getSelf(JS##T##Array)/****/\
	printf("qsort\n");\
	SGLIB_ARRAY_SINGLE_QUICK_SORT(T, self->arr, self->size, compare);\
}

//merge sort arraylist
#define __JS_DecArrStableSort(T) \
void __js_arr_mergeHelper##T(const int l, const int mid, const int r, T *L, T *R, Compare##T compare);\
void __js_arr_merge_sort##T(int l, int r, T *L, T *R, Compare##T compare);\
void __js_arr_mergeSort##T(Compare##T compare);

#define __JS_DefArrStableSort(T) \
void __js_arr_mergeHelper##T(const int l, const int mid, const int r, T *L, T *R, Compare##T compare)\
{\
	/****/getSelf(JS##T##Array)/****/\
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
		if (compare(L[i], R[j]) <= 0)\
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
void __js_arr_merge_sort##T(int l, int r, T *L, T *R, Compare##T compare)\
{\
	/****/getSelf(JS##T##Array)/****/\
	if (l < r)\
	{\
		/* Same as (l+r)/2, but avoids overflow for large l and h */\
		int mid = l + (r - l) / 2;\
		this; __js_arr_merge_sort##T(l, mid, L, R, compare);\
		this; __js_arr_merge_sort##T(mid + 1, r, L, R, compare); \
		m(self); __js_arr_mergeHelper##T(l, mid, r, L, R, compare);\
	}\
}\
void __js_arr_mergeSort##T(Compare##T compare) {\
	/****/getSelf(JS##T##Array)/****/\
	int n = (int)(self->size / 2) + 1;\
	T *L = (T *)malloc(sizeof(T) * n);\
	T *R = (T *)malloc(sizeof(T) * n);\
	this; __js_arr_merge_sort##T(0, self->size - 1, L, R, compare);\
	free(L); free(R); \
}

#define __JS_DecArrMap(T) \
void __js_arr_map_##T(F_Map##T map_func);
#define __JS_DefArrMap(T) \
void __js_arr_map_##T(F_Map##T map_func) {\
	/****/getSelf(JS##T##Array)/****/\
	int i;\
	for (i = 0; i < self->size; i++) \
		map_func(self->arr + i);\
}

#define __JS_DecArrReduce(T) \
void __js_arr_reduce_##T(F_Reduce##T reduce_func, T *acc);
#define __JS_DefArrReduce(T) \
void __js_arr_reduce_##T(F_Reduce##T reduce_func, T *acc) {\
	/****/getSelf(JS##T##Array)/****/\
	int i; \
	for(i = 0; i < self->size; i++) \
		reduce_func(acc, self->arr + i);\
}


//destructor
#define __JS_DecArrFree(T) \
void __js_arr_free_##T();
#define __JS_DefArrFree(T) \
void __js_arr_free_##T() { \
	/****/getSelf(JS##T##Array)/****/\
	int i;\
	if (self->freeItem) {\
		for(i = 0; i < self->size; i++)\
			self->freeItem(self->arr[i]);\
	}\
	free(self->arr); \
	free(self); \
}



#endif
