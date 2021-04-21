# pabloariasal [C++ - Initialization of Static Variables](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/)

> NOTE: 写的非常好的一篇文章

## [Static Variables](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#static-variables)

As the compiler translates your program it must decide how to deal with variables introduced: When should a variable be initialized? What’s the initial value? When should it be destroyed?

> NOTE: 其实，这所讨论的就是lifetime of object的问题，这在`C++\Language-reference\Basic-concept\Object\Lifetime-and-storage-duration`章节中已经进行了讨论。

Most of the time the compiler must deal with *dynamic* variables, i.e. variables that are initialized and destroyed at runtime: local (block-scope) variables, function arguments, non-static class members, etc.

The compiler has little chance to initialize such variables before execution starts: How is it supposed to know what arguments will be passed to a function?, or if a given code block will be reached? The answers may even vary from execution to execution, so their initialization and destruction must happen on demand, at runtime, *dynamically*.

> NOTE: automatic storage duration和dynamic storage duration

There is, however, a category of variables that can (and should) be initialized before the program starts: **static variables**. Global (namespace) variables or static class members[1](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#fn:2) live for the entire execution of the program: they must be initialized before `main()` is run and destroyed after execution finishes. Such variables have *static storage duration*.

The lifetime of static variables doesn’t depend on the execution: they always exist; forever; no matter what. This leads to the beautiful property that they can be potentially evaluated and initialized at compile time.

## [The Two Stages of Static Variable Initialization](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#the-two-stages-of-static-variable-initialization)

As discussed, variables with *static storage duration* must be initialized once before the program starts and destroyed after execution terminates.

Initialization of static variables happens in two consecutive stages: *static* and *dynamic* initialization.

### Static initialization

**Static initialization** happens first and usually at **compile time**. If possible, initial values for static variables are evaluated during compilation and burned into the data section of the executable. Zero runtime overhead, early problem diagnosis, and, as we will see later, safe. This is called [constant initialization](https://en.cppreference.com/w/cpp/language/constant_initialization). In an ideal world all static variables are const-initialized.

If the initial value of a static variable can’t be evaluated at compile time, the compiler will perform **zero-initialization**. Hence, during **static initialization** all **static variables** are either **const-initialized** or **zero-initialized**.

> NOTE: 
>
> 一、总的来说，static initialization执行如下两种initialization之一:
>
> 1、constant initialization
>
> 2、zero initialization

### Dynamic initialization

After **static initialization**, **dynamic initialization** takes place. **Dynamic initialization** happens at runtime for variables that can’t be evaluated at compile time[2](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#fn:1). Here, **static variables** are initialized every time the executable is run and not just once during compilation.



## [The Green Zone - Constant Initialization](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#the-green-zone---constant-initialization)



## [Force Const Initialization with `constexpr`](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#force-const-initialization-with-constexpr)

One big problem with static variable initialization is that it is not always clear if a variable is being initialized at compile time or at runtime.

One way to make sure that variables are const-initialized (i.e. compile time) is by declaring them `constexpr`, this will force the compiler to treat them as constant expressions and perform their evaluation and initialization at compile time.

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



## [Your Second Line of Defense: `constinit`](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#your-second-line-of-defense-constinit)

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

## [The Yellow Zone - Dynamic Initialization](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#the-yellow-zone---dynamic-initialization)

Imagine you need an immutable global `std::string` to store the software version. You probably don’t want this object to be instantiated every time the program runs, but rather create it once and embed it into the executable as read-only memory. In other words, you want `constexpr`:

> NOTE: 
>
> 1、其实就是constant initialization

```c++
#include <string>
static constexpr auto VERSION = std::string("3.4.1");

int main()
{
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra

```

> NOTE: 
>
> 1、编译报错如下:
>
> ```C++
> test.cpp:2:52: error: the type 'const std::__cxx11::basic_string<char>' of 'constexpr' variable 'VERSION' is not literal
>  static constexpr auto VERSION = std::string("3.4.1");
>                                                     ^
> In file included from /usr/include/c++/8/string:52,
>                  from test.cpp:1:
> /usr/include/c++/8/bits/basic_string.h:77:11: note: 'std::__cxx11::basic_string<char>' is not literal because:
>      class basic_string
>            ^~~~~~~~~~~~
> /usr/include/c++/8/bits/basic_string.h:77:11: note:   'std::__cxx11::basic_string<char>' has a non-trivial destructor
> 
> ```
>
> 

But life isn’t that easy:

```c++
error: constexpr variable cannot have non-literal type
```

The compiler is complaining because `std::string` defines a non-trivial destructor. That means that `std::string` is probably allocating some resource that must be freed upon destruction, in this case memory. This is a problem, if we create an `std::string` at compile time the managed memory must be somehow copied into the binary as well, as it won’t be available when the executable is run!

In other words, `std::string("3.4.1")` is not a constant expression so we can’t force the compiler to const-initialize it!

> NOTE: 
>
> 1、constexpr、literal type、trivial的内容非常好
>
> 2、上述解释是非常好的，下面是一些快速总结:
>
>  `std::string` allocate dynamic memory，这是在compile time无法做到的
>
>  `std::string` 不是literal type

We must give up:

```C++
const auto VERSION = std::string("3.4.1");
```

Your compiler is happy, but for the price of moving the initialization from compile time to runtime. `VERSION` must be initialized as part of dynamic initialization, not static.

> NOTE: 
>
> 1、CppCoreGuidelines-Per.11 Move computation from run time to compile time

Good news is that at runtime we can allocate memory. Bad news is that this isn’t as nice, safe, or efficient as static initialization, but isn’t inherently bad either. Dynamic initialization will be inevitable in some cases, not everything can be done at compile time, for example:

> NOTE: 
>
> 1、tradeoff: compile time computation and run time computation

```C++
// generate a random number every time executable is run
const auto RANDOM = generateRandomNumber();

```



The future looks bright, however. Even when moving slowly, smart people have been working on several proposals to augment the capabilities of `constexpr` to types like `std::string` or `std::vector`. The idea here is to allow for memory allocations at compile time and later flash the object alongside its managed memory into the data section of the binary.

> NOTE: 
>
> 1、好像那个版本已经支持了，TODO

## [The Red Zone - Static Initialization Order Fiasco](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#the-red-zone---static-initialization-order-fiasco)

Dynamic initialization of static variables suffers from a very scary defect: the order in which variables are initialized at runtime is not always well-defined.

Within a single compilation unit, static variables are initialized in the same order as they are defined in the source (this is called *Ordered Dynamic Initialization*). Across compilation units, however, the order is undefined: you don’t know if a static variable defined in `a.cpp` will be initialized before or after one in `b.cpp`.

This turns into a very serious issue if the initialization of a variable in `a.cpp` depends on another one defined `b.cpp` . This is called the [Static Initialization Order Fiasco](https://isocpp.org/wiki/faq/ctors#static-init-order). Consider this example:

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

## [Solving The Static Initialization Order Fiasco](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#solving-the-static-initialization-order-fiasco)

Encountering the **static initialization order fiasco** is often a symptom of poor software design. IMHO the best way to solve it is by refactoring the code to break the initialization dependency of **globals** across **compilation units**. Make your modules self-contained and strive for **constant initialization**.

If refactoring is not an option, one common solution is the [Initialization On First Use](https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use). The basic idea is to design your static variables that are not constant expressions (i.e. those that must be initialized at runtime) in a way that they are created *when they are accessed for the first time*. This approach uses a static local variable inspired by the [Meyer’s Singleton](http://laristra.github.io/flecsi/src/developer-guide/patterns/meyers_singleton.html). With this strategy it is possible to control the time when static variables are initialized at runtime, avoiding use-before-init.

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