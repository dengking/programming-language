# cppreference [Placeholder type specifiers (since C++11)](https://en.cppreference.com/w/cpp/language/auto) 



1、For variables, specifies that the type of the variable that is being declared will be automatically deduced from its initializer.

(since C++11)

2、For functions, specifies that the return type will be deduced from its return statements.

(since C++14)

3、For non-type template parameters, specifies that the type will be deduced from the argument.

(since C++17)



```
type-constraint(optional) auto	(1)	(since C++11)
type-constraint(optional) decltype ( auto )	(2)	(since C++14)
```

\1) type is deduced using the rules for [template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction#Other_contexts).

\2) type is [`decltype(expr)`](https://en.cppreference.com/w/cpp/language/decltype), where `expr` is the initializer.
