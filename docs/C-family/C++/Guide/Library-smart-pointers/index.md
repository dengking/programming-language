# Smart pointer

“Smart pointer”即“智能指针”

## What can smart pointer do?

### 1、smart pointer能够解决manual memory management所面临的问题

在`Manual-memory-management`中对这些问题进行了总结。

### 2、符合[intentional programming](https://en.wikipedia.org/wiki/Intentional_programming)，能够清楚地表达ownership

smart pointer符合[intentional programming](https://en.wikipedia.org/wiki/Intentional_programming)

[C++ Core Guidelines](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)的P: Philosophy中有如下两条：

- P.1: Express ideas directly in code
- P.3: Express intent

[intentional programming](https://en.wikipedia.org/wiki/Intentional_programming)显然是符合上述两条的，C++ smart pointer显然也符合上述两条的；C++ smart pointer所表达的intent是**ownership**。关于此，在`Cpp-Core-Guidelines-Passing-Smart-Pointers.md`中将对**ownership**进行非常详细的说明。

本节的意思是`c++`的`std::unique_ptr`显式地表示由caller来管理指针。在原文本节的第二段告诉了我们，`c++`的`std::unique_ptr`是通过type来显式表达intent，这属于[intentional programming](https://en.wikipedia.org/wiki/Intentional_programming)。

### 3、smart pointer符合[Inversion of control principle](https://en.wikipedia.org/wiki/Inversion_of_control)

对于row pointer，需要由programmer来管理其lifetime，对于smart pointer，它们的lifetime无需programmer来管理：

- `shared_ptr`由reference count来管理
- `unique_ptr`由scope来管理



## C++ smart pointer应该覆盖所以pointer的功能

1、dereference

2、opaque pointer、incomplete type

3、covariant return type

无法覆盖的情况: smart pointer不支持 covariant return type，但是raw pointer是支持的，关于这个说法，参见 https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rh-copy



## wikipedia [Smart pointer](https://en.wikipedia.org/wiki/Smart_pointer)

> NOTE: smart pointer不仅仅能够用于memory management，还能够用于其他的resource management。



### [Features](https://en.wikipedia.org/wiki/Smart_pointer#Features)

Smart pointers can facilitate [intentional programming](https://en.wikipedia.org/wiki/Intentional_programming) by expressing, in the type, how the memory of the referent of the pointer will be managed. For example, if a `C++` function returns a pointer, there is no way to know whether the caller should delete the memory of the referent when the caller is finished with the information.



```C++
SomeType* AmbiguousFunction();  // What should be done with the result?
```

Traditionally, naming conventions have been used to resolve the ambiguity,[[5\]](https://en.wikipedia.org/wiki/Smart_pointer#cite_note-5) which is an error-prone, labor-intensive approach. [C++11](https://en.wikipedia.org/wiki/C%2B%2B11) introduced a way to ensure correct memory management in this case by declaring the function to return a `unique_ptr`,

```
std::unique_ptr<SomeType> ObviousFunction();
```

The declaration of the function return type as a `unique_ptr` makes explicit the fact that the caller takes ownership of the result, and the `C++` runtime ensures that the memory will be reclaimed（回收） automatically. Before [C++11](https://en.wikipedia.org/wiki/C%2B%2B11), `unique_ptr` can be replaced with [auto_ptr](https://en.wikipedia.org/wiki/Auto_ptr).



### `unique_ptr`

`std::auto_ptr` is [deprecated](https://en.wikipedia.org/wiki/Deprecation) under `C++11` and completely removed from [C++17](https://en.wikipedia.org/wiki/C%2B%2B17). The copy constructor and assignment operators of `auto_ptr` do not actually copy the stored pointer. Instead, they [transfer it](https://en.wikipedia.org/wiki/Auto_ptr#Semantics), leaving the prior `auto_ptr` object empty. This was one way to implement strict ownership, so that only one `auto_ptr` object can own the pointer at any given time. This means that `auto_ptr` should not be used where copy semantics are needed. Since `auto_ptr` already existed with its copy semantics, it could not be upgraded to be a move-only pointer without breaking [backward compatibility](https://en.wikipedia.org/wiki/Backward_compatibility) with existing code.

### `shared_ptr` and `weak_ptr`

A `shared_ptr` is a container for a [raw pointer](https://en.wikipedia.org/wiki/Raw_pointer). It maintains [reference counting](https://en.wikipedia.org/wiki/Reference_counting) ownership of its contained pointer in cooperation with all copies of the `shared_ptr`. An object referenced by the contained raw pointer will be destroyed when and only when all copies of the `shared_ptr` have been destroyed.

```c++
std::shared_ptr<int> p0(new int(5));  // Valid, allocates 1 integer and initialize it with value 5.
std::shared_ptr<int[]> p1(new int[5]);  // Valid, allocates 5 integers.
std::shared_ptr<int[]> p2 = p1;  // Both now own the memory.

p1.reset();  // Memory still exists, due to p2.
p2.reset();  // Deletes the memory, since no one else owns the memory.
```



## cppreference [Smart pointers](https://en.cppreference.com/w/cpp/memory)



## `unique_ptr` and `shared_ptr` and `weak_ptr`

learncpp [15.6 — std::shared_ptr#Shared pointers can be created from unique pointers](https://www.learncpp.com/cpp-tutorial/15-6-stdshared_ptr/)

> A std::unique_ptr can be converted into a std::shared_ptr via a special std::shared_ptr constructor that accepts a std::unique_ptr r-value. The contents of the std::unique_ptr will be moved to the std::shared_ptr.
>
> However, std::shared_ptr can not be safely converted to a std::unique_ptr. This means that if you’re creating a function that is going to return a smart pointer, you’re better off returning a std::unique_ptr and assigning it to a std::shared_ptr if and when that’s appropriate.





## Reading list

### internalpointers [A beginner's look at smart pointers in modern C++](https://www.internalpointers.com/post/beginner-s-look-smart-pointers-modern-c)



### [intro/smart pointers](https://en.cppreference.com/book/intro/smart_pointers)



### [What is a smart pointer and when should I use one?](https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one)