# Virtual method table



## wikipedia [Virtual method table](https://en.wikipedia.org/wiki/Virtual_method_table)

A **virtual method table** (**VMT**), **virtual function table**, **virtual call table**, [dispatch table](https://en.wikipedia.org/wiki/Dispatch_table), **vtable**, or **vftable** is a mechanism used in a [programming language](https://en.wikipedia.org/wiki/Programming_language) to support [dynamic dispatch](https://en.wikipedia.org/wiki/Dynamic_dispatch) (or [run-time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)) [method](https://en.wikipedia.org/wiki/Method_(computer_programming)) [binding](https://en.wikipedia.org/wiki/Name_binding)).

Whenever a class defines a [virtual function](https://en.wikipedia.org/wiki/Virtual_function) (or method), most compilers add a **hidden member variable** to the class that points to an array of pointers to (virtual) functions called the **virtual method table**. These pointers are used at runtime to invoke the appropriate function implementations, because at compile time it may not yet be known if the base function is to be called or a derived one implemented by a class that inherits from the base class.

There are many different ways to implement such **dynamic dispatch**, but use of **virtual method tables** is especially common among [C++](https://en.wikipedia.org/wiki/C%2B%2B) and related languages (such as [D](https://en.wikipedia.org/wiki/D_(programming_language)) and [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language))). Languages that separate the programmatic interface of objects from the implementation, like [Visual Basic](https://en.wikipedia.org/wiki/Visual_Basic) and [Delphi](https://en.wikipedia.org/wiki/Object_Pascal), also tend to use this approach, because it allows objects to use a different implementation simply by using a different set of method pointers.

Suppose a program contains three [classes](https://en.wikipedia.org/wiki/Class_(computer_programming)) in an [inheritance](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)) hierarchy: a [superclass](https://en.wikipedia.org/wiki/Superclass_(computer_science)), `Cat`, and two [subclasses](https://en.wikipedia.org/wiki/Subclass_(computer_science)), `HouseCat` and `Lion`. Class `Cat` defines a [virtual function](https://en.wikipedia.org/wiki/Virtual_function) named `speak`, so its subclasses may provide an appropriate implementation (e.g. either `meow` or `roar`). When the program calls the `speak` function on a `Cat` reference (which can refer to an instance of `Cat`, or an instance of `HouseCat` or `Lion`), the code must be able to determine which implementation of the function the call should be *dispatched* to. This depends on the actual class of the object, not the class of the reference to it (`Cat`). The class can not generally be determined *statically*(that is, at [compile time](https://en.wikipedia.org/wiki/Compile_time)), so neither can the compiler decide which function to call at that time. The call must be dispatched to the right function *dynamically* (that is, at [run time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase))) instead.

### Implementation

An object's **virtual method table** will contain the [addresses](https://en.wikipedia.org/wiki/Memory_address) of the object's dynamically bound methods. Method calls are performed by fetching the method's address from the object's **virtual method table**. The **virtual method table** is the same for all objects belonging to the same class, and is therefore typically shared between them. Objects belonging to type-compatible classes (for example siblings in an inheritance hierarchy) will have **virtual method tables** with the same **layout**: the address of a given method will appear at **the same offset** for all type-compatible classes. Thus, fetching the method's address from a given offset into a **virtual method table** will get the method corresponding to the object's actual class.[[1\]](https://en.wikipedia.org/wiki/Virtual_method_table#cite_note-1)

The [C++](https://en.wikipedia.org/wiki/C%2B%2B) standards do not mandate exactly how dynamic dispatch must be implemented, but compilers generally use minor variations on the same basic model.

Typically, the compiler creates a separate **virtual method table** for each class. When an object is created, a pointer to this table, called the **virtual table pointer**, **vpointer** or **VPTR**, is added as a hidden member of this object. As such, the compiler must also generate "hidden" code in the [constructors](https://en.wikipedia.org/wiki/Constructor_(object-oriented_programming)) of each class to initialize a new object's **virtual table pointer** to the address of its class's **virtual method table**.

Many compilers place the virtual table pointer as the last member of the object; other compilers place it as the first; portable source code works either way.[[2\]](https://en.wikipedia.org/wiki/Virtual_method_table#cite_note-2) For example, [g++](https://en.wikipedia.org/wiki/G%2B%2B) previously placed the pointer at the end of the object.

### Example

Consider the following class declarations in [C++ syntax](https://en.wikipedia.org/wiki/C%2B%2B_syntax):

```cpp
class B1 {
public:
  virtual ~B1() {}
  void f0() {}
  virtual void f1() {}
  int int_in_b1;
};

class B2 {
public:
  virtual ~B2() {}
  virtual void f2() {}
  int int_in_b2;
};
```

used to derive the following class:

```cpp
class D : public B1, public B2 {
public:
  void d() {}
  void f2() {}  // override B2::f2()
  int int_in_d;
};
```

and the following piece of C++ code:

```cpp
B2 *b2 = new B2();
D  *d  = new D();
```

g++ 3.4.6 from [GCC](https://en.wikipedia.org/wiki/GNU_Compiler_Collection) produces the following 32-bit memory layout for the object `b2`:[[nb 1\]](https://en.wikipedia.org/wiki/Virtual_method_table#cite_note-4)

```shell
b2:
  +0: pointer to virtual method table of B2
  +4: value of int_in_b2

virtual method table of B2:
  +0: B2::f2()   
```

and the following memory layout for the object `d`:

```shell
d:
  +0: pointer to virtual method table of D (for B1)
  +4: value of int_in_b1
  +8: pointer to virtual method table of D (for B2)
 +12: value of int_in_b2
 +16: value of int_in_d

Total size: 20 Bytes.

virtual method table of D (for B1):
  +0: B1::f1()  // B1::f1() is not overridden

virtual method table of D (for B2):
  +0: D::f2()   // B2::f2() is overridden by D::f2()
```

Note that those functions not carrying the keyword `virtual` in their declaration (such as `f0()` and `d()`) do not generally appear in the **virtual method table**. There are exceptions for special cases as posed by the [default constructor](https://en.wikipedia.org/wiki/Default_constructor).

Also note the [virtual destructors](https://en.wikipedia.org/wiki/Virtual_function#Virtual_destructors) in the base classes, `B1` and `B2`. They are necessary to ensure `delete d` can free up memory not just for `D`, but also for `B1` and `B2`, if `d` is a pointer or reference to the types `B1` or `B2`. They were excluded from the memory layouts to keep the example simple. [[nb 2\]](https://en.wikipedia.org/wiki/Virtual_method_table#cite_note-5)

Overriding of the method `f2()` in class `D` is implemented by duplicating the virtual method table of `B2` and replacing the pointer to `B2::f2()` with a pointer to `D::f2()`.

### Multiple inheritance and thunks

The g++ compiler implements the [multiple inheritance](https://en.wikipedia.org/wiki/Multiple_inheritance) of the classes `B1` and `B2` in class `D` using two virtual method tables, one for each base class. (There are other ways to implement multiple inheritance, but this is the most common.) This leads to the necessity for "pointer fixups", also called [thunks](https://en.wikipedia.org/wiki/Thunk_(programming)), when [casting](https://en.wikipedia.org/wiki/Type_conversion).

Consider the following C++ code:

```cpp
D  *d  = new D();
B1 *b1 = d;
B2 *b2 = d;
```

While `d` and `b1` will point to the same memory location after execution of this code, `b2` will point to the location `d+8` (eight bytes beyond the memory location of `d`). Thus, `b2` points to the region within `d` that "looks like" an instance of `B2`, i.e., has the same memory layout as an instance of `B2`.

### Invocation

A call to `d->f1()` is handled by dereferencing `d`'s `D::B1` **vpointer**, looking up the `f1` entry in the **virtual method table**, and then dereferencing that pointer to call the code.

In the case of single inheritance (or in a language with only single inheritance), if the **vpointer** is always the first element in `d` (as it is with many compilers), this reduces to the following pseudo-C++:

```cpp
(*((*d)[0]))(d)
```

Where *d refers to the virtual method table of D and [0] refers to the first method in the virtual method table. The parameter d becomes the ["this" pointer](https://en.wikipedia.org/wiki/This_(computer_science)) to the object.

In the more general case, calling `B1::f1()` or `D::f2()` is more complicated:

```
(*(*(d[+0]/*pointer to virtual method table of D (for B1)*/)[0]))(d)   /* Call d->f1() */
(*(*(d[+8]/*pointer to virtual method table of D (for B2)*/)[0]))(d+8) /* Call d->f2() */
```

The call to d->f1() passes a B1 pointer as a parameter. The call to d->f2() passes a B2 pointer as a parameter. This second call requires a fixup to produce the correct pointer. The location of B2::f2 is not in the virtual method table for D.

By comparison, a call to `d->f0()` is much simpler:

```
(*B1::f0)(d)
```

### Efficiency

A **virtual call** requires at least an extra indexed dereference and sometimes a "fixup" addition, compared to a non-virtual call, which is simply a jump to a compiled-in pointer. Therefore, calling **virtual functions** is inherently slower than calling **non-virtual functions**. An experiment done in 1996 indicates that approximately 6–13% of execution time is spent simply dispatching to the correct function, though the overhead can be as high as 50%.[[4\]](https://en.wikipedia.org/wiki/Virtual_method_table#cite_note-6)The cost of virtual functions may not be so high on modern CPU architectures due to much larger caches and better [branch prediction](https://en.wikipedia.org/wiki/Branch_predictor).

Furthermore, in environments where [JIT compilation](https://en.wikipedia.org/wiki/Just-in-time_compilation) is not in use, virtual function calls usually cannot be [inlined](https://en.wikipedia.org/wiki/Inline_expansion). In certain cases it may be possible for the compiler to perform a process known as *devirtualization* in which, for instance, the lookup and indirect call are replaced with a conditional execution of each inlined body, but such optimizations are not common.

To avoid this overhead, compilers usually avoid using virtual method tables whenever the call can be resolved at [compile time](https://en.wikipedia.org/wiki/Compile_time).

Thus, the call to `f1` above may not require a table lookup because the compiler may be able to tell that `d` can only hold a `D` at this point, and `D` does not override `f1`. Or the compiler (or optimizer) may be able to detect that there are no subclasses of `B1` anywhere in the program that override `f1`. The call to `B1::f1` or `B2::f2` will probably not require a table lookup because the implementation is specified explicitly (although it does still require the 'this'-pointer fixup).

### Comparison with alternatives

The virtual method table is generally a good performance trade-off to achieve dynamic dispatch, but there are alternatives, such as [binary tree dispatch](https://en.wikipedia.org/w/index.php?title=Binary_tree_dispatch&action=edit&redlink=1), with higher performance but different costs.[[5\]](https://en.wikipedia.org/wiki/Virtual_method_table#cite_note-7)

However, virtual method tables only allow for [single dispatch](https://en.wikipedia.org/wiki/Single_dispatch) on the special "this" parameter, in contrast to [multiple dispatch](https://en.wikipedia.org/wiki/Multiple_dispatch) (as in [CLOS](https://en.wikipedia.org/wiki/Common_Lisp_Object_System) or [Dylan](https://en.wikipedia.org/wiki/Dylan_(programming_language))), where the types of all parameters can be taken into account in dispatching.

Virtual method tables also only work if dispatching is constrained to a known set of methods, so they can be placed in a simple array built at compile time, in contrast to [duck typing](https://en.wikipedia.org/wiki/Duck_typing) languages (such as [Smalltalk](https://en.wikipedia.org/wiki/Smalltalk), [Python](https://en.wikipedia.org/wiki/Python_(programming_language)) or [JavaScript](https://en.wikipedia.org/wiki/JavaScript)).

Languages that provide either or both of these features often dispatch by looking up a string in a [hash table](https://en.wikipedia.org/wiki/Hash_table), or some other equivalent method. There are a variety of techniques to make this faster (e.g., [interning](https://en.wikipedia.org/wiki/String_interning)/tokenizing method names, caching lookups, [just-in-time compilation](https://en.wikipedia.org/wiki/Just-in-time_compilation)).

### See also

- [Virtual function](https://en.wikipedia.org/wiki/Virtual_function)
- [Virtual inheritance](https://en.wikipedia.org/wiki/Virtual_inheritance)
- [Branch table](https://en.wikipedia.org/wiki/Branch_table)



