# c-generic-data-structures
some common data structures implemented in **C** (compiles under `-std=c99` and above), supporting generic types.
And the APIs are provided in a objective orienting style.

## About the OOP framework
Since all of these structures use my OOP framework written in ```jscobj2.h```, the APIs are also OOP like.
Let ```pInst``` be a pointer to an instance of data structure defined in this repo (e.g. an ArrayList) , 
You can access its members as usual using ```->```, (e.g. ```pInst->size```).
But when you want to call a method, you should use the ```m()``` macro. 
For example, ```m(pInst)->pushBack(10)``` 
append 10 at the end of the arraylist (assume pInst points to an arraylist instance).
## JSArrayList
header file: ```JSArrayList.h```
#### APIs
##### Declare and Define
```c
Declare_JSArrayList(T) //declare class JSArrayList<T> (an JSArrayList whose items have type T)

Define_JSArrayList(T) //define class JSArrayList<T>
```
##### Type Name
```c
JSArrayList(T) //type name of a *pointer* to an JSArrayList<T> instance
```
##### Constructors
```c
NewJSArrayList(T); //element type as the only parameter
NewJSArrayListFull(T, int capacity, void (*freeItem)(T *)); //type, initial capacity, and custom deallocator for each item (can be NULL)
```
##### Methods
```c
//append the item at the end of the array
void pushBack(T item); 
  
//append all n items at the end of the array
void pushAll(T *items, int n); 
  
//append all items in the otherList to the end of the array
void expend(ArrayList<T> * otherList); 

//insert the item at the index
void insert(T item, int index); 
  
//pop the last item
T popBack();

//pop the item at index
T popAt(int index);

//find the index of item, if the item does not exist, returns -1.
int indexOf(T item); 

//create a sublist of the list with the indecies [start, end). 
JSArrayList(T) subList(int start, int end); 

//sort the list using quick sort
void sort(JSCompare(T) comparator);

//sort the list using merge sort
void stableSort(JSCompare(T) comparator);

// apply map_func to every items in the array
void map(void (*map_func)(T *t));

/*
  same as 
  for each item in arraylist:
    reduce_func(acc, &item);
*/
void reduce(void (*reduce_func)(T *t1, T *t2), T *acc);

// free the arraylist
void free();
  
```

##### Public Members
```c
T *arr; //the array of the items, you can read/write arr[i] 
//(Note: it won't call freeItem when you overwrite a value in the array)

size_t size; //number of items, readonly

```
#### Examples
```c
/*In a header file*/
typedef struct student {
  char *name;
  int age;
}student, *pStudent;
Declare_JSArrayList(pStudent)
```
```c
/*In a source file*/
Define_JSArrayList(pStudent)

int comparePStudent(const pStudent *t1, const pStudent *t2) {
  pStudent p1 = *t1, p2 = * t2;
  return strcmp(p1->name, p2->name);
}
void freePStudent(pStudent p) {
  free(p->name);
  free(p);
}
pStudent newStudent(const char *name, int age) {
  pStudent p = (pStudent)malloc(sizeof(student));
  p->name = (char *)malloc(strlen(name) + 1);
  strcpy(p->name, name);
  p->age = age;
  return p;
}

int main() {
  ArrayList(pStudent) list = NewArrayListFull(pStudent, 16, );
  list->freeItem = freePStudent;
  m(list)->pushBack(newStudent("Bob", 20));
  m(list)->pushBack(newStudent("Alice", 19));
  m(list)->insert(newStudent("Eve", 21), 1);
  m(list)->sort();
  for(int i = 0; i < list->size; i++) {
    printf("%s %d\n",list->arr[i].name, list->arr[i].age);
  }
  m(list)->free();
  return 0;
}

  
```

## JSHashSet
header file: 
```
JSHashSet.h
```
#### APIs
##### Declare and Define
```c
Declare_JSHashSet(K) // Declare class HashSet<K> (an HashSet whose keys have type K)

Define_JSHashSet(K) // Define class HashSet<K>

```
##### Type Name
```c
JSHashSet(K) //type name of an pointer to an HashSet<K> instance
```
##### Constructors
```c
JSHashSet(K) NewJSHashSet(K, unsigned int(*hash_function)(K)); //prototype of the constructor of HashMap<K, V>

JSHashSet(K) NewJSHashSetFull(K, hash, cmp, freeKey); 
```
##### Methods
```c
//put a key into the HashSet.
void put(K k);

//check if the hashmap contains the given key. returns 1(true) or 0(false)
int  containsKey(K k);

//remove the key-value pair of the key k. If key does not exist, it does nothing.
void  remove(K k);

//remove all key-value pair in the hashmap.
void  clear();

//free the hashmap instance
void free();
```
##### Public Members
```c
size_t size; // number of items
```
## JSHashMap
header file: 
```
JSHashMap.h
```
#### APIs
##### Declare and Define
```c
Declare_JSHashMap(K, V) // Declare class HashMap<K, V> (an HashMap whose keys have type K and values has type V)

Define_JSHashMap(K, V) // Define class HashMap<K, V>
```
##### Type Names
```c
JSHashMap(K, V) //type name of an pointer to an HashMap<K, V> instance
```
##### Constructors
```c
JSHashMap(K, V) NewJSHashMap(K, V, unsigned int(*hash_function)(K)); //prototype of the constructor of HashMap<K, V>

JSHashMap(K, V) NewJSHashMapFull(K, V, hash, cmp, freeKey, freeValue); 
```
##### Methods
```c
//put a key-value pair (k, v) into the hashmap.
void put(K k, V v);

//get the corresponding value of a given key. If the key does not exist, it simply aborts.
V    get(K k);

//check if the hashmap contains the given key. returns 1(true) or 0(false)
int  containsKey(K k);

//remove the key-value pair of the key k. If key does not exist, it does nothing.
void  remove(K k);

//remove all key-value pair in the hashmap.
void  clear();


//free the hashmap instance
void free();
```
##### Public Members
```c
size_t size; // number of items
```
#### Examples
```c
/*In a header file*/

typedef char *string;

Declare_JSHashMap(string, int)
```
```c
/*In a source file*/

Define_JSHashMap(string, int)

string String(const char *s) {
	int sz = strlen(s);
	string str = (string)malloc(sz + 1);
	strcpy(str, s);
	return str;
}

unsigned int hashString(string str) {
	unsigned int hashVal;
	int i = 0;
	for (hashVal = 00; str[i] != '\0'; i++) {
		hashVal = str[i] + 31 * hashVal;
	}
	return hashVal;
}

int main() {
	JSHashMap(string, int) hashmap = NewJSHashMapFull(string, int, hashString, (JSCompare(string)) strcmp, (void (*)(char *))free, NULL);
	m(hashmap)->put(String("abc"), 10);
	m(hashmap)->put(String("qfdv"), 12);
	m(hashmap)->put(String("hello"), -22);
	m(hashmap)->put(String("hashmap"), 0);
	printf("%d\n", m(hashmap)->get("hello"));
	JSHashMapForEach(string, int, hashmap, e) {
		printf("%s %d\n", e->key, e->value);
	}
	m(map)->free();
	return 0;
}
```
## JSPriorityQueue
header file: ```JSPriorityQueue.h```
#### APIs
##### Declare and Define
```c
Declare_JSPriorityQueue(T) //declare class JSPriorityQueue<T> (an PriorityQueue whose items have type T)

Define_JSPriorityQueue(T) //define class JSPriorityQueue<T>
```
##### Type Name and Constructor
```c
PriorityQueue(T) //type name of a pointer to an PriorityQueue<T> instance
  
/* 
   prototype of the constructor of ArrayList<T>
   Note that the comparator should returns a positive number when t1 has a higher priority then t2. 
   (And 0 when equal, negative when less) 
*/
PriorityQueue(T) NewPriorityQueue(T, JSCompare(T)); 

```
##### Methods
```c
//add an item into the priority queue
void add(T elem);

//clear all items in the queue
void clear();

//returns 0 if the queue does not contain the elem. Otherwise returns a value that is not 0.
int contains(T elem);

/* Return the element at the top of the queue (highest priority), does not remove it from the queue.
*/
T peek();

/* Same as peek(), except this method will remove the item that has the higher priority from the queue. */
T poll();

//remove one instance of elem from the queue. Return 1 if successfully removed one item, 0 if the item is not found.
int remove(T elem);

//return the number of elements in the queue.
size_t size();

```

##### Public Members

NONE

#### Examples
```c
/*In a header file*/
typedef char *string;
typedef struct word {
	string v;
	size_t count;
}word;
Declare_JSPriorityQueue(word)
```
```c
/*In a source file*/
Define_JSPriorityQueue(word)

int cmpword(const word *w1, const word *w2) {
	return w1->count - w2->count;
}

int main() {
	JSPriorityQueue(word) pq = NewJSPriorityQueue(word, cmpword);
	word w1 = { "hello", 5 };
	word w2 = { "world", 10 };
	word w3 = { "c++", 8 };
	word w4 = { "java", 1 };
	m(pq)->add(w1);
	m(pq)->add(w2);
	m(pq)->add(w3);
	m(pq)->add(w4);
	while (m(pq)->size()) {
		word t = m(pq)->poll();
		printf("%s %d\n", t.v, t.count);
	}
	m(pq)->free();
	return 0;
}
  
```
