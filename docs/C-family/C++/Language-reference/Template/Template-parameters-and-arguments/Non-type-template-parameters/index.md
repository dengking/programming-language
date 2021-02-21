# Non-type template parameters

“non-type template parameters”即非类型模板参数。

## cppreference [Template parameters and template arguments](https://en.cppreference.com/w/cpp/language/template_parameters)

### Non-type template parameter

> NOTE: Non-type template parameter即非类型模板参数，显然，它所参数化的不是type，而是value，显然non-type template parameter相当于function parameter，而对应的template entity其实相当于一个function，关于此的最最典型的例子是维基百科[Template metaprogramming](https://en.wikipedia.org/wiki/Template_metaprogramming)的Compile-time class generation段所枚举的`factorial`例子。使用Non-type template parameter可以实现functional programming，这是一种compile-time computation，这个观点在维基百科[Template metaprogramming](https://en.wikipedia.org/wiki/Template_metaprogramming)中已经提出。
>
> Non-type template的一个典型的例子就是[Boost.Array](https://www.boost.org/doc/libs/1_73_0/doc/html/array.html)，源码路径：
>
> https://github.com/boostorg/array/blob/develop/include/boost/array.hpp

| syntax                                |      |               |
| ------------------------------------- | ---- | ------------- |
| *type* *name*(optional)               | (1)  |               |
| *type* *name*(optional) `=` *default* | (2)  |               |
| *type* `...` *name*(optional)         | (3)  | (since C++11) |
| *placeholder* *name*                  | (4)  | (since C++17) |

> NOTE: 
>
> 1、通过上述syntax可以看出，`name`是optional的，我们将省略了`name`的称为anonymous non type template parameter，典型的例子为: 
>
> a、detection idiom

A non-type template parameter must have a *structural type*, which is one of the following types (optionally cv-qualified, the qualifiers are ignored):

1、[lvalue reference type](https://en.cppreference.com/w/cpp/language/reference#lvalue_references) (to object or to function);

2、an [integral type](https://en.cppreference.com/w/cpp/language/type);

3、a [pointer type](https://en.cppreference.com/w/cpp/language/pointer) (to object or to function);

4、a [pointer to member type](https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_members) (to member object or to member function);

5、an [enumeration type](https://en.cppreference.com/w/cpp/language/enum);

6、[std::nullptr_t](https://en.cppreference.com/w/cpp/types/nullptr_t); (since C++11)

7、a [floating-point type](https://en.cppreference.com/w/cpp/language/type);

8、a [literal](https://en.cppreference.com/w/cpp/named_req/LiteralType) class type with the following properties: (since C++20)

> NOTE: 关于此的例子参见[Literal Classes as Non-type Template Parameters in C++20](https://ctrpeach.io/posts/cpp20-class-as-non-type-template-param/)



> NOTE: 需要将上述的type和下面的argument进行对应：
>
> 上述[floating-point type](https://en.cppreference.com/w/cpp/language/type)、[integral type](https://en.cppreference.com/w/cpp/language/type)、[enumeration type](https://en.cppreference.com/w/cpp/language/enum)对应的是下面的integral and arithmetic types
>
> 上述[pointer type](https://en.cppreference.com/w/cpp/language/pointer) (to object or to function)，分别对应下面的For pointers to objects、For pointers to functions
>
> 上述a [pointer to member type](https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_members) (to member object or to member function)对应的是下面的For pointers to members
>
> 上述[lvalue reference type](https://en.cppreference.com/w/cpp/language/reference#lvalue_references)对应的是下面的For lvalue reference parameters

### [Template non-type arguments](https://en.cppreference.com/w/cpp/language/template_parameters#Template_non-type_arguments)

(until C++17)

- For integral and arithmetic types, the template argument provided during instantiation must be a [converted constant expression](https://en.cppreference.com/w/cpp/language/constant_expression) of the template parameter's type (so certain implicit conversion applies).
- For pointers to objects, the template arguments have to designate the address of a complete object with static [storage duration](https://en.cppreference.com/w/cpp/language/storage_duration) and a [linkage](https://en.cppreference.com/w/cpp/language/storage_duration#Linkage) (either internal or external), or a constant expression that evaluates to the appropriate null pointer or [std::nullptr_t](https://en.cppreference.com/w/cpp/types/nullptr_t) value.
- For pointers to functions, the valid arguments are pointers to functions with linkage (or constant expressions that evaluate to null pointer values).
- For lvalue reference parameters, the argument provided at instantiation cannot be a **temporary**, an unnamed lvalue, or a named lvalue with no linkage (in other words, the argument must have linkage).
- For pointers to members, the argument has to be a pointer to member expressed as `&Class::Member` or a constant expression that evaluates to null pointer or [std::nullptr_t](https://en.cppreference.com/w/cpp/types/nullptr_t) value.

In particular, this implies that string literals, addresses of array elements, and addresses of non-static members cannot be used as template arguments to instantiate templates whose corresponding non-type template parameters are pointers to objects.

> NOTE: 上述给出了冗杂的限制，其实总的来说，只要compiler能够在compile-time或者这个value，那么它就可以用做argument。正如在stackoverflow [Non-type template parameters](https://stackoverflow.com/questions/5687540/non-type-template-parameters) # [A](https://stackoverflow.com/a/5687562)中所总结的：
>
> A non-type template argument provided within a template argument list is an expression whose value can be determined at compile time. Such arguments must be:

## stackoverflow [Non-type template parameters](https://stackoverflow.com/questions/5687540/non-type-template-parameters)

I understand that the non-type template parameter should be a **constant integral expression**. Can someone shed light why is it so ?

```cpp
template <std::string temp>
void foo()
{
     // ...
}
```

> ```cpp
> error C2993: 'std::string' : illegal type for non-type template parameter 'temp'.
> ```

I understand what a constant integral expression is. What are the reasons for not allowing non-constant types like `std::string` as in the above snippet ?

### [A](https://stackoverflow.com/a/5687575)



```c++
#include <string>
#include <iostream>
using namespace std;

template <std::string * temp> //pointer to object
void f()
{
   cout << *temp << endl;
}

template <std::string & temp> //reference to object
void g()
{
     cout << temp << endl;
     temp += "...appended some string";
}

std::string s; //must not be local as it must have external linkage!

int main() {
        s = "can assign values locally";
        f<&s>();
        g<s>();
        cout << s << endl;
        return 0;
}
```



### [A](https://stackoverflow.com/a/5687562)

A non-type template argument provided within a template argument list is an expression whose value can be determined at compile time. Such arguments must be:

> constant expressions, addresses of functions or objects with external linkage, or addresses of static class members.

Also, string literals are objects with **internal linkage**, so you can't use them as template arguments. You cannot use a **global pointer**, either. Floating-point literals are not allowed, given the obvious possibility of rounding-off errors.

## Pointer and reference as non-type template argument example

stackoverflow [Non-type template parameters](https://stackoverflow.com/questions/5687540/non-type-template-parameters) # [A](https://stackoverflow.com/a/5687575) 中给出了例子

stackoverflow [How do you pass a templated class instance as a template parameter to another template?](https://stackoverflow.com/questions/45559355/how-do-you-pass-a-templated-class-instance-as-a-template-parameter-to-another-te)

> NOTE: 其中给出了一些例子



## String literal and non-type template parameter

c++ string can not be non type template parameter，在下面文章中对此进行了讨论:

### keha [Literal Classes as Non-type Template Parameters in C++20](https://blog.keha.dev/posts/cpp20-class-as-non-type-template-param/)



### stackoverflow [String literals not allowed as non type template parameters](https://stackoverflow.com/questions/5547852/string-literals-not-allowed-as-non-type-template-parameters)

> NOTE: 这篇文章的解释是非常好的

Because string literals are objects with **internal linkage** (two string literals with the same value but in different modules are different objects), you can't use them as template arguments either:

TODO [A](https://stackoverflow.com/a/5548016)

> NOTE: 这个解释是非常好的，目前还没有阅读