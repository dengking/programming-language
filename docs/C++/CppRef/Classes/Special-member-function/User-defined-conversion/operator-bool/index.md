# `operator bool` and safe bool problem



## stackoverflow [What is the meaning of “operator bool() const”](https://stackoverflow.com/questions/4600295/what-is-the-meaning-of-operator-bool-const)





## The safe bool problem

### cppreference [Implicit conversions](https://en.cppreference.com/w/cpp/language/implicit_conversion) # The safe bool problem

Until the introduction of **explicit conversion functions** in C++11, designing a class that should be usable in boolean contexts (e.g. `if(obj) { ... }`) presented a problem: given a user-defined conversion function, such as `T::operator bool() const;`, the implicit conversion sequence allowed one additional standard conversion sequence after that function call, which means the resultant `bool` could be converted to `int`, allowing such code as `obj << 1;` or `int i = obj;`.

One early solution for this can be seen in [std::basic_ios](https://en.cppreference.com/w/cpp/io/basic_ios), which defines `operator!` and operator `void*`(until C++11), so that the code such as `if(std::cin) {...}` compiles because `void*` is convertible to `bool`, but int `n = std::cout;` does not compile because `void*` is not convertible to `int`. This still allows nonsense code such as `delete std::cout;` to compile, and many pre-C++11 third party libraries were designed with a more elaborate solution, known as the [Safe Bool idiom](http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Safe_bool).

#### (since C++11)

The [explicit bool conversion](https://en.cppreference.com/w/cpp/language/explicit) can also be used to resolve the safe bool problem`explicit operator bool() const { ... }`



### stackoverflow [Is the safe-bool idiom obsolete in C++11?](https://stackoverflow.com/questions/6242768/is-the-safe-bool-idiom-obsolete-in-c11)



```C++
explicit operator bool() const;
```

> NOTE:
>
> 1、simplify C++



#### [A](https://stackoverflow.com/a/6242777)

Yes. This is *the* example for problems with only having implicit user-defined conversions and explicit user-defined conversion operators were practically [invented because of this problem](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2333.html) and to replace all the safe-bool stuff with something a lot cleaner and more logical.





### stackoverflow [Conversion function for error checking considered good?](https://stackoverflow.com/questions/6242296/conversion-function-for-error-checking-considered-good)



I'd like to have a simple way of checking for an object to be valid. I thought of a simple conversion function, something like this:

```cpp
operator bool() const { return is_valid; }
```

Checking for it to be valid would be very simple now

```cpp
// is my object invalid?
if (!my_object) std::cerr << "my_object isn't valid" << std::endl;
```

Is this considered a good practise?

#### [A](https://stackoverflow.com/a/6242355)

In C++03, you need to use the [safe bool idiom](http://www.artima.com/cppsource/safebool.html) to avoid evil things:

```cpp
int x = my_object; // this works
```

In C++11 you can use an explicit conversion:

```cpp
explicit operator bool() const
{
    // verify if valid
    return is_valid;
}
```

This way you need to be explicit about the conversion to bool, so you can no longer do crazy things by accident (in C++ you can always do crazy things on purpose):

```cpp
int x = my_object; // does not compile because there's no explicit conversion
bool y = bool(my_object); // an explicit conversion does the trick
```

> NOTE: 
>
> 1、体现了C++的灵活、自由

This still works as normal in places like `if` and `while` that require a boolean expression, because the condition of those statements is *contextually converted* to bool:

```cpp
// this uses the explicit conversion "implicitly"
if (my_object)
{
    ...
}
```