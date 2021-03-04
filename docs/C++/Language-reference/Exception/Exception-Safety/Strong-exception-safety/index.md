# Strong exception safety

1、实现strong exception safety是C++ programmer的非常重要的一个目标，strong exception safety有着诸多优良的特性能够保证改善program

2、C++提供了非常多的technique、idiom来保证能够实现strong exception safety，这些technique、idiom是C++ programmer需要进行学习的



## How to implement strong exception safety?

1、实现"commit-or-rollback" semantics、transaction

2、保证resource的安全，不会leak

> NOTE:
>
> ? RAII，是否是basic exception safety？

那如何实现呢？下面是我的一些总结:

a、使用explicit try-catch 

在 "boost [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library](https://www.boost.org/community/exception_safety.html) # 6 Should we take everything we can get? "中演示了这种写法: 

```C++
template <class T>                                // 1 
void SearchableStack<T>::push(const T& t)         // 2 
{                                                       // 3 
    set<T>::iterator i = set_impl.insert(t);      // 4 
    try                                                 // 5 
    {                                                   // 6 
        list_impl.push_back(i);                         // 7 
    }                                                   // 8 
    catch(...)                                          // 9 
    {                                                   // 10 
        set_impl.erase(i);                              // 11 
        throw;                                          // 12 
    }                                                   // 13 
}                                                       // 14 
```



b、使用Scope-Guard idiom

这种方法不需要使用explicit try-catch ，参见 `Scope-Guard` 章节



c、RAII

这是C++中非常重要的一种idiom，参见 `RAII` 章节



d、copy-and-swap

它是一种广泛采用的实现strong exception safety的idiom，参见 `Copy-and-swap` 章节