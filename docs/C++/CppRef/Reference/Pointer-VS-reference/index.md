# Pointer VS reference



## Pointer is an object, while reference is not

这一点是在阅读cppreference [Pointer declaration#Pointer to function](https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_functions)中下面这一段时受启发而想到的:

> Unlike functions or references to functions, pointers to functions are objects and thus can be stored in arrays, copied, assigned, etc.

在cppreference [Object](https://en.cppreference.com/w/cpp/language/object)中，也对此进行了说明:

> The following entities are not objects: value, reference, function

## Pointer to `void` but no reference to `void`

在cppreference [Reference declaration](https://en.cppreference.com/w/cpp/language/reference)中已经说明：

> There are no references to void 

pointer to `void`是允许的。



## Null pointer but no null reference

null pointer表示的是point to nothing，在C family language中，这是存在的；但是不存在null reference，即reference to nothing。

关于这一点，在cppreference [dynamic_cast conversion](https://en.cppreference.com/w/cpp/language/dynamic_cast)中关于cast fail的情况的讨论中描述了：

> If the cast fails and *new-type* is a pointer type, it returns a null pointer of that type. If the cast fails and *new-type* is a reference type, it throws an exception that matches a handler of type [std::bad_cast](https://en.cppreference.com/w/cpp/types/bad_cast).



## stackoverflow [What are the differences between a pointer variable and a reference variable in C++?](https://stackoverflow.com/questions/57483/what-are-the-differences-between-a-pointer-variable-and-a-reference-variable-in)

### [A](https://stackoverflow.com/a/57492)

1) A pointer can be re-assigned:

```cpp
int x = 5;
int y = 6;
int *p;
p = &x;
p = &y;
*p = 10;
assert(x == 5);
assert(y == 10);
```

A reference cannot, and must be assigned at initialization:

```cpp
int x = 5;
int y = 6;
int &r = x;
```

> NOTE: A *reference* can be thought of as a *constant pointer* 

2) A pointer has its own memory address and size on the stack (4 bytes on x86), whereas a reference shares the same memory address (with the original variable) but also takes up some space on the stack. Since a reference has the same address as the original variable itself, it is safe to think of a reference as another name for the same variable. Note: What a pointer points to can be on the stack or heap. Ditto a reference. My claim in this statement is not that a pointer must point to the stack. A pointer is just a variable that holds a memory address. This variable is on the stack. Since a reference has its own space on the stack, and since the address is the same as the variable it references. More on [stack vs heap](https://stackoverflow.com/questions/79923/what-and-where-are-the-stack-and-heap#79936). This implies that there is a real address of a reference that the compiler will not tell you.

NOTE: Pointer is an object, while reference is not

```cpp
int x = 0;
int &r = x;
int *p = &x;
int *p2 = &r;
assert(p == p2);
```

3) You can have pointers to pointers to pointers offering extra levels of indirection. Whereas references only offer one level of indirection.

```cpp
int x = 0;
int y = 0;
int *p = &x;
int *q = &y;
int **pp = &p;
pp = &q;//*pp = q
**pp = 4;
assert(y == 4);
assert(x == 0);
```

4) A pointer can be assigned `nullptr` directly, whereas reference cannot. If you try hard enough, and you know how, you can make the address of a reference `nullptr`. Likewise, if you try hard enough, you can have a reference to a pointer, and then that reference can contain `nullptr`.

> NOTE: Null pointer but no null reference

```cpp
int *p = nullptr;
int &r = nullptr; <--- compiling error
int &r = *p;  <--- likely no compiling error, especially if the nullptr is hidden behind a function call, yet it refers to a non-existent int at address 0
```

5) Pointers can iterate over an array; you can use `++` to go to the next item that a pointer is pointing to, and `+ 4` to go to the 5th element. This is no matter what size the object is that the pointer points to.

6) A pointer needs to be dereferenced with `*` to access the memory location it points to, whereas a reference can be used directly. A pointer to a class/struct uses `->` to access it's members whereas a reference uses a `.`.

7) References cannot be stuffed into an array, whereas pointers can be (Mentioned by user @litb)

8) Const references can be bound to temporaries. Pointers cannot (not without some indirection):

```cpp
const int &x = int(12); //legal C++
int *y = &int(12); //illegal to dereference a temporary.
```

This makes `const&` safer for use in argument lists and so forth.



### [A](https://stackoverflow.com/a/596750)

> NOTE: 这篇文章的总结是非常棒的，优先阅读[这个回答](https://stackoverflow.com/a/596750)，因为它是概括性的，然后阅读[这个回答](https://stackoverflow.com/a/57492)，因为它枚举了非常多的具体情况。

**What's a C++ reference (*for C programmers*)**

A *reference* can be thought of as a *constant pointer* (not to be confused with a pointer to a constant value!) with automatic indirection, ie the compiler will apply the `*` operator for you.

> NOTE: 关于 constant pointer，参见`C-and-C++\Pointer-array-alias\Pointer\Pointer.md`。

All references must be initialized with a **non-null value** or compilation will fail. It's neither possible to get the address of a reference - the address operator will return the address of the referenced value instead - nor is it possible to do arithmetics on references.

> NOTE: no null reference

C programmers might dislike C++ references as it will no longer be obvious when indirection happens or if an argument gets passed by value or by pointer without looking at function signatures.

C++ programmers might dislike using pointers as they are considered unsafe - although references aren't really any safer than constant pointers except in the most trivial cases - lack the convenience of automatic indirection and carry a different semantic connotation.

Consider the following statement from the [*C++ FAQ*](https://isocpp.org/wiki/faq/references#overview-refs):

> Even though a reference is often implemented using an address in the underlying assembly language, please do *not* think of a reference as a funny looking pointer to an object. A reference *is* the object. It is not a pointer to the object, nor a copy of the object. It *is* the object.

But if a reference *really* were the object, how could there be dangling references? In unmanaged languages, it's impossible for references to be any 'safer' than pointers - there generally just isn't a way to reliably alias values across scope boundaries!

**Why I consider C++ references useful**

Coming from a C background, C++ references may look like a somewhat silly concept, but one should still use them instead of pointers where possible: Automatic indirection *is* convenient, and references become especially useful when dealing with [RAII](https://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization) - but not because of any perceived safety advantage, but rather because they make writing idiomatic code less awkward.

RAII is one of the central concepts of C++, but it interacts non-trivially with copying semantics. Passing objects by reference avoids these issues as no copying is involved. If references were not present in the language, you'd have to use pointers instead, which are more cumbersome to use, thus violating the language design principle that the best-practice solution should be easier than the alternatives.

> RAII是c++的核心概念之一，但它与复制语义的交互并不简单。通过引用传递对象可以避免这些问题，因为不涉及复制。如果该语言中没有引用，则必须使用指针，使用起来更麻烦，因此违反了最佳实践解决方案应该比替代方案更容易的语言设计原则。





## [LeetBook C++ 面试突击](https://leetcode-cn.com/leetbook/detail/cpp-interview-highlights/) # 指针和引用的区别？

1、指针所指向的内存空间在程序运行过程中可以改变，而引用所绑定的对象一旦绑定就不能改变。（是否可变）

2、指针本身在内存中占有内存空间，引用相当于变量的别名，在内存中不占内存空间。（是否占内存）

3、指针可以为空，但是引用必须绑定对象。（是否可为空）

4、指针可以有多级，但是引用只能一级。（是否能为多级）

