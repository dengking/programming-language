# josephmansfield [C++ sequenced-before graphs](https://josephmansfield.uk/articles/c++-sequenced-before-graphs.html)

```C++
i = i++
```

This expression, and many like it, demonstrate the **sequencing rules** of C++ and how they can cause your program to behave in ways you might not expect. What should the result of this expression be given some initial value for `i`? Questions like this are extremely frequent on [Stack Overflow](https://stackoverflow.com/questions/tagged/c++). In this case, the answer is that this code is completely undefined — it could do anything. Literally anything (at least formally speaking).

## Unspecified behavior and undefined behavior and implementation-defined behavior

The C++ standard defines an **execution path** according to your code. Given particular inputs, the program will always follow this **execution path**. Sometimes, the standard allows multiple possible execution paths for your program. This gives compilers extra freedom to optimize in various ways. When the standard allows a particular set of possible paths, this is known as ***unspecified behavior***. In other cases, the standard gives absolutely no requirements about the behavior of your program, and this is known as ***undefined behavior***. Undefined behavior is certainly not something you want in your code.

**Implementation-defined behavior** is a subset of unspecified behavior, for which the implementation is required to document its choice of behavior.