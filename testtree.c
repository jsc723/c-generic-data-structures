#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define USE_JSCOBJ

#include "JSTreeSet.h"
#define CMP_INT(a, b) (a - b)
#define CMP_INT_GT(a, b) (b - a)

Declare_JSTreeSet(int, CMP_INT)
Define_JSTreeSet(int, CMP_INT)
Declare_JSTreeSet(int, CMP_INT_GT)
Define_JSTreeSet(int, CMP_INT_GT)

void inorder(JSTreeSetEntry(int, CMP_INT_GT) node) {
    if(node) {
        inorder(node->left);
        printf("%d\n", node->key);
        inorder(node->right);
    }
}

int main() {
    JSTreeSet(int, CMP_INT_GT) set = NewJSTreeSet(int, CMP_INT_GT);
    methodof(set)->put(199);
    methodof(set)->put(23);
    methodof(set)->put(1);
    methodof(set)->put(344);
    methodof(set)->put(5556);
    methodof(set)->put(-90);
    printf("size = %d\n", set->size);
    printf("test member 344: %d\n", methodof(set)->containsKey(344));
    printf("test member 346: %d\n", methodof(set)->containsKey(346));
    methodof(set)->remove(344);
    inorder(set->tree);
    printf("test member 344: %d\n", methodof(set)->containsKey(344));
    JSTreeSetForEach(int, CMP_INT_GT, set, entry) {
        printf("%d\n", entry->key);
    }
    while(set->size) {
        int n = methodof(set)->pollMax();
        printf(":%d\n", n);
    }
    methodof(set)->free();
    printf("done\n");
    return 0;
}