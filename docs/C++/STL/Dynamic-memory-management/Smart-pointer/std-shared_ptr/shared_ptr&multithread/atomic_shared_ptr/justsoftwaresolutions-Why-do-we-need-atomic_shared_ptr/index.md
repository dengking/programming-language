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

If one thread is traversing the list, and another is removing the first element, there is a potential for a race.

1、Thread X reads the `head` pointer for the list and gets a pointer to `A`.

2、Thread Y removes `A` from the list and deletes it.

3、Thread X tries to access the `data` for node `A`, but node `A` has been deleted, so we have a dangling pointer and undefined behaviour.

How can we fix it?

The first thing to change is to make all our `std::unique_ptr`s into `std::shared_ptr`s, and have our traversal function take a `std::shared_ptr` copy rather than using a raw pointer. That way, node `A` won't be deleted immediately, since our traversing thread still holds a reference.

