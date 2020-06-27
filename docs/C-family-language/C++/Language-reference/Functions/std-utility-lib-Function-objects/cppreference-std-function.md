[TOC]

# [std::function](https://en.cppreference.com/w/cpp/utility/functional/function)

Class template `std::function` is a general-purpose polymorphic function wrapper. Instances of `std::function` can store, copy, and invoke any [*Callable*](https://en.cppreference.com/w/cpp/named_req/Callable) *target* -- functions, [lambda expressions](https://en.cppreference.com/w/cpp/language/lambda), [bind expressions](https://en.cppreference.com/w/cpp/utility/functional/bind), or other function objects, as well as pointers to member functions and pointers to data members.

***SUMMARY*** : 上面这段话中的polymorphic 是非常重要的，它表明`std::function`是支持多态的；这是`c++`对functional programming的支持；

The stored callable object is called the *target* of `std::function`. If a `std::function` contains no target, it is called *empty*. Invoking the *target* of an *empty* `std::function` results in [std::bad_function_call](https://en.cppreference.com/w/cpp/utility/functional/bad_function_call) exception being thrown.

***SUMMARY*** : the stored callable object和`std::function`之间的关系就好比thread和`std::thread`之间的关系。

`std::function` satisfies the requirements of [*CopyConstructible*](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) and [*CopyAssignable*](https://en.cppreference.com/w/cpp/named_req/CopyAssignable).

***SUMMARY*** : `std::function`它满足  [*CopyConstructible*](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) 和[*CopyAssignable*](https://en.cppreference.com/w/cpp/named_req/CopyAssignable) ，这传达出的一个semantic是两个 `std::function` object能够表示相同的target；想想，`std::function`是否是 [*MoveConstructible*](https://en.cppreference.com/w/cpp/named_req/MoveConstructible) 和 [*MoveAssignable*](https://en.cppreference.com/w/cpp/named_req/MoveAssignable) 的呢？





### Example

```c++
#include <functional>
#include <iostream>
 
struct Foo {
    Foo(int num) : num_(num) {}
    void print_add(int i) const { std::cout << num_+i << '\n'; }
    int num_;
};
 
void print_num(int i)
{
    std::cout << i << '\n';
}
 
struct PrintNum {
    void operator()(int i) const
    {
        std::cout << i << '\n';
    }
};
 
int main()
{
    // store a free function
    std::function<void(int)> f_display = print_num;
    f_display(-9);
 
    // store a lambda
    std::function<void()> f_display_42 = []() { print_num(42); };
    f_display_42();
 
    // store the result of a call to std::bind
    std::function<void()> f_display_31337 = std::bind(print_num, 31337);
    f_display_31337();
 
    // store a call to a member function
    std::function<void(const Foo&, int)> f_add_display = &Foo::print_add;
    const Foo foo(314159);
    f_add_display(foo, 1);
    f_add_display(314159, 1);// 后首先调用Foo(314159)构造一个Foo的对象
 
    // store a call to a data member accessor
    std::function<int(Foo const&)> f_num = &Foo::num_;
    std::cout << "num_: " << f_num(foo) << '\n';
 
    // store a call to a member function and object
    using std::placeholders::_1;
    std::function<void(int)> f_add_display2 = std::bind( &Foo::print_add, foo, _1 );
    f_add_display2(2);
 
    // store a call to a member function and object ptr
    std::function<void(int)> f_add_display3 = std::bind( &Foo::print_add, &foo, _1 );
    f_add_display3(3);
 
    // store a call to a function object
    std::function<void(int)> f_display_obj = PrintNum();
    f_display_obj(18);
}
```





# [std::function::function](https://en.cppreference.com/w/cpp/utility/functional/function/function)

Constructs a `std::function` from a variety of sources.

1-2) Creates an *empty* function.

3-4) Copies (3) or moves (4) the *target* of `other` to the *target* of `*this`（copy和move的都是 `other` 的target）. If  `other`  is *empty*, `*this` will be *empty* after the call too. For (4), `other` is in a valid but unspecified state after the call.

5) Initializes the *target* with `std::move(f)`. If `f` is a null pointer to function or null pointer to member, `*this` will be *empty* after the call. This constructor does not participate in **overload resolution** unless `f` is [*Callable*](https://en.cppreference.com/w/cpp/named_req/Callable) for argument types `Args...` and return type `R`. (since C++14)



When the *target* is a function pointer or a [std::reference_wrapper](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper), **small object optimization** is guaranteed, that is, these targets are always directly stored inside the [std::function](https://en.cppreference.com/w/cpp/utility/functional/function) object, no dynamic allocation takes place. Other large objects may be constructed in dynamic allocated storage and accessed by the [std::function](https://en.cppreference.com/w/cpp/utility/functional/function) object through a pointer.



# [std::function::operator=](https://en.cppreference.com/w/cpp/utility/functional/function/operator%3D)

|                                                              |      |
| ------------------------------------------------------------ | ---- |
| function& operator=( const function& other );                | (1)  |
| function& operator=( function&& other );                     | (2)  |
| function& operator=( [std::nullptr_t](http://en.cppreference.com/w/cpp/types/nullptr_t) ); | (3)  |
| template< class F >  function& operator=( F&& f );           | (4)  |
| template< class F >  function& operator=( [std::reference_wrapper](http://en.cppreference.com/w/cpp/utility/functional/reference_wrapper)<F> f ); | (5)  |

Assigns a new *target* to `std::function`.

1) Assigns a copy of *target* of `other`, as if by executing `function(other).swap(*this)`;

***THINKING*** : `function(other).swap(*this)` 首先使用`function(other)`构造一个临时对象，然后和`*this`进行`swap` 。这应该是[copy-and-swap idiom](https://en.wikibooks.org/wiki/More_C++_Idioms/Copy-and-swap)的应用。

2) Moves the *target* of `other` to `*this`. `other` is in a valid state with an unspecified value.

3) Drops the current *target*. `*this` is *empty* after the call.

4) Sets the *target*  of `*this`  to the callable `f`, as if by executing `function<F>(f).swap(*this)`;. This operator does not participate in overload resolution unless `f` is [*Callable*](https://en.cppreference.com/w/cpp/named_req/Callable) for argument types `Args...` and return type `R`. (since C++14)

5) Sets the *target* of `*this` to a copy of `f`, as if by executing `function(f).swap(*this)`;

