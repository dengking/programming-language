# inline

`inline`的含义是: 内联。它是C family language中非常常见的specifier，本文对它进行总结，下面是参考内容: 

1) cppreference [`inline` specifier](https://en.cppreference.com/w/cpp/language/inline)

2) cppreference [C++ keywords: inline](https://en.cppreference.com/w/cpp/keyword/inline)

3) cppreference [Namespaces#Inline namespaces](https://en.cppreference.com/w/cpp/language/namespace#Inline_namespaces)

下面以programming paradigm来对inline的用法进行总结: 

## Non-OOP

### inline function

参见:

1) cppreference [`inline` specifier](https://en.cppreference.com/w/cpp/language/inline)

2) `C-and-C++\From-source-code-to-exec\Compile\Optimization\inline`章节

### inline variable

参见:

1) cppreference [`inline` specifier](https://en.cppreference.com/w/cpp/language/inline)

## OOP

目前只有static member才能够inline，所以在`C++\Language-reference\Classes\Members\Static-member`对OOP中的`inline`相关的内容进行了总结。

inline member data

inline member function 

## inline namespace

参见: cppreference [Namespaces#Inline namespaces](https://en.cppreference.com/w/cpp/language/namespace#Inline_namespaces) 。



## TODO: defined inline

"defined inline"是在阅读 cppreference [Non-static member functions](https://en.cppreference.com/w/cpp/language/member_functions) 时发现的。

```C++
class S
{
	int mf4() const // can be defined inline
	{
		return data;
	} 
	int data;
};

```