# Constant initialization

## pabloariasal [C++ - Initialization of Static Variables](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/)

**Static initialization** happens first and usually at **compile time**. If possible, initial values for static variables are evaluated during compilation and burned into the data section of the executable. Zero runtime overhead, early problem diagnosis, and, as we will see later, safe. This is called [constant initialization](https://en.cppreference.com/w/cpp/language/constant_initialization). In an ideal world all static variables are const-initialized.



### Force Const Initialization with `constexpr`

One big problem with static variable initialization is that it is not always clear if a variable is being initialized at compile time or at runtime.

One way to make sure that variables are const-initialized (i.e. compile time) is by declaring them `constexpr`, this will force the compiler to treat them as constant expressions and perform their evaluation and initialization at compile time.

### Your Second Line of Defense: `constinit`[Permalink](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/#your-second-line-of-defense-constinit)

`constinit` is a keyword introduced in the c++20 standard. It works just as `constexpr`, as it forces the compiler to evaluate a variable at compile time, but with the difference that it **doesn’t imply const**.

As a consequence, variables declared `constinit` are always const-(or zero-)initialized, but can be mutated at runtime, i.e. don’t land in read-only data section in the binary.

## cppreference [Constant initialization](https://en.cppreference.com/w/cpp/language/constant_initialization)