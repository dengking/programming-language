# Non-static data members



## cppreference [Non-static data members](https://pd.codechef.com/docs/cpp/cpp/language/data_members.html)



### Layout

> NOTE: 
>
> 一、这部分内容放到了 `Object-layout` 中

### Member initialization

> NOTE: 
>
> 这部分内容放到了 `Member-initialization` 章节

### Usage

> NOTE: 
>
> 一、这一段给出了一些使用data member的example

3) In unevaluated operands.(since C++03)

```C++
struct S
{
   int m;
   static const std::size_t sz = sizeof m; // OK: m in unevaluated operand
};
std::size_t j = sizeof(S::m + 42); // OK: even though there is no "this" object for m
```

