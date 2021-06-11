# C++17 `constexpr if`

## cppreference [if statement](https://en.cppreference.com/w/cpp/language/if) # [Constexpr If](https://en.cppreference.com/w/cpp/language/if#Constexpr_If) (since C++17)





## 我的实践

下面是我之前写过的一个程序：

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

上述程序算法无法编译通过的，因为当给函数`GetToken`提供一个没有字段`AccountIndex`的`ReqFieldType`时，compiler在编译`Token  = ReqField->AccountIndex;`时，是会complain：`ReqFieldType`没有字段`AccountIndex`的。

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

### 

C++17 `constexpr if` 能够对上述代码进行简化。

