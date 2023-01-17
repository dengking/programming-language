# Return type deduction



## cppreference [Function declaration # Return type deduction](https://en.cppreference.com/w/cpp/language/function#Return_type_deduction) (since C++14) 

If the *decl-specifier-seq* of the function declaration contains the keyword `auto`, trailing return type may be omitted, and will be deduced by the compiler from the type of the expression used in the [return](https://en.cppreference.com/w/cpp/language/return) statement. If the return type does not use `decltype(auto)`, the deduction follows the rules of [template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction#Other_contexts):



> NOTE:
>
> 一、最后一句话的意思可以反过来进行理解:  如果 return type 使用 `decltype(auto)` 的话，那么 deduction 将不按照 "rules of [template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction#Other_contexts)"，那么它的意思是是否是: 如果 return type 使用 `auto` 的话，deduction 将按照 "rules of [template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction#Other_contexts)"？结合下面的例子，我觉得答案是的。关于这一点，在 cppreference [Placeholder type specifiers (since C++11)](https://en.cppreference.com/w/cpp/language/auto) 中有这样的描述:
>
> > ```
> > type-constraint(optional) auto	(1)	(since C++11)
> > type-constraint(optional) decltype ( auto )	(2)	(since C++14)
> > ```
> >
> > \1) type is deduced using the rules for [template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction#Other_contexts).
> >
> > \2) type is [`decltype(expr)`](https://en.cppreference.com/w/cpp/language/decltype), where `expr` is the initializer.



```c++
int x = 1;
auto f() { return x; }        // return type is int
const auto& f() { return x; } // return type is const int&
```

If the return type is `decltype(auto)`, the return type is as what would be obtained if the expression used in the return statement were wrapped in [decltype](https://en.cppreference.com/w/cpp/language/decltype):

```C++
int x = 1;
decltype(auto) f() { return x; }  // return type is int, same as decltype(x)
decltype(auto) f() { return(x); } // return type is int&, same as decltype((x))
```

(note: "`const decltype(auto)&`" is an error, `decltype(auto)` must be used on its own)

> NOTE:
>
> 一、[C++14](https://en.cppreference.com/w/cpp/14)  [return type deduction for functions](https://en.cppreference.com/w/cpp/language/function#Return_type_deduction_.28since_C.2B.2B14.29)  `decltype(auto)` 是对C++ 11 `auto` + `decltype` + trailing return type的简化，参见`decltype` 章节。



## see also



stackoverflow [What are some uses of decltype(auto)?](https://stackoverflow.com/questions/24109737/what-are-some-uses-of-decltypeauto) 

```c++
template<typename ContainerType, typename IndexType>                // C++14
decltype(auto) grab(ContainerType&& container, IndexType&& index)
{
  authenticateUser();
  return std::forward<ContainerType>(container)[std::forward<IndexType>(index)];
}
```



## example

### stackoverflow [c++11: Templated wrapper function](https://stackoverflow.com/questions/25495448/c11-templated-wrapper-function)