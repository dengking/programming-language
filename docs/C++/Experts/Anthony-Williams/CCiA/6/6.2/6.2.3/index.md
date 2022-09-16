# 6.2.3 A thread-safe queue using fine-grained locks and condition variables

In listings 6.2 and 6.3 you have one protected data item (`data_queue`) and thus one mutex. In order to use finer-grained locking, you need to look inside the queue at its constituent(组成的) parts and associate one mutex with each distinct data item.

> NOTE:
>
> 一、简而言之: 每个元素一个lock

## *head* pointer

The simplest data structure for a queue is a singly linked list, as shown in figure 6.1. The queue contains a *head* pointer, which points to the first item in the list, and each item then points to the next item. Data items are removed from the queue by replacing the *head* pointer with the pointer to the next item and then returning the data from the old head.

![](./Figure-6.1-A-queue-represented-using-a-single-linked-list.png)



## *tail* pointer

Items are added to the queue at the other end. In order to do this, the queue also contains a *tail* pointer, which refers to the last item in the list. New nodes are added by changing the *next* pointer of the last item to point to the new node and then updating the tail pointer to refer to the new item. When the list is empty, both the head and tail pointers are NULL.

## Listing 6.4 A simple single-threaded queue implementation

The following listing shows a simple implementation of such a queue based on a cutdown version of the interface to the queue in listing 6.2; you have only one `try_pop()` function and no `wait_and_pop()` because this queue supports only single-threaded use.



```c++
template <typename T>
class queue
{
private:
    struct node
    {
        T data;
        std::unique_ptr<node> next;
        node(T data_) : data(std::move(data_))
        {
        }
    };
    std::unique_ptr<node> head; // 1
    node *tail; // 2

public:
    queue()
    {
    }

    queue(const queue &other) = delete;
    queue &operator=(const queue &other) = delete;
    std::shared_ptr<T> try_pop()
    {
        if (!head)
        {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> const res(
            std::make_shared<T>(std::move(head->data)));
        std::unique_ptr<node> const old_head = std::move(head);
        head = std::move(old_head->next);
        return res;
    }
    void push(T new_value)
    {
        std::unique_ptr<node> p(new node(std::move(new_value)));
        node *const new_tail = p.get();
        if (tail)
        {
            tail->next = std::move(p); // 4
        }
        else
        {
            head = std::move(p); // 5
        }
        tail = new_tail; // 6
    }
};
```

First off, note that listing 6.4 uses `std::unique_ptr<node>` to manage the nodes, because this ensures that they (and the data they refer to) get deleted when they’re no longer needed, without having to write an explicit delete. This **ownership chain** is managed from `head`, with `tail` being a raw pointer to the last node.

> NOTE:
>
> 一、如何理解ownership chain？
>
> 结合上述代码来说是指: 
>
> 1、`std::unique_ptr<node> head; // 1`
>
> 2、
>
> ```c++
> struct node
> {
>     std::unique_ptr<node> next;
> }
> ```
>
> 显然，这种写法的妙处就是: "without having to write an explicit delete"。
>
> 这种写法蕴含着: 每个node的掌握着它的 `next` 的ownership。
>
> 另外需要注意的是: 当只有一个node的时候，head和tail同时指向这个node
>
> 二、思考: 为什么`tail` 使用raw pointer？
>
> 1、**ownership chain** 是从前往后的，这就是说`tail`是不take ownership的
>
> 2、当只有一个node的时候，此时head 和 tail同时指向这个node，由于使用`unique_ptr`，显然只能有一个take ownership
>
> 

Although this implementation works fine in a single-threaded context, a couple of things will cause you problems if you try to use fine-grained locking in a multithreaded context. Given that you have two data items (`head` 1 and `tail` 2), you could in principle use two mutexes, one to protect `head` and one to protect `tail`, but there are a couple of problems with that.

The most obvious problem is that `push()` can modify both `head` 5 and `tail` 6, so it would have to lock both mutexes. This isn’t too much of a problem, although it’s unfortunate, because locking both mutexes would be possible. The critical problem is that both `push()` and `pop()` access the `next` pointer of a node: `push()` updates `tail->next` 4, and `try_pop()` reads `head->next` 3. If there’s a single item in the queue, then `head==tail`, so both `head->next` and `tail->next` are the same object, which therefore requires protection. Because you can’t tell if it’s the same object without reading both `head` and `tail`, you now have to lock the same mutex in both `push()` and `try_pop()`, so you’re no better off than before. Is there a way out of this dilemma?

> NOTE:
>
> 一、



## ENABLING CONCURRENCY BY SEPARATING DATA

You can solve this problem by preallocating a **dummy node** with no data to ensure that there’s always at least one node in the queue to separate the node being accessed at the head from that being accessed at the tail.  For an empty queue, head and tail now both point to the dummy node rather than being NULL. This is fine, because try_pop() doesn’t access head->next if the queue is empty. If you add a node to the queue (so there’s one real node), then head and tail now point to separate nodes, so there’s no race on head->next and tail->next. The downside is that you have to add an extra level of indirection to store the data by pointer in order to allow the dummy nodes. The following listing shows how the implementation looks now.



## Listing 6.5 A simple queue with a dummy node

```c++
template <typename T>
class queue
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    std::unique_ptr<node> head;
    node *tail;

public:
    queue() : head(new node), tail(head.get())
    {
    }
    queue(const queue &other) = delete;
    queue &operator=(const queue &other) = delete;
    std::shared_ptr<T> try_pop()
    {
        if (head.get() == tail)
        {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> const res(head->data);
        std::unique_ptr<node> old_head = std::move(head);
        head = std::move(old_head->next);
        return res;
    }
    void push(T new_value)
    {
        std::shared_ptr<T> new_data(
            std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node);
        tail->data = new_data;
        node *const new_tail = p.get();
        tail->next = std::move(p);
        tail = new_tail;
    }
};

```



## Listing 6.6 A thread-safe queue with fine-grained locking

```c++
template <typename T>
class threadsafe_queue
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    std::mutex head_mutex;
    std::unique_ptr<node> head;
    std::mutex tail_mutex;
    node *tail;
    node *get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }
    std::unique_ptr<node> pop_head()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);

        if (head.get() == get_tail())
        {
            return nullptr;
        }
        std::unique_ptr<node> old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head;
    }

public:
    threadsafe_queue() : head(new node), tail(head.get())
    {
    }
    threadsafe_queue(const threadsafe_queue &other) = delete;
    threadsafe_queue &operator=(const threadsafe_queue &other) = delete;
    std::shared_ptr<T> try_pop()
    {
        std::unique_ptr<node> old_head = pop_head();
        return old_head ? old_head->data : std::shared_ptr<T>();
    }
    void push(T new_value)
    {
        std::shared_ptr<T> new_data(
            std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node);
        node *const new_tail = p.get();
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data = new_data;
        tail->next = std::move(p);
        tail = new_tail;
    }
};
```

