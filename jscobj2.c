#include "jscobj2.h"
#include <stdlib.h>
#include <stdio.h>
/*
	Version: 2.0
	By Sicheng Jiang (UC Berkeley)
	An framework for objective programming in C.
	2018/04/19
*/

void *_obj_arg_;
void *__method__(void *obj){_obj_arg_ = obj;return obj;}


int DFT_CMP(const void *a, const void *b) { printf("comparer not defined!"); system("pause"); exit(1); }

__DefDefaultCmpN(int)
__DefDefaultCmpN(short)
__DefDefaultCmpN(char)
__DefDefaultCmpN(long)
__DefDefaultCmpN(size_t)
__DefDefaultCmpN(float)
__DefDefaultCmpN(double)


