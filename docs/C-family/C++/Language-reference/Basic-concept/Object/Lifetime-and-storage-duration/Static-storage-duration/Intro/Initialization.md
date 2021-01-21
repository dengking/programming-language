# Initialization of Static Variables



## pabloariasal [C++ - Initialization of Static Variables](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/)

> NOTE: 写的非常好的一篇文章

### [Static Variables](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#static-variables)

As the compiler translates your program it must decide how to deal with variables introduced: When should a variable be initialized? What’s the initial value? When should it be destroyed?

> NOTE: 其实，这所讨论的就是lifetime of object的问题，这在`C++\Language-reference\Basic-concept\Object\Lifetime-and-storage-duration`章节中已经进行了讨论。

Most of the time the compiler must deal with *dynamic* variables, i.e. variables that are initialized and destroyed at runtime: local (block-scope) variables, function arguments, non-static class members, etc.

The compiler has little chance to initialize such variables before execution starts: How is it supposed to know what arguments will be passed to a function?, or if a given code block will be reached? The answers may even vary from execution to execution, so their initialization and destruction must happen on demand, at runtime, *dynamically*.

> NOTE: automatic storage duration和dynamic storage duration

There is, however, a category of variables that can (and should) be initialized before the program starts: **static variables**. Global (namespace) variables or static class members[1](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#fn:2) live for the entire execution of the program: they must be initialized before `main()` is run and destroyed after execution finishes. Such variables have *static storage duration*.

The lifetime of static variables doesn’t depend on the execution: they always exist; forever; no matter what. This leads to the beautiful property that they can be potentially evaluated and initialized at compile time.

### [The Two Stages of Static Variable Initialization](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#the-two-stages-of-static-variable-initialization)

As discussed, variables with *static storage duration* must be initialized once before the program starts and destroyed after execution terminates.

Initialization of static variables happens in two consecutive stages: *static* and *dynamic* initialization.

**Static initialization** happens first and usually at **compile time**. If possible, initial values for static variables are evaluated during compilation and burned into the data section of the executable. Zero runtime overhead, early problem diagnosis, and, as we will see later, safe. This is called [constant initialization](https://en.cppreference.com/w/cpp/language/constant_initialization). In an ideal world all static variables are const-initialized.

If the initial value of a static variable can’t be evaluated at compile time, the compiler will perform **zero-initialization**. Hence, during **static initialization** all **static variables** are either **const-initialized** or **zero-initialized**.

After **static initialization**, **dynamic initialization** takes place. **Dynamic initialization** happens at runtime for variables that can’t be evaluated at compile time[2](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#fn:1). Here, **static variables** are initialized every time the executable is run and not just once during compilation.



### [The Green Zone - Constant Initialization](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#the-green-zone---constant-initialization)



#### [Force Const Initialization with `constexpr`](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#force-const-initialization-with-constexpr)

```c++

struct Point
{
    float x{};
    float y{};
};

constexpr Point movePoint(Point p, float x_offset, float y_offset)
{
    return {p.x + x_offset, p.y + y_offset};
}

// OK const-initialized, enforced by compiler
constexpr auto P2 = movePoint({3.f, 4.f}, 1.f, -1.f);

// still const-initialized, but not enforced by compiler
auto P1 = P2;

struct Line
{
   Line(Point p1, Point p2)
   {
   }
   // ...
};

// Compile error, l can't be const-initialized (no constexpr constructor)
constexpr Line l({6.7f, 5.7f}, {5.4, 3.2});

int main()
{
    
}
```

> NOTE: `g++ --std=c++11 test.cpp`无法编译通过

`constexpr` must be your first choice when declaring global variables (assuming you really need a global state to begin with). `constexpr` variables are not just initialized at compile time, but `constexpr` implies `const` and immutable state is always the right way.

#### [Your Second Line of Defense: `constinit`](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#your-second-line-of-defense-constinit)

`constinit` is a keyword introduced in the c++20 standard. It works just as `constexpr`, as it forces the compiler to evaluate a variable at compile time, but with the difference that it **doesn’t imply const**.

As a consequence, variables declared `constinit` are always const-(or zero-)initialized, but can be mutated at runtime, i.e. don’t land in read-only data section in the binary.

```c++
constexpr auto N1{54}; // OK const-init ensured
constinit auto N2{67}; // OK const-init ensured, mutable

int square(int n)
{
  return n * n;
}
constinit auto N3 = square(N2); // Compilation error square(N2) not constant expression

int main()
{
  ++N2; // OK
  ++N1; // Compilation error
  return EXIT_SUCCESS;
}
```

### [The Yellow Zone - Dynamic Initialization](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#the-yellow-zone---dynamic-initialization)

```c++
constexpr auto VERSION = std::string("3.4.1");
```

But life isn’t that easy:

```c++
error: constexpr variable cannot have non-literal type
```





The future looks bright, however. Even when moving slowly, smart people have been working on several proposals to augment the capabilities of `constexpr` to types like `std::string` or `std::vector`. The idea here is to allow for memory allocations at compile time and later flash the object alongside its managed memory into the data section of the binary.

> NOTE: 这个非常有趣

### [The Red Zone - Static Initialization Order Fiasco](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#the-red-zone---static-initialization-order-fiasco)

Dynamic initialization of static variables suffers from a very scary defect: the order in which variables are initialized at runtime is not always well-defined.

Within a single compilation unit, static variables are initialized in the same order as they are defined in the source (this is called *Ordered Dynamic Initialization*). Across compilation units, however, the order is undefined: you don’t know if a static variable defined in `a.cpp` will be initialized before or after one in `b.cpp`.

```c++
// a.cpp
int duplicate(int n)
{
    return n * 2;
}
auto A = duplicate(7); // A is dynamic-initialized
```



```c++
// b.cpp
#include <iostream>

extern int A;
auto B = A; // dynamic initialized

int main()
{
  std::cout << B << std::endl;
  return EXIT_SUCCESS;
}
```

> 编译 `g++ --std=c++11 a.cpp b.cpp -o test`

This program is ill-formed. It may print `14` or `0` (all static variables are at least **zero-initialized** during **static initialization**), depending if the **dynamic initialization** of `A` happens before `B` or not.

Note that this problem can only happen during the **dynamic initialization phase** and not during **static initialization**, as during **compile time** it is impossible to access a value defined in another **compilation unit**. This makes **compile time initialization** so much safer than **dynamic initialization**, as it doesn’t suffer from the **static initialization order fiasco**.

### [Solving The Static Initialization Order Fiasco](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#solving-the-static-initialization-order-fiasco)

Encountering the **static initialization order fiasco** is often a symptom of poor software design. IMHO the best way to solve it is by refactoring the code to break the initialization dependency of **globals** across **compilation units**. Make your modules self-contained and strive for **constant initialization**.



```cpp
// a.h
int& A();
```



```c++
// a.cpp
#include "a.h"

int duplicate(int n)
{
    return n * 2;
}

auto& A()
{
  static auto a = duplicate(7); // Initiliazed first time A() is called
  return a;
}
```



```c++
// b.cpp
#include <iostream>
#include "a.h"

auto B = A();

int main()
{
  std::cout << B << std::endl;
  return EXIT_SUCCESS;
}
```



编译:`g++ --std=c++11 a.cpp b.cpp -o test`