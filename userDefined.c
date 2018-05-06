#include "userDefined.h"

word Word(const char *s, size_t count) {
	word w = { String(s), count };
	return w;
}


void str_cpy(char *dst, const char *src) {
	char *p = dst;
	while (*src)
		*(p++) = *(src++);
	*p = '\0';
}

string emptyStr(size_t sz) {
	string s = (string)malloc(sz * sizeof(char));
	s[0] = '\0';
	return s;
}
int strCMP(string *s1, string *s2) {
	return strcmp(*s1, *s2);
}
void strUpper(string *p) {
	string s = *p;
	while (*s)
		*s++ = toupper(*s);
}

void strConCat(string *acc, string *ps) {
	int n = strlen(*acc);
	str_cpy(*acc + n, *ps);
}

Define_CArrayList(int)

Define_CArrayList(string)

Define_CHashMap(int, double)

Define_CHashMap(string, int)
