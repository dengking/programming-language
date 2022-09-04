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
> "std::shared_ptr 引用计数是原子的。增加或减少引用计数需要原子增量或减量。这比非原子递增/递减慢了一百倍，更不用说如果我们递增和递减同一个计数器，我们最终会得到确切的数字，在这个过程中浪费了大量的时间和资源。"