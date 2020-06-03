# Template parameters and template arguments

关于parameter和argument的含义，参见工程[programming-language](https://dengking.github.io/programming-language/)的Theory/Subroutine章节。



## cppreference [Template parameters and template arguments](https://en.cppreference.com/w/cpp/language/template_parameters)

> NOTE: 原文由于没有table of content，所以在阅读的时候可能无法掌握作者的行文思路，其实正如标题蕴含的，原文主要分为两大块：template parameter和template argument，下面我根据原文的内容增加了header level以使结构清晰，可以看到原文的结构其实是非常清晰的，两大节基本上一一对应。

### Template parameters

Every [template](https://en.cppreference.com/w/cpp/language/templates) is parametrized by one or more template parameters, indicated in the *parameter-list* of the template declaration syntax:

```c++
template < parameter-list > declaration	
```

Each parameter in *parameter-list* may be:

- a non-type template parameter;
- a type template parameter;
- a template template parameter.

> NOTE:  共有上述三大类template parameter

#### Non-type template parameter

> NOTE: non-type template 是比较复杂的，放到了单独的一篇中进行介绍。

#### Type template parameter

> NOTE: 这种是非常常见的



#### Template template parameter

> Template template parameter的含义是使用一个template来作为template parameter。

### Template arguments

#### Template non-type arguments

- For integral and arithmetic types, the template argument provided during instantiation must be a [converted constant expression](https://en.cppreference.com/w/cpp/language/constant_expression) of the template parameter's type (so certain implicit conversion applies).
- For pointers to objects, the template arguments have to designate the address of a complete object with static [storage duration](https://en.cppreference.com/w/cpp/language/storage_duration) and a [linkage](https://en.cppreference.com/w/cpp/language/storage_duration#Linkage) (either internal or external), or a constant expression that evaluates to the appropriate null pointer or [std::nullptr_t](https://en.cppreference.com/w/cpp/types/nullptr_t) value.
- For pointers to functions, the valid arguments are pointers to functions with linkage (or constant expressions that evaluate to null pointer values).
- For lvalue reference parameters, the argument provided at instantiation cannot be a temporary, an unnamed lvalue, or a named lvalue with no linkage (in other words, the argument must have linkage).
- For pointers to members, the argument has to be a pointer to member expressed as &Class::Member or a constant expression that evaluates to null pointer or [std::nullptr_t](https://en.cppreference.com/w/cpp/types/nullptr_t) value.

#### Template type arguments

#### Template template arguments

#### Default template arguments



## modernescpp [Types-, Non-Types, and Templates as Template Parameters](https://www.modernescpp.com/index.php/types-non-types-and-templates-as-template-parameters)

这篇文章不错，其实的例子非常好。

## 一些问题

### Container as template parameter

stl的container都是模板类，如何将它们作为template parameter呢？这是本节探讨的一个话题。

[STL container as a template parameter](https://stackoverflow.com/questions/18613770/stl-container-as-a-template-parameter)

http://www.cs.technion.ac.il/users/yechiel/c++-faq/template-of-template.html



### Can namepace as template parameter

[Why can't namespaces be template parameters?](https://stackoverflow.com/questions/12905951/why-cant-namespaces-be-template-parameters)

[Alternative to using namespace as template parameter](https://stackoverflow.com/questions/55612759/alternative-to-using-namespace-as-template-parameter)