#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define USE_JSCOBJ
#include "../JSArrayList.h"

Declare_JSArrayList(int)
Define_JSArrayList(int)
Declare_JSArrayList(double)
Define_JSArrayList(double)

int main() {
    JSArrayList(double) lst = NewJSArrayList(double);
    printf("inited\n");
    methodof(lst)->pushBack(0.1);
    methodof(lst)->pushBack(0.44);
    methodof(lst)->pushBack(0.3);
    methodof(lst)->pushBack(0.14);
    printf("pushed\n");
    methodof(lst)->stableSort(JS_DEFAULT_CMP(double));
    printf("sorted\n");
    for(int i = 0; i < lst->size; i++) {
        printf("%lf ", lst->arr[i]);
    }
    printf("\n");
    return 0;
}