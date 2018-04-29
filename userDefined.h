#ifndef USER_DEFINED_H
#define USER_DEFINED_H

#include <string.h>
#include <ctype.h>
#include "CArrayList.h"
#include "CHashMap.h"
#include "CPriorityQueue.h"
typedef char *string;

typedef struct word {
	string v;
	size_t count;
}word, *pWord;

string String(const char *s);

word Word(const char *s, size_t count);
void str_cpy(char *dst, const char *src);
string emptyStr(size_t sz);
int strCMP(string *s1, string *s2);
void strUpper(string *p);
void strConCat(string *acc, string *ps);




Declare_CArrayList(int)
Declare_CArrayList(string)
Declare_CHashMap(int, double)
Declare_CHashMap(string, int)

#endif
