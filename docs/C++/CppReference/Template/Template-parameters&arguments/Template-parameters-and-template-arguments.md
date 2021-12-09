# Template parameters and template arguments

1、可以使用function parameter 和 function argument 来类比template parameter 和 template argument

2、关于parameter和argument的含义，参见工程[programming-language](https://dengking.github.io/programming-language/)的Theory/Subroutine章节。



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

> NOTE: Template template parameter的含义是使用一个template来作为template parameter，这在 `Template-template-parameter` 章节进行了讨论。

### Template arguments

#### Template non-type arguments

#### Template type arguments

#### Template template arguments

#### Default template arguments



## 一些问题

### Container as template parameter

参见 `Container-as-template-parameter` 章节

### Can namepace as template parameter

[Why can't namespaces be template parameters?](https://stackoverflow.com/questions/12905951/why-cant-namespaces-be-template-parameters)

[Alternative to using namespace as template parameter](https://stackoverflow.com/questions/55612759/alternative-to-using-namespace-as-template-parameter)