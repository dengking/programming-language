# Requiring or Prohibiting Heap-based Objects

"Requiring"的含义是"要求"，"Prohibiting"的含义是"禁止"。

## More C++ Idioms [Requiring or Prohibiting Heap-based Objects](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Requiring_or_Prohibiting_Heap-based_Objects)



### Solution and Sample Code

#### Requiring heap-based objects

```c++
class HeapOnly {
  public:
    HeapOnly() {} 
    void destroy() const { delete this; }
  protected:
    ~HeapOnly() {}
};
HeapOnly h1;     // Destructor is protected so h1 can't be created globally
HeapOnly func()  // Compiler error because destructor of temporary is protected
{
  HeapOnly *hoptr = new HeapOnly; // This is ok. No destructor is invoked automatically for heap-based objects
  return *hoptr;
}
int main(void) {
  HeapOnly h2; // Destructor is protected so h2 can't be created on stack
}
```

#### Prohibiting heap-based objects

```c++
class NoHeap {
protected:
  static void * operator new(std::size_t);      // #1: To prevent allocation of scalar objects
  static void * operator new [] (std::size_t);  // #2: To prevent allocation of array of objects
};
class NoHeapTwo : public NoHeap {
};
int main(void) {
  new NoHeap;        // Not allowed because of #1
  new NoHeap[1];     // Not allowed because of #2
  new NoHeapTwo[10];  // Not allowed because of inherited protected new operator (#2).
}
```