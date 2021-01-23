# cppreference [Copy elision](https://en.cppreference.com/w/cpp/language/copy_elision)

Omits copy and move (since C++11) constructors, resulting in zero-copy pass-by-value semantics.

> NOTE: 这是C++对value semantic的增强

## Mandatory elision of copy/move operations(C++17)

> NOTE: 强制消除

Under the following circumstances, the compilers are required to omit the copy and move construction of class objects, even if the copy/move constructor and the destructor have observable side-effects. 

> NOTE: 比较激进的optimization

The objects are constructed directly into the storage where they would otherwise be copied/moved to. 

> NOTE: 看了上面这段话，我想到了
>
> 1、construct in place
>
> 2、placement new

The copy/move constructors need not be present or accessible:

> NOTE: 需要注意的是:
>
> 1、上面仅仅说明了copy/move constructors need not be present or accessible，而没有说明destructor

### 1、return

In a [return statement](https://en.cppreference.com/w/cpp/language/return), when the operand is a [prvalue](https://en.cppreference.com/w/cpp/language/value_category) of the same class type (ignoring [cv-qualification](https://en.cppreference.com/w/cpp/language/cv)) as the function return type:

```C++
T f() {
    return T();
}
 
f(); // only one call to default constructor of T
```

> NOTE: 其实在C++17之前的compiler已经实现了这种optimization

The destructor of the type returned must be accessible at the point of the return statement and non-deleted, even though no T object is destroyed.

> NOTE: constructor则相反

### 2、initialization

In the initialization of an object, when the initializer expression is a [prvalue](https://en.cppreference.com/w/cpp/language/value_category) of the same class type (ignoring [cv-qualification](https://en.cppreference.com/w/cpp/language/cv)) as the variable type:

```C++
T x = T(T(f())); // only one call to default constructor of T, to initialize 
```



## Non-mandatory elision of copy/move (since C++11) operations





## Notes

Copy elision is the only allowed form of optimization (until C++14)one of the two allowed forms of optimization, alongside [allocation elision and extension](https://en.cppreference.com/w/cpp/language/new#Allocation), (since C++14) that can change the observable side-effects. Because some compilers do not perform copy elision in every situation where it is allowed (e.g., in debug mode), programs that rely on the side-effects of copy/move constructors and destructors are not portable.



### (since C++11)

In a return statement or a throw-expression, if the compiler cannot perform copy elision but the conditions for copy elision are met or would be met, except that the source is a function parameter, the compiler will attempt to use the move constructor even if the object is designated by an lvalue; see [return statement](https://en.cppreference.com/w/cpp/language/return#Notes) for details.

