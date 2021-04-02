# I see a monad in your future

## Functional Patterns in C++

1、Concurrency

2、First class functions

3、Generic programming

4、Memory Management (move semantics)



Math nomenclature(术语)

1、Functor

2、Applicative Functor

3、Monad

4、Monoid

## Higher Order Functions

```C++
for_each(v.begin(), v.end(), [](char c) { cout << c; });
```



```C++
transform(v.begin(), v.end(), w.begin(), [](int x) { 
    return x * x; 
});
```



1、Sorting: compare function

2、Find, `copy_if`: predicate function

3、Accumulate: binary function



## Combinators

1、Currying(柯里化), partial application: bind

2、Combining algorithms

```C++
v.erase(remove_if(v.begin(), v.end(),
                  bind(logical_and<bool>(),
                       bind(greater<int>(), _1, -10),
                       bind(less<int>(), _1, 10))),
        v.end());

```

> NOTE:
>
> 1、上面这段代码是什么意思？
>
> 删除大于`-10`且小于`10`的元素。
>
> 

## Future

Channel for passing data (John Reppy, ML)

1、Promise

2、Future

## Library Design

1、Composability

2、Orthogonality (Separation of concerns)