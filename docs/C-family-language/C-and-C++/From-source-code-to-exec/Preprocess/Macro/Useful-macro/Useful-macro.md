# Useful macro



## [3 Types of Macros That Improve C++ Code](https://www.fluentcpp.com/2019/05/14/3-types-of-macros-that-improve-c-code/)



### Useful macro #1: The macro that bridges a gap between two C++ features

```c++
#define DEFINE_VISITABLE() \
   virtual ReturnType Accept(BaseVisitor& guest) \
   { return AcceptImpl(*this, guest); }
```



### Useful macro #2: The macro that shortens a redundant expression

#### `FWD`

来源：Vittorio Romeo’s `FWD` macro

```c++
template<typename MyType, typename MyOtherType>
void f(MyType&& myValue, MyOtherType&& myOtherValue)
{
    g(std::forward<MyType>(myValue), std::forward<MyOtherType>(myOtherValue));
}
```

Vittorio proposes to use the following macro:

```c++
#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
```

Here is how the previous code now looks like by using it:

```c++
template<typename MyType, typename MyOtherType>
void f(MyType&& myValue, MyOtherType&& myOtherValue)
{
    g(FWD(myValue), FWD(myOtherValue));
}
```
#### `noexcept(noexcept(`

来源：item 14 of [Effective Modern C++](https://www.amazon.com/gp/product/1491903996/ref=as_li_tl?ie=UTF8&camp=1789&creative=9325&creativeASIN=1491903996&linkCode=as2&tag=fluentcpp-20&linkId=c3922df74051882502a2d72f2e0e7f28)





## macro function





https://stackoverflow.com/questions/163365/how-do-i-make-a-c-macro-behave-like-a-function



## [C++ Use macros to define class member of a single class](https://stackoverflow.com/questions/42367626/c-use-macros-to-define-class-member-of-a-single-class)





## [Can I comment multi-line macros?](https://stackoverflow.com/questions/24751387/can-i-comment-multi-line-macros)