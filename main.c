#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "CHashMap.h"
#include "CPriorityQueue.h"
#include "CHashMap.h"
#include "userDefined.h"
#include "CHashTable.h"

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
int str_eq(const string *p1, const string *p2) {
	return strcmp(*p1, *p2) == 0;
}

Declare_JSPriorityQueue(double)
Define_JSPriorityQueue(double)

Declare_JSPriorityQueue(word)
Define_JSPriorityQueue(word)

Declare_CHashMap(int, string)
Define_CHashMap(int, string)

Declare_CHashMap(string, int)
Define_CHashMap(string, int)

Declare_JSHashTable(int)
Define_JSHashTable(int)

string String(const char *s) {
	int sz = strlen(s);
	string str = (string)malloc(sz + 1);
	str_cpy(str, s);
	return str;
}

int cmpword(const word w1, const word w2) {
	return w1.count - w2.count;
}

int main() {
	JSPriorityQueue(word) pq = NewJSPriorityQueue(word, cmpword);
	word w1 = { "hello", 5 };
	word w2 = { "I", 10 };
	word w3 = { "say", 8 };
	word w4 = { "world", 1 };
	m(pq)->add(w1);
	m(pq)->add(w2);
	m(pq)->add(w3);
	m(pq)->add(w4);
	word t;
	while (m(pq)->size()) {
		t = m(pq)->poll();
		printf("%s %d\n", t.v, t.count);
	}
	m(pq)->free();

	HashMap(string, int) map = NewHashMap(string, int, hashString, (Comparestring) strcmp);
	for(int i = 0; i < 20; i++) {
		string s = randomStr(10);
		printf("insert %s %d\n", s, i);
		m(map)->put(s, i);
	}
	m(map)->remove("phqghumeay");
	// HashMapEntry(string, int) head = m(map)->enumerate(), p;
	// for(p = head->next; p != head; p = p->next) {
	// 	printf("%s %d\n", p->key, p->value);
	// }
	HashMapForEach(string, int, map, e) {
		printf("%s %d\n", e->key, e->value);
	}

	JSHashTable(int) set = NewJSHashTable(int, hashInt, JS_DEFAULT_CMP(int));
	for(int i = 100; i < 120; i++) {
		m(set)->put(i);
	}
	m(set)->put(10);
	m(set)->put(14);
	m(set)->put(120);
	m(set)->put(1248);
	m(set)->remove(100);
	JSHashTableForEach(int, set, p) {
		printf("%d\n", p->key);
	}

	return 0;
}