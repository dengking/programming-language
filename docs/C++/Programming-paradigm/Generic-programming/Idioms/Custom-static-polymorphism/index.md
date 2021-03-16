# Custom static polymorphism 

C++除了语言内置的static polymorphism，还支持让用户自己建立起static polymorphism，这极大的增加了C++语言的灵活性。



## Dispatch based on constant

关于constant，参见`C++\Language-reference\Expressions\Constant-expressions`。

实现方式有:

1) enum dispatch : 参见`C++\Language-reference\Enum`的"Static dispatch"段



## Tag dispatch

参见`C++\Idiom\TMP\SFINAE-trait-enable-if\Tag-Dispatching`



## enum dispatch VS tag dispatch

enum是named constant，而tag是type。它们都可以用来实现static dispatch，但是实现的成本、方式确实不同的:

1) tag可以用作type parameter，而enum无法，它如果要用于template，则需要使用SFINAE

### TODO

Google c++ static polymorphism on enum values

fluentcpp [When to Use Enums and When to Use Tag Dispatching in C++](https://www.fluentcpp.com/2018/05/01/when-to-use-enums-and-when-to-use-tag-dispatching-in-cpp/)

stackoverflow [Polymorphic Enum in C++](https://stackoverflow.com/questions/3117462/polymorphic-enum-in-c)



## draft



### Example: conditional compiling by detection idiom

下面是碰到过的一个例子：

如果结构体有字段`AccountIndex`，则使用它的`AccountIndex`字段来作为token，否则使用另外一种获取token的算法，通过`has_member_AccountIndex` trait来判断结构体是否有字段`AccountIndex`，下面是我第一次的实现：

```c++
template<typename ReqFieldType>
TokenType GetToken(ReqFieldType* ReqField)
{
    TokenType Token;
    if(has_member_AccountIndex<ReqFieldType>::value)
    {
        Token  = ReqField->AccountIndex;
    }
	else
    {
        Token = Algorithm2();
    }
		return Token;
}
```

上述程序算法无法编译通过的，因为当给函数GetToken提供一个没有字段`AccountIndex`的`ReqFieldType`时，compiler在编译`Token  = ReqField->AccountIndex;`时，是会complain：`ReqFieldType`没有字段`AccountIndex`的。

所以，我们需要使用conditional compiling，下面是实现代码：

```c++
	template<typename ReqFieldType>
	auto GetToken(ReqFieldType* ReqField)->typename std::enable_if< has_member_AccountIndex<ReqFieldType>::value, TokenType>::type
	{
		TokenType Token  = ReqField->AccountIndex;
		return Token;
	}


	template<typename ReqFieldType>
	auto GetToken(ReqFieldType* ReqField)->typename std::enable_if<has_member_AccountIndex<ReqFieldType>::value, TokenType>::type
	{
        TokenType Token = Algorithm2();
		return Token;
	}
```

### C++17 `constexpr if`

C++17 `constexpr if` 能够对上述代码进行简化，参见`C++\Language-reference\Statements\Selection-statements\Constexpr-if.md`。