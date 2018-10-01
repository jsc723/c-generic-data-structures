#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "CHashMap.h"
#include "CPriorityQueue.h"
#include "userDefined.h"


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

Declare_CPriorityQueue(double)
Define_CPriorityQueue(double)

Declare_CPriorityQueue(word)
Define_CPriorityQueue(word)

string String(const char *s) {
	int sz = strlen(s);
	string str = (string)malloc(sz + 1);
	str_cpy(str, s);
	return str;
}

int cmpword(const word *w1, const word *w2) {
	return w1->count - w2->count;
}

int main() {
	PriorityQueue(word) pq = NewPriorityQueue(word)(cmpword);
	word w1 = { "hello", 5 };
	word w2 = { "I", 10 };
	word w3 = { "say", 8 };
	word w4 = { "yukarin", 1 };
	m(pq)->add(w1);
	m(pq)->add(w2);
	m(pq)->add(w3);
	m(pq)->add(w4);
	Optional(word) t;
	while ((t = m(pq)->poll()).valid) {
		printf("%s %d\n", t.value.v, t.value.count);
	}
	m(pq)->free();
	system("pause");
	return 0;
}