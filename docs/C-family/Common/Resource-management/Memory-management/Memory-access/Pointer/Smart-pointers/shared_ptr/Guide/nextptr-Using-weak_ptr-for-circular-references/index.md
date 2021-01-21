# nextptr [shared_ptr - basics and internals with examples](https://www.nextptr.com/tutorial/ta1358374985/shared_ptr-basics-and-internals-with-examples)

## **1. Overview**

The C++11 *`std::shared_ptr<T>`* is a shared ownership smart pointer type. Several *`shared_ptr`* instances can share the management of an object's lifetime through a common *control block*. The managed object is deleted when the last owning *`shared_ptr`* is destroyed (or is made to point to another object). Memory management by *`shared_ptr`* is deterministic because the timing of a managed object's destruction is predictable and in the developer's control. Hence, *`std::shared_ptr`* brings deterministic automatic memory management to C++, without the overhead of garbage collection. Here is a basic example of *`shared_ptr`*:

```C++
//some struct
struct Some {
 int x;
};

void useless(std::shared_ptr<Some> p) {
 //Change the underlying object
 p->x = 20;
}

void spam() {

 //Create/initialize shared_ptr<Some>
 auto one = std::shared_ptr<Some>(new Some());
 //Another shared_ptr<Some> pointing nowhere 
 std::shared_ptr<Some> two;

 //Change the underlying object
 one->x = 10;
 //Read through shared_ptr
 std::cout << "x: " << one->x << "\n"; //x: 10

 //Pass to a function by value. This increases the ref count.
 useless(one);

 //Underlying object is changed
 std::cout << "x: " << one->x << "\n"; //x: 20

 //Assign to another shared_ptr
 two = one;

 //'one' and 'two' are pointing to the same object
 std::cout << std::boolalpha << (one.get() == two.get()) << "\n"; //true

 /*  On Return:
 1. 'one' and 'two' are destroyed
 2.  Ref count reaches zero
 3. 'Some' is destroyed */
}
```

This article is specific to general usage and internals of *`shared_ptr`*. It does not cover the fundamentals of smart pointers and assumes that the reader is familiar with those. Having looked at the basic usage, let's move on to the internals of *`shared_ptr`* that make it work.

## **2. Internals**

In a typical implementation, a *`shared_ptr`* contains only two pointers: 

1、a raw pointer to the managed object that is returned by *get()*, and 

2、a pointer to the control block.

A *`shared_ptr`* control block at least includes 

1、a pointer to the managed object or the object itself, 

2、a reference counter, and 

> NOTE: reference counter是在control block中，而不是在object中

3、a weak counter. 



And depending on how a *shared_ptr* is initialized, the control block can also contain other data, most notably, a deleter and an allocator. The following figure corresponds to the example in the previous section. It shows the conceptual memory layout of the two *`shared_ptr`* instances managing the object:

![shared_ptr control block](https://cdn.nextptr.com/images/uimages/ST5xPgtrtB0ZluZibn6rSw3p.png)

------

Next, we talk about the details of the most relevant parts of the **control bloc**k. You would see that the memory layout of *shared_ptr* can deviate(脱离) from the above illustration depending on how it is constructed.

### **2.1. Pointer to Managed Object (or Managed Object)**

A control block contains a pointer to the managed object, which is used for deleting the object. One interesting fact is that the managed pointer in the control block could be different in type (and even value) from the raw pointer in the *shared_ptr*. This leads to a few fascinating use cases. In the following example, the types of the raw pointer and the managed pointer are different, but they are compatible and have the same values:

```C++
//A shared_ptr<void> managing an int
//The raw pointer is void*
auto vp = std::shared_ptr<void>(new int()); //OK
//However, we can't do much with 'vp'    

//Another example
//Inheritance with no virtual destructor 
struct A { 
 //stuff..
 ~A() { std::cout << "~A\n"; } //not virtual
};
struct B : A { 
 //stuff..
 ~B() { std::cout << "~B\n"; } //not virtual
};

//shared_ptr<A> managing a B object
//raw pointer is A* and managed pointer is B*
auto pa = std::shared_ptr<A>(new B()); //OK

pa.reset(); //Calls B's destructor
```

The inheritance example above is rather contrived(人为的、不自然的). It shows that despite the destructor being not *virtual*, the correct derived class (*B*) destructor is invoked when the base class (*A*) *shared_ptr* is *reset*. That works because the control block is destroying the object through `*B`**, not through the raw pointer `*A`**. Nevertheless, the destructor should be declared *virtual* in the classes that are meant to be used polymorphically. This example intends to merely show how a *shared_ptr* works.

There is an even more exotic(奇异的) *aliasing constructor* of *shared_ptr* that can initialize a *shared_ptr* from a raw pointer and an unrelated *shared_ptr*. Consequently, an aliasing constructor can produce a *shared_ptr* that shares the management of one object but points to another object (usually a subobject of the managed object). For instance:

```C++
struct Yolk { };
struct White { };
struct Egg {
 White w;
 Yolk y;
};

auto ep = std::shared_ptr<Egg>(new Egg());

//Aliasing constructor to construct shared_ptr<Yolk>
//yp shares ownership with ep but points to subobject ep->y
auto yp = std::shared_ptr<Yolk>(ep, &ep->y);     
```

The in-depth treatment of aliasing constructor deserves(应得) its own space. I encourage you to check out "[Aliasing constructed shared_ptr as key of map or set](https://www.nextptr.com/question/qa1355324734/aliasing-constructed-shared_ptr-as-key-of-map-or-set)" for a more persuasive use case.

There is more discussion about the managed object pointer in the 'Deleter' section below when we talk about the *type erasure*.

Before we delve(钻研) into more intricate(复杂的) details, let's talk about the *std::make_shared*. We mentioned above that the control block could either contain a pointer to the managed object or the object itself. The control block is dynamically allocated. Constructing the managed object in-place within the **control block** can avoid the two separate memory allocations for the object and the control block, resulting in an uncomplicated(简单的) control block and better performance. The *std::make_shared* is a preferred way to construct a *shared_ptr* because it builds the managed object within the control block:

```
auto sp = std::make_shared<std::string>("Hello"); //Creates the std::string in the control block 
std::cout << *sp << "\n"; //Hello
```

### **2.2. Reference Counter**

The reference counter, which is incremented and decremented atomically, tracks the number of owning *shared_ptr* instances. The reference count increases as a new *shared_ptr* is constructed, and it decreases as an owning *shared_ptr* is destroyed. One exception to that is the reference count is left unchanged when a *shared_ptr* is *moved* because the move-constructor transfers the ownership from the source to the newly constructed *shared_ptr*. The managed object is disposed of when the reference count reaches zero.

> NOTE: 是否有copy constructor？

*std::shared_ptr* ownership is also affected by the copy and move assignment operators. The copy assignment operator decreases the reference count of the destination (LHS) *shared_ptr* and increases the reference count of the source (RHS) *shared_ptr*. Whereas, the move assignment operator decreases the reference count of the destination (LHS) but does not change the reference count of the source (RHS).

> NOTE: 为什么这样设计？

Let's explore another example that exhibits the lifecycle of an object managed by a few *shared_ptr* instances. As you go through the code, refer the following figure for the different stages:

```c++
void baz(std::shared_ptr<int> p3) {
 //Stage 3
 std::cout << "@3 Ref Count: " << p3.use_count() << "\n"; //@3 Ref Count: 3
}

int main() {
 //Create a shared_ptr<int>
 auto p1 = std::make_shared<int>(0);
 //Stage 1
 std::cout << "@1 Ref Count: " << p1.use_count() << "\n"; //@1 Ref Count: 1

 { // Block
  //Create copy
  auto p2 = p1;
  //Stage 2
  std::cout << "@2 Ref Count: " << p2.use_count() << "\n"; //@2 Ref Count: 2
  //Will create another copy
  baz(p2);
  //Stage 4
  std::cout << "@4 Ref Count: " << p2.use_count() << "\n"; //@4 Ref Count: 2
 }

 //Stage 5
 std::cout << "@5 Ref Count: " << p1.use_count() << "\n"; //@5 Ref Count: 1
 //reset
 p1.reset();
 //Stage 6
 std::cout << "@6 Ref Count: " << p1.use_count() << "\n"; //@6 Ref Count: 0
 return 0;
}
```

------

![shared_ptr object lifecycle](https://cdn.nextptr.com/images/uimages/Jux0ZcBOJb2Stbf0X_w-5kjF.png)

------

### **2.3. Weak Counter**

A control block also keeps the count of *weak_ptr* associated with it in a weak counter. An *std::weak_ptr* is a smart pointer that serves as a weak reference to an *std::shared_ptr* managed object. When a *weak_ptr* is created from a *shared_ptr*, it refers to the same **control block** but does not share the ownership of the managed object. It is not possible to directly access the managed object through a *weak_ptr*. A *weak_ptr* must be copied to a *shared_ptr* to acquire access to the managed object.

The following multithreaded example shows how a *shared_ptr* can be created from a *weak_ptr* as long as the managed object is alive. A reader thread periodically tries to acquire a *shared_ptr<`std::atomic_int`>* from a *weak_ptr<`std::atomic_int`>* and logs the value. If the reader thread cannot acquire a *shared_ptr* in an iteration, it exits. A writer thread periodically changes the *shared_ptr* managed `std::atomic_int` value a few times and exits. When the writer thread exits, the *shared_ptr* held by it is destroyed, and the reader thread can no longer get a *shared_ptr* from its *weak_ptr*, which makes the reader thread to also exit. The program terminates when both the threads exit:

```c++
int main() {

 auto sp = std::shared_ptr<std::atomic_int>(new std::atomic_int());

 //Reader
 //A weak_ptr is created and captured (syntax requires requires c++14).
 std::thread r([wp = std::weak_ptr<std::atomic_int>(sp)]() { //weak_ptr created. ref count: 1, weak count: 1
  while(true) {
   //Acquire a shared_ptr through lock() 
   if(auto p = wp.lock()) {
    //shared_ptr acquired. ref count is 1 or 2
    std::cout << *p << "\n";
   } else {
    //shared_ptr could not be acquired. ref count 0
    break;
   }
   //sleep
   std::this_thread::sleep_for(std::chrono::seconds(1));
  }
 });

 //Writer
 //The shared_ptr is moved and captured so the ref count stays 1
 //If the shared_ptr is copied instead of moved, this program will never 
 //     end because the reader would never exit (try that!).
 //Move in capture clause requires c++14
 std::thread w([mp = std::move(sp)]() { //shared_ptr moved. ref count: 1
  for(int i=1; i<=5; i++) {
   *mp = i; //change managed object
   std::this_thread::sleep_for(std::chrono::seconds(1));
  }
 });

 //Join the threads.
 w.join();
 r.join();
 return 0;
}
```

The weak count is the number of existing *weak_ptr*. The weak count does not play any role in deciding the lifetime of the managed object, which is deleted when the reference count reaches zero. However, the control block itself is not deleted until the weak count also reaches zero.

> NOTE: smart pointer是一种更加高级的控制方式，无需由programmer进行显式的停止，而是依赖于reference counting机制

### **2.4. Deleter**

When a *shared_ptr* is initialized with a pointer, its control block contains a deleter function object (or function pointer), which is invoked to destroy the managed object. If a custom deleter is not provided to the *shared_ptr* constructor, a default deleter (e.g., *std::default_delete*) is used that calls the `delete` operator.

The deleter is *type-erased* for two reasons. First, a deleter is an optional argument to a *shared_ptr* constructor, not a template parameter. Hence, a *shared_ptr's* type is deleter agnostic. Second, a deleter is a function object (or a function pointer), e.g., *function<`void(T\*)`>*. This indirection makes *shared_ptr* independent of the details of how the managed object is deleted. This loose-coupling of *shared_ptr* with the deleter makes it quite flexible. For instance, in the example below, a `vector<shared_ptr<T>>` can be in its compilation unit entirely oblivious to the knowledge of how an incomplete type *T* is deleted:

```C++
//A compilation unit
class Thing; //only declaration. 'Thing' is incomplete here.
void foo(std::shared_ptr<Thing> thing) {
 std::vector<std::shared_ptr<Thing>> vec;
 vec.push_back(thing);
}
//+------------------- + ---------------------------+
//Different compilation unit
struct Thing {
 //stuff..
};
void foo(std::shared_ptr<Thing>);

int main() {
 //Default deleter
 foo(std::shared_ptr<Thing>(new Thing()));
 //Custom lambda deleter 
 foo(std::shared_ptr<Thing>(new Thing(), [](Thing* p) {
  delete p;
 }));
}
```

### **2.5. Allocator**

The control block itself is allocated by an allocator that must satisfy the [Allocator](https://en.cppreference.com/w/cpp/named_req/Allocator) requirements. When a custom allocator is not provided, the *std::allocator* is used that dynamically allocates the control block. The control block keeps a copy of the allocator, which is *type-erased* like the deleter. There are two ways to use a custom allocator. One is to provide a custom allocator when initializing the *shared_ptr* with a managed object pointer, as shown below. Note that this *shared_ptr* constructor also requires a deleter:

```
struct SomeData { };
//Allocator must be defined
auto sp = std::shared_ptr<SomeData>(new SomeData(), std::default_delete<SomeData>(), Allocator<SomeData>());
```

Another way to use a custom allocator is to utilize *std::allocate_shared* that can construct the managed object in-place within a custom allocated control block. Therefore, the *std::allocate_shared* is like *std::make_shared*, except that it takes a custom allocator:

```
auto sp = std::allocate_shared<SomeData>(Allocator<SomeData>());
```

## **3. Conclusion**

The *std::shared_ptr<`T`>* is a handy yet straightforward utility. But under its simplicity lie extensive details that make it work. Dereferencing a *shared_ptr* is nearly as fast as a raw pointer, but constructing or copying a *shared_ptr* is certainly more expensive. Nonetheless, for most applications, this cost is reasonable for automatic memory management.



# nextptr [Using weak_ptr for circular references](https://www.nextptr.com/tutorial/ta1382183122/using-weak_ptr-for-circular-references)

## **What is weak_ptr?**

An *std::weak_ptr* is a non-owning smart pointer that maintains a weak reference to an *std::shared_ptr* managed object. Multiple *shared_ptr* instances can share the ownership of a managed object. The managed object is deleted when the last owning *shared_ptr* is destroyed. Therefore, a *shared_ptr* is a strong reference to a managed object. On the other hand, a *weak_ptr* is a weak reference that does not control the lifetime of a managed object but merely acts as a limited handle, from which a strong reference (*shared_ptr*) can be acquired when needed.

Let's look at an example of *weak_ptr*:

```C++
std::thread observer;

void observe(std::weak_ptr<int> wp) {
 //Start observer thread
 observer = std::thread([wp](){
  while(true) {
   std::this_thread::sleep_for(std::chrono::seconds(1));

   //Try acquiring a shared_ptr from weak_ptr
   if(std::shared_ptr<int> p = wp.lock()) {
    //Success
    std::cout << "Observing: " << *p << "\n";
   } else {
    //The managed object is destroyed. 
    std::cout << "Stop\n";
    break;
   }
  }
 });
}

int main() {

 { //Block Start  
  auto sp = std::shared_ptr<int>(new int());
  //Create a weak_ptr<int> from sp for observing
  observe(sp);  
  //Wait few seconds
  std::this_thread::sleep_for(std::chrono::seconds(5));

  //shared_ptr is destroyed and the
  // managed object is deleted when
  // block ends
 }


 //Wait for the observer thread to end
 observer.join();
 return 0;
}
```

In the contrived(人为的) example above, an observer thread periodically checks the value of a *shared_ptr* managed object. The observer thread, however, does not own the object through a *shared_ptr*. It has a *weak_ptr* and periodically tries to acquire a temporary *shared_ptr* from it to access the object. The observer thread quits when it fails to obtain a *shared_ptr*.

The *main* thread starts the observer thread and provides a *weak_ptr<`int`>*. It lets the original owning *shared_ptr<`int`>* live for a few seconds by sleeping. When the original *shared_ptr* is destroyed, the managed object is deleted, and the observer thread quits because it cannot acquire a *shared_ptr*.

As shown above, it is not possible to directly access the managed object through a *weak_ptr*. A *weak_ptr* has to be converted to a *shared_ptr* to access the managed object. The conversion fails if the managed object gets deleted.

There are a few situations where *weak_ptr* can be quite useful. One of them is to avoid the circular references of *shared_ptr*. We will look at that in the next section.

## **Circular References**

One of the biggest concerns dealing with the raw pointers is that sometimes it is hard to ensure that a raw pointer is not dangling or valid. **Automatic memory management** by *shared_ptr* leads to a safer and easier to maintain code. As long as a component or function holds a *shared_ptr*, the object managed by the *shared_ptr* stays in memory.

However, there is a caveat(警告) related to the use of *shared_ptr* when it comes to cyclic dependencies or circular references. Two objects are said to have circular references when they hold references to each other. Circular references ordinarily(通常) manifest(展现) in event-driven applications. For instance, in the following code, classes *Back* and *Forth* communicate with one another through *shared_ptr* references to each other:

```C++
//Forward declaration
struct Forth;

struct Back {     
 //stuff...
 //Sends messages to Forth...
 std::shared_ptr<Forth> forth;
};

struct Forth {
 //stuff...
 //Sends messages to Back...
 std::shared_ptr<Back> back;
};
```

Using *shared_ptr* for cyclic dependency causes the lifetime of *Back* and *Forth* to depend on each other. The instances of *Back* and *Forth* can stay in memory and cause memory-leak even when no other part of the application can reach them because they are both holding the *shared_ptr* to each other.

Following illustration shows the memory-leak when all other strong references to both *Back* and *Forth* are destroyed, and no part of the application can reach them:

![Circular Strong References](https://cdn.nextptr.com/images/uimages/bvpOc_eZySeZuKwz6hRYwTop.png)

## **Dealing with Circular References**

There are several ways to avoid the memory-leak mentioned above, and depending on an application, the workarounds could be very involved and ugly. For instance, in some cases, we might be able to change one of the references (e.g., the reference to *Back* in *Forth*) to a raw pointer. By doing that, we are letting only one class control the lifetime of the other and avoid a memory leak. However, that solution is too specific and is not applicable in those situations where the lives of *Back* and *Forth* should be entirely independent of each other. It is ideal to use weak references (*weak_ptr*) in these circumstances where classes need to have cyclic links without controlling the lifetime of each other.

Let's take a more realistic example where we use weak references to avoid circular references. In an event-driven application (e.g., a UI application), there are sources of events and listeners that consume events. A *Listener* is registered with a *Source* for consuming events. An ostensibly simple approach is that a *Source* keeps a strong reference to a *Listener* to dispatch events:

```c++
struct Event {
 //..
};

class Listener {
public:
 void onEvent(Event e) {
  //Handle event from Source
 }
 //...
};

class Source {
public: 
 void dispatchEvent(Event e) {
  if(listener)
   listener->onEvent(e);
 }

 void 
 registerListener(const std::shared_ptr<Listener>& lp) {
  listener = lp;
 }
 //...
private:
 //Strong reference to Listener
 std::shared_ptr<Listener> listener;
};
```

But the above design causes the lifetime of a *Listener* to be influenced by the lifetime of a *Source*. The situation could be further exacerbated if a *Listener* is a big object and stays in memory for longer than it should. The existence of a *Source* and a *Listener* should be mutually independent, and only their respective holders should control their lifetimes.

It is tempting to use cyclic references here between *Source* and *Listener* so that they both can explicitly detach from one another when the time comes. But that would require both *Source* and *Listener* to be expressly disposed of by their holders, which might not be feasible or be very tricky at best. A better way is to use a weak reference from *Source* to *Listener*, as shown below:

```c++
class Source {
public: 
 void dispatchEvent(Event e) {
  //Acquire strong ref to listener
  if(auto listener = weakListener.lock()) {
    listener->onEvent(e);
  } else {
    //Handle if required
  }
 }

 void 
 registerListener(const std::shared_ptr<Listener>& lp) {
  weakListener = lp;
 }
 //...
private: 
 //Weak reference to Listener
 std::weak_ptr<Listener> weakListener;
};
```

Following illustration shows the relationships between *Source*, *Listener*, and their respective holders:

![Source Listener Weak Reference](https://cdn.nextptr.com/images/uimages/W-gpKUpFVYzgbdzctByn2ApW.png)

By having a weak reference to a *Listener*, we have separated the lifetimes of *Source* and *Listener*. A *Source* converts the *weak_ptr<Listener>* to a temporary *shared_ptr<Listener>* on-demand when it has to dispatch an event. When a *Listener* is destroyed, its *Source* cannot forward the events, and that can be handled appropriately depending on the application.

## **Conclusion**

A reference type that guarantees the existence of the referred object is of paramount importance for writing a safer code. *std::shared_ptr* is a strong reference that provides this assurance. However, the firm control over the lifetime of an object by *shared_ptr* is not desirable in some cases. In those cases, a *weak_ptr* that can be converted to a strong reference on-demand is preferable for a more straightforward design.