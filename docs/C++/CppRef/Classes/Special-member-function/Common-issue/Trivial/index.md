# Trivial

一、按照 CSDN [C++ trivial和non-trivial构造函数及POD类型](https://blog.csdn.net/a627088424/article/details/48595525) 中的说法: "trivial"的含义是: "无意义"。

二、按wikipedia [Modification to the definition of plain old data](https://en.wikipedia.org/wiki/C++11#Modification_to_the_definition_of_plain_old_data)中的说法，`trivial`是c++11引入的。

## CSDN [C++ trivial和non-trivial构造函数及POD类型](https://blog.csdn.net/a627088424/article/details/48595525)

**trivial**意思是**无意义**，这个trivial和non-trivial是对类的四种函数来说的：

1、构造函数(ctor)

2、复制构造函数(copy)

3、赋值函数(assignment)

4、析构函数(dtor)

如果至少满足下面3条里的一条：

1、显式(explict)定义了这四种函数。

2、类里有非静态非POD的数据成员。

3、有基类。

那么上面的四种函数是**non-trivial**函数，比如叫non-trivial ctor、non-trivial copy…，也就是说**有意义**的函数，里面有一下必要的操作，比如类成员的初始化，释放内存等。

那个POD意思是Plain Old Data，也就是C++的内建类型或传统的C结构体类型。POD类型必然有trivial ctor/dtor/copy/assignment四种函数。

那这有什么用处呢？

如果这个类都是trivial ctor/dtor/copy/assignment函数，我们对这个类进行构造、析构、拷贝和赋值时可以采用最有效率的方法，不调用无所事事正真的那些ctor/dtor等，而直接采用内存操作如malloc()、memcpy()等提高性能，这也是SGI STL内部干的事情。

## Constructor and destructor

trivial表示**无意义**的，对于trivial default constructor 、trivial destructor，是可以不调用的:

1、cppreference [Lifetime # Storage reuse](https://en.cppreference.com/w/cpp/language/lifetime#Storage_reuse) 

> A program is not required to call the destructor of an object to end its lifetime if the object is [trivially-destructible](https://en.cppreference.com/w/cpp/language/destructor#Trivial_destructor) or if the program does not rely on the side effects of the destructor.

2、CSDN [C++ trivial和non-trivial构造函数及POD类型](https://blog.csdn.net/a627088424/article/details/48595525)

### cppreference [Default constructors](https://en.cppreference.com/w/cpp/language/default_constructor) # Trivial default constructor

The default constructor for class `T` is trivial (i.e. performs no action) if all of the following is true:

1、The constructor is not user-provided (i.e., is implicitly-defined or defaulted on its first declaration)

2、`T` has no virtual member functions

3、`T` has no virtual base classes

4、`T` has no non-static members with default initializers.  (since C++11)

5、Every direct base of `T` has a trivial default constructor

6、Every non-static member of class type (or array thereof) has a trivial default constructor

A trivial default constructor is a constructor that performs no action. All data types compatible with the C language (POD types) are trivially default-constructible.



### cppreference [Destructors](https://en.cppreference.com/w/cpp/language/destructor) # Trivial destructor

The destructor for class `T` is trivial if all of the following is true:

1、The destructor is not user-provided (meaning, it is either implicitly declared, or explicitly defined as defaulted on its first declaration)

2、The destructor is not virtual (that is, the base class destructor is not virtual)

3、All direct base classes have trivial destructors

4、All non-static data members of class type (or array of class type) have trivial destructors

A trivial destructor is a destructor that performs no action. Objects with trivial destructors don't require a delete-expression and may be disposed of by simply deallocating their storage. All data types compatible with the C language (POD types) are trivially destructible.



## Copy

### cppreference [Copy constructors](https://en.cppreference.com/w/cpp/language/copy_constructor) # [Trivial copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor#Trivial_copy_constructor)

The copy constructor for class `T` is trivial if all of the following are true:

1、it is not user-provided (that is, it is implicitly-defined or defaulted) ;

2、`T` has no virtual member functions;

3、`T` has no virtual base classes;

4、the copy constructor selected for every direct base of `T` is trivial;

5、the copy constructor selected for every non-static class type (or array of class type) member of `T` is trivial;

A trivial copy constructor for a non-union class effectively copies every scalar subobject (including, recursively, subobject of subobjects and so forth) of the argument and performs no other action. However, padding bytes need not be copied, and even the object representations of the copied subobjects need not be the same as long as their values are identical.

[*TriviallyCopyable*](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable) objects can be copied by copying their object representations manually, e.g. with [std::memmove](https://en.cppreference.com/w/cpp/string/byte/memmove). All data types compatible with the C language (POD types) are trivially copyable.



### cppreference [Copy assignment operator](https://en.cppreference.com/w/cpp/language/copy_assignment) # Trivial copy assignment operator

The copy assignment operator for class `T` is trivial if all of the following is true:

1、it is not user-provided (meaning, it is implicitly-defined or defaulted) ;

2、`T` has no virtual member functions;

3、`T` has no virtual base classes;

4、the copy assignment operator selected for every direct base of `T` is trivial;

5、the copy assignment operator selected for every non-static class type (or array of class type) member of `T` is trivial.

A trivial copy assignment operator makes a copy of the object representation as if by [std::memmove](https://en.cppreference.com/w/cpp/string/byte/memmove). All data types compatible with the C language (POD types) are trivially copy-assignable.

## Move

### cppreference [Move constructors](https://en.cppreference.com/w/cpp/language/move_constructor) # [Trivial move constructor](https://en.cppreference.com/w/cpp/language/move_constructor#Trivial_move_constructor)

The move constructor for class `T` is trivial if all of the following is true:

1、it is not user-provided (meaning, it is **implicitly-defined** or **defaulted**);

> NOTE: 
>
> 1、需要对 "**implicitly-defined** or **defaulted**" 所执行的行为有完整认知

2、`T` has no virtual member functions;

3、`T` has no virtual base classes;

4、the move constructor selected for every direct base of `T` is trivial;

5、the move constructor selected for every non-static class type (or array of class type) member of `T` is trivial.

A trivial move constructor is a constructor that performs the same action as the trivial copy constructor, that is, makes a copy of the object representation as if by [std::memmove](https://en.cppreference.com/w/cpp/string/byte/memmove). All data types compatible with the C language (POD types) are trivially movable.



### cppreference [Move assignment operator](https://en.cppreference.com/w/cpp/language/move_assignment) # Trivial move assignment operator

The move assignment operator for class `T` is trivial if all of the following is true:

1、It is not user-provided (meaning, it is implicitly-defined or defaulted);

2、`T` has no virtual member functions;

3、`T` has no virtual base classes;

4、the move assignment operator selected for every direct base of `T` is trivial;

5、the move assignment operator selected for every non-static class type (or array of class type) member of `T` is trivial.

A trivial move assignment operator performs the same action as the trivial copy assignment operator, that is, makes a copy of the object representation as if by [std::memmove](https://en.cppreference.com/w/cpp/string/byte/memmove). All data types compatible with the C language (POD types) are trivially move-assignable.



### 陷阱: 导致dangling

#### stackoverflow [c++ type trait to say “trivially movable” - examples of](https://stackoverflow.com/questions/45747932/c-type-trait-to-say-trivially-movable-examples-of)



#### 修正方法

参见 `Relocate` 章节。

## 术语总结

在阅读cppreference的时候，多次碰到这个词语，有：

- [TrivialType](https://en.cppreference.com/w/cpp/named_req/TrivialType)

- [TriviallyCopyable](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable)
- [Trivial default constructor](https://en.cppreference.com/w/cpp/language/default_constructor#Trivial_default_constructor)
- [Trivial copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor#Trivial_copy_constructor)
- [Trivial move constructor](https://en.cppreference.com/w/cpp/language/move_constructor#Trivial_move_constructor)
- [Trivial copy assignment operator](https://en.cppreference.com/w/cpp/language/copy_assignment#Trivial_copy_assignment_operator)
- [Trivial move assignment operator](https://en.cppreference.com/w/cpp/language/move_assignment#Trivial_move_assignment_operator)
- [Trivial destructor](https://en.cppreference.com/w/cpp/language/destructor#Trivial_destructor)





## 素材

1、howtobuildsoftware [What is a trivial function?](https://www.howtobuildsoftware.com/index.php/how-do/2pm/c-language-lawyer-c-14-what-is-a-trivial-function)

https://stackoverflow.com/questions/3899223/what-is-a-non-trivial-constructor-in-c

https://www.geeksforgeeks.org/trivial-classes-c/

http://olafurw.com/2019-02-14-trivial-code/

https://isocpp.org/blog/2018/03/quick-q-what-is-a-non-trivial-constructor-in-cpp

https://www.geeksforgeeks.org/trivial-classes-c/

