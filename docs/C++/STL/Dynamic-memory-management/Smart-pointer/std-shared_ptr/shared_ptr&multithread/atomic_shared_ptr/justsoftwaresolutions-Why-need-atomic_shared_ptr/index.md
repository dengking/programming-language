# justsoftwaresolutions [Why do we need atomic_shared_ptr?](https://www.justsoftwaresolutions.co.uk/threading/why-do-we-need-atomic_shared_ptr.html) 

One of the new class templates provided in the upcoming [Concurrency TS](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4501.html) is [`atomic_shared_ptr`](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4501.html#atomic), along with its counterpart `atomic_weak_ptr`. As you might guess, these are the `std::shared_ptr` and `std::weak_ptr` equivalents of `std::atomic<T*>`, but why would one need them? Doesn't `std::shared_ptr` already have to synchronize the reference count?

## `std::shared_ptr` and multiple threads

`std::shared_ptr` works great in multiple threads, *provided each thread has its own copy or copies*. In this case, the changes to the reference count are indeed synchronized, and everything just works, provided of course that what you do with the shared data is correctly synchronized.



```c++
#include <thread>
#include <memory>
#include <chrono>
#include <iostream>

using namespace std;

class MyClass
{
    int id_{0};

public:
    MyClass(int id) : id_(id) {}
    void do_stuff()
    {
        cout << "do_stuff:" << id_ << endl;
    }
};
void do_stuff_with(std::shared_ptr<MyClass> sp)
{
    sp->do_stuff();
}
void thread_func(std::shared_ptr<MyClass> sp)
{
    sp->do_stuff();
    std::shared_ptr<MyClass> sp2 = sp;
    do_stuff_with(sp2);
}
int main()
{
    std::shared_ptr<MyClass> sp(new MyClass{1});
    std::thread thread1(thread_func, sp);
    std::thread thread2(thread_func, sp);
    thread2.join();
    thread1.join();
}
```

In this example, you need to ensure that it is safe to call `MyClass::do_stuff()` and `do_stuff_with()` from multiple threads concurrently on the same instance, but the reference counts are handled OK.

![](./shared_ptr_separate_instances.png)

**The problems come when you try and share a single `std::shared_ptr` instance between threads.**

## Sharing a `std::shared_ptr` instance between threads

I could provide a trivial example of a `std::shared_ptr` instance shared between threads, but instead we'll look at something a little more interesting, to give a better feel for why you might need it.

Consider for a moment a simple singly-linked list, where each node holds a pointer to the next:



```c++
#include <thread>
#include <memory>
#include <chrono>
#include <iostream>
#include <functional>

using namespace std;

class MyClass
{
    int id_{0};

public:
    MyClass(int id) : id_(id) {}
    void do_stuff()
    {
        cout << "do_stuff:" << id_ << endl;
    }
};
class MyList
{
public:
    // constructors etc. omitted.
    struct Node
    {
        MyClass data;
        std::unique_ptr<Node> next;
        Node(int id) : data{id}
        {
        }
    };
    std::unique_ptr<Node> head;

    void traverse(std::function<void(MyClass &)> f)
    {
        Node *p = head.get();
        while (p)
        {
            f(p->data);
            p = p->next.get();
        }
    }
};

int main()
{
    MyList l;
    l.head = make_unique<MyList::Node>(1);
    l.head->next = make_unique<MyList::Node>(2);
    l.head->next->next = make_unique<MyList::Node>(3);
    l.traverse([](MyClass &data)
               { data.do_stuff(); }
               );
}
```

If we're going to access this from 2 threads, then we have a choice:

1、We could wrap the whole object with a mutex, so only one thread is accessing the list at a time, or

2、We could try and allow concurrent accesses.

For the sake of this article, we're going to assume we're allowing concurrent accesses.

Assuming the list is immutable, this is fine, but immutable lists are no fun! We want to **remove** items from the front of our list. What changes do we need to make to support that?

## Removing from the front of the list

If everything was single threaded, removing an element would be easy:

```c++
class MyList
{
    void pop_front()
    {
        Node *p = head.get();
        if (p)
        {
            head = std::move(p->next);
        }
    }
};
```

If the list is not empty, the new head is the next pointer of the old head. However, we've got multiple threads accessing this list, so things aren't so straightforward.

Suppose we have a list of 3 elements.

A -> B -> C

### Race condition

If one thread is traversing the list, and another is removing the first element, there is a potential for a race.

1、Thread X reads the `head` pointer for the list and gets a pointer to `A`.

2、Thread Y removes `A` from the list and deletes it.

3、Thread X tries to access the `data` for node `A`, but node `A` has been deleted, so we have a dangling pointer and undefined behaviour.

How can we fix it?

The first thing to change is to make all our `std::unique_ptr`s into `std::shared_ptr`s, and have our traversal function take a `std::shared_ptr` copy rather than using a raw pointer. That way, node `A` won't be deleted immediately, since our traversing thread still holds a reference.



```c++
#include <thread>
#include <memory>
#include <chrono>
#include <iostream>
#include <functional>

using namespace std;

class MyClass
{
    int id_{0};

public:
    MyClass(int id) : id_(id) {}
    void do_stuff()
    {
        cout << "do_stuff:" << id_ << endl;
    }
};

class MyList
{
public:
    // constructors etc. omitted.
    struct Node
    {
        MyClass data;
        std::shared_ptr<Node> next;
        Node(int id) : data{id}
        {
        }
    };
    std::shared_ptr<Node> head;

    void traverse(std::function<void(MyClass &)> f)
    {
        std::shared_ptr<Node> p = head; // traverse function hold a reference to shared data
        while (p)
        {
            f(p->data);
            p = p->next;
        }
    }
    void pop_front()
    {
        std::shared_ptr<Node> p = head; // pop_front function hold a reference to shared data
        if (p)
        {
            head = std::move(p->next);
        }
    }
    // constructors etc. omitted.
};

int main()
{
    MyList l;
    l.head = make_shared<MyList::Node>(1);
    l.head->next = make_shared<MyList::Node>(2);
    l.head->next->next = make_shared<MyList::Node>(3);

    std::thread thread1([&]()
                        { 
                            l.traverse([](MyClass &data)
                                     { 
                                        data.do_stuff(); 
                                     }); 
                        });
    std::thread thread2([&]()
                        { l.pop_front(); });
    thread2.join();
    thread1.join();
}
```

Unfortunately that only fixes that race condition. There is a second race that is just as bad.

## The second race condition

The second **race condition** is on `head` itself. In order to traverse the list, thread `X` has to read `head`. In the mean time, thread `Y` is updating `head`. This is the very definition of a *data race*, and is thus **undefined behaviour**.

We therefore need to do something to fix it.

We could use a mutex to protect `head`. This would be more fine-grained than a whole-list mutex, since it would only be held for the brief time when the pointer was being read or changed. However, we don't need to: we can use `std::experimental::atomic_shared_ptr` instead.



> NOTE:
>
> 一、使用mutex的方式: 
>
> ```c++
> #include <thread>
> #include <mutex>
> #include <memory>
> #include <chrono>
> #include <iostream>
> #include <functional>
> 
> using namespace std;
> 
> class MyClass
> {
>     int id_{0};
> 
> public:
>     MyClass(int id) : id_(id) {}
>     void do_stuff()
>     {
>         cout << "do_stuff:" << id_ << endl;
>     }
> };
> 
> class MyList
> {
> public:
>     // constructors etc. omitted.
>     struct Node
>     {
>         MyClass data;
>         std::shared_ptr<Node> next;
>         Node(int id) : data{id}
>         {
>         }
>     };
>     std::mutex m;
>     std::shared_ptr<Node> head;
> 
>     void traverse(std::function<void(MyClass &)> f)
>     {
>         std::lock_guard<std::mutex> guard(m);
>         std::shared_ptr<Node> p = head; // traverse function hold a reference to shared data
> 
>         while (p)
>         {
>             f(p->data);
>             p = p->next;
>         }
>     }
>     void pop_front()
>     {
>         std::lock_guard<std::mutex> guard(m);
>         std::shared_ptr<Node> p = head; // pop_front function hold a reference to shared data
>         if (p)
>         {
>             head = std::move(p->next);
>         }
>     }
>     // constructors etc. omitted.
> };
> 
> int main()
> {
>     MyList l;
>     l.head = make_shared<MyList::Node>(1);
>     l.head->next = make_shared<MyList::Node>(2);
>     l.head->next->next = make_shared<MyList::Node>(3);
> 
>     std::thread thread1([&]()
>                         { l.traverse([](MyClass &data)
>                                      { data.do_stuff(); }); });
>     std::thread thread2([&]()
>                         { l.pop_front(); });
>     thread1.join();
>     thread2.join();
> }
> ```
>
> 上述版本是能够正常运行的，但是它加锁的scope太大了
>
> 二、如何使用更加fine-gained lock呢？
>
> 



The implementation is allowed to use a mutex internally with `atomic_shared_ptr`, in which case we haven't gained anything with respect to performance or concurrency, but we *have* gained by reducing the maintenance load on our code. We don't have to have an explicit mutex data member, and we don't have to remember to lock it and unlock it correctly around every access to `head`. Instead, we can defer all that to the implementation with a single line change:

```c++
class MyList{
    std::experimental::atomic_shared_ptr<Node> head;
};
```

Now, the read from `head` no longer races with a store to `head`: the implementation of `atomic_shared_ptr` takes care of ensuring that the load gets either the new value or the old one without problem, and ensuring that the reference count is correctly updated.



Unfortunately, the code is still not bug free: what if 2 threads try and remove a node at the same time.

## Race 3: double removal

As it stands, `pop_front` assumes it is the only modifying thread, which leaves the door wide open for race conditions. If 2 threads call `pop_front` concurrently, we can get the following scenario:

1、Thread X loads `head` and gets a pointer to node A.

2、Thread Y loads `head` and gets a pointer to node A.

3、Thread X replaces `head` with `A->next`, which is node B.

4、Thread Y replaces `head` with `A->next`, which is node B.

So, two threads call `pop_front`, but only one node is removed. That's a bug.

The fix here is to use the ubiquitous(无处不在的) `compare_exchange_strong` function, a staple of any programmer who's ever written code that uses atomic variables.

```c++
class MyList{
    void pop_front(){
        std::shared_ptr<Node> p=head;
        while(p &&
            !head.compare_exchange_strong(p,p->next));
    }
};
```

If `head` has changed since we loaded it, then the call to `compare_exchange_strong` will return `false`, and reload `p` for us. We then loop again, checking that `p` is still non-null.

This will ensure that two calls to `pop_front` removes two nodes (if there are 2 nodes) without problems either with each other, or with a traversing thread.

Experienced lock-free programmers might well be thinking "what about the ABA problem?" right about now. Thankfully, we don't have to worry!

## What no ABA problem?

That's right, `pop_front` does not suffer from the ABA problem. Even assuming we've got a function that adds new values, we can never get a new value of `head` the same as the old one. This is an additional benefit of using `std::shared_ptr`: the old node is kept alive as long as one thread holds a pointer. So, thread X reads `head` and gets a pointer to node A. This node is now kept alive until thread X destroys or reassigns that pointer. That means that no new node can be allocated with the same address, so if `head` is equal to the value `p` then it really must be the same node, and not just some imposter that happens to share the same address.