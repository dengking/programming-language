# 6.3.2 Writing a thread-safe list using locks

A list is one of the most basic data structures, so it should be straightforward to write a thread-safe one, shouldn’t it? Well, that depends on what facilities you’re after, and you need one that offers iterator support, something I shied away from adding to your map on the basis that it was too complicated. The basic issue with STL-style iterator support is that the iterator must hold some kind of reference into the internal data structure of the container. If the container can be modified from another thread, this reference must somehow remain valid, which essentially requires that the **iterator** hold a lock on some part of the structure. Given that the lifetime of an STL-style iterator is completely outside the control of the container, this is a bad idea.

> NOTE:
>
> 一、上面描述的本质是iterator invalidation。
>
> 

The alternative is to provide iteration functions such as `for_each` as part of the container itself. This puts the container squarely(直接的) in charge of the iteration and locking, but it does fall foul of(抵触、违背) the deadlock avoidance guidelines from chapter 3. In order for `for_each` to do anything useful, it must call user-supplied code while holding the **internal lock**. Not only that, but it must also pass a reference to each item to this user-supplied code in order for the user-supplied code to work on this item. You could avoid this by passing a copy of each item to the user-supplied code, but that would be expensive if the data items were large. So, for now you’ll leave it up to the user to ensure that they don’t cause **deadlock** by acquiring locks in the user-supplied operations and don’t cause data races by storing the references for access outside the locks. In the case of the list being used by the lookup table, this is perfectly safe, because you know you’re not going to do anything naughty.



The basic idea with fine-grained locking for a linked list is to have one mutex per node. If the list gets big, that’s a lot of mutexes! The benefit here is that operations on separate parts of the list are truly concurrent: each operation holds only the locks on the nodes it’s actually interested in and unlocks each node as it moves on to the next. The next listing shows an implementation of just such a list.

## Listing 6.13 A thread-safe list with iteration support



```c++
#include <mutex>
#include <memory>

template <typename T>
class threadsafe_list
{
    struct node // 1
    {
        std::mutex m;
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
        node() : next() // 2
        {
        }
        node(T const &value) : data(std::make_shared<T>(value)) // 3
        {
        }
    };
    node head;

public:
    threadsafe_list()
    {
    }
    ~threadsafe_list()
    {
        remove_if([](node const &)
                  { return true; });
    }
    threadsafe_list(threadsafe_list const &other) = delete;
    threadsafe_list &operator=(threadsafe_list const &other) = delete;
    void push_front(T const &value)
    {
        std::unique_ptr<node> new_node(new node(value)); // 4
        std::lock_guard<std::mutex> lk(head.m);
        new_node->next = std::move(head.next); // 5
        head.next = std::move(new_node);       // 6
    }
    template <typename Function>
    void for_each(Function f) // 7
    {
        node *current = &head;
        std::unique_lock<std::mutex> lk(head.m);       // 8
        while (node *const next = current->next.get()) // 9
        {
            std::unique_lock<std::mutex> next_lk(next->m); // 10
            lk.unlock();                                   // 11
            f(*next->data);                                // 12
            current = next;
            lk = std::move(next_lk); // 13
        }
    }
    template <typename Predicate>
    std::shared_ptr<T> find_first_if(Predicate p) // 14
    {
        node *current = &head;
        std::unique_lock<std::mutex> lk(head.m);
        while (node *const next = current->next.get())
        {
            std::unique_lock<std::mutex> next_lk(next->m);
            lk.unlock();
            if (p(*next->data)) // 15
            {
                return next->data; // 16
            }
            current = next;
            lk = std::move(next_lk);
        }
        return std::shared_ptr<T>();
    }
    template <typename Predicate>
    void remove_if(Predicate p) // 17
    {
        node *current = &head;
        std::unique_lock<std::mutex> lk(head.m);
        while (node *const next = current->next.get())
        {
            std::unique_lock<std::mutex> next_lk(next->m);
            if (p(*next->data)) // 18
            {
                std::unique_ptr<node> old_next = std::move(current->next);
                current->next = std::move(next->next); // 19
                next_lk.unlock();
            } // 20
            else
            {
                lk.unlock(); // 21
                current = next;
                lk = std::move(next_lk);
            }
        }
    }
};
```

> NOTE:
>
> 一、显然当需要read、write `head`的时候，是都需要加锁的
>
> 二、对于`push_front` ，它仅仅需要修改head，因此只需要lock head即可。
>
> 三、对于`for_each`，它需要为每个access的node都加上lock

The `threadsafe_list<>` from listing 6.13 is a singly linked list, where each entry is a node structure 1. A default-constructed node is used for the head of the list, which starts with a `NULL` `next` pointer 2. New nodes are added with the `push_front()` function; first a new node is constructed 4, which allocates the stored data on the heap 3, while leaving the next pointer as `NULL`. You then need to acquire the lock on the mutex for the `head` node in order to get the appropriate next value 5 and insert the node at the front of the list by setting `head.next` to point to your new node 6. So far, so good:
you only need to lock one mutex in order to add a new item to the list, so there’s no risk of **deadlock**. Also, the slow memory allocation happens outside the lock, so the lock is only protecting the update of a couple of pointer values that can’t fail. On to the iterative functions.

### `for_each()`

First up, let’s look at `for_each()` 7. This operation takes a `Function` of some type to apply to each element in the list; in common with most standard library algorithms, it takes this function by value and will work with either a genuine function or an object of a type with a function call operator. In this case, the function must accept a value of type `T` as the sole parameter. Here’s where you do the **hand-over-hand locking**. To start with, you lock the mutex on the `head` node 8. It’s then safe to obtain the pointer to the next node (using `get()` because you’re not taking ownership of the pointer). If that pointer isn’t `NULL` 9, you lock the mutex on that node 10 in order to process the data. Once you have the lock on that node, you can release the lock on
the previous node 11 and call the specified function 12. Once the function completes, you can update the current pointer to the node you just processed and move the ownership of the lock from `next_lk` out to `lk` 13. Because `for_each` passes each data item directly to the supplied Function, you can use this to update the items if necessary or copy them into another container, or whatever. This is entirely safe if the function is well behaved, because the mutex for the node holding the data item is held across the call.