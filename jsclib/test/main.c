#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define USE_JSCOBJ
#include "../JSHashMap.h"
#include "../JSPriorityQueue.h"
#include "../JSTreeSet.h"
#include "userDefined.h"
#include "../JSHashSet.h"

unsigned int hashInt(int i) { return i; }

unsigned int hashString(string str) {
	unsigned int hashVal;
	int i = 0;
	for (hashVal = 00; str[i] != '\0'; i++) {
		hashVal = str[i] + 31 * hashVal;
	}
	return hashVal;
}

string randomStr(int n) {
	string s = (string)malloc(n + 1);
	int i;
	for (i = 0; i < n; i++) {
		s[i] = rand() % 26 + 'a';
	}
	s[i] = '\0';
	return s;
}

Declare_JSPriorityQueue(double)
Define_JSPriorityQueue(double)

Declare_JSPriorityQueue(word)
Define_JSPriorityQueue(word)

//Declare_JSHashMap(int, string)
//Define_JSHashMap(int, string)

Declare_JSHashMap(string, int)
Define_JSHashMap(string, int)

Declare_JSHashSet(int)
Define_JSHashSet(int)


Declare_JSTreeSet(int, JS_DFT_CMP_int)
Define_JSTreeSet(int, JS_DFT_CMP_int)

string String(const char *s) {
	int sz = strlen(s);
	string str = (string)malloc(sz + 1);
	str_cpy(str, s);
	return str;
}

int cmpword(const word w1, const word w2) {
	return w1.count - w2.count;
}

void freeInt(int i) {
	printf("free %d\n", i);
}
void inorder(JSTreeSetEntry(int, JS_DFT_CMP_int) node) {
    if(node) {
        inorder(node->left);
        printf("%d\n", node->key);
        inorder(node->right);
    }
}

int main() {
	JSPriorityQueue(word) pq = NewJSPriorityQueue(word, cmpword);
	word w1 = { "hello", 5 };
	word w2 = { "I", 10 };
	word w3 = { "say", 8 };
	word w4 = { "world", 1 };
	methodof(pq)->add(w1);
	methodof(pq)->add(w2);
	methodof(pq)->add(w3);
	methodof(pq)->add(w4);
	word t;
	while (methodof(pq)->size()) {
		t = methodof(pq)->poll();
		printf("%s %d\n", t.v, t.count);
	}
	methodof(pq)->free();

	JSHashMap(string, int) map = NewJSHashMapFull(string, int, hashString, (JSCompare(string))strcmp, (void (*)(char *))free, freeInt);
	for(int i = 0; i < 20; i++) {
		string s = randomStr(10);
		printf("insert %s %d\n", s, i);
		methodof(map)->put(s, i);
	}
	methodof(map)->remove("phqghumeay");
	// HashMapEntry(string, int) head = methodof(map)->enumerate(), p;
	// for(p = head->next; p != head; p = p->next) {
	// 	printf("%s %d\n", p->key, p->value);
	// }
	JSHashMapForEach(string, int, map, e) {
		printf("%s %d\n", e->key, e->value);
	}
	methodof(map)->free();

	JSHashSet(int) set = NewJSHashSetFull(int, hashInt, JS_DEFAULT_CMP(int), freeInt);
	for(int i = 100; i < 120; i++) {
		methodof(set)->put(i);
	}
	methodof(set)->put(10);
	methodof(set)->put(14);
	methodof(set)->put(120);
	methodof(set)->put(1248);
	methodof(set)->remove(100);
	JSHashSetForEach(int, set, p) {
		printf("%d\n", p->key);
	}
	methodof(set)->free();

	JSTreeSet(int, JS_DFT_CMP_int) treeset = NewJSTreeSet(int, JS_DFT_CMP_int);
    methodof(treeset)->put(199);
    methodof(treeset)->put(23);
    methodof(treeset)->put(1);
    methodof(treeset)->put(344);
    methodof(treeset)->put(5556);
    methodof(treeset)->put(-90);
    printf("size = %d\n", treeset->size);
    printf("test member 344: %d\n", methodof(treeset)->containsKey(344));
    printf("test member 346: %d\n", methodof(treeset)->containsKey(346));
    methodof(treeset)->remove(344);
    inorder(treeset->tree);
    printf("test member 344: %d\n", methodof(treeset)->containsKey(344));
    JSTreeSetForEach(int, JS_DFT_CMP_int, treeset, entry) {
        printf("%d\n", entry->key);
    }
    while(treeset->size) {
        int n = methodof(treeset)->pollMax();
        printf(":%d\n", n);
    }
    methodof(treeset)->free();
    printf("done\n");
	return 0;
}