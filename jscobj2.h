#ifndef JSCOBJ_H
#define JSCOBJ_H
/*
	Version: 2.0
	By Sicheng Jiang (UC Berkeley)
	An framework for objective programming in C.
	Note: this framework is not designed for a parallelized program
	2018/04/19

-----------------------------Usage------------------------------------

1) define a class: (better in a header file)
	Define_Class(class_name) //define class name and its attributes
		attribute1;
		attribute2;
		...
	With_Methods(class_name) //define the methods of the class
		function_pointer1;
		function_pointer2;
		...
	End_Class(T)

Note: If you want to put the pointer of class_name in its defination,
you can use pclass_name for convenience. In other words, the statement
	typedef class_name *pclass_name;
is included in the macro Define_Class().

2) define methods: (better in a source file)
	... method_1(...){ //matches defination of function_pointer1
		Method_Of(class_name) 
		...
	}
	... method_2(...){ //matches defination of function_pointer2
		Method_Of(class_name)
		...
	}
	...

3) install methods: (also in the source file, after step 1 and 2)
	Install_Methods(class_name)
		method_1,
		method_2,
		...
		method_n
	End_Install()

4) An jscobj object MUST lives in heap. And you MUST use it through its pointer.
	This means you MUST allocate some memory to its pointer to use it. And when allocating,
	DO NOT use malloc/calloc/realloc, but use Alloc_Instance(name, class_name) instead.
	For example, if you want to allocate a block of memory for Student *p, where Student is 
	an object defined by this framework, you write:
		Alloc_Instance(p, Student);
	This has the same effect of 
		Student *p = (Student *)malloc(sizeof(Student));
	but did some extra work to initialize something that is needed for the framework.

	Note: you can free the pointer normally using free().

5) Usage:
	a) out of the class
		Alloc_Instance(s, Student); //create a new Student instance
		...					//initialize it
		s->name;			//refering to its attribute name
		m(s)->set_age(20);  //calling its method

	b) in a method
	... method_1(...){ 
		Method_Of(class_name) //you cannot use 'self' without this line
		...
		self;	//self refers to the address of the object calling this method.
		self->attribute1;	 //refering to its attribute
		m(self)->method_2(); //calling its method
		this->method_2();	//same as the previous line, in other words, 
							//'this' is equivalent to 'm(self)'.
		...
	}	
*/
#include <stdio.h>

#define this m(self)
#define m(pInst) (pInst)->_method_(pInst)->__vftb__

extern void *_obj_arg_;
void *__method__(void *obj);

#define VFTB(T) _VFTB_##T
#define pVFTB(T) _VFTB_##T *

//--------------------first group---------------------
#define Define_Class(T) \
typedef struct _VFTB_##T _VFTB_##T; \
typedef struct T *p##T;\
typedef struct T {\
	_VFTB_##T *__vftb__; \
	p##T (* _method_)(p##T ps);

#define With_Methods(T) \
}T; \
struct _VFTB_##T {

#define End_Class(T) \
}; \
T * T##_Method(T *ps); \
extern _VFTB_##T __vftb__##T;\

//--------------------second group----------------------------------------
#define Install_Methods(T) \
T * T##_Method(T *ps) { return (T *)__method__(ps); } \
_VFTB_##T __vftb__##T = {

#define End_Install() };

//-------------------third group---------------------------------------
#define Alloc_Instance(pInst,T) \
p##T pInst = (p##T)malloc(sizeof(T));\
pInst->_method_ = T##_Method; \
pInst->__vftb__ = &__vftb__##T; 

//-------------------fourth group--------------------------------------
#define Method_Of(T) getSelf(T)

//#define Method(T, name, parament_list) \
//_##T##_##name parament_list { getSelf(T);

//#define End_Method(T, name) \
//} __vftb__##T.name = _##T##_##name;

//--------------------private macro---------------------------------------------
#define getSelf(T) T * self = (T *)_obj_arg_;
//#define dec_Method(T) struct T * (* _method_)(struct T * ps)

#define __DecDefaultEqual(ClassType, Y) \
int DFT_EQ_##ClassType##_##Y(const Y *a, const Y *b);

#define __DefDefaultEqual(ClassType, Y) \
int DFT_EQ_##ClassType##_##Y(const Y *a, const Y *b) {\
	char *p = (char *)a; \
	char *q = (char *)b; \
	size_t i = 0, sz = sizeof(Y);\
	while(i < sz) {\
		if (p[i] != q[i]) return 0;\
		i++;\
	}\
	return 1;\
} 

#define DFT_EQUAL(ClassType, Y) DFT_EQ_##ClassType##_##Y
//---------------------------------------------------------------------



//----------default comparer---------//

int DFT_CMP(const void *a, const void *b);

#define __DecDefaultCmpN(Y) \
int DFT_CMP_N_##Y(const Y *a, const Y *b);

#define __DefDefaultCmpN(Y) \
int DFT_CMP_N_##Y(const Y *a, const Y *b) { double t = *a - *b;\
	if (t == 0) return 0;\
	if (t > 0) return 1;\
	return -1;\
}

__DecDefaultCmpN(int)
__DecDefaultCmpN(short)
__DecDefaultCmpN(char)
__DecDefaultCmpN(long)
__DecDefaultCmpN(size_t)
__DecDefaultCmpN(float)
__DecDefaultCmpN(double)

#define DFT_CMPN(Y) DFT_CMP_N_##Y

#define _Define_Optional(T)\
typedef struct _Optional_##T {\
	int valid; T value;\
}_Optional_##T;

#define Optional(T) _Optional_##T


#endif
