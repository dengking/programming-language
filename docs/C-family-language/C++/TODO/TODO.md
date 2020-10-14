# TODO

## trailing return type

越来越多的programming language支持trailing return type，比如:

1) C++

2) python

3) Rust

下面是关于它的一些文章:

arne-mertz [Trailing Return Types, East Const, and Code Style Consistency](https://arne-mertz.de/2018/05/trailing-return-types-east-const-and-code-style-consistency/) : 

论述了采用trailing returntype的好处。

## C++ RVO 和 prvalue

这是我一直以来的疑惑，上周又碰到了，原因是在阅读[Lifetime](https://en.cppreference.com/w/cpp/language/lifetime)的[Temporary object lifetime](https://en.cppreference.com/w/cpp/language/lifetime)章节的时候，其中有这样的描述:

> returning a prvalue from a function



于是我Google: "returning a prvalue from a function C++"，得到的回答中，大多数是在讨论C++ RVO 和 prvalue的，下面是我觉得讨论的比较好的:

- stackoverflow [C++ return an 'prvalue', from a function with a formal param 'rvalue' reference with assgined 'prvalue'?](https://stackoverflow.com/questions/27115814/c-return-an-prvalue-from-a-function-with-a-formal-param-rvalue-reference)

- cppreference [Copy elision](https://en.cppreference.com/w/cpp/language/copy_elision)

对RVO进行的讨论

- stackoverflow [c++11 Return value optimization or move? [duplicate]](https://stackoverflow.com/questions/17473753/c11-return-value-optimization-or-move)

- stackoverflow [C++11 rvalues and move semantics confusion (return statement)](https://stackoverflow.com/questions/4986673/c11-rvalues-and-move-semantics-confusion-return-statement)

这篇文章其实是已经收录的

### value semantic

显而易见的是: 两者都和value semantic有关，显然它们都是C++对value semantic的支持。

## Lifetime of a temporary

需要将[Reference initialization](https://en.cppreference.com/w/cpp/language/reference_initialization)中"Lifetime of a temporary"章节的内容移到lifetime章节。



## Lifetime of OOP object

在OOP object的lifetime中，它的initialization是涉及到非常多的问题的，下面是我需要进一步进行汇总整理的:

### destruction

Destruction sequence:

参见[Destructors#Destruction sequence](https://en.cppreference.com/w/cpp/language/destructor)

### construction

Initialization order:

[Constructors and member initializer lists#Initialization order](https://en.cppreference.com/w/cpp/language/constructor#Initialization_order)



### Calling virtual during construction and destruction

[During construction and destruction](https://en.cppreference.com/w/cpp/language/virtual#During_construction_and_destruction)



## TMP

### C++ compile time `isinstanceof`

bing: "C++ TMP whether the type of a reference is the same as a type", 下面是一些有价值的内容: 

stackoverflow [How to check if two types are same at compiletime(bonus points if it works with Boost strong typedef)](https://stackoverflow.com/questions/13071340/how-to-check-if-two-types-are-same-at-compiletimebonus-points-if-it-works-with)

bing: c++ compile time isinstanceof



## `unordered_map`

判断`map`中是否存在一个key，如果存在，则取该元素。下面是C++中的安全的做法，避免了`[]`默认添加一个元素。

```C++
auto Itor = m_Tables.find(TableName);
if (Itor == m_Tables.end())
{
    return &m_NotExist;
}
else
{
    return *Itor;
}
```



## static constexpr member data

之前遇到过重要的问题:

```C++
struct ServiceTrait
{
	static constexpr const char* Name = "报单服务";
};
```

然后在下面的macro中，使用`Name`

```C++
#define LOG_MSG_INFO(...)                                                           \
	if (CommunicateLogger::Instance().IsEnable())                                   \
	{                                                                               \
		SPDLOG_LOGGER_INFO(CommunicateLogger::Instance().GetLogger(), __VA_ARGS__); \
	}
```

下面这种写法能够支持编译并链接: 

```C++
LOG_MSG_INFO("{}的响应:{}", std::string(ServiceTrait::Name));
```

下面这种写法，虽然能够正常编译通过，但是`ldd`检查发现了undefined symbol: 

```C++
LOG_MSG_INFO("{}的响应:{}", ServiceTrait::Name);
```

这貌似是template + macro的问题。

下面是的内容static `constexpr` member data: 

stackoverflow [Undefined reference to static constexpr char[]](https://stackoverflow.com/questions/8016780/undefined-reference-to-static-constexpr-char)



## undefined reference

这是常见的链接错误,下面是比较好的内容:

stackoverflow [What is an undefined reference/unresolved external symbol error and how do I fix it?](https://stackoverflow.com/questions/12573816/what-is-an-undefined-reference-unresolved-external-symbol-error-and-how-do-i-fix)



## C++ `default` keyWord

重要用于special member function，下面是一些描述它的文章:

stackoverflow [The new syntax “= default” in C++11](https://stackoverflow.com/questions/20828907/the-new-syntax-default-in-c11)



## RAII and stack unwind

基于RAII来进行开发，告诉我们在constructor、destructor中，分别执行相反的操作，stack unwind是实现机制；两者的结合能够实现非常好的效果，发挥出最大的威力。

它们其实是属于stack order问题的，关于stack order，参见工程discrete。