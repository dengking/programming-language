# static_cast



## [Why use static_cast(x) instead of (int)x?](https://stackoverflow.com/questions/103512/why-use-static-castintx-instead-of-intx)



[A](https://stackoverflow.com/a/103868)

The main reason is that classic C casts make no distinction between what we call `static_cast<>()`, `reinterpret_cast<>()`, `const_cast<>()`, and `dynamic_cast<>()`. These four things are completely different.

A `static_cast<>()` is usually safe. There is a valid conversion in the language, or an appropriate constructor that makes it possible. The only time it's a bit risky is when you cast down to an **inherited class**; you must make sure that the object is actually the descendant that you claim it is, by means external to the language (like a flag in the object). A `dynamic_cast<>()` is safe as long as the result is checked (pointer) or a possible exception is taken into account (reference).

A `reinterpret_cast<>()` (or a `const_cast<>()`) on the other hand is always dangerous. You tell the compiler: "trust me: I know this doesn't look like a `foo` (this looks as if it isn't mutable), but it is".

> NOTE: `reinterpret_cast<>()` 是 explicit。

The first problem is that it's almost impossible to tell which one will occur in a C-style cast without looking at large and disperse pieces of code and knowing all the rules.

Let's assume these:

```cpp
class CMyBase
{
};

class CDerivedClass: public CMyBase
{
	//...
};
class CMyOtherStuff
{
	//...
};
int main()
{
	CMyBase *pSomething; // filled somewhere
}

```

Now, these two are compiled the same way:

```cpp
class CMyBase
{
};

class CDerivedClass: public CMyBase
{
	//...
};
class CMyOtherStuff
{
	//...
};
int main()
{
	CMyBase *pSomething; // filled somewhere
	CDerivedClass *pMyObject;
	pMyObject = static_cast<CDerivedClass*>(pSomething); // Safe; as long as we checked
	/* Same as static_cast<>
	 Safe; as long as we checked
	 but harder to read*/
	pMyObject = (CDerivedClass*) (pSomething);

}
// g++ test.cpp

```

> 

However, let's see this almost identical code:

```cpp
class CMyBase
{
};

class CDerivedClass: public CMyBase
{
	//...
};
class CMyOtherStuff
{
	//...
};
int main()
{
	CMyBase *pSomething; // filled somewhere
	CMyOtherStuff *pOther;
	pOther = static_cast<CMyOtherStuff*>(pSomething); // Compiler error: Can't convert
	/* No compiler error.
	 Same as reinterpret_cast<>
	 and it's wrong!!!*/
	pOther = (CMyOtherStuff*) (pSomething);

}
// g++ test.cpp

```

> NOTE: 上述程序，编译报错如下：
>
> ```c++
> test.cpp: 在函数‘int main()’中:
> test.cpp:17:49: 错误：从类型‘CMyBase*’到类型‘CMyOtherStuff*’中的 static_cast 无效
>   pOther = static_cast<CMyOtherStuff*>(pSomething); // Compiler error: Can't convert
> ```
>
> 

As you can see, there is no easy way to distinguish between the two situations without knowing a lot about all the classes involved.

The second problem is that the C-style casts are too hard to locate. In complex expressions it can be very hard to see C-style casts. It is virtually impossible to write an automated tool that needs to locate C-style casts (for example a search tool) without a full blown C++ compiler front-end. On the other hand, it's easy to search for "static_cast<" or "reinterpret_cast<".

```cpp
pOther = reinterpret_cast<CMyOtherStuff*>(pSomething);
      // No compiler error.
      // but the presence of a reinterpret_cast<> is 
      // like a Siren with Red Flashing Lights in your code.
      // The mere typing of it should cause you to feel VERY uncomfortable.
```

That means that, not only are C-style casts more dangerous, but it's a lot harder to find them all to make sure that they are correct.



## cppreference [static_cast conversion](https://en.cppreference.com/w/cpp/language/static_cast)

Only the following conversions can be done with static_cast, except when such conversions would cast away *constness* or *volatility*.

> NOTE: `static_cast`是需要维护CV的，否则会编译报错

### 1) 

If there is an [implicit conversion sequence](https://en.cppreference.com/w/cpp/language/implicit_cast) from *expression* to *`new_type`*, or if **overload resolution** for a [direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization) of an object or reference of type *`new_type`* from *`expression`* would find at least one viable（可行的） function, then `static_cast<new_type>(expression)` returns the imaginary（虚构的） variable `Temp` initialized as if by `new_type Temp(expression);`, which may involve [implicit conversions](https://en.cppreference.com/w/cpp/language/implicit_cast), a call to the [constructor](https://en.cppreference.com/w/cpp/language/constructor) of *`new_type`* or a call to a [user-defined conversion operator](https://en.cppreference.com/w/cpp/language/cast_operator). For non-reference *`new_type`*, the result object of the `static_cast` prvalue expression is what's direct-initialized. (since C++17)

> NOTE: 上面这段话，`if`、`or if`描述了两种情况下，`static_cast`的行为：`new_type Temp(expression);`
>
> 原文没有提供例子，不容易理解



### 3) (since C++11)

If *`new_type`* is an **rvalue reference type**, `static_cast` converts the value of glvalue, class prvalue, or array prvalue (until C++17)any lvalue (since C++17) *expression* to  ***xvalue*** referring to the same object as the expression, or to its **base sub-object** (depending on *`new_type`*). If the target type is an inaccessible or ambiguous base of the type of the expression, the program is ill-formed. If the expression is a [bit field](https://en.cppreference.com/w/cpp/language/bit_field) lvalue, it is first converted to prvalue of the underlying type. This type of `static_cast` is used to implement move semantics in `std::move`.

> [gcc](https://github.com/gcc-mirror/gcc)/[libstdc++-v3](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3)/[include](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include)/[bits](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include/bits)/[move.h](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/move.h)
>
> ```c++
> /**
>  *  @brief  Convert a value to an rvalue.
>  *  @param  __t  A thing of arbitrary type.
>  *  @return The parameter cast to an rvalue-reference to allow moving it.
>  */
> template<typename _Tp>
> constexpr typename std::remove_reference<_Tp>::type&&
> move(_Tp&& __t) noexcept
> {	return static_cast<typename std::remove_reference<_Tp>::type&&>(__t);}
> ```
>



## 维基百科[static_cast](https://en.wikipedia.org/wiki/Static_cast)



## Examples

维基百科[Curiously recurring template pattern#Polymorphic copy construction](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern#Polymorphic_copy_construction)

```c++
    std::unique_ptr<AbstractShape> clone() const override {
        return std::make_unique<Derived>(static_cast<Derived const&>(*this));
    }
```

将`this`转换为指向derived class的对象，如果缺少`const`，则会编译会报：

```
casts away qualifiers
```



[Question of using static_cast on “this” pointer in a derived object to base class](https://stackoverflow.com/questions/4543670/question-of-using-static-cast-on-this-pointer-in-a-derived-object-to-base-clas)