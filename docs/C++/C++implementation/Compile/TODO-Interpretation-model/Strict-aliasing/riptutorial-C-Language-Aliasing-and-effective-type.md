# riptutorial [C Language Aliasing and effective type](https://riptutorial.com/c/topic/1301/aliasing-and-effective-type)



## Remarks[#](https://riptutorial.com/c/topic/1301/aliasing-and-effective-type#remarks)

Violations of **aliasing rules** and of violating the effective type of an object are two different things and should not be confounded.

- *Aliasing* is the property of two pointers `a` and `b` that refer to the same object, that is that `a == b`.
- The *effective type* of a data object is used by C to determine which operations can be done on that object. In particular the effective type is used to determine if two pointers can alias each other.

Aliasing can be a problem for optimization, because changing the object through one pointer, `a` say, can change the object that is visible through the other pointer, `b`. If your C compiler would have to assume that pointers could always alias each other, regardless of their type and provenance, many optimization opportunities would be lost, and many programs would run slower.

C's strict aliasing rules refers to cases in the compiler *may assume* which objects do (or do not) alias each other. There are two rules of thumb that you always should have in mind for data pointers.

> Unless said otherwise, two pointers with the same base type may alias.

> Two pointers with different base type cannot alias, unless at least one of the two types is a character type.

Here *base type* means that we put aside type qualifications such as `const`, e.g. If `a` is `double*` and `b` is `const double*`, the compiler *must* generally assume that a change of `*a` may change `*b`.

Violating the second rule can have catastrophic results. Here violating the strict aliasing rule means that you present two pointers `a` and `b` of different type to the compiler which in reality point to the same object. The compiler then may always assume that the two point to different objects, and will not update its idea of `*b` if you changed the object through `*a`.

If you do so the behavior of your program becomes undefined. Therefore, C puts quite severe restrictions on pointer conversions in order to help you to avoid such situation to occur accidentally.

> Unless the source or target type is `void`, all pointer conversions between pointers with different base type must be *explicit*.

Or in other words, they need a *cast*, unless you do a conversion that just adds a qualifier such as `const` to the target type.

Avoiding pointer conversions in general and casts in particular protects you from aliasing problems. Unless you really need them, and these cases are very special, you should avoid them as you can.