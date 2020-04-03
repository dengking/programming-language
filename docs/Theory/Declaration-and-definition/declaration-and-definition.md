

以下是总结的关于declaration和definition的区分：

- [Understanding “extern” keyword in C](https://www.geeksforgeeks.org/understanding-extern-keyword-in-c/)

- [Linkage in C and C++](https://www.embedded.com/design/prototyping-and-development/4007522/Linkage-in-C-and-C-)

- [What is the difference between a definition and a declaration?](https://stackoverflow.com/questions/1410563/what-is-the-difference-between-a-definition-and-a-declaration)



# [What is the difference between a definition and a declaration?](https://stackoverflow.com/questions/1410563/what-is-the-difference-between-a-definition-and-a-declaration)



## [A](https://stackoverflow.com/a/1410632)

A **declaration** introduces an identifier and describes its type, be it a type, object, or function. A declaration is **what the compiler needs** to accept references to that identifier. These are declarations:

```c
extern int bar;
extern int g(int, int);
double f(int, double); // extern can be omitted for function declarations
class foo; // no extern allowed for type declarations
```

***SUMMARY*** : 参见  [Forward declaration](https://en.wikipedia.org/wiki/Forward_declaration)  

A **definition** actually instantiates/implements this identifier. It's **what the linker needs** in order to **link** references to those entities. These are definitions corresponding to the above declarations:

```c
int bar;
int g(int lhs, int rhs) {return lhs*rhs;}
double f(int i, double d) {return i+d;}
class foo {};
```

A definition can be used in the place of a declaration.

An identifier can be *declared* as often as you want. Thus, the following is legal in C and C++:

```c
double f(int, double);
double f(int, double);
extern double f(int, double); // the same as the two above
extern double f(int, double);
```

However, it must be *defined* exactly once. If you forget to define something that's been declared and referenced somewhere, then the **linker** doesn't know what to link references to and complains about a missing **symbols**. If you define something more than once, then the linker doesn't know *which* of the definitions to link references to and complains about **duplicated symbols**.

***SUMMARY*** : 第一个错误是undefined symbol，这类问题在`compiler-error-undefined-symbol.md`中有描述；

------

Since the debate what is a class *declaration* vs. a class *definition* in `C++` keeps coming up (in answers and comments to other questions) , I'll paste a quote from the `C++` standard here.
At 3.1/2, C++03 says:

> A declaration is a definition unless it [...] is a class name declaration [...].

3.1/3 then gives a few examples. Amongst them:

```c++
[Example: [...]
struct S { int a; int b; }; // defines S, S::a, and S::b [...]
struct S; // declares S
—end example
```

To sum it up: The `C++` standard considers `struct x;` to be a *declaration* and `struct x {};` a *definition*. (In other words, **"forward declaration" a misnomer**, since there are no other forms of class declarations in `C++`.)

Thanks to [litb (Johannes Schaub)](https://stackoverflow.com/users/34509/johannes-schaub-litb) who dug out the actual chapter and verse in one of his answers.

