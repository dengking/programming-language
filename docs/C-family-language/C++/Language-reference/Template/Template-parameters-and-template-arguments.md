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



> NOTE: Non-type template parameter即非类型模板参数，关于它的例子参见：
>
> https://stackoverflow.com/questions/45559355/how-do-you-pass-a-templated-class-instance-as-a-template-parameter-to-another-te



#### Type template parameter



#### Template template arguments

> Template template arguments的含义是使用一个template来作为template argument。

### Template arguments

#### Template non-type arguments

#### Template type arguments

#### Template template arguments

#### Default template arguments



## 一些问题

### Container as template parameter

stl的container都是模板类，如何将它们作为template parameter呢？这是本节探讨的一个话题。

[STL container as a template parameter](https://stackoverflow.com/questions/18613770/stl-container-as-a-template-parameter)

http://www.cs.technion.ac.il/users/yechiel/c++-faq/template-of-template.html



### Can namepace as template parameter

[Why can't namespaces be template parameters?](https://stackoverflow.com/questions/12905951/why-cant-namespaces-be-template-parameters)

[Alternative to using namespace as template parameter](https://stackoverflow.com/questions/55612759/alternative-to-using-namespace-as-template-parameter)