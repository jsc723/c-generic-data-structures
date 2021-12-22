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
## ArrayList
header file: ```CArrayList.h```
#### APIs
##### Declare and Define
```c
Declare_CArrayList(T) //declare class ArrayList<T> (an ArrayList whose items have type T)

Define_CArrayList(T) //define class ArrayList<T>
```
##### Type Name and Constructor
```c
ArrayList(T) //type name of a pointer to an ArrayList<T> instance
  
NewArrayList(T) //name of the constructor of ArrayList<T>
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
ArrayList<T> *subList(int start, int end); 

//sort the list using merge sort, must set the comparater first (see the next session)
void sort();

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
T *arr; //the array of the items

size_t size; //number of items

//the comparater of the items, you must set this if you want to use sort
int (*compare)(const T *t1, const T *t2); 

/*
  check if two items are equal, there is a default equals function that compares two items byte by byte.
  If you are storing pointers, you probably need to change this.
*/
int (*equals)(const T *t1, const T *t2);  

/*
  If you are storing pointers then you probably need to set this to tell arraylist how to free an item.
  If you set this, the arraylist calls it on every item before it frees itself in ArrayList<T>::free(). 
*/
void (*freeItem)(T item);



```
#### Examples
```c
/*In a header file*/
typedef struct student {
  char *name;
  int age;
}student, *pStudent;
Declare_CArrayList(pStudent)
```
```c
/*In a source file*/
Define_CArrayList(pStudent)

int comparePStudent(const pStudent *t1, const pStudent *t2) {
  pStudent p1 = *t1, p2 = * t2;
  return strcmp(p1->name, p2->name);
}
int equalsPStudent(const pStudent *t1, const pStudent *t2) {
  pStudent p1 = *t1, p2 = * t2;
  return strcmp(p1->name, p2->name) == 0 && p1->age == p2->age;
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
  ArrayList(pStudent) list = NewArrayList(pStudent)();
  list->compare = comparePStudent;
  list->equals = equalsPStudent;
  list->freeItem = freePStudent;
  m(list)->pushBack(newStudent("Bob", 20));
  m(list)->pushBack(newStudent("Alice", 19));
  m(list)->insert(newStudent("Eve", 21), 1);
  m(list)->sort();
  for(int i = 0; i < list->size; i++) {
    printf("%s %d\n",list->arr[i].name, list->arr[i].age);
  }
  return 0;
}

  
```
## HashMap
header file: ```CHashMap.h```
#### APIs
##### Declare and Define
```c
Declare_CHashMap(K, V) // Declare class HashMap<K, V> (an HashMap whose keys have type K and values has type V)

Define_CHashMap(K, V) // Define class HashMap<K, V>
```
##### Type Names and Constructor
```c

HashMap(K, V) //type name of an pointer to an HashMap<K, V> instance

HashMap(K, V) NewHashMap(K, V)(unsigned int(*hash_function)(K)); //prototype of the constructor of HashMap<K, V>

HashMapEntry(K, V) //type name of an pointer to an HashMapEntry<K, V> instance
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

//make a shallow copy of the hashmap
HashMap(K, V) clone();

/* return the head of the circular double linked list
**** do not modify anything (except the values in the key-value pair)
otherwise the hash map can't work appropriately ****
	the first item is at head->next
	the last item is at head->prev
	when there is 0 item, head->next = head->prev = head;
*/
HashMapEntry(K, V) (*enumerate)();

//free the hashmap instance
void free();
```
##### Public Members
```c
size_t size; // number of items

HashMapEntry(K, V) head; //head of circular double linked list of items

unsigned int (*h)(K k); //hash function of key

int (*equals)(const K *k1, const K *k2); //equals() function for keys

void (*freeKey)(K key); //free() function for keys

void (*freeValue)(V value); //free() function for values
```
#### Examples
```c
/*In a header file*/

typedef char *string;

Declare_CHashMap(string, int)
```
```c
/*In a source file*/

Define_CHashMap(string, int)

string String(const char *s) {
	int sz = strlen(s);
	string str = (string)malloc(sz + 1);
	strcpy(str, s);
	return str;
}

int str_eq(const string *p1, const string *p2) {
	return strcmp(*p1, *p2) == 0;
}

int main() {
	HashMap(string, int) hashmap = NewHashMap(string, int)(hashString);
	hashmap->equals = str_eq;
	hashmap->freeKey = free;
	m(hashmap)->put(String("abc"), 10);
	m(hashmap)->put(String("qfdv"), 12);
	m(hashmap)->put(String("hello"), -22);
	m(hashmap)->put(String("hashmap"), 0);
	printf("%d\n", m(hashmap)->get("hello"));
	HashMapEntry(string, int) head = m(hashmap)->enumerate(), p = head->next;
	while (p != head) {
		printf("%s %d\n", p->key, p->value);
		p = p->next;
	}
	p = head->prev;
	while (p != head) {
		printf("%s %d\n", p->key, p->value);
		p = p->prev;
	}
	return 0;
}
```
## PriorityQueue
header file: ```CPriorityQueue.h```
#### APIs
##### Declare and Define
```c
Declare_CPriorityQueue(T) //declare class PriorityQueue<T> (an PriorityQueue whose items have type T)

Define_CPriorityQueue(T) //define class PriorityQueue<T>
```
##### Type Name and Constructor
```c
PriorityQueue(T) //type name of a pointer to an PriorityQueue<T> instance
  
/* 
   prototype of the constructor of ArrayList<T>
   Note that the comparator should returns a positive number when t1 has a higher priority then t2. 
   (And 0 when equal, negative when less) 
*/
PriorityQueue(T) NewPriorityQueue(T)(int (*comparator)(const T *t1, const T *t2)); 

```
##### Methods
```c
//add an item into the priority queue
void add(T elem);

//clear all items in the queue
void clear();

//returns 0 if the queue does not contain the elem. Otherwise returns a value that is not 0.
int contains(T elem);

/* Return an Optional(T) struct that contains the item with the highest priority in the queue without removing it from the queue. 
   For example, 
   Optional(T) t = m(pq)->peek();
   t.valid; //a boolean value that indicates whether t.value field has a valid value.
   t.value; //refers to the item with type T
   When the queue is empty, t.valid is set to 0.
   When the queue is not empty, t.valid is set to 1, and t.value contains the value of the item that has the highest priority.
*/
Optional(T) peek();

/* Same as peek(), except this method will remove the item that has the higher priority from the queue. */
Optional(T) poll();

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
Declare_CPriorityQueue(word)
```
```c
/*In a source file*/
Define_CPriorityQueue(word)

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
  
```
