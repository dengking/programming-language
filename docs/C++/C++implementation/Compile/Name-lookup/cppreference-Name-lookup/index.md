# Name lookup

这是C++ compile flow的第一步，C++的name lookup是非常复杂的，涉及的内容非常多。

## cppreference [Name lookup](https://en.cppreference.com/w/cpp/language/lookup)

**Name lookup** is the procedure by which a [name](https://en.cppreference.com/w/cpp/language/name), when encountered in a program, is associated with the [declaration](https://en.cppreference.com/w/cpp/language/declarations) that introduced it.

For example, to compile [std::cout](http://en.cppreference.com/w/cpp/io/cout) << [std::endl](http://en.cppreference.com/w/cpp/io/manip/endl);, the compiler performs:

1、**unqualified name lookup** for the name `std`, which finds the declaration of namespace `std` in the header `<iostream>`

>  NOTE: 因为`#include <iostream>`，则包含了namespace `std`的声明

2、**qualified name lookup** for the name `cout`, which finds a variable declaration in the namespace `std`

3、**qualified name lookup** for the name `endl`, which finds a function template declaration in the namespace `std`

4、both [argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl) for the name `operator <<` which finds multiple function template declarations in the namespace `std` and **qualified name lookup** for the name `std::ostream::operator<<` which finds multiple member function declarations in class `std::ostream`

> NOTE: 
>
> 1、在 cppreference [Argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl) 中，对`std::cout << "Test\n";`有着更多的解释

For function and function template names, **name lookup** can associate *multiple* declarations with the same name, and may obtain additional declarations from [argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl). [Template argument deduction](https://en.cppreference.com/w/cpp/language/function_template) may also apply, and the set of declarations is passed to [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution), which selects the declaration that will be used. [Member access](https://en.cppreference.com/w/cpp/language/access) rules, if applicable, are considered only after **name lookup** and **overload resolution**.

> NOTE: 完整的编译过程涉及到了非常多的内容

For all other names (variables, namespaces, classes, etc), **name lookup** must produce a *single* declaration in order for the program to compile. Lookup for a name in a scope finds all declarations of that name, with one exception, known as the "struct hack" or "type/non-type hiding": Within the same scope, some occurrences of a name may refer to a declaration of a `class`/`struct`/`union`/`enum` that is not a typedef, while all other occurrences of the same name either all refer to the same variable, non-static data member (since C++14), or enumerator, or they all refer to possibly overloaded function or function template names. In this case, there is no error, but the type name is hidden from lookup (the code must use [elaborated type specifier](https://en.cppreference.com/w/cpp/language/elaborated_type_specifier) to access it).

> NOTE: 上面两段话分别概述了function的name lookup和others的name lookup。需要注意它们的异同。

### Types of lookup

If the **name** appears immediately to the right of the scope resolution operator `::` or possibly after `::` followed by the disambiguating keyword `template`, see

- [Qualified name lookup](https://en.cppreference.com/w/cpp/language/qualified_lookup)

Otherwise, see

- [Unqualified name lookup](https://en.cppreference.com/w/cpp/language/unqualified_lookup)

### See also

- [Scope](https://en.cppreference.com/w/cpp/language/scope)
- [Argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl)
- [Template argument deduction](https://en.cppreference.com/w/cpp/language/function_template)
- [Overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution)



