	#include "jscobj2.h"
    __thread void *_js_obj_arg_;
	void *__js_method__(void *obj){ return _js_obj_arg_ = obj; }
	//int DFT_CMP(const void *a, const void *b) { printf("comparer not defined!"); system("pause"); exit(1); }

	__JS_DefDefaultCmpN(int)
	__JS_DefDefaultCmpN(short)
	__JS_DefDefaultCmpN(char)
	__JS_DefDefaultCmpN(long)
	__JS_DefDefaultCmpN(size_t)
	__JS_DefDefaultCmpNF(float)
	__JS_DefDefaultCmpNF(double)