#ifndef JSCOBJ_H
#define JSCOBJ_H
/*
	Version: 2.1
	By Sicheng Jiang
	An framework for objective programming in C.
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
	End_Class(class_name)

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
		self;	//self refers to the pointer of the object calling this method.
		self->attribute1;	 //refering to its attribute
		m(self)->method_2(); //calling its method
		this->method_2();	//same as the previous line, in other words, 
							//'this' is equivalent to 'm(self)'.
		...
	}	
*/
#include <stdio.h>

#define this methodof(self)
#define methodof(pInst) ((_js_obj_arg_ = (pInst)), ((pInst)->__vftb__))

extern __thread void *_js_obj_arg_;

#define VFTB(T) _VFTB_##T
#define pVFTB(T) _VFTB_##T *

//--------------------first group---------------------
#define Define_Class(T) \
typedef struct _VFTB_##T _VFTB_##T; \
typedef struct T *p##T;\
typedef struct T {\
	_VFTB_##T *__vftb__;

#define With_Methods(T) \
}T; \
struct _VFTB_##T {

#define End_Class(T) \
}; \
extern _VFTB_##T __vftb__##T;\

//--------------------second group----------------------------------------
#define Install_Methods(T) \
_VFTB_##T __vftb__##T = {

#define End_Install() };

//-------------------third group---------------------------------------
#define Alloc_Instance(pInst,T) \
p##T pInst = (p##T)malloc(sizeof(T));\
pInst->__vftb__ = &__vftb__##T; 

//-------------------fourth group--------------------------------------
#define Method_Of(T) getSelf(T)

//#define Method(T, name, parament_list) \
//_##T##_##name parament_list { getSelf(T);

//#define End_Method(T, name) \
//} __vftb__##T.name = _##T##_##name;

//--------------------private macro---------------------------------------------
#define getSelf(T) T * self = (T *)_js_obj_arg_;
//#define dec_Method(T) struct T * (* _method_)(struct T * ps)

//--------------------utils---------------------------------------
#define JSCompare(T) JSCompare_##T
#define JS__DefCompare(T) typedef int (*JSCompare(T))(const T t1, const T t2);

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

//int DFT_CMP(const void *a, const void *b);

#define __JS_DecDefaultCmpN(Y) \
int JS_DFT_CMP_N_##Y(const Y a, const Y b);

#define __JS_DefDefaultCmpN(Y) \
int JS_DFT_CMP_N_##Y(const Y a, const Y b) { return a - b;}

#define __JS_DefDefaultCmpNF(Y) \
int JS_DFT_CMP_N_##Y(const Y a, const Y b) { double t = a - b;\
	if (t == 0) return 0;\
	if (t > 0) return 1;\
	return -1;\
}

__JS_DecDefaultCmpN(int)
__JS_DecDefaultCmpN(short)
__JS_DecDefaultCmpN(char)
__JS_DecDefaultCmpN(long)
__JS_DecDefaultCmpN(size_t)
__JS_DecDefaultCmpN(float)
__JS_DecDefaultCmpN(double)

#define JS_DEFAULT_CMP(Y) JS_DFT_CMP_N_##Y

#endif
