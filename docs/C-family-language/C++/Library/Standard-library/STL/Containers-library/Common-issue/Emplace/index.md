# `emplace`系列函数



## Implementation

在学习perfect forwarding的时候，阅读了thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c/) ，其中以 `emplace_back` 为例来进行说明进行说明，所以我又查阅了 cppreference [`std::vector<T,Allocator>::emplace_back`](https://en.cppreference.com/w/cpp/container/vector/emplace_back)，其中有这样的说明:

> Appends a new element to the end of the container. The element is constructed through [std::allocator_traits::construct](https://en.cppreference.com/w/cpp/memory/allocator_traits/construct), which typically uses placement-new to construct the element in-place at the location provided by the container.

`emplace`的实现是依赖于:

1、placement new的

2、perfect forwarding

3、Variadic Template

## Optimization: avoid copy

`emplace` 通过avoid copy来实现Optimization。消除了temporary。

## stackoverflow [push_back vs emplace_back](https://stackoverflow.com/questions/4303513/push-back-vs-emplace-back)

[A](https://stackoverflow.com/a/4306581)

Instead of taking a `value_type` it takes a variadic list of arguments, so that means that you can now perfectly forward the arguments and construct directly an object into a container without a temporary at all.

That's useful because no matter how much cleverness RVO and move semantic bring to the table there is still complicated cases where a push_back is likely to make unnecessary copies (or move). For example, with the traditional `insert()` function of a `std::map`, you have to create a temporary, which will then be copied into a `std::pair<Key, Value>`, which will then be copied into the map :

```C++
std::map<int, Complicated> m;
int anInt = 4;
double aDouble = 5.0;
std::string aString = "C++";

// cross your finger so that the optimizer is really good
m.insert(std::make_pair(4, Complicated(anInt, aDouble, aString))); 

// should be easier for the optimizer
m.emplace(4, anInt, aDouble, aString);
```

## stackoverflow [C++ std::vector emplace vs insert [duplicate]](https://stackoverflow.com/questions/14788261/c-stdvector-emplace-vs-insert)

[A](https://stackoverflow.com/a/14788288)

Emplace takes the arguments necessary to construct an object in place, whereas insert takes (a reference to) an object.

```C++
struct Foo
{
  Foo(int n, double x);
};

std::vector<Foo> v;
v.emplace(someIterator, 42, 3.1416);
v.insert(someIterator, Foo(42, 3.1416));
```

## zpjiang [Copy constructor and emplace_back() in C++](https://zpjiang.me/2018/08/08/Copy-Constructor-and-std-vector-emplace-back/)

## abseil [Tip of the Week #112: emplace vs. push_back](https://abseil.io/tips/112)

这篇介绍地非常好



## `emplace_back`如何选择constructor？

### 20200404

今天碰到了这样的一个问题：

```c++
class CIStockInfoWrapper{
	CIStockInfoWrapper(int MktType, std::string Code)
			: m_MktType(MktType), m_Code(Code)
	{
		m_HQInfo = new HQInfo(m_Code, m_MktType);
	}
};

......
    
std::list<CIStockInfoWrapper> m_HQInfoList;
m_HQInfoList.emplace_back(MktType, std::to_string(i));
```

如果将`CIStockInfoWrapper(int MktType, std::string Code)`的说明修改为`CIStockInfoWrapper(int MktType, std::string& Code)`，则程序就编译报错：

```
hq_client_wrapper/mock_hq_accessor.h:73:63:   required from here
/usr/include/c++/4.8.2/bits/stl_list.h:114:71: 错误：对‘CIStockInfoWrapper::CIStockInfoWrapper(int&, std::basic_string<char>)’的调用没有匹配的函数
  : __detail::_List_node_base(), _M_data(std::forward<_Args>(__args)...)

./hq_client_wrapper/hqsvr_stock_info_wrapper.h:22:7: 附注： 备选需要 1 实参，但提供了 2 个
```



## STL container `emplace`系列函数

基本上STL container都提供了emplace系列函数。

|                                                              |                                                              |      |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ---- |
| [std::vector](https://en.cppreference.com/w/cpp/container/vector) | [emplace](https://en.cppreference.com/w/cpp/container/vector/emplace) (C++11) <br>[emplace_back](https://en.cppreference.com/w/cpp/container/vector/emplace_back) (C++11) |      |
|                                                              |                                                              |      |
|                                                              |                                                              |      |

