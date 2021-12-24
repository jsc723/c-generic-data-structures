#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USE_JSCOBJ

#include "JSList.h"

Declare_JSList(int)
Define_JSList(int)
Declare_JSList(double)
Define_JSList(double)

void printList(JSList(int) list) {
    printf("list [%d]: ", list->size);
    JSListForEach(int, list, entry) {
        printf("%d <-> ", entry->value);
    }
    printf("\n");
}
int main() {
    JSList(int) lst = NewJSList(int);
    methodof(lst)->reverse();
    JSListEntry(int) arr[10];
    for(int i = 0; i < 10; i++) {
        arr[i] = NewJSListEntry(int, i);
        methodof(lst)->pushBack(arr[i]);
    }
    printList(lst);
    methodof(lst)->popBack();
    methodof(lst)->popFront();
    printList(lst);
    methodof(lst)->remove(arr[5]);
    methodof(lst)->remove(arr[3]);
    printList(lst);
    methodof(lst)->insertAfter(arr[2], arr[5]);
    methodof(lst)->insertBefore(arr[6], arr[3]);
    printList(lst);
    while(lst->size) {
        printf("pop %d \n", methodof(lst)->popFront()->value);
    }
    for(int i = 0; i < 5; i++) {
        methodof(lst)->pushFront(arr[i]);
    }
    for(int i = 5; i < 10; i++) {
        methodof(lst)->pushBack(arr[i]);
    }
    printList(lst);
    methodof(lst)->sort(JS_DEFAULT_CMP(int));
    printList(lst);
    methodof(lst)->reverse();
    printList(lst);
    printf("find 5: %d\n", methodof(lst)->find(5, JS_DEFAULT_CMP(int))->value);
    methodof(lst)->clear();
    printList(lst);
    methodof(lst)->free();
    return 0;
}