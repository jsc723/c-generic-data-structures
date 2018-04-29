# c-generic-data-structures
some common data structures implemented in **C** (more specifically, C99), supporting generic types.
And the APIs are provided in a objective orienting style.
I'm still working on it. But the ArrayList and HashMap should be usable.
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
```c
Declare_CArrayList(T) //declare an ArrayList with type T (denoted as ArrayList<T>)

Define_CArrayList(T) //define an ArrayList with type T

ArrayList(T) //Define a pointer to an ArrayList<T> instance
  
NewArrayList(T) //constructor

```
```c
/*
  append the item at the end of the array,
  it returns the pointer to its virtual function table, the same thing returnd by m(), so that you can use it 
  to call another method.
  This is just for convenience. If you don't know what it is, just treat pVFTB(ArrayList<T>) as void.
*/
pVFTB(ArrayList<T>) pushBack(T item); 
  
//append all n items at the end of the array
pVFTB(ArrayList<T>) pushAll(T *items, int n); 
  
//append all items in the otherList to the end of the array
pVFTB(ArrayList<T>) expend(ArrayList<T> * otherList); 

//insert the item at the index
pVFTB(ArrayList<T>) insert(T item, int index); 
  
//pop the last item
T popBack();

//pop the item at index
T popAt(int index);

//find the index of item, if the item does not exist, returns -1.
int indexOf(T item); 

//create a sublist of the list with the indecies [start, end). 
ArrayList<T> *subList(int start, int end); 

//sort the list using merge sort, must set the comparater first (see the next session)
pVFTB(ArrayList<T>) sort();

// apply map_func to every items in the array
pVFTB(ArrayList<T>) map(void (*map_func)(T *t));

// returns reduce_func(acc, reduce_func(&item0, reduce_func(&item1, ...)))
pVFTB(ArrayList<T>) reduce(void (*reduce_func)(T *t1, T *t2), T *acc);

// free the arraylist
void free();
  
```

#### Public Members
```c
T *arr; //the array of the items

size_t size; //number of items

//the comparater of the items, you must set this if you want to use sort
int (*compare)(const T *t1, const T *t2); 

/*
  check if two items are equal, there is a default value that compares two items byte by byte.
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
coming soon
## PriorityQueue
coming soon
