# Scope

本文讨论c++的scope，scope是C++中的重要概念，C++中的很多内容、概念都是基于scope建立的，下面是于此相关的内容的梳理:

一、static、compile-time

scope是一个静态的概念，compiler根据scope规则进行compile；

二、dynamic、runtime:

C++中的很多dynamic、runtime内容，其实可以使用scope来进行理解和刻画，关于此，参见 `C++\Language-reference\Basic-concept\Execution-model` 章节。



## cppreference [Scope](https://en.cppreference.com/w/cpp/language/scope)

> NOTE: 

Each [name](https://en.cppreference.com/w/cpp/language/name) that appears in a C++ program is only valid in some possibly discontiguous(不连接的) portion of the source code called its *scope*.



### Block scope

> NOTE: block scope非常重要，它是后面将要介绍的这些scope的基石，后面会对其进行专门的介绍。
>
> 
>
> 
>
> C++ block是compound statement的同义词: [block (compound statement)](https://en.cppreference.com/w/cpp/language/statements#Compound_statements) ，因此后续当需要找 [compound statement](https://en.cppreference.com/w/cpp/language/statements#Compound_statements) 的scope规则的时候，可以查看这一段。
>
> Function block应该就是属于block scope；
>
> 可以认为C++中每个block scope都有一个自己的stack。

The potential scope of a variable introduced by a declaration in a [block (compound statement)](https://en.cppreference.com/w/cpp/language/statements#Compound_statements) begins at the point of declaration and ends at the end of the block. Actual scope is the same as potential scope unless there is a nested block with a declaration that introduces identical name

> NOET: 上面这一段引入了potential scope和actual scope的概念，这两个概念其实是为了描述具备nesting关系的两个scope中定义了两个相同的variable的情况。原文给出了例子。

```C++
int main()
{
    int a = 0; // scope of the first 'a' begins
    ++a; // the name 'a' is in scope and refers to the first 'a'
    {
        int a = 1; // scope of the second 'a' begins
                   // scope of the first 'a' is interrupted
        a = 42;    // 'a' is in scope and refers to the second 'a'                 
    } // block ends, scope of the second 'a' ends
      //             scope of the first 'a' resumes
} // block ends, scope of the first 'a' ends
int b = a; // Error: name 'a' is not in scope
```

#### Exception handler

The potential scope of a name declared in an exception handler begins at the point of declaration and ends when the exception handler ends, and is not in scope in another exception handler or in the enclosing block.

```C++
try {   
    f();
} catch(const std::runtime_error& re) { // scope of re begins
    int n = 1; // scope of n begins
    std::cout << re.what(); // re is in scope
} // scope of re ends, scope of n ends
 catch(std::exception& e) {
    std::cout << re.what(); // error: re is not in scope
    ++n; // error: n is not in scope
}
```

#### Other [compound statement](https://en.cppreference.com/w/cpp/language/statements#Compound_statements) 

> NOTE: 原文这一段对各种 [compound statement](https://en.cppreference.com/w/cpp/language/statements#Compound_statements) 的scope进行了介绍。

### Function parameter scope

> NOTE: 



### Namespace scope

The top-level scope of a translation unit ("file scope" or "global scope") is also a namespace and is properly called "global namespace scope". The potential scope of any entity declared in the global namespace scope begins at the declaration and continues to the end of the translation unit.

### Class scope

### Enumeration scope

### Template parameter scope

### Point of declaration



## Rule of c++ scope

关于rule of c++ scope，cppreference [Scope](https://en.cppreference.com/w/cpp/language/scope)虽然信息描述c++语言中的各种scope，但是并没有描述清楚这些scope之间的关系。我们已经知道，scope之间是具备nesting关系（参见工程[data-structure](https://dengking.github.io/data-structure/)）的，所以我们需要搞清楚c++的各种scope之间的nesting关系。

### Nesting relation of c++ scope

其实，梳理清楚c++的scope之间的关系非常简单：思考能否在一个scope中声明另外一个scope，比如：能否在function中定义class、能否在class中定义namespace。下面按照自顶向下的顺序来描述c++的scope的nesting关系：

1) file scope / global scope

2) namespace scope

3) class scope 

4) Enumeration scope

5) function scope

6) block scope

需要注意的是，上述的nesting关系仅仅描述了部分，而没有包含所有的。

## `{}` and scope

C++中，`{}`对应一个scope，也就是在c++中`{}`所包围的区域就是一个scope，c++是允许nesting scope的。

按照这个原则，我们再来联系c++的function、class 、namespace 、Enumeration 的declaration，显然它们都是使用的`{}`，所以它们都是一个scope。