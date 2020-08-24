# Prefer cast function to C-style cast

本节标题的函数是：优先使用`C++`提供的cast函数：

- `static_cast`
- `dynamic_cast`
- `reinterpret_cast`
- `const_cast`

在下面的这篇文章中，详细的说明了原因：

## stackoverflow [Why use static_cast(x) instead of (int)x?](https://stackoverflow.com/questions/103512/why-use-static-castintx-instead-of-intx)



[A](https://stackoverflow.com/a/103868)

### Classic C casts make no distinction 

The main reason is that classic C casts make no distinction between what we call `static_cast<>()`, `reinterpret_cast<>()`, `const_cast<>()`, and `dynamic_cast<>()`. These four things are completely different.

> NOTE: C-style cast无法准确地、显示地表达intent。反过来说，type cast operator促进了[intentional programming](https://en.wikipedia.org/wiki/Intentional_programming)，这在下一篇中进行了介绍。

A `static_cast<>()` is usually safe. There is a valid conversion in the language, or an appropriate constructor that makes it possible. The only time it's a bit risky is when you cast down to an **inherited class**; you must make sure that the **object** is actually the **descendant** that you claim it is, by means external to the language (like a flag in the object). A `dynamic_cast<>()` is safe as long as the result is checked (pointer) or a possible exception is taken into account (reference).

> NOTE: 这段话中所描述的“cast down to an **inherited class**”其实就是downcast，它对应的是cppreference [static_cast conversion](https://en.cppreference.com/w/cpp/language/static_cast)中的`2)`。

A `reinterpret_cast<>()` (or a `const_cast<>()`) on the other hand is always dangerous. You tell the compiler: "trust me: I know this doesn't look like a `foo` (this looks as if it isn't mutable), but it is".

> NOTE: `reinterpret_cast<>()` 是 make dangerous operation explicit。

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
> pOther = static_cast<CMyOtherStuff*>(pSomething); // Compiler error: Can't convert
> ```
>
> 

As you can see, there is no easy way to distinguish between the two situations without knowing a lot about all the classes involved.

### C-style casts are too hard to locate

The second problem is that the C-style casts are too hard to locate. In complex expressions it can be very hard to see C-style casts. It is virtually impossible to write an automated tool that needs to locate C-style casts (for example a search tool) without a full blown C++ compiler front-end. On the other hand, it's easy to search for "static_cast<" or "reinterpret_cast<".

```cpp
pOther = reinterpret_cast<CMyOtherStuff*>(pSomething);
      // No compiler error.
      // but the presence of a reinterpret_cast<> is 
      // like a Siren with Red Flashing Lights in your code.
      // The mere typing of it should cause you to feel VERY uncomfortable.
```

That means that, not only are C-style casts more dangerous, but it's a lot harder to find them all to make sure that they are correct.



