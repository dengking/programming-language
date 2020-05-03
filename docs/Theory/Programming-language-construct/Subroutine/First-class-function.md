# First class function

## 维基百科[First-class function](https://en.wikipedia.org/wiki/First-class_function)

> NOTE: 我是先看到closure，在来看这篇文章的， 现在看来，两者之间的关系是：为了支持first-class function，才引入了closure。

In [computer science](https://en.wikipedia.org/wiki/Computer_science), a [programming language](https://en.wikipedia.org/wiki/Programming_language) is said to have **first-class functions** if it treats [functions](https://en.wikipedia.org/wiki/Function_(programming)) as [first-class citizens](https://en.wikipedia.org/wiki/First-class_citizen). This means the language supports:

-  passing functions as arguments to other functions
-  returning functions as the values from other functions,
-  assigning function to variables or storing them in data structures.[[1\]](https://en.wikipedia.org/wiki/First-class_function#cite_note-1) 
-  Some programming language theorists require support for [anonymous functions](https://en.wikipedia.org/wiki/Anonymous_function) (function literals) as well. 

In languages with **first-class functions**, the [names](https://en.wikipedia.org/wiki/Name_(computer_science)) of functions do not have any special status; they are treated like ordinary [variables](https://en.wikipedia.org/wiki/Variable_(computer_science)) with a [function type](https://en.wikipedia.org/wiki/Function_type). The term was coined by [Christopher Strachey](https://en.wikipedia.org/wiki/Christopher_Strachey) in the context of "functions as first-class citizens" in the mid-1960s. 

> NOTE：对于python这种类型的语言来说，一切皆对象，所以在python中，函数也是对象，所以它对first-class function支持的实现应该是相对容易的。对比来看，在c++中，并没有function type，那么c++是如何实现的呢？

**First-class functions** are a necessity for the [functional programming](https://en.wikipedia.org/wiki/Functional_programming) style, in which the use of [higher-order functions](https://en.wikipedia.org/wiki/Higher-order_function) is a standard practice. A simple example of a **higher-ordered function** is the *map* function, which takes, as its arguments, a function and a **list**, and returns the **list** formed by applying the function to each member of the list. For a language to support *map*, it must support passing a function as an argument.

> NOTE：map是一个 **higher-ordered function**，它本身是一个函数，它以另外一个函数作为参数。

There are certain implementation difficulties in passing functions as arguments or returning them as results, especially in the presence of [non-local variables](https://en.wikipedia.org/wiki/Non-local_variable) introduced in [nested](https://en.wikipedia.org/wiki/Nested_function) and [anonymous functions](https://en.wikipedia.org/wiki/Anonymous_function). Historically, these were termed the [funarg problems](https://en.wikipedia.org/wiki/Funarg_problem), the name coming from "function argument".[[5\]](https://en.wikipedia.org/wiki/First-class_function#cite_note-5) In early **imperative languages**(命令式语言) these problems were avoided by either not supporting functions as result types (e.g. [ALGOL 60](https://en.wikipedia.org/wiki/ALGOL_60), [Pascal](https://en.wikipedia.org/wiki/Pascal_(programming_language))) or omitting(省略) nested functions and thus **non-local variables** (e.g. [C](https://en.wikipedia.org/wiki/C_(programming_language))). The early functional language [Lisp](https://en.wikipedia.org/wiki/Lisp_(programming_language)) took the approach of [dynamic scoping](https://en.wikipedia.org/wiki/Dynamic_scoping), where **non-local variables** refer to the closest definition of that variable at the point where the function is executed, instead of where it was defined. Proper support for [lexically scoped](https://en.wikipedia.org/wiki/Lexically_scoped) **first-class functions** was introduced in [Scheme](https://en.wikipedia.org/wiki/Scheme_(programming_language)) and requires handling references to functions as [closures](https://en.wikipedia.org/wiki/Closure_(computer_science)) instead of bare [function pointers](https://en.wikipedia.org/wiki/Function_pointer),[[4\]](https://en.wikipedia.org/wiki/First-class_function#cite_note-strachey-4) which in turn makes [garbage collection](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science)) a necessity.

> NOTE: 这段话中提及到了[dynamic scoping](https://en.wikipedia.org/wiki/Dynamic_scoping)和 [lexically scoped](https://en.wikipedia.org/wiki/Lexically_scoped)，结合下面的内容可知， [lexically scoped](https://en.wikipedia.org/wiki/Lexically_scoped)对实现first-class function更有价值，它的实现需要依赖 [closures](https://en.wikipedia.org/wiki/Closure_(computer_science))。

### Concepts

In this section we compare how particular programming idioms are handled in a functional language with **first-class functions** ([Haskell](https://en.wikipedia.org/wiki/Haskell_(programming_language))) compared to an **imperative language**(命令式语言) where functions are **second-class citizens** ([C](https://en.wikipedia.org/wiki/C_(programming_language))).

> NOTE: 本章的内容是对比支持**first-class functions**的一些programming idioms中，函数式编程语言和**imperative language**(命令式语言) 的实现细节。

#### Higher-order functions: passing functions as arguments

Further information:[Higher-order function](https://en.wikipedia.org/wiki/Higher-order_function)

In languages where functions are **first-class citizens**, functions can be passed as arguments to other functions in the same way as other values (a function taking another function as argument is called a **higher-order function**). In the language [Haskell](https://en.wikipedia.org/wiki/Haskell_(programming_language)):

```haskell
map :: (a -> b) -> [a] -> [b]
map f []     = []
map f (x:xs) = f x : map f xs
```

Languages where functions are not **first-class** often still allow one to write **higher-order functions** through the use of features such as [function pointers](https://en.wikipedia.org/wiki/Function_pointer) or [delegates](https://en.wikipedia.org/wiki/Delegate_(CLI)). In the language [C](https://en.wikipedia.org/wiki/C_(programming_language)):

```c
void map(int (*f)(int), int x[], size_t n) {
    for (int i = 0; i < n; i++)
        x[i] = f(x[i]);
}
```

When comparing the two samples, one should note that there are a number of differences between the two approaches that are *not* directly related to the support of **first-class functions**. The `Haskell` sample operates on [lists](https://en.wikipedia.org/wiki/List_(computing)), while the `C` sample operates on [arrays](https://en.wikipedia.org/wiki/Array_data_structure). Both are the most natural compound data structures in the respective languages and making the C sample operate on linked lists would have made it unnecessarily complex. This also accounts for the fact that the C function needs an additional parameter (giving the size of the array.) The C function updates the array [in-place](https://en.wikipedia.org/wiki/In-place), returning no value, whereas in Haskell data structures are [persistent](https://en.wikipedia.org/wiki/Persistent_data_structure) (a new list is returned while the old is left intact(完整).) The `Haskell `sample uses [recursion](https://en.wikipedia.org/wiki/Recursion) to traverse the list, while the C sample uses [iteration](https://en.wikipedia.org/wiki/Iteration). Again, this is the most natural way to express this function in both languages, but the `Haskell` sample could easily have been expressed in terms of a [fold](https://en.wikipedia.org/wiki/Fold_(higher-order_function)) and the C sample in terms of recursion. Finally, the Haskell function has a [polymorphic](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)) type, as this is not supported by C we have fixed all type variables to the type constant `int`.

#### Anonymous and nested functions

Further information: [Anonymous function](https://en.wikipedia.org/wiki/Anonymous_function) and [Nested function](https://en.wikipedia.org/wiki/Nested_function)

In languages supporting **anonymous functions**, we can pass such a function as an argument to a higher-order function:

```haskell
main = map (\x -> 3 * x + 1) [1, 2, 3, 4, 5]
```

In a language which does not support anonymous functions, we have to **bind** it to a name instead:

```c++
int f(int x) {
    return 3 * x + 1;
}

int main() {
    int list[] = {1, 2, 3, 4, 5};
    map(f, list, 5);
}
```

#### Non-local variables and closures

Further information: [Non-local variable](https://en.wikipedia.org/wiki/Non-local_variable) and [Closure (computer science)](https://en.wikipedia.org/wiki/Closure_(computer_science))

Once we have **anonymous or nested functions**, it becomes natural for them to refer to variables outside of their body (called ***non-local variables***):

```c++
main = let a = 3
           b = 1
        in map (\x -> a * x + b) [1, 2, 3, 4, 5]
```

If functions are represented with bare(裸) function pointers(裸指针，说明没有携带外层函数的一些信息), we can not know anymore how the value that is outside of the function's body should be passed to it, and because of that a **closure** needs to be built manually(手动地来构造closure). Therefore we can not speak of "first-class" functions here.

```c
typedef struct {
    int (*f)(int, int, int);
    int *a;
    int *b;
} closure_t;

void map(closure_t *closure, int x[], size_t n) {
    for (int i = 0; i < n; ++i)
        x[i] = (*closure->f)(*closure->a, *closure->b, x[i]);
}

int f(int a, int b, int x) {
    return a * x + b;
}

void main() {
    int l[] = {1, 2, 3, 4, 5};
    int a = 3;
    int b = 1;
    closure_t closure = {f, &a, &b};
    map(&closure, l, 5);
}
```

Also note that the `map` is now specialized to functions referring to two `int`s outside of their environment. This can be set up more generally, but requires more [boilerplate code](https://en.wikipedia.org/wiki/Boilerplate_code). If `f` would have been a [nested function](https://en.wikipedia.org/wiki/Nested_function) we would still have run into the same problem and this is the reason they are not supported in C.

#### Higher-order functions: returning functions as results

When returning a function, we are in fact returning its **closure**(返回一个函数，升级上是返回该函数的closure). In the C example any **local variables** captured by the **closure** will go out of scope once we return from the function that builds the **closure**(当从构造closure的函数中返回时，任何由closure捕获的**local variables**将离开他们原本所属的scope，即超出scope). Forcing the **closure** at a later point will result in undefined behaviour, possibly corrupting the stack. This is known as the [upwards funarg problem](https://en.wikipedia.org/wiki/Upwards_funarg_problem).

总结：Higher-order functions不仅仅指那些以函数作为参数的函数，还指那些返回函数的函数。参见[Higher-order function](https://en.wikipedia.org/wiki/Higher-order_function)。

#### Assigning functions to variables

[Assigning](https://en.wikipedia.org/wiki/Assignment_(computer_science)) functions to [variables](https://en.wikipedia.org/wiki/Variable_(computer_science)) and storing them inside (global) datastructures potentially suffers from the same difficulties as returning functions.

```haskell
f :: [[Integer] -> [Integer]]
f = let a = 3
        b = 1
     in [map (\x -> a * x + b), map (\x -> b * x + a)]
```

#### Equality of functions

Further information: [Function equality](https://en.wikipedia.org/w/index.php?title=Function_equality&action=edit&redlink=1)

As one can test most literals and values for equality, it is natural to ask whether a programming language can support testing functions for equality. On further inspection, this question appears more difficult and one has to distinguish between several types of function equality:[[7\]](https://en.wikipedia.org/wiki/First-class_function#cite_note-7)

- [Extensional equality](https://en.wikipedia.org/wiki/Extensional_equality)

  Two functions *f* and *g* are considered extensionally equal if they agree on their outputs for all inputs (∀*x*. *f*(*x*) = *g*(*x*)). Under this definition of equality, for example, any two implementations of a [stable sorting algorithm](https://en.wikipedia.org/wiki/Stable_sorting_algorithm), such as [insertion sort](https://en.wikipedia.org/wiki/Insertion_sort) and [merge sort](https://en.wikipedia.org/wiki/Merge_sort), would be considered equal. Deciding on extensional equality is [undecidable](https://en.wikipedia.org/wiki/Undecidable_problem) in general and even for functions with finite domains often intractable. For this reason no programming language implements function equality as extensional equality.

- [Intensional equality](https://en.wikipedia.org/wiki/Intensional_equality)

  Under intensional equality, two functions *f* and *g* are considered equal if they have the same "internal structure". This kind of equality could be implemented in [interpreted languages](https://en.wikipedia.org/wiki/Interpreted_language) by comparing the [source code](https://en.wikipedia.org/wiki/Source_code) of the function bodies (such as in Interpreted Lisp 1.5) or the [object code](https://en.wikipedia.org/wiki/Object_code) in [compiled languages](https://en.wikipedia.org/wiki/Compiled_language). Intensional equality implies extensional equality (under the assumption that the functions do not depend on the value of the [program counter](https://en.wikipedia.org/wiki/Program_counter).)

- [Reference equality](https://en.wikipedia.org/w/index.php?title=Reference_equality&action=edit&redlink=1)

  Given the impracticality of implementing extensional and intensional equality, most languages supporting testing functions for equality use reference equality. All functions or closures are assigned a unique identifier (usually the address of the function body or the closure) and equality is decided based on equality of the identifier. Two separately defined, but otherwise identical function definitions will be considered unequal. Referential equality implies intensional and extensional equality. Referential equality breaks [referential transparency](https://en.wikipedia.org/wiki/Referential_transparency_(computer_science)) and is therefore not supported in [pure](https://en.wikipedia.org/w/index.php?title=Purity_(computer_science)&action=edit&redlink=1) languages, such as Haskell.
  

### Type theory

Main article: [Function type](https://en.wikipedia.org/wiki/Function_type)

In [type theory](https://en.wikipedia.org/wiki/Type_theory), the type of functions accepting values of type *A* and returning values of type *B* may be written as *A* → *B* or *B**A*. In the [Curry-Howard correspondence](https://en.wikipedia.org/wiki/Curry-Howard_correspondence), [function types](https://en.wikipedia.org/wiki/Function_type) are related to [logical implication](https://en.wikipedia.org/wiki/Logical_implication); lambda abstraction corresponds to discharging hypothetical assumptions and function application corresponds to the [modus ponens](https://en.wikipedia.org/wiki/Modus_ponens) inference rule. Besides the usual case of programming functions, type theory also uses first-class functions to model [associative arrays](https://en.wikipedia.org/wiki/Associative_array) and similar [data structures](https://en.wikipedia.org/wiki/Data_structure).

In [category-theoretical](https://en.wikipedia.org/wiki/Category_theory) accounts of programming, the availability of **first-class functions** corresponds to the [closed category](https://en.wikipedia.org/wiki/Closed_category) assumption. For instance, the [simply typed lambda calculus](https://en.wikipedia.org/wiki/Simply_typed_lambda_calculus) corresponds to the internal language of [cartesian closed categories](https://en.wikipedia.org/wiki/Cartesian_closed_category).

### Language support

Functional programming languages, such as [Scheme](https://en.wikipedia.org/wiki/Scheme_(programming_language)), [ML](https://en.wikipedia.org/wiki/ML_(programming_language)), [Haskell](https://en.wikipedia.org/wiki/Haskell_(programming_language)), [F#](https://en.wikipedia.org/wiki/F_Sharp_(programming_language)), and [Scala](https://en.wikipedia.org/wiki/Scala_(programming_language)), all have **first-class functions**. When [Lisp](https://en.wikipedia.org/wiki/Lisp_(programming_language)), one of the earliest functional languages, was designed, not all aspects of first-class functions were then properly understood, resulting in functions being **dynamically scoped**. The later [Scheme](https://en.wikipedia.org/wiki/Scheme_(programming_language)) and [Common Lisp](https://en.wikipedia.org/wiki/Common_Lisp) dialects do have **lexically scoped** first-class functions.

总结：对于上面这段话的理解依赖于理解**dynamically scoped**和**lexically scoped**。

Many scripting languages, including [Perl](https://en.wikipedia.org/wiki/Perl), [Python](https://en.wikipedia.org/wiki/Python_(programming_language)), [PHP](https://en.wikipedia.org/wiki/PHP), [Lua](https://en.wikipedia.org/wiki/Lua_(programming_language)), [Tcl](https://en.wikipedia.org/wiki/Tcl)/Tk, [JavaScript](https://en.wikipedia.org/wiki/JavaScript) and [Io](https://en.wikipedia.org/wiki/Io_(programming_language)), have **first-class functions**.

For imperative languages, a distinction has to be made between Algol and its descendants such as Pascal, the traditional **C family**, and the modern garbage-collected variants. The Algol family has allowed **nested functions** and **higher-order** taking function as arguments, but not higher-order functions that return functions as results (except Algol 68, which allows this). The reason for this was that it was not known how to deal with **non-local variables** if a nested-function was returned as a result (and Algol 68 produces runtime errors in such cases).

The C family allowed both passing functions as arguments and returning them as results, but avoided any problems by not supporting **nested functions**(显然不支持**nested functions**就避免了**non-local variables**问题). (The `gcc` compiler allows them as an extension.) As the usefulness of returning functions primarily lies in the ability to return **nested functions** that have captured **non-local variables**, instead of top-level functions, these languages are generally not considered to have first-class functions.

Modern imperative languages often support garbage-collection making the implementation of first-class functions feasible. First-class functions have often only been supported in later revisions of the language, including C# 2.0 and Apple's Blocks extension to C, C++ and Objective-C. C++11 has added support for **anonymous functions** and **closures** to the language, but because of the **non-garbage collected** nature of the language, special care has to be taken for **non-local variables** in functions to be returned as results (see below).

#### C++

[C++11](https://en.wikipedia.org/wiki/C%2B%2B11) closures can capture non-local variables 

- by reference (without extending their lifetime)
- by copy construction 
- by [move construction](https://en.wikipedia.org/wiki/C%2B%2B11#Rvalue_references_and_move_constructors) (the variable lives as long as the closure does). 

The former potentially avoids an expensive copy and allows to modify the original variable but is unsafe in case the closure is returned (see [dangling references](https://en.wikipedia.org/wiki/Dangling_reference)). 

The second is safe if the **closure** is returned but requires a copy and cannot be used to modify the original variable (which might not exist any more at the time the closure is called).

 The latter is safe if the closure is returned and does not require a copy but cannot be used to modify the original variable either.

#### Python

Explicit partial application with `functools.partial` since version 2.5, and `operator.methodcaller` since version 2.6.

