# Macro



## Debug macro

### `gcc -E`

https://stackoverflow.com/a/4575001

## Trick

本节讨论在定义macro的时候的一些技巧



### `do{}while(0)` in macro

https://stackoverflow.com/questions/154136/why-use-apparently-meaningless-do-while-and-if-else-statements-in-macros

https://stackoverflow.com/questions/257418/do-while-0-what-is-it-good-for

https://stackoverflow.com/questions/154136/why-use-apparently-meaningless-do-while-and-if-else-statements-in-macros

https://stackoverflow.com/questions/1067226/c-multi-line-macro-do-while0-vs-scope-block





## Useful macro



### fluentcpp [3 Types of Macros That Improve C++ Code](https://www.fluentcpp.com/2019/05/14/3-types-of-macros-that-improve-c-code/)



#### Useful macro #1: The macro that bridges a gap between two C++ features

```c++
#define DEFINE_VISITABLE() \
   virtual ReturnType Accept(BaseVisitor& guest) \
   { return AcceptImpl(*this, guest); }
```



#### Useful macro #2: The macro that shortens a redundant expression

##### `FWD`

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

##### `noexcept(noexcept(`

来源：item 14 of [Effective Modern C++](https://www.amazon.com/gp/product/1491903996/ref=as_li_tl?ie=UTF8&camp=1789&creative=9325&creativeASIN=1491903996&linkCode=as2&tag=fluentcpp-20&linkId=c3922df74051882502a2d72f2e0e7f28)





### macro function

https://stackoverflow.com/questions/163365/how-do-i-make-a-c-macro-behave-like-a-function

[C++ Use macros to define class member of a single class](https://stackoverflow.com/questions/42367626/c-use-macros-to-define-class-member-of-a-single-class)

[Can I comment multi-line macros?](https://stackoverflow.com/questions/24751387/can-i-comment-multi-line-macros)





### Use macro to define function

对于template class，如果将function definition放到单独的文件，则需要使用如下语法

```c++
template<typename T1, typename T2...>
void C<T1, T2...>::Function()
{

}
```

如果有多个template parameter，则写起来就非常不便利，那么如何来缓解这个问题呢？使用c++ macro来进行实现：

```c++
/**
 * 模板声明的宏
 */
#define TEMPLATE_DECLARATION template <typename T1,	typename T2, typename T3, ...>

/**
 * 模板参数声明
 */
#define TEMPLATE_PARAMETER_DECLARATION T1, T2, T3 ...

/**
 * 定义成员方法的宏
 */
#define DEFINE_METHOD(RETURN, METHOD) \
	TEMPLATE_DECLARATION              \
	RETURN ClassName<TEMPLATE_PARAMETER_DECLARATION>::METHOD
```



下面是一个简单的例子：

```c++
#define DEFINE_METHOD(METHOD) template<typename T>\
C<T>::METHOD \

template<typename T>
class C
{
public:
	C();
	T m_t;
};
DEFINE_METHOD(C())
{m_t = 0;
}

int main()
{

}

```