#ifndef JS_LIST_H
#define JS_LIST_H

/*
	An RB Doubly linked list implemented in C using sglib.
    By Sicheng Jiang
	2021/12/23
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "sglib.h"
#include "jscobj2.h"

#define JSList(T) pJSList##T
#define NewJSList(T) NewJSList##T()
#define NewJSListEntry(T, value) NewJSListEntry##T(value)
#define JSListEntry(T) pJS_List_Node_##T

#define Declare_JSList(T)\
typedef struct JS_List_Node_##T *pJS_List_Node_##T;\
typedef struct JS_List_Node_##T {\
    T value;\
    pJS_List_Node_##T prev;\
    pJS_List_Node_##T next;\
}JS_List_Node_##T;\
\
JS__DefCompare(T)\
Define_Class(JSList##T)\
	JSListEntry(T) head;\
    JSListEntry(T) tail;\
	size_t size; /*num of items*/\
With_Methods(JSList##T)\
    JSListEntry(T)    (*front)();\
    JSListEntry(T)    (*back)();\
    void (*pushFront)(JSListEntry(T) elem);\
    void (*pushBack)(JSListEntry(T) elem);\
    JSListEntry(T) (*popFront)();\
    JSListEntry(T) (*popBack)();\
    void (*remove)(JSListEntry(T) elem);\
    void (*insertAfter)(JSListEntry(T) where, JSListEntry(T) elem);\
    void (*insertBefore)(JSListEntry(T) where, JSListEntry(T) elem);\
    JSListEntry(T) (*find)(T value, JSCompare(T) compare);\
    void (*reverse)();\
    void (*sort)(JSCompare(T) compare);\
	void (*clear)();\
	void (*free)();\
End_Class(JSList##T)\
\
JSListEntry(T) __JS_ListFront##T();\
JSListEntry(T) __JS_ListBack##T();\
void __JS_ListPushFront##T(JSListEntry(T) elem);\
void __JS_ListPushBack##T(JSListEntry(T) elem);\
JSListEntry(T) __JS_ListPopFront##T();\
JSListEntry(T) __JS_ListPopBack##T();\
void __JS_ListRemove##T(JSListEntry(T) elem);\
void __JS_ListInsertAfter##T(JSListEntry(T) where, JSListEntry(T) elem);\
void __JS_ListInsertBefore##T(JSListEntry(T) where, JSListEntry(T) elem);\
JSListEntry(T) __JS_ListFind##T(T value, JSCompare(T) compare);\
void __JS_ListReverse##T();\
void __JS_ListSort##T(JSCompare(T) compare);\
void __JS_ListClear##T();\
void __JS_ListFree##T();\
pJSList##T NewJSList##T();\
JSListEntry(T) NewJSListEntry##T();

/* ---------------------------------------------------------------------------------- */

#define Define_JSList(T) \
JSListEntry(T) __JS_ListFront##T(){\
    /****/getSelf(JSList##T)/****/\
    return self->head;\
}\
JSListEntry(T) __JS_ListBack##T() {\
    /****/getSelf(JSList##T)/****/\
    return self->tail;\
}\
void __JS_ListPushFront##T(JSListEntry(T) elem){\
    /****/getSelf(JSList##T)/****/\
    SGLIB_DL_LIST_ADD_BEFORE(JS_List_Node_##T, self->head, elem, prev, next);\
    if(self->head->prev) self->head = self->head->prev;\
    if (self->tail == NULL) self->tail = elem;\
    self->size++;\
}\
void __JS_ListPushBack##T(JSListEntry(T) elem){\
    /****/getSelf(JSList##T)/****/\
    SGLIB_DL_LIST_ADD_AFTER(JS_List_Node_##T, self->tail, elem, prev, next);\
    if(self->tail->next) self->tail = self->tail->next;\
    if (self->head == NULL) self->head = elem;\
    self->size++;\
}\
JSListEntry(T) __JS_ListPopFront##T() {\
    /****/getSelf(JSList##T)/****/\
    JSListEntry(T) elem = methodof(self)->front();\
    methodof(self)->remove(elem);\
    return elem;\
}\
JSListEntry(T) __JS_ListPopBack##T(){\
    /****/getSelf(JSList##T)/****/\
    JSListEntry(T) elem = methodof(self)->back();\
    methodof(self)->remove(elem);\
    return elem;\
}\
void __JS_ListRemove##T(JSListEntry(T) elem){\
    /****/getSelf(JSList##T)/****/\
    if (elem) {\
        if (elem == self->tail) self->tail = self->tail->prev;\
        SGLIB_DL_LIST_DELETE(JS_List_Node_##T, self->head, elem, prev, next);\
        self->size--;\
    }\
}\
void __JS_ListInsertAfter##T(JSListEntry(T) where, JSListEntry(T) elem){\
    /****/getSelf(JSList##T)/****/\
    SGLIB_DL_LIST_ADD_AFTER(JS_List_Node_##T, where, elem, prev, next);\
    if (self->tail == where) self->tail = elem;\
    self->size++;\
}\
void __JS_ListInsertBefore##T(JSListEntry(T) where, JSListEntry(T) elem){\
        /****/getSelf(JSList##T)/****/\
    SGLIB_DL_LIST_ADD_BEFORE(JS_List_Node_##T, where, elem, prev, next);\
    if (self->head == where) self->head = elem;\
    self->size++;\
}\
JSListEntry(T) __JS_ListFind##T(T value, JSCompare(T) compare){\
    /****/getSelf(JSList##T)/****/\
    for(JSListEntry(T) entry = self->head; entry; entry = entry->next){\
        if (compare(entry->value, value)==0) return entry;\
    }\
    return NULL;\
}\
void __JS_ListReverse##T(){\
    /****/getSelf(JSList##T)/****/\
    SGLIB_DL_LIST_REVERSE(JS_List_Node_##T, self->head, prev, next);\
    JSListEntry(T) temp = self->head;\
    self->head = self->tail;\
    self->tail = temp;\
}\
void __JS_ListSort##T(JSCompare(T) compare) {\
    /****/getSelf(JSList##T)/****/\
    if (self->head) {\
        SGLIB_DL_LIST_SORT_VALUE(JS_List_Node_##T, self->head, compare, prev, next, value);\
    }\
}\
void __JS_ListClear##T(){\
    /****/getSelf(JSList##T)/****/\
    self->head = self->tail = NULL;\
    self->size = 0;\
}\
void __JS_ListFree##T(){\
    /****/getSelf(JSList##T)/****/\
    methodof(self)->clear();\
    free(self);\
}\
pJSList##T NewJSList##T() {\
    /****/Alloc_Instance(self, JSList##T)/****/\
    self->head = NULL;\
    self->tail = NULL;\
    self->size = 0;\
}\
JSListEntry(T) NewJSListEntry##T(T value){\
    JSListEntry(T) entry = (JSListEntry(T))malloc(sizeof(JSListEntry(T)));\
    entry->value = value;\
    entry->prev = NULL;\
    entry->next = NULL;\
}\
Install_Methods(JSList##T) \
    __JS_ListFront##T,\
    __JS_ListBack##T,\
    __JS_ListPushFront##T,\
    __JS_ListPushBack##T,\
    __JS_ListPopFront##T,\
    __JS_ListPopBack##T,\
    __JS_ListRemove##T,\
    __JS_ListInsertAfter##T,\
    __JS_ListInsertBefore##T,\
    __JS_ListFind##T,\
    __JS_ListReverse##T,\
    __JS_ListSort##T,\
    __JS_ListClear##T,\
    __JS_ListFree##T \
End_Install()



#define JSListForEach(T, _list_, entry) \
for(JSListEntry(T) entry = (_list_)->head; entry; entry = entry->next)

#endif