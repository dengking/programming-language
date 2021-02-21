# Template template parameter

 Template template parameter的含义是使用一个template来作为template parameter。

## modernescpp [Types-, Non-Types, and Templates as Template Parameters](https://www.modernescpp.com/index.php/types-non-types-and-templates-as-template-parameters)

> 这篇文章不错，其实的例子非常好。

```c++
// templateTemplateParameters.cpp

#include <iostream>
#include <list>
#include <vector>
#include <string>

template <typename T, template <typename, typename> class Cont >   // (1)
class Matrix{
public:
  explicit Matrix(std::initializer_list<T> inList): data(inList){  // (2)
    for (auto d: data) std::cout << d << " ";
  }
  int getSize() const{
    return data.size();
  }

private:
  Cont<T, std::allocator<T>> data;                                 // (3)                               

};

int main(){

  std::cout << std::endl;

                                                                    // (4)
  Matrix<int, std::vector> myIntVec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; 
  std::cout << std::endl;
  std::cout << "myIntVec.getSize(): " << myIntVec.getSize() << std::endl;

  std::cout << std::endl;

  Matrix<double, std::vector> myDoubleVec{1.1, 2.2, 3.3, 4.4, 5.5}; // (5)
  std::cout << std::endl;
  std::cout << "myDoubleVec.getSize(): "  << myDoubleVec.getSize() << std::endl;

  std::cout << std::endl;
                                                                    // (6)
  Matrix<std::string, std::list> myStringList{"one", "two", "three", "four"};  
  std::cout << std::endl;
  std::cout << "myStringList.getSize(): " << myStringList.getSize() << std::endl;

  std::cout << std::endl;

}
```

### With default template argument

```c++
template<typename PushFieldType, template<typename = PushFieldType> typename  PushListType>
void UpdatePushList(PushFieldType* pField,  PushListType<PushFieldType> & PushList)
{

}
```



## stackoverflow [What are some uses of template template parameters?](https://stackoverflow.com/questions/213761/what-are-some-uses-of-template-template-parameters)

> NOTE: 非常好的文章

I've seen some examples of C++ using template template parameters (that is templates which take templates as parameters) to do policy-based class design. What other uses does this technique have?

### [A](https://stackoverflow.com/a/213811)

I think you need to use template template syntax to pass a parameter whose type is a template dependent on another template like this:

```cpp
template <template<class> class H, class S>
void f(const H<S> &value) {
}
```

Here, `H` is a template, but I wanted this function to deal with all specializations of `H`.

**NOTE**: I've been programming c++ for many years and have only needed this once. I find that it is a rarely needed feature (of course handy when you need it!).

I've been trying to think of good examples, and to be honest, most of the time this isn't necessary, but let's contrive(设计、发明) an example. Let's pretend that `std::vector` *doesn't* have a `typedef value_type`.

So how would you write a function which can create variables of the right type for the vectors elements? This would work.

```cpp
template <template<class, class> class V, class T, class A>
void f(V<T, A> &v) {
    // This can be "typename V<T, A>::value_type",
    // but we are pretending we don't have it

    T temp = v.back();
    v.pop_back();
    // Do some work on temp

    std::cout << temp << std::endl;
}
```

**NOTE**: `std::vector` has two template parameters, type, and allocator, so we had to accept both of them. Fortunately, because of type deduction, we won't need to write out the exact type explicitly.

which you can use like this:

```cpp
f<std::vector, int>(v); // v is of type std::vector<int> using any allocator
```

or better yet, we can just use:

```cpp
f(v); // everything is deduced, f can deal with a vector of any type!
```

**UPDATE**: Even this contrived example, while illustrative, is no longer an amazing example due to c++11 introducing `auto`. Now the same function can be written as:

```cpp
template <class Cont>
void f(Cont &v) {

    auto temp = v.back();
    v.pop_back();
    // Do some work on temp

    std::cout << temp << std::endl;
}
```

which is how I'd prefer to write this type of code.

### [A](https://stackoverflow.com/a/214900)

Here is a simple example taken from ['Modern C++ Design - Generic Programming and Design Patterns Applied'](https://rads.stackoverflow.com/amzn/click/com/0201704315) by Andrei Alexandrescu:

He uses a classes with template template parameters in order to implement the policy pattern:

```cpp
// Library code
template <template <class> class CreationPolicy>
class WidgetManager : public CreationPolicy<Widget>
{
   ...
};
```

He explains: *Typically, the host class already knows, or can easily deduce, the template argument of the policy class. In the example above, WidgetManager always manages objects of type Widget, so requiring the user to specify Widget again in the instantiation of CreationPolicy is redundant and potentially dangerous.In this case, library code can use template template parameters for specifying policies.*

The effect is that the client code can use 'WidgetManager' in a more elegant way:

```cpp
typedef WidgetManager<MyCreationPolicy> MyWidgetMgr;
```

Instead of the more cumbersome, and error prone way that a definition lacking template template arguments would have required:

```cpp
typedef WidgetManager< MyCreationPolicy<Widget> > MyWidgetMgr;
```



### [A](https://stackoverflow.com/a/14311714)

> NOTE: generic container printer

Actually, usecase for template template parameters is rather obvious. Once you learn that C++ stdlib has gaping hole of not defining stream output operators for standard container types, you would proceed to write something like:

```cpp
template<typename T>
static inline std::ostream& operator<<(std::ostream& out, std::list<T> const& v)
{
    out << '[';
    if (!v.empty()) {
        for (typename std::list<T>::const_iterator i = v.begin(); ;) {
            out << *i;
            if (++i == v.end())
                break;
            out << ", ";
        }
    }
    out << ']';
    return out;
}
```

Then you'd figure out that code for vector is just the same, for forward_list is the same, actually, even for multitude of map types it's still just the same. Those template classes don't have anything in common except for meta-interface/protocol, and using template template parameter allows to capture the commonality in all of them. Before proceeding to write a template though, it's worth to check a reference to recall that sequence containers accept 2 template arguments - for value type and allocator. While allocator is defaulted, we still should account for its existence in our template operator<<:

```cpp
template<template <typename, typename> class Container, class V, class A>
std::ostream& operator<<(std::ostream& out, Container<V, A> const& v)
...
```

Voila, that will work automagically for all present and future sequence containers adhering to the standard protocol. To add maps to the mix, it would take a peek at reference to note that they accept 4 template params, so we'd need another version of the operator<< above with 4-arg template template param. We'd also see that std:pair tries to be rendered with 2-arg operator<< for sequence types we defined previously, so we would provide a specialization just for std::pair.

Btw, with C+11 which allows variadic templates (and thus should allow variadic template template args), it would be possible to have single operator<< to rule them all. For example:

```cpp
#include <iostream>
#include <vector>
#include <deque>
#include <list>

template<typename T, template<class,class...> class C, class... Args>
std::ostream& operator <<(std::ostream& os, const C<T,Args...>& objs)
{
    os << __PRETTY_FUNCTION__ << '\n';
    for (auto const& obj : objs)
        os << obj << ' ';
    return os;
}

int main()
{
    std::vector<float> vf { 1.1, 2.2, 3.3, 4.4 };
    std::cout << vf << '\n';

    std::list<char> lc { 'a', 'b', 'c', 'd' };
    std::cout << lc << '\n';

    std::deque<int> di { 1, 2, 3, 4 };
    std::cout << di << '\n';

    return 0;
}
```

**Output**

```cpp
std::ostream &operator<<(std::ostream &, const C<T, Args...> &) [T = float, C = vector, Args = <std::__1::allocator<float>>]
1.1 2.2 3.3 4.4 
std::ostream &operator<<(std::ostream &, const C<T, Args...> &) [T = char, C = list, Args = <std::__1::allocator<char>>]
a b c d 
std::ostream &operator<<(std::ostream &, const C<T, Args...> &) [T = int, C = deque, Args = <std::__1::allocator<int>>]
1 2 3 4 
```



## stackoverflow [Templated class specialization where template argument is a template](https://stackoverflow.com/questions/4189945/templated-class-specialization-where-template-argument-is-a-template)

Google template template specialization	



