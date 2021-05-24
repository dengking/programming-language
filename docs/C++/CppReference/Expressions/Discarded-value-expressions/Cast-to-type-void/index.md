# Cast-expression that casts to the type `void`

可以利用“the argument of a cast-expression that casts to the type `void` is a discard-value expression”来实现一些效果，下面是一些案例：

1、cppreference [Expression SFINAE on the return type](https://en.cppreference.com/w/cpp/language/sfinae#Expression_SFINAE)

> A common idiom is to use expression SFINAE on the return type, where the expression uses the comma operator, whose left subexpression is the one that is being examined (cast to `void` to ensure the user-defined operator comma on the returned type is not selected), and the right subexpression has the type that the function is supposed to return.

2、stackoverflow [How do I best silence a warning about unused variables?](https://stackoverflow.com/questions/1486904/how-do-i-best-silence-a-warning-about-unused-variables) # [A](https://stackoverflow.com/a/1486931)

```c
// silence a warning about unused variables，https://stackoverflow.com/a/1486931
#define UNUSED(expr) do { (void)(expr); } while (0)
```

Example: 在cppreference [static_cast conversion](https://en.cppreference.com/w/cpp/language/static_cast)的`4)`就是说明的discard value expression，Example也对此进行了说明。



