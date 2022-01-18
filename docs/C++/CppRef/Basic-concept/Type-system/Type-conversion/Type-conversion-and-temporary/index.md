# Type conversion and temporary

需要考虑type conversion是否会创建一个temporary。需要从底层来进行考虑。





https://gustedt.wordpress.com/2014/04/02/dont-use-casts-i/

https://en.wikibooks.org/wiki/C%2B%2B_Programming/Programming_Languages/C%2B%2B/Code/Statements/Variables/Type_Casting


https://wiki.sei.cmu.edu/confluence/display/c/INT02-C.%2BUnderstand%2Binteger%2Bconversion%2Brules

## [What exactly is a type cast in C/C++?](https://stackoverflow.com/questions/7558837/what-exactly-is-a-type-cast-in-c-c)

What exactly is a type cast in C/C++? How does the compiler check if an explicit typecast is needed (and valid)? Does it compare the space required for an value? If I have for example:

```c
int a;
double b = 15.0;
a = (int) b;
```

If I remember correctly a double value requires more space (was it 8 bytes?!) than an integer (4 bytes). And the internal representation of both are completely different (complement on two/mantissa). So what happens internally? The example here is quite straightforward, but in C/C++ there are plentiful typecasts.

How does the compiler know (or the programmer) if I can cast e.g. `FOO` to `BAR`?

***COMMENTS***

Any complete explanation of type casts needs to include standard conversions, user-defined conversions, `reinterpret_cast`, `const_cast`, upcasts, downcasts, cross-casts, RTTI, and value types. (And I think I forgot a topic.) – [Ben Voigt](https://stackoverflow.com/users/103167/ben-voigt) [Sep 26 '11 at 17:38](https://stackoverflow.com/questions/7558837/what-exactly-is-a-type-cast-in-c-c#comment9162516_7558837)

possible duplicate of [When should static_cast, dynamic_cast and reinterpret_cast be used?](http://stackoverflow.com/questions/332030/when-should-static-cast-dynamic-cast-and-reinterpret-cast-be-used) – [Mark B](https://stackoverflow.com/users/251738/mark-b) [Sep 26 '11 at 17:50](https://stackoverflow.com/questions/7558837/what-exactly-is-a-type-cast-in-c-c#comment9162682_7558837)



### [A](https://stackoverflow.com/a/7558911)

A type cast is basically a conversion from one type to another. It can be implicit (i.e., done automatically by the compiler, perhaps losing info in the process) or explicit (i.e., specified by the developer in the code). The space occupied by the types is of secondary importance. More important is the applicability (and sometimes convenice) of conversion.

It is possible for implicit conversions to lose information, signs can be lost / gained, and overflow / underflow can occur. The compiler will not protect you from these events, except maybe through a warning that is generated at compile time. Slicing can also occur when a derived type is implicitly converted to a base type (by value).

For conversions that can be downright dangerous (e.g., from a base to a derived type), the C++ standard requires an explicit cast. Not only that, but it offers more restrictive explicit casts, such as `static_cast`, `dynamic_cast`, `reinterpret_cast`, and `const_cast`, each of which further restricts the explicit cast to only a subset of possible conversions, reducing the potential for casting errors.

Valid conversions, both implicit and explict are ultimately defined by the C/C++ standards, although in C++, the developer has the ability to extend conversions for user defined types, both implicit and explicit, via the use of constructors and overloaded (cast) operators.

The complete rules for which casts are allowed by the standards and which are not can get quite intricate. I have tried to faithfully present a somewhat concise summary of some of those rules in this answer. If you are truly interested in what is and is not allowed, I strongly urge you to visit the standards and read the respective sections on type conversion.



### [A](https://stackoverflow.com/a/7559375)

Just want to mention something frequently overlooked:

- A cast always creates a temporary of the target type (although if the target type is a reference, you won't notice).

This can be important. For example:

```C
#include <iostream>

void change_one_print_other( int& a, const int& b )
{
    a = 0;
    std::cout << b << "\n";
}

int main(void)
{
    int x = 5, y = 5;

    change_one_print_other(x, x);
    change_one_print_other(y, static_cast<int>(y));
}
```

That cast LOOKS useless. But [looks can be deceiving](http://rextester.com/FIC8075).



***COMMENTS*** :

"*A cast always creates a temporary of the target type (although if the target type is a reference, you won't notice)*" If the target type is a reference, than the cast will not create a temporary; it will make a lvalue. `void change_one_print_other( int& a, const int& b ); change_one_print_other(y, static_cast<int>(y));` If you want to avoid this mistake (passing a reference to a temporary), you need to take a pointer: `void change_one_print_other( int& a, const int* pb );` To everybody here: I am new to this system, sorry if I don't use this site's features correctly. – [curiousguy](https://stackoverflow.com/users/963864/curiousguy) [Sep 27 '11 at 12:06](https://stackoverflow.com/questions/7558837/what-exactly-is-a-type-cast-in-c-c#comment9176129_7559375)



Ben Voigt wrote: "Since references are transparent, you can't tell if a temporary reference was created or not. The temporary reference is bound to the same target, and yes it's an lvalue (unless it's an rvalue reference, of course)." (comment of a deleted post) – [curiousguy](https://stackoverflow.com/users/963864/curiousguy) [Sep 27 '11 at 12:08](https://stackoverflow.com/questions/7558837/what-exactly-is-a-type-cast-in-c-c#comment9176154_7559375)



I meant to say that a temporary is never a reference, and a reference is never a temporary. Unless it changed, a temporary is a rvalue and a reference is a lvalue. This may be nitpicking, but IMO when dealing with C++ it is important to use the most precise vocabulary. – [curiousguy](https://stackoverflow.com/users/963864/curiousguy) [Sep 27 '11 at 12:10](https://stackoverflow.com/questions/7558837/what-exactly-is-a-type-cast-in-c-c#comment9176207_7559375)

 

@curiousguy: `(*p)` is an lvalue, even when `p` is a temporary. You can think about a reference the same way, but since naming a reference always yields the value it refers to (the reference is totally transparent), it's impossible to tell whether a temporary reference was created. Just like you can't ask the compiler how big a reference is, you can only ask how big the target of the reference is. But that doesn't mean that the reference doesn't take up storage -- it frequently does. – [Ben Voigt](https://stackoverflow.com/users/103167/ben-voigt) [Sep 27 '11 at 14:05](https://stackoverflow.com/questions/7558837/what-exactly-is-a-type-cast-in-c-c#comment9178573_7559375)

 

Apparently, reinterpret_cast does not create this but, yes, static_cast does. – [user1284631](https://stackoverflow.com/users/1284631/user1284631) [Sep 17 '14 at 14:45](https://stackoverflow.com/questions/7558837/what-exactly-is-a-type-cast-in-c-c#comment40525712_7559375)



## improgrammer [Type Casting In C Language](https://www.improgrammer.net/type-casting-c-language/)

**Type casting** is a way to convert a variable from one data type to another data type. For example, if you want to store a long value into a simple integer then you can typecast long to int. You can convert values from one type to another explicitly using the cast operator. There are two types of **type casting** in c language that are Implicit conversions and Explicit Conversions. In this article, we also learn about the **difference between type casting and type conversions.**

> NOTE : 将`long`存储到`int`类型，`long`类型的长度大于`int`类型，显然`int`类型是无法将`long`类型的全部都存入的呀。