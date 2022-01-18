# wikipedia [Type qualifier](https://en.wikipedia.org/wiki/Type_qualifier)

In the [C](https://en.wikipedia.org/wiki/C_(programming_language)), [C++](https://en.wikipedia.org/wiki/C%2B%2B), and [D](https://en.wikipedia.org/wiki/D_(programming_language)) [programming languages](https://en.wikipedia.org/wiki/Programming_language), a **type qualifier** is a [keyword](https://en.wikipedia.org/wiki/Keyword_(computer_programming)) that is applied to a [type](https://en.wikipedia.org/wiki/Data_type), resulting in a *qualified type.* For example, `const int` is a qualified type representing a constant integer, while `int` is the corresponding *unqualified type*, simply an integer. In D these are known as *type constructors,* by analogy with [constructors in object-oriented programming](https://en.wikipedia.org/wiki/Constructor_(object-oriented_programming)).

**Type qualifiers** are a way of expressing additional information about a value through the [type system](https://en.wikipedia.org/wiki/Type_system), and ensuring [correctness](https://en.wikipedia.org/wiki/Correctness_(computer_science)) in the use of the data. Type qualifiers are not generally used outside the C/C++ family of languages: many languages have a notion of **constants**, but express this by the [name binding](https://en.wikipedia.org/wiki/Name_binding) being constant (a "variable that doesn't vary"), rather than through the **type system**; see [alternatives](https://en.wikipedia.org/wiki/Type_qualifier#Alternatives), below.

> NOTE: :在type前加上type qualifier就像创造了一个新的类型。

## By language

### C/C++

As of 2014 and [C11](https://en.wikipedia.org/wiki/C11_(C_standard_revision)), there are four **type qualifiers** in standard C: [`const`](https://en.wikipedia.org/wiki/Const-correctness) ([C89](https://en.wikipedia.org/wiki/ANSI_C)), [`volatile`](https://en.wikipedia.org/wiki/Volatile_(computer_programming))  ([C89](https://en.wikipedia.org/wiki/ANSI_C)), [`restrict`](https://en.wikipedia.org/wiki/Restrict)   ([C99](https://en.wikipedia.org/wiki/C99)) and `_Atomic` ([C11](https://en.wikipedia.org/wiki/C11_(C_standard_revision))) – the latter has a private name to avoid clashing with user-defined names.[[1\]](https://en.wikipedia.org/wiki/Type_qualifier#cite_note-1) The first two of these, `const` and `volatile`, are also present in `C++`, and are the only **type qualifiers** in `C++`. Thus in `C++` the term "*cv*-qualified type" (for **const** and **volatile**) is often used for "qualified type", while the terms "*c*-qualified type" and "*v*-qualified type" are used when only one of the qualifiers is relevant.

Of these, `const` is by far the best-known and most used, appearing in the C and C++ [standard libraries](https://en.wikipedia.org/wiki/Standard_library) and encountered in any significant use of these languages, which must satisfy [const-correctness](https://en.wikipedia.org/wiki/Const-correctness). The other qualifiers are used for low-level programming, and while widely used there, are rarely used by typical programmers. For a time however `volatile` was used by some `C++` programmers for **synchronization** during threading, though this was discouraged and is now broken in most compilers.

## Syntax

See also: [C syntax § Type qualifiers](https://en.wikipedia.org/wiki/C_syntax#Type_qualifiers)

In C and `C++`, a type is given in a function [declaration](https://en.wikipedia.org/wiki/Declaration_(computer_programming)) or variable declaration by giving one or more type specifiers, and optionally type qualifiers. For example, an integer variable can be declared as:

```c
int x;
```

where `int` is the type specifier. An unsigned integer variable can be declared as:

```
unsigned int x;
```

where both `unsigned` and `int` are type specifiers. A constant unsigned integer variable can be declared as:

```
const unsigned int x;
```

where `const` is a type qualifier, which the qualified type of `x` is `const unsigned int` and the unqualified type is `unsigned int`.

Variable declarations further have an optional [storage class specifier](https://en.wikipedia.org/wiki/Storage_class_specifier). Properly this is a separate topic, distinct from the type, though `const` on a variable declaration is *also* taken to have implications for the storage class, namely that it can be stored in read-only memory.

> NOTE:  : specifier和qualifiers存在差异

## Volatile-correctness

See also: [Const-correctness](https://en.wikipedia.org/wiki/Const-correctness)

The other qualifier in C and C++, `volatile`, indicates that an object may be changed by something external to the program at any time and so must be re-read from memory every time it is accessed.

The qualifier is most often found in code that manipulates hardware directly (such as in [embedded systems](https://en.wikipedia.org/wiki/Embedded_system) and [device drivers](https://en.wikipedia.org/wiki/Device_driver)) and in [multithreaded](https://en.wikipedia.org/wiki/Multithreading_(computer_architecture)) applications (though often used incorrectly in that context; see external links at [volatile variable](https://en.wikipedia.org/wiki/Volatile_variable)). It can be used in exactly the same manner as `const` in declarations of variables, pointers, references, and member functions, and in fact, `volatile` is sometimes used to implement a similar design-by-contract strategy which [Andrei Alexandrescu](https://en.wikipedia.org/wiki/Andrei_Alexandrescu) calls `volatile`-correctness,[[2\]](https://en.wikipedia.org/wiki/Type_qualifier#cite_note-2) though this is far less common than `const`-correctness. The `volatile` qualifier also can be stripped by `const_cast`, and it can be combined with the `const` qualifier as in this sample:

```c
// Set up a reference to a read-only hardware register that is
// mapped in a hard-coded memory location.
const volatile int & hardwareRegister  = *reinterpret_cast<int*>(0x8000);

int currentValue = hardwareRegister; // Read the memory location
int newValue = hardwareRegister;     // Read it again

hardwareRegister = 5; // Error, cannot write to a const location
```

Because `hardwareRegister` is `volatile`, there is no guarantee that it will hold the same value on two successive reads even though the programmer cannot modify it. The semantics here indicate that the register's value is read-only but not necessarily unchanging.



## Alternatives

See also: [const-correctness](https://en.wikipedia.org/wiki/Const-correctness)

Other languages take a different approach, considering constancy a property of an *identifier* (or [name binding](https://en.wikipedia.org/wiki/Name_binding)), not a *type.* Such languages thus have constant identifiers (corresponding to "variables" that do not vary) with single assignment, but do not have a notion of const-correctness: since constancy is not part of the type, there is no possibility of type mismatch. Examples include [Ada 83](https://en.wikipedia.org/wiki/Ada_83) with constant objects and a `constant` keyword,[[10\]](https://en.wikipedia.org/wiki/Type_qualifier#cite_note-10)[[a\]](https://en.wikipedia.org/wiki/Type_qualifier#cite_note-11)and Java with the `final` keyword.



