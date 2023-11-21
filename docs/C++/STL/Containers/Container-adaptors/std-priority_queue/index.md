# `std::priority_queue`

一、通过 binary heap 的实现原理来理解max heap、min heap 和 comparator 之间的关系

在 labuladong [图文详解二叉堆，实现优先级队列](https://mp.weixin.qq.com/s/o7tdyLiYm668dpUWd-x7Lg) 中，对 binary heap 的实现原理进行了非常好的介绍。

binary heap 所强调的是 parent node 和 children node之间的关系，所以它的comparator是基于 parent node 和 children node 的；

binary heap 的原理是: 

1、在insert的时候，首先将元素插入到末尾，然后swim(上浮)；显然，对于max heap，我们总是需要将max放到堆顶，当 `less(parent, children)` 的时候，就将元素换到parent的位置，从而实现swim(上浮)；

2、在delete的时候，首先将末尾元素换到头，然后sink；

总的来说，

1、对于max heap，无论是swim(上浮)还是sink(下沉)，都是当`less(parent, children)`的时候；

2、对于min heap，无论是swim(上浮)还是sink(下沉)，都是当`great(parent, children)`的时候；

## cppreference [std::priority_queue](https://en.cppreference.com/w/cpp/container/priority_queue)

Defined in header [`<queue>`](https://en.cppreference.com/w/cpp/header/queue)

```c++
template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>
> class priority_queue;
```

> NOTE: 
>
> 可以看到，它默认使用的`Compare`是`std::less`，因此它默认是max heap

A priority queue is a container adaptor that provides constant time lookup of the largest (by default) element, at the expense of logarithmic insertion and extraction.

A user-provided `Compare` can be supplied to change the ordering, e.g. using `std::greater<T>` would cause the **smallest element** to appear as the [top()](https://en.cppreference.com/w/cpp/container/priority_queue/top).

> NOTE: 
>
> 需要注意的是，默认是最大堆

Working with a `priority_queue` is similar to managing a [heap](https://en.cppreference.com/w/cpp/algorithm/make_heap) in some **random access container**, with the benefit of not being able to accidentally invalidate the heap.

> NOTE: 这一段话其实透露出了`priority_queue`的实现，它结合了`heap`和一个random access container（比如`std::vector`）。

```C++
#include <functional>
#include <queue>
#include <vector>
#include <iostream>
 
template<typename T>
void print_queue(T q) { // NB: pass by value so the print uses a copy
    while(!q.empty()) {
        std::cout << q.top() << ' ';
        q.pop();
    }
    std::cout << '\n';
}
 
int main() {
    std::priority_queue<int> q;
 
    const auto data = {1,8,5,6,3,4,0,9,7,2};
 
    for(int n : data)
        q.push(n);
 
    print_queue(q);
 
    std::priority_queue<int, std::vector<int>, std::greater<int>>
        q2(data.begin(), data.end());
 
    print_queue(q2);
 
    // Using lambda to compare elements.
    auto cmp = [](int left, int right) { return (left ^ 1) < (right ^ 1); };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> q3(cmp);
 
    for(int n : data)
        q3.push(n);
 
    print_queue(q3);
}
// g++ test.cpp --std=c++11
```





## 最小堆

### 方式一: custom comparator by lambda

[LeetCode-787. c++简单易懂的Dijkstra算法](https://leetcode-cn.com/problems/cheapest-flights-within-k-stops/solution/cjian-dan-yi-dong-de-dijkstrasuan-fa-by-w05aa/) 

```C++
class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int K) {
        // 构建图
        vector<vector<int>> graph(n, vector<int>(n, 0));
        for (auto& flight : flights)
        {
            graph[flight[0]][flight[1]] = flight[2];
        }

        // 优先级队列，按照价格的最小堆
        auto cmp = [](const vector<int>& a, const vector<int>& b) -> bool
        {
            return a[1] > b[1];
        };
        priority_queue<vector<int>, deque<vector<int>>, decltype(cmp)> q(cmp);
        // 插入到数组里： 当前点，价格，最大飞行次数（K=0的时候也能至少飞一次，所以是中转次数+1）
        q.push({src, 0, K+1});
        while (!q.empty())
        {
            const vector<int>& curr = q.top();
            int node = curr[0];
            int price = curr[1];
            int k = curr[2];
            q.pop();

            if (node == dst)
            {
                return price;
            }

            // 判断是否还可以再飞一次
            if (k > 0)
            {
                // 遍历目前可以达到的边
                for (int i = 0; i < n; ++i)
                {
                    if (graph[node][i] > 0)
                    {
                        q.push({i, price + graph[node][i], k-1});
                    }
                }
            }

        }

        // 都找不到结果，则返回-1
        return -1;
    }
};

```

### 方式二: `std::greater`

[LeetCode-703. 数据流中的第 K 大元素-简单](https://leetcode.cn/problems/kth-largest-element-in-a-stream/)

```c++
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class KthLargest
{
    std::priority_queue<int, std::vector<int>, std::greater<int>> q; // 最小堆
    int k;

public:
    KthLargest(int k, vector<int> &nums)
    {
        this->k = k;
        for (auto &&num : nums)
        {
            addImpl(num);
        }
    }

    int add(int val)
    {
        addImpl(val);
        return q.top();
    }

private:
    void addImpl(int val)
    {
        if (q.size() < k || val > q.top())
        {
            if (q.size() == k)
            {
                q.pop();
            }
            q.push(val);
        }
    }
};

/**
 * Your KthLargest object will be instantiated and called as such:
 * KthLargest* obj = new KthLargest(k, nums);
 * int param_1 = obj->add(val);
 */

int main()
{
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra -g

```



## Move out element from `std::priority_queue`

### stackoverflow [Move out element of std priority_queue in C++11](https://stackoverflow.com/questions/20149471/move-out-element-of-std-priority-queue-in-c11)

Minimal working example.

```cpp
#include <cassert>
#include <list>
#include <queue>
//#define USE_PQ

struct MyClass
{
    const char* str;
    MyClass(const char* _str) : str(_str) {}
    MyClass(MyClass&& src) { str = src.str; src.str = nullptr; }
    MyClass(const MyClass&) = delete;
};

struct cmp_func
{
    bool operator() (const MyClass&, const MyClass&) const
    {
        return true;
    }
};

typedef std::priority_queue<MyClass, std::vector<MyClass>, cmp_func> pq_type;

#ifdef USE_PQ
MyClass remove_front(pq_type& l)
{
    MyClass moved = std::move(l.top());
    // error from the above line:
    // use of deleted function ‘MyClass::MyClass(const MyClass&)’
    l.pop();
    return std::move(moved);
}
#else
MyClass remove_front(std::list<MyClass>& l)
{
    MyClass moved = std::move(l.front());
    l.erase(l.begin());
    return std::move(moved);
}
#endif

int main()
{
    const char* hello_str = "Hello World!";
    MyClass first(hello_str);
#ifdef USE_PQ
    pq_type l;
    l.push(std::move(first));
    MyClass moved = remove_front(l);
#else
    std::list<MyClass> l;
    l.push_back(std::move(first));
    MyClass moved = remove_front(l);
#endif
    assert(moved.str);
    assert(!first.str);
    return 0;
}
```

***SUMMARY*** ： 上述code中，`MyClass`是movable但是not copyable。



So this works. Now remove the comment signs from line 4 and it says that copy constructors would be needed (mine is deleted). Also, it misses `operator=`. Questions:

- What is the difference here?
- Can the problem be fixed? If yes, how, if no, why not?

Note: You can also use boost's `priority_queue` for your answer, but I got the same error with it.

***COMMENTS*** :

`priority_queue::top()` returns **const reference**, so even after move it is still an **lvalue**. – [Siyuan Ren](https://stackoverflow.com/users/832878/siyuan-ren) [Nov 22 '13 at 16:17](https://stackoverflow.com/questions/20149471/move-out-element-of-std-priority-queue-in-c11#comment30032408_20149471)

@C.R. `std::move` is just a cast to an **rvalue reference**, so any lvalue of type `const T` will be converted to `const T&&`, which is an **rvalue** -- even though you cannot acquire resources through it. – [dyp](https://stackoverflow.com/users/420683/dyp) [Nov 22 '13 at 17:29](https://stackoverflow.com/questions/20149471/move-out-element-of-std-priority-queue-in-c11#comment30034953_20149471) 



#### [A](https://stackoverflow.com/a/20149745)

That seems to be an oversight(疏忽) in the design of `std::priority_queue<T>`. There doesn't appear to be a way to directly move (not copy) an element out of it. The problem is that `top()` returns a `const T&`, so that cannot bind to a `T&&`. And `pop()` returns `void`, so you can't get it out of that either.

However, there's a workaround. It's as good as guaranteed that the objects inside the **priority queue** are not actually `const`. They are normal objects, the queue just doesn't give mutable access to them. Therefore, it's perfectly legal to do this:

```cpp
MyClass moved = std::move(const_cast<MyClass&>(l.top()));
l.pop();
```

As @DyP pointed out in comments, you should make certain that the **moved-from object** is still viable（可行的） for being passed to the queue's comparator. And I believe that in order to preserve the preconditions of the queue, it would have to compare the same as it did before (which is next to impossible to achieve).

Therefore, you should encapsulate the `cast & top()` and `pop()` calls in a function and make sure no modifications to the queue happen in between. If you do that, you can be reasonably certain the **comparator** will not be invoked on the moved-from object.

And of course, such a function should be extremely well documented.

------

Note that whenever you provide a **custom copy/move constructor** for a class, you should provide the corresponding **copy/move assignment operator** as well (otherwise, the class can behave inconsistently). So just give your class a deleted copy assignment operator and an appropriate move assignment operator.

(Note: Yes, there are situations when you want a move-constructible, but not move-assignable class, but they're extremely rare (and you'll know them if you ever find them). **As a rule of thumb, always provide the ctor and assignment op at the same time**)

***COMMENTS*** : 

There's already a proposal in the the isocpp proposals forum from May to solve this issue, see [groups.google.com/a/isocpp.org/d/msg/std-proposals/TIst1FOdveo/…](https://groups.google.com/a/isocpp.org/d/msg/std-proposals/TIst1FOdveo/D54Uo-QuGfUJ) – [dyp](https://stackoverflow.com/users/420683/dyp) [Nov 27 '13 at 18:00](https://stackoverflow.com/questions/20149471/move-out-element-of-std-priority-queue-in-c11#comment30202584_20149745)





#### [A](https://stackoverflow.com/a/22738515)

There might be a very good reason why there is no non-(const-ref) top(): modifying the object would break the `priority_queue` invariant. So that `const_cast` trick is probably only going to work if you pop right after.



#### [A](https://stackoverflow.com/a/38222303)

Depending on what type you want to store in the priority queue, an alternative to Angew's solution, that avoids the `const_cast` and removes some of the opportunities for shooting oneself in the foot, would be to wrap the element type as follows:

```cpp
struct Item {
    mutable MyClass element;
    int priority; // Could be any less-than-comparable type.

    // Must not touch "element".
    bool operator<(const Item& i) const { return priority < i.priority; }
};
```

Moving the element out of the queue would then be done as such:

```cpp
MyClass moved = std::move(l.top().element);
l.pop();
```

That way, there are no special requirements on the move semantics of `MyClass` to preserve the order relation on the invalidated object, and there will be no section of code where invariants of the priority queue are invalidated.



### stackoverflow [How to move elements out of STL priority queue](https://stackoverflow.com/questions/22047964/how-to-move-elements-out-of-stl-priority-queue)

`C++`'s STL priority queue have a void `pop()` method, and a const ref `top()` method. Thus, if you want to move elements out of the queue, you have to do something like this:

```cpp
T moved = std::move(const_cast<T&>(myQueue.top())));
myQeue.pop();
```

This effectively casts the top to not a constant, so that it can be moved (rather than copied). I don't like this code, because the forced move may invalidate the invariants of the priority queue, which *should* not matter because of the pop, but things *could* go wrong.

Is there a better way to accomplish the pop/move? Why is there no T&& top_and_pop() function?

***COMMENTS*** : 

Related/duplicate: [Move out element of std priority_queue in C++11](http://stackoverflow.com/q/20149471/420683) – [dyp](https://stackoverflow.com/users/420683/dyp) [Feb 26 '14 at 17:51](https://stackoverflow.com/questions/22047964/how-to-move-elements-out-of-stl-priority-queue#comment33431398_22047964)

Related: [How to get a non-const top element from priority_queue with user-defined objects?](http://stackoverflow.com/q/16754745/420683) and [Getting a unique_ptr out of a priority queue](http://stackoverflow.com/q/16661038/420683) – [dyp](https://stackoverflow.com/users/420683/dyp) [Feb 26 '14 at 17:53](https://stackoverflow.com/questions/22047964/how-to-move-elements-out-of-stl-priority-queue#comment33431466_22047964)

Relevant discussions in isocpp/future proposals: [groups.google.com/a/isocpp.org/d/topic/std-proposals/…](https://groups.google.com/a/isocpp.org/d/topic/std-proposals/Tp_HjVlXa7M/discussion) and [groups.google.com/a/isocpp.org/d/topic/std-proposals/…](https://groups.google.com/a/isocpp.org/d/topic/std-proposals/TIst1FOdveo/discussion) – [dyp](https://stackoverflow.com/users/420683/dyp) [Feb 26 '14 at 17:56](https://stackoverflow.com/questions/22047964/how-to-move-elements-out-of-stl-priority-queue#comment33431589_22047964) 



#### [A](https://stackoverflow.com/a/22050490)

`std::priority_queue` is basically a thin layer on top of the *heap algorithms*. You can easily create your own *priority queue* with:

- [std::vector](http://de.cppreference.com/w/cpp/container/vector)
- [std::push_heap](http://en.cppreference.com/w/cpp/algorithm/push_heap)
- [std::pop_heap](http://en.cppreference.com/w/cpp/algorithm/pop_heap)

Using these building blocks, the implementation is trivial, and you can easily implement a *moving* pop operation. The following listing contains a minimal, working implementation:

```cpp
template <typename Type, typename Compare = std::less<Type>>
class queue
{
private:
    std::vector<Type> _elements;
    Compare _compare;
public:
    explicit queue(const Compare& compare = Compare())
        : _compare{compare}
    { }
    void push(Type element)
    {
        _elements.push_back(std::move(element));
        std::push_heap(_elements.begin(), _elements.end(), _compare);
    }
    Type pop()
    {
        std::pop_heap(_elements.begin(), _elements.end(), _compare);
        Type result = std::move(_elements.back());
        _elements.pop_back();
        return std::move(result);
    }
};
```

***COMMENTS*** :

+1 I'd also should check/`static_assert` for `is_nothrow_move_constructible` (even in a minimal implementation), otherwise you might lose an element in `pop`. – [dyp](https://stackoverflow.com/users/420683/dyp) [Feb 27 '14 at 17:21](https://stackoverflow.com/questions/22047964/how-to-move-elements-out-of-stl-priority-queue#comment33478191_22050490)

This should get the job done, but it's pretty verbose. – [Ant6n](https://stackoverflow.com/users/1899587/ant6n) [Mar 2 '14 at 1:59](https://stackoverflow.com/questions/22047964/how-to-move-elements-out-of-stl-priority-queue#comment33563793_22050490)

The `std::move` in `return std::move(result)` is redundant. – [jupp0r](https://stackoverflow.com/users/1034255/jupp0r) [Jul 14 '16 at 11:55](https://stackoverflow.com/questions/22047964/how-to-move-elements-out-of-stl-priority-queue#comment64157442_22050490)





### stackoverflow [Difference between std::set and std::priority_queue](https://stackoverflow.com/questions/10141841/difference-between-stdset-and-stdpriority-queue)

Since both `std::priority_queue` and `std::set` (and `std::multiset`) are data containers that store elements and allow you to access them in an **ordered fashion**, and have same **insertion complexity** `O(log n)`, what are the advantages of using one over the other (or, what kind of situations call for the one or the other?)?

While I know that the underlying structures are different, I am not as much interested in the difference in their implementation as I am in the comparison their **performance** and **suitability** for various uses.

**Note:** I know about the no-duplicates in a set. That's why I also mentioned `std::multiset` since it has the exactly same behavior as the `std::set` but can be used where the data stored is allowed to compare as equal elements. So please, don't comment on single/multiple keys issue.