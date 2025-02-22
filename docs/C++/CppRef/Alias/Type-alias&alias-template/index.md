# Type alias, alias template (since C++11) 

“type alias”即“类型别名”，即给一个类型，起一个新名字。

## cppreference [Type alias, alias template (since C++11)](https://en.cppreference.com/w/cpp/language/type_alias)

**Type alias** is a name that refers to a previously defined type (similar to [`typedef`](https://en.cppreference.com/w/cpp/language/typedef)).

**Alias template** is a name that refers to a family of types.

### Example

```c++
#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>

// type alias, identical to
// typedef std::ios_base::fmtflags flags;
using flags = std::ios_base::fmtflags;
// the name 'flags' now denotes a type:
flags fl = std::ios_base::dec;

// type alias, identical to
// typedef void (*func)(int, int);
using func = void (*)(int, int);

// the name 'func' now denotes a pointer to function:
void example(int, int) {}

func f = example;

// alias template
template<class T>
using ptr = T *;
// the name 'ptr<T>' is now an alias for pointer to T
ptr<int> x;

// type alias used to hide a template parameter
template<class CharT>
using mystring = std::basic_string<CharT, std::char_traits<CharT>>;

mystring<char> str;

// type alias can introduce a member typedef name
template<typename T>
struct Container {
    using value_type = T;
};

// which can be used in generic programming
template<typename ContainerT>
void info(const ContainerT &c) {
    typename ContainerT::value_type T;
    std::cout << "ContainerT is `" << typeid(decltype(c)).name() << "`\n"
                                                                    "value_type is `" << typeid(T).name() << "`\n";
}

// type alias used to simplify the syntax of std::enable_if
template<typename T>
using Invoke = typename T::type;

template<typename Condition>
using EnableIf = Invoke<std::enable_if<Condition::value>>;

template<typename T, typename = EnableIf<std::is_polymorphic<T>>>
int fpoly_only(T) { return 1; }

struct S {
    virtual ~S() {}
};

int main() {
    Container<int> c;
    info(c); // Container::value_type will be int in this function
//  fpoly_only(c); // error: enable_if prohibits this
    S s;
    fpoly_only(s); // okay: enable_if allows this
}
```





## C++ using

### cppreference  [using](https://en.cppreference.com/w/cpp/keyword/using) keyword

> NOTE: C++11引入了`using`关键字。



### See also

internalpointers [The differences between "using" and "typedef" in modern C++](https://www.internalpointers.com/post/differences-between-using-and-typedef-modern-c)

> **A template declaration cannot appear at block scope**: 
>
> Alias templates on the other hand follow the same rules of any other templated thing in C++: they cannot appear inside a block. They are actual template declarations, after all!

stackoverflow [What is the difference between 'typedef' and 'using' in C++11?](https://stackoverflow.com/questions/10747810/what-is-the-difference-between-typedef-and-using-in-c11)



## Aliase template 

有两种实现方式：

1、C++11 using

2、[Type-Generator idiom](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Generator)（参见`Type-Generator`章节）

### C++11 aliase template

#### wikipedia [C++11#Template aliases](https://en.wikipedia.org/wiki/C%2B%2B11#Template_aliases)

> NOTE: 它介绍了引入`using`的原因



#### Example

C++11的`using`就是用来解决这个问题的，在internalpointers [The differences between "using" and "typedef" in modern C++](https://www.internalpointers.com/post/differences-between-using-and-typedef-modern-c)中对此进行了介绍。

```c++
template<typename T1, typename T2> 
using Map = std::map<T1, std::vector<T2>>;
```

[C++ template typedef](https://stackoverflow.com/questions/2795023/c-template-typedef)

```c++
template <size_t N>
using Vector = Matrix<N, 1>;
```



### Pre-C++11: type generator idiom



#### [More C++ Idioms/Type Generator](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Generator)



```c++
#include <map>
#include <string>
#include <iostream>

template<class Value>
struct Directory {
    typedef std::map<std::string, Value, std::less<std::string>/*, __gnu_cxx::malloc_allocator<std::string>*/ > type;
};


int main() {
    Directory<int>::type;    // gives a map of string to integers.
    Directory<float>::type;  // gives a map of string to floats.
    Directory<int>::type age; // This is a map.
    std::transform(
            age.begin(),
            age.end(),
            std::ostream_iterator<std::string>(std::cout, "\n"),
            _Select1st<std::map<std::string, int>::value_type>()
    );
}

```



```c++
#include <map>
#include <string>
#include <iostream>

template<class Value>
struct Directory {
    typedef std::map<std::string, Value, std::less<std::string>/*, __gnu_cxx::malloc_allocator<std::string>*/> type;
    typedef _Select1st<typename type::value_type> KeySelector;
    typedef _Select2nd<typename type::value_type> ValueSelector;
};

int main() {
    Directory<int>::type age;    // This is a map.
    std::transform(
            age.begin(),
            age.end(),
            std::ostream_iterator<std::string>(std::cout, "\n"),
            Directory<int>::KeySelector()
    );
}
```



### Examples

#### C++ aliases for partial templates

stackoverflow [C++ typedef for partial templates](https://stackoverflow.com/questions/2996914/c-typedef-for-partial-templates)

```c++
#include <vector>

template<class A, class B, class C>
class X {
};

template<class B, class C>
typedef X<std::vector<B>, B, C> Y;
```

上述程序是存在语法错误的，下面是正确的写法:

##### C++11 `using` aliase template

```c++
#include <vector>

template<class A, class B, class C>
class X {
};

template<typename B, typename C>
using Y = X<std::vector<B>, B, C>;
```

##### Pre-C++11: type generator idiom

```c++
#include <vector>

template<class A, class B, class C>
class X {
};

template<class B, class C>
struct Y {
    typedef X<std::vector<B>, B, C> type;
};
```



#### Example

在drdobbs [The New C++: Typedef Templates](https://www.drdobbs.com/the-new-c-typedef-templates/184403850)中对上述这种不使用c++11 `using`的写法进行了介绍，下面是其中给出的example

```c++
template<typename T>
struct SharedPtr {
    typedef Loki::SmartPtr
            <
                    T,                // note, T still varies
                    RefCounted,       // but everything else is fixed
                    NoChecking,
                    false,
                    PointsToOneObject,
                    SingleThreaded,
                    SimplePointer<T> // note, T can be used as here
            >
            Type;
};

SharedPtr<int>::Type p; // sample usage, "::Type" is ugly
```



#### Example

probablydance [Alias templates with partial specialization, SFINAE and everything](https://probablydance.com/2014/01/16/alias-templates-with-partial-specialization-sfinae-and-everything/)

```c++
template<typename T>
using MyVector = std::vector<T, MyAllocator<T>>; // alias-declaration
MyVector<T> myvector; // instantiating with MyAllocator;
```



```c++
template<size_t Size, typename T>
struct Vector
{
    T elements[Size];
    //...
    Vector operator*(const T & rhs) const
    {
        Vector copy(*this);
        for (T & element : copy.elements)
        {
            element *= rhs;
        }
        return copy;
    }
    //...
};

typedef Vector<2, float> Vec2f;
typedef Vector<3, float> Vec3f;
```



To get my code to compile I would either have to define my multiplication operator above twice, or I would have to add a conversion operator to `Vector<1, float>`. Both of which I didn’t want to do, because really I wanted `Vector<1, float>` to be the same as just `float`. Meaning I wanted

```c++
static_assert(std::is_same<Vector<1, float>, float>::value, "a scalar is a scalar");
```

So this is where I wanted an alias-declaration with a partial specialization. Which the standard forbids. Turns out you can solve that by adding a layer of indirection. This is my solution:

```c++
namespace detail
{
template<size_t Size, typename T>
struct Vector
{
    // the code from above goes here
};
template<size_t Size, typename T>
struct VectorTypedef
{
    typedef Vector<Size, T> type;
};
template<typename T>
struct VectorTypedef<1, T>
{
    typedef T type;
};
}
template<size_t Size, typename T>
using Vector = typename detail::VectorTypedef<Size, T>::type;
```

Which is exactly what I wanted. If you think about this, this actually makes templates more powerful than they were in C++03. In C++03 you could create templates that were completely different structs depending on the template arguments,like `std::vector<bool>` vs. `std::vector<unsigned char>`, but the classes would always have to be related. You could never have an unrelated type from a library or a fundamental type behind a template. Now you can.



####  Example: introduce a member typedef name

在例子中演示了这种写法：

https://en.cppreference.com/w/cpp/language/type_alias

```c++
// type alias can introduce a member typedef name
template<typename T>
struct Container { using value_type = T; };
```

下面是一个demo

```c++
template<class T>
class C {
    using TT = T;
};

int main() {}
```



### C++14 TransformationTrait

参见 `C++\What-is-new-in-C++\C++14\TransformationTrait` 章节。

