# Overload and return type

1、C++不支持overload by return type

2、但是我们可以使用一些特殊的technique，来实现类似于Overload and return type的效果

## stackoverflow [Overloading by return type](https://stackoverflow.com/questions/9568852/overloading-by-return-type)

### [A](https://stackoverflow.com/a/9568890)

No there isn't. You can't overload methods based on return type.

Overload resolution takes into account the **function signature**. A function signature is made up of:

- function name
- cv-qualifiers
- parameter types

And here's the quote:

**1.3.11 signature**

> the information about a function that participates in overload resolution (13.3): its parameter-type-list (8.3.5) and, if the function is a class member, the cv-qualifiers (if any) on the function itself and the class in which the member function is declared. [...]

**Options:**

1) change the method name:

```cpp
class My {
public:
    int getInt(int);
    char getChar(int);
};
```

2) out parameter:

```cpp
class My {
public:
    void get(int, int&);
    void get(int, char&);
}
```

3) templates... overkill in this case.

### [A](https://stackoverflow.com/a/9569120)

t's possible, but I'm not sure that it's a technique I'd recommend for beginners. As in other cases, when you want the choice of functions to depend on how the return value is used, you use a proxy; first define functions like `getChar` and `getInt`, then a generic `get()` which returns a Proxy like this:

```cpp
class Proxy
{
    My const* myOwner;
public:
    Proxy( My const* owner ) : myOwner( owner ) {}
    operator int() const
    {
        return myOwner->getInt();
    }
    operator char() const
    {
        return myOwner->getChar();
    }
};
```

Extend it to as many types as you need.



## 模板化 + SFINAE 实现 "constexpr if"，达到 overload by return type的效果

下面是我第一次写的program: 

```C++
/**
 * 工厂方法
 * @param Msg
 * @return
 */
template<typename ServiceTrait>
struct GenericUnpackerFactory
{
	static auto New(CRspMsg &Msg) -> typename std::enable_if<!has_member_UnpackerType<ServiceTrait>::value,typename LdpMsgUnpackerTrait<ServiceTrait::ServiceMsgType, typename ServiceTrait::RspFieldType>::UnpackerType>::type
	{
		using UnpackerType = typename LdpMsgUnpackerTrait<ServiceTrait::ServiceMsgType, typename ServiceTrait::RspFieldType>::UnpackerType;
		return UnpackerType { Msg };
	}
	static auto New(CRspMsg &Msg) -> typename std::enable_if<has_member_UnpackerType<ServiceTrait>::value,typename ServiceTrait::UnpackerType>::type
	{
		using UnpackerType = typename ServiceTrait::UnpackerType;
		return UnpackerType { Msg };
	}
};
```

上述code，编译报错如下:

```c++
./derivative_api/../../common/api_framework/protocol/ldp_msg_protocol.h:800:14: 错误：‘static typename std::enable_if<has_member_UnpackerType<ServiceTrait>::value, typename ServiceTrait::UnpackerType>::type GenericUnpackerFactory<ServiceTrait>::New(CRspMsg&)’无法被重载
  static auto New(CRspMsg &Msg) -> typename std::enable_if<has_member_UnpackerType<ServiceTrait>::value,typename ServiceTrait::UnpackerType>::type
              ^
./derivative_api/../../common/api_framework/protocol/ldp_msg_protocol.h:795:14: 错误：与‘static typename std::enable_if<(! has_member_UnpackerType<ServiceTrait>::value), typename LdpMsgUnpackerTrait<ServiceTrait:: ServiceMsgType, typename ServiceTrait::RspFieldType>::UnpackerType>::type GenericUnpackerFactory<ServiceTrait>::New(CRspMsg&)’
  static auto New(CRspMsg &Msg) -> typename std::enable_if<!has_member_UnpackerType<ServiceTrait>::value,typename LdpMsgUnpackerTrait<ServiceTrait::ServiceMsgType, typename ServiceTrait::RspFieldType>::UnpackerType>::type

```



通过将其"模板化 + SFINAE + `std::enable_if`" ，实现"constexpr if"，这样在只会有一个function被选中，因此就避免了overload resolution，就使问题得到了解决:

```C++
/**
 * 工厂方法
 * @param Msg
 * @return
 */
template<typename ServiceTrait>
struct GenericUnpackerFactory
{
	template<typename RspMsgType = CRspMsg>
	static auto New(RspMsgType &Msg) -> typename std::enable_if<!has_member_UnpackerType<ServiceTrait>::value,typename LdpMsgUnpackerTrait<ServiceTrait::ServiceMsgType, typename ServiceTrait::RspFieldType>::UnpackerType>::type
	{
		using UnpackerType = typename LdpMsgUnpackerTrait<ServiceTrait::ServiceMsgType, typename ServiceTrait::RspFieldType>::UnpackerType;
		return UnpackerType { Msg };
	}
	template<typename RspMsgType = CRspMsg>
	static auto New(RspMsgType &Msg) -> typename std::enable_if<has_member_UnpackerType<ServiceTrait>::value,typename ServiceTrait::UnpackerType>::type
	{
		using UnpackerType = typename ServiceTrait::UnpackerType;
		return UnpackerType { Msg };
	}
};
```



## TODO

artificial-mind [Overloading by Return Type in C++](https://artificial-mind.net/blog/2020/10/10/return-type-overloading)

