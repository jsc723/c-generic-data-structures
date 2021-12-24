#ifndef JS_TREESET_H
#define JS_TREESET_H

/*
	An RB TreeSet implemented in C using sglib.
    By Sicheng Jiang
	2021/12/22
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "sglib.h"
#include "jscobj2.h"

#define JSTreeSet(K, C) pJSTreeSet##K##_##C
#define NewJSTreeSet(K, C) NewJSTreeSet##K##_##C()
#define JSTreeSetEntry(K, C) pJS_RBTree_Node_##K##_##C



/* ---------------------------------------DECLARE---------------------------------------------- */

#define Declare_JSTreeSet(K, C)\
typedef struct JS_RBTree_Node_##K##_##C *pJS_RBTree_Node_##K##_##C;\
typedef struct JS_RBTree_Node_##K##_##C {\
    K key;\
    char color;\
    JSTreeSetEntry(K, C) left;\
    JSTreeSetEntry(K, C) right;\
}JS_RBTree_Node_##K##_##C;\
int JS_RBTree_Node_##K##_##C##_Comparator(pJS_RBTree_Node_##K##_##C a, pJS_RBTree_Node_##K##_##C b);\
\
Define_Class(JSTreeSet##K##_##C)\
	JSTreeSetEntry(K, C) tree;\
	size_t size; /*num of items*/\
With_Methods(JSTreeSet##K##_##C)\
    K    (*peekMin)();\
    K    (*pollMin)();\
    K    (*peekMax)();\
    K    (*pollMax)();\
	void (*put)(K k);\
	int  (*containsKey)(K k);\
	void (*remove)(K k);\
	void (*clear)();\
	void(*free)();\
End_Class(JSTreeSet##K##_##C)\
\
SGLIB_DEFINE_RBTREE_PROTOTYPES(JS_RBTree_Node_##K##_##C, left, right, color, JS_RBTree_Node_##K##_##C##_Comparator);\
K __JS_TreeSetPeekMin##K##_##C();\
K __JS_TreeSetPollMin##K##_##C();\
K __JS_TreeSetPeekMax##K##_##C();\
K __JS_TreeSetPollMax##K##_##C();\
void __JS_TreeSetPut##K##_##C(K k);\
int  __JS_TreeSetContainsKey##K##_##C(K k);\
void __JS_TreeSetRemove##K##_##C(K k);\
void __JS_TreeSetClear##K##_##C();\
void __JS_TreeSetFree##K##_##C();\
pJSTreeSet##K##_##C NewJSTreeSet##K##_##C();



/* ---------------------------------------DEFINE---------------------------------------------- */

#define Define_JSTreeSet(K, C)\
SGLIB_DEFINE_RBTREE_FUNCTIONS(JS_RBTree_Node_##K##_##C, left, right, color, JS_RBTree_Node_##K##_##C##_Comparator);\
int JS_RBTree_Node_##K##_##C##_Comparator(pJS_RBTree_Node_##K##_##C a, pJS_RBTree_Node_##K##_##C b) {\
    return C(a->key, b->key);\
}\
K __JS_TreeSetPeekMin##K##_##C() {\
    /****/getSelf(JSTreeSet##K##_##C)/****/\
    pJS_RBTree_Node_##K##_##C p = self->tree;\
    while(p->left) p = p->left;\
    return p->key;\
}\
K __JS_TreeSetPollMin##K##_##C() {\
    /****/getSelf(JSTreeSet##K##_##C)/****/\
    pJS_RBTree_Node_##K##_##C p = self->tree;\
    K res;\
    while(p->left) p = p->left;\
    res = p->key;\
    sglib_JS_RBTree_Node_##K##_##C##_delete(&(self->tree), p);\
    free(p);\
    self->size--;\
    return res;\
}\
K __JS_TreeSetPeekMax##K##_##C() {\
    /****/getSelf(JSTreeSet##K##_##C)/****/\
    pJS_RBTree_Node_##K##_##C p = self->tree;\
    while(p->right) p = p->right;\
    return p->key;\
}\
K __JS_TreeSetPollMax##K##_##C(){\
    /****/getSelf(JSTreeSet##K##_##C)/****/\
    pJS_RBTree_Node_##K##_##C p = self->tree;\
    K res;\
    while(p->right) p = p->right;\
    res = p->key;\
    sglib_JS_RBTree_Node_##K##_##C##_delete(&(self->tree), p);\
    free(p);\
    self->size--;\
    return res;\
}\
void __JS_TreeSetPut##K##_##C(K k) {\
    /****/getSelf(JSTreeSet##K##_##C)/****/\
    JS_RBTree_Node_##K##_##C e, *t;\
    e.key = k;\
    if (sglib_JS_RBTree_Node_##K##_##C##_find_member(self->tree, &e)==NULL) {\
        t = (JS_RBTree_Node_##K##_##C *)calloc(1, sizeof(JS_RBTree_Node_##K##_##C));\
        t->key = k;\
        sglib_JS_RBTree_Node_##K##_##C##_add(&(self->tree), t);\
        self->size++;\
    }\
}\
int __JS_TreeSetContainsKey##K##_##C(K k) {\
    /****/getSelf(JSTreeSet##K##_##C)/****/\
    JS_RBTree_Node_##K##_##C e;\
    e.key = k;\
    return !!sglib_JS_RBTree_Node_##K##_##C##_find_member(self->tree, &e);\
}\
void __JS_TreeSetRemove##K##_##C(K k) {\
    /****/getSelf(JSTreeSet##K##_##C)/****/\
    JS_RBTree_Node_##K##_##C e, *result;\
    e.key = k;\
    if(sglib_JS_RBTree_Node_##K##_##C##_delete_if_member(&(self->tree), &e, &result)){\
        self->size--;\
    }\
}\
static void __JS_TreeSetClearAux##K##_##C(JSTreeSetEntry(K, C) node) {\
    if (node) {\
        __JS_TreeSetClearAux##K##_##C(node->left);\
        __JS_TreeSetClearAux##K##_##C(node->right);\
        free(node);\
    }\
}\
void __JS_TreeSetClear##K##_##C() {\
    /****/getSelf(JSTreeSet##K##_##C)/****/\
    __JS_TreeSetClearAux##K##_##C(self->tree);\
    self->size = 0;\
}\
void __JS_TreeSetFree##K##_##C() {\
    /****/getSelf(JSTreeSet##K##_##C)/****/\
    methodof(self)->clear();\
    free(self);\
}\
pJSTreeSet##K##_##C NewJSTreeSet##K##_##C() {\
    /****/Alloc_Instance(set, JSTreeSet##K##_##C)/****/\
    set->tree = NULL;\
    set->size = 0;\
}\
\
Install_Methods(JSTreeSet##K##_##C) \
    __JS_TreeSetPeekMin##K##_##C,\
    __JS_TreeSetPollMin##K##_##C,\
    __JS_TreeSetPeekMax##K##_##C,\
    __JS_TreeSetPollMax##K##_##C,\
    __JS_TreeSetPut##K##_##C,\
    __JS_TreeSetContainsKey##K##_##C,\
    __JS_TreeSetRemove##K##_##C,\
    __JS_TreeSetClear##K##_##C,\
    __JS_TreeSetFree##K##_##C \
End_Install()\

#define JSTreeSetForEach(K, C, _set_, entry) \
for(struct sglib_JS_RBTree_Node_##K##_##C##_iterator  _it_ = {0}; _it_.order >= 0; _it_.order = -1)\
for(JSTreeSetEntry(K, C) entry=sglib_JS_RBTree_Node_##K##_##C##_it_init_inorder(&_it_,(_set_)->tree); entry!=NULL; entry=sglib_JS_RBTree_Node_##K##_##C##_it_next(&_it_)) 


#endif