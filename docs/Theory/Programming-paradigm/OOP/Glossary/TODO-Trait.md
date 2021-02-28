# Trait 

## wikipedia [Trait (computer programming)](https://en.wikipedia.org/wiki/Trait_(computer_programming))

In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), a **trait** is a concept used in [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming), which represents a set of [methods](https://en.wikipedia.org/wiki/Method_(computer_programming)) that can be used to extend the functionality of a [class](https://en.wikipedia.org/wiki/Class_(computer_science)).

## Characteristics

**Traits** both provide a set of methods that implement behaviour to a class, and require that the class implement a set of methods that [parameterize](https://en.wikipedia.org/wiki/Parameter_(computer_programming)) the provided behaviour.

For inter-object communication, **traits** are somewhat between an object-oriented [protocol (interface)](https://en.wikipedia.org/wiki/Protocol_(object-oriented_programming)) and a [mixin](https://en.wikipedia.org/wiki/Mixin). An **interface** may define one or more behaviors via [***method signatures***](https://en.wikipedia.org/wiki/Function_prototype), while a **trait** defines behaviors via full ***method definitions***: i.e., it includes the [body of the methods](https://en.wikipedia.org/wiki/Class_implementation_file). In contrast, mixins include full ***method definitions*** and may also carry [state](https://en.wikipedia.org/wiki/State_(computer_science)) through member variable, while traits usually don't.

> NOTE: this paragraph summarize the difference between **interface**,**trait** and **mixin**.

Hence an object defined as a **trait** is created as the composition of methods, which can be used by other classes without requiring [multiple inheritance](https://en.wikipedia.org/wiki/Multiple_inheritance). In case of a [naming collision](https://en.wikipedia.org/wiki/Naming_collision), when more than one trait to be used by a class has a method with the same name, the programmer must explicitly disambiguate which one of those methods will be used in the class; thus manually solving the *diamond problem* of multiple inheritance. This is different from other composition methods in object-oriented programming, where conflicting names are automatically resolved by [scoping rules](https://en.wikipedia.org/wiki/Scope_(computer_science)).

> NOTE: trait can be used by other classes without requiring multiple inheritance whereas mixin need

Whereas(然而) mixins can be composed only using the [inheritance](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)) operation, traits offer a much wider selection of operations, including:[[3\]](https://en.wikipedia.org/wiki/Trait_(computer_programming)#cite_note-3)[[4\]](https://en.wikipedia.org/wiki/Trait_(computer_programming)#cite_note-4)

- *symmetric sum*: an operation that merges two disjoint traits to create a new trait
- *override* (or *asymmetric sum*): an operation that forms a new trait by adding methods to an existing trait, possibly overriding some of its methods
- *alias*: an operation that creates a new trait by adding a new name for an existing method
- *exclusion*: an operation that forms a new trait by removing a method from an existing trait. (Combining this with the alias operation yields a *shallow rename* operation).

Traits are composed in the following ways:

- Trait composition is commutative; the ordering of adding traits does not matter. For example, given trait *S* = *A* + *B*, then trait *T* = *B* + *A* is the same as *S*.
- Conflicting methods are excluded from the composition.
- Nested traits are equivalent to flattened traits; the composition hierarchy does not affect the traits behaviour. For example, given trait *S* = *A* + *X*, where *X* = *B* + *C*, then trait *T* = *A* + *B* + *C* is the same as *S*.[[1\]](https://en.wikipedia.org/wiki/Trait_(computer_programming)#cite_note-schaerli-ecoop-2003-1)

## Supported languages

- [C++](https://en.wikipedia.org/wiki/C%2B%2B): Used in [Standard Template Library](https://en.wikipedia.org/wiki/Standard_Template_Library) and the [C++ standard library](https://en.wikipedia.org/wiki/C%2B%2B_standard_library) to support generic [container classes](https://en.wikipedia.org/wiki/Container_(data_structure))[[7\]](https://en.wikipedia.org/wiki/Trait_(computer_programming)#cite_note-7)[[8\]](https://en.wikipedia.org/wiki/Trait_(computer_programming)#cite_note-8) and in the [Boost](https://en.wikipedia.org/wiki/Boost_C%2B%2B_Libraries) TypeTraits library.

