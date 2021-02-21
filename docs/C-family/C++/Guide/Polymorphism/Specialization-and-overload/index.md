# Specialization and overload

本文对C++的两种static polymorphism方式:

1、specialization

2、overload 

进行探讨。



## Full specialization and partial specialization



## Function specialization and function overload

在 `Function-template` 章节对此进行了深入讨论。



## 准确表达overload and specialization

C++的语法是比较冗杂的，有时候稍微的差异，表达的含义却是天差地别，这一点在template + overload的时候尤其明显，下面结合如下两篇文章中的内容进行说明：

1、stackoverflow [Template Specialization VS Function Overloading](https://stackoverflow.com/questions/7108033/template-specialization-vs-function-overloading)

2、accu [An introduction to C++ Traits](https://accu.org/index.php/journals/442)

### stackoverflow [Template Specialization VS Function Overloading](https://stackoverflow.com/questions/7108033/template-specialization-vs-function-overloading)

> NOTE: 其中给出了如下例子: 

```c++
#include <iostream>
template <typename T> void foo(T)
{
    std::cout<<"foo(T)"<<std::endl;
}
template <typename T> void foo(T*) // overload of foo(T)
{
    std::cout<<"foo(T*)"<<std::endl;
}
template <> void foo<int>(int*) // specialisation of foo(T*)
{
    std::cout<<"foo(int*)"<<std::endl;
}

int main()
{
foo(new int); // calls foo<int>(int*);
}

```



```c++
#include <iostream>
template <typename T> void foo(T)
{
    std::cout<<"foo(T)"<<std::endl;
}
template<> void foo<int*>(int*) // specialisation of foo(T)
{
    std::cout<<"foo<int*>(int*)"<<std::endl;
}
template<typename T> void foo(T*) // overload of foo(T*)
{
    std::cout<<"foo(T*)"<<std::endl;
}

int main()
{
foo(new int); // calls foo<int>(int*);
}

```

### accu [An introduction to C++ Traits](https://accu.org/index.php/journals/442)

> NOTE: 其中给出的例子

`is_pointer`

Now, while **fully specialized templates** are useful and in my experience, the most common sort of trait class specialization, I think that it is worth quickly looking at **partial specialization**, in this case, `boost::is_pointer` [[boost](https://accu.org/index.php/journals/442#boost)]. Again, a default template class is defined:

```c++
template< typename T > 
struct is_pointer{ 
  static const bool value = false; 
};
```

And a **partial specialization** for all pointer types is added:

```c++
template< typename T > 
struct is_pointer< T* >{ 
  static const bool value = true; 
};
```

简而言之：

1、`template< typename T > struct is_pointer`

2、`template< typename T > struct is_pointer< T* >//partial specialization`



