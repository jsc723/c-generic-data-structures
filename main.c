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

__Dec_CPQ(double)
__DecSwapUpDown(double)
__Dec_CPQ_Helper(double)
__DecCPQ_add(double)
__DecCPQ_clear(double)
__DecCPQ_contains(double)
__DecCPQ_peek(double)
__DecCPQ_poll(double)
__DecCPQ_remove(double)
__DecCPQ_size(double)
__DecNewCPQ(double)


Define_CArrayList(double)
__DefSwap(double)
__DefUp(double)
__DefDown(double)
__Def_CPQ_Helper(double)
__DefCPQ_add(double)
__DefCPQ_clear(double)
__DefCPQ_contains(double)
__DefCPQ_peek(double)
__DefCPQ_poll(double)
__DefCPQ_remove(double)
__DefCPQ_size(double)
__DefNewCPQ(double)


int main() {
	PriorityQueue(double) pq = NewPriorityQueue(double)(DFT_CMPN(double));
	m(pq)->add(2.0);
	m(pq)->add(9.0);
	m(pq)->add(4.0);
	m(pq)->add(5.0);
	m(pq)->add(1.0);
	m(pq)->add(12.0);
	m(pq)->add(19.0);
	m(pq)->add(4.0);
	m(pq)->add(15.0);
	m(pq)->add(1.0);
	while (m(pq)->size() > 0) {
		printf("%lf\n", m(pq)->poll().value);
	}
	system("pause");
	return 0;
}