# Smart pointer



## 维基百科[Smart pointer](https://en.wikipedia.org/wiki/Smart_pointer)

> NOTE: smart pointer不仅仅能够用于memory management，还能够用于其他的resource management。

> NOTE: smart pointer能够解决的问题其实就是manual memory management所面临的问题，在`Manual-memory-management\Manual-memory-management.md`中对这些问题进行了总结。
>
> 



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

> NOTE: 本节的意思是`c++`的`std::unique_ptr`显式地表示由caller来管理指针。在原文本节的第二段告诉了我们，`c++`的`std::unique_ptr`是通过type来显式表达intent，这属于[intentional programming](https://en.wikipedia.org/wiki/Intentional_programming)。

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







## internalpointers [A beginner's look at smart pointers in modern C++](https://www.internalpointers.com/post/beginner-s-look-smart-pointers-modern-c)



## [intro/smart pointers](https://en.cppreference.com/book/intro/smart_pointers)



## [What is a smart pointer and when should I use one?](https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one)