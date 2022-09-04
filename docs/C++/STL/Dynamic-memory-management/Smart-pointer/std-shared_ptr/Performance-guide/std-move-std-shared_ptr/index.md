# `std::move` `std::shared_ptr`

在阅读 justsoftwaresolutions [Why do we need atomic_shared_ptr?](https://www.justsoftwaresolutions.co.uk/threading/why-do-we-need-atomic_shared_ptr.html) 时，其中有如下代码片段:

```C++
    void pop_front(){
        std::shared_ptr<Node> p=head;
        if(p){
            head=std::move(p->next);
        }
    }
```

上述 `head=std::move(p->next)` 中 `std::move(p->next)` 的写法引起了我的好奇。



## stackoverflow [Why would I std::move an std::shared_ptr?](https://stackoverflow.com/questions/41871115/why-would-i-stdmove-an-stdshared-ptr)

I have been looking through the [Clang source code](https://clang.llvm.org/doxygen/CompilerInstance_8cpp_source.html#l00069) and I found this snippet:

```cpp
void CompilerInstance::setInvocation(
    std::shared_ptr<CompilerInvocation> Value) {
  Invocation = std::move(Value);
}
```

Why would I want to `std::move` an `std::shared_ptr`?

Is there any point transferring ownership on a shared resource?

Why wouldn't I just do this instead?

```cpp
void CompilerInstance::setInvocation(
    std::shared_ptr<CompilerInvocation> Value) {
  Invocation = Value;
}
```



### [A](https://stackoverflow.com/a/41874953/10173843)

I think that the one thing the other answers did not emphasize enough is the point of *speed*.

`std::shared_ptr` reference count is *atomic*. increasing or decreasing the reference count **requires *atomic* increment or decrement**. This is hundred times *slower* than *non-atomic* increment/decrement, not to mention that if we increment and decrement the same counter we wind up with the exact number, wasting a ton of time and resources in the process.

> NOTE:
>
> 一、翻译如下:
>
> "`std::shared_ptr` 引用计数是原子的。增加或减少引用计数需要原子增量或减量。这比非原子递增/递减慢了一百倍，更不用说如果我们递增和递减同一个计数器，我们最终会得到确切的数字，在这个过程中浪费了大量的时间和资源。"

By moving the `shared_ptr` instead of copying it, we "steal" the *atomic* reference count and we nullify the other `shared_ptr`. "stealing" the reference count is not *atomic*, and it is hundred times faster than copying the `shared_ptr` (and causing *atomic* reference increment or decrement).

Do note that this technique is used purely for optimization. copying it (as you suggested) is just as fine functionality-wise.



#### comments

Is it really *hundred* times faster? Do you have **benchmarks** for this? – [xaviersjs](https://stackoverflow.com/users/1196033/xaviersjs)

@xaviersjs The assignment requires an atomic increment followed by an atomic decrement when `Value` goes out of scope. Atomic operations can take hundreds of clock cycles. So yes, it really is that much slower. – [Adisak](https://stackoverflow.com/users/14904/adisak)

@Adisak that's the first I've heard the fetch and add operation ([en.wikipedia.org/wiki/Fetch-and-add](https://en.wikipedia.org/wiki/Fetch-and-add)) could take hundreds of cycles more than a basic increment. Do you have a reference for that? – [xaviersjs](https://stackoverflow.com/users/1196033/xaviersjs)

@xaviersjs : [stackoverflow.com/a/16132551/4238087](https://stackoverflow.com/a/16132551/4238087) With register operations being a few cycles, 100's (100-300) of cycles for atomic fits the bill. Although metrics are from 2013, this still seems to be true especially for multi-socket NUMA systems. – [russianfool](https://stackoverflow.com/users/4238087/russianfool)

> NOTE:
>
> 一、上面所讨论的是 [atomic operation cost](https://stackoverflow.com/questions/2538070/atomic-operation-cost) 

See [herbsutter.com/2013/06/05/…](https://herbsutter.com/2013/06/05/gotw-91-solution-smart-pointer-parameters/) for a discussion of perf implications of passing a `shared_ptr` by value. – [dqbydt](https://stackoverflow.com/users/3367247/dqbydt)

### [A](https://stackoverflow.com/a/41871181/10173843)

By using `move` you avoid increasing, and then immediately decreasing, the number of shares. That might save you some expensive atomic operations on the use count.



## stackoverflow [Cost of copy vs move std::shared_ptr](https://stackoverflow.com/questions/70053971/cost-of-copy-vs-move-stdshared-ptr)