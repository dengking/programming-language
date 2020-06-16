# Type conversion

## 维基百科[Type conversion](https://en.wikipedia.org/wiki/Type_conversion)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), **type conversion**[[1\]](https://en.wikipedia.org/wiki/Type_conversion#cite_note-:0-1)[[2\]](https://en.wikipedia.org/wiki/Type_conversion#cite_note-2), **type casting**[[1\]](https://en.wikipedia.org/wiki/Type_conversion#cite_note-:0-1)[[3\]](https://en.wikipedia.org/wiki/Type_conversion#cite_note-:1-3), **type coercion**[[3\]](https://en.wikipedia.org/wiki/Type_conversion#cite_note-:1-3)**,** and **type juggling**[[4\]](https://en.wikipedia.org/wiki/Type_conversion#cite_note-4)[[5\]](https://en.wikipedia.org/wiki/Type_conversion#cite_note-5) are different ways of changing an [expression](https://en.wikipedia.org/wiki/Expression_(computer_science)) from one [data type](https://en.wikipedia.org/wiki/Data_type) to another. An example would be the conversion of an [integer](https://en.wikipedia.org/wiki/Integer_(computer_science)) value into a [floating point](https://en.wikipedia.org/wiki/Floating_point) value or its textual representation as a [string](https://en.wikipedia.org/wiki/String_(computer_science)), and vice versa. **Type conversions** can take advantage of certain features of [type hierarchies](https://en.wikipedia.org/wiki/Type_hierarchy) or [data representations](https://en.wikipedia.org/wiki/Data_representation). Two important aspects of a **type conversion** are whether it happens *implicitly* (automatically) or *explicitly[1]*[[6\]](https://en.wikipedia.org/wiki/Type_conversion#cite_note-:2-6), and whether the **underlying data representation** is converted from one representation into another, or a given representation is merely *reinterpreted* as the representation of another data type[[6\]](https://en.wikipedia.org/wiki/Type_conversion#cite_note-:2-6)[[7\]](https://en.wikipedia.org/wiki/Type_conversion#cite_note-7). In general, both [primitive](https://en.wikipedia.org/wiki/Primitive_data_type) and [compound data types](https://en.wikipedia.org/wiki/Compound_data_type) can be converted.

> NOTE: 上述对type conversion所涉及的两类问题总结得非常好：
>
> 第一个问题是type conversion的进行是implicit 还是explicit
>
> 第二个问题则是关于关于type conversion的底层实现了：它到底是通过change **underlying data representation**，还是通过reinterpret  **underlying data representation**。

> NOTE : 参见 [A Tutorial on Data Representation](https://www.ntu.edu.sg/home/ehchua/programming/java/DataRepresentation.html) 可知，不同类型的数据underlying的data representation是不同的；	

Each [programming language](https://en.wikipedia.org/wiki/Programming_language) has its own rules on how types can be converted. Languages with [strong typing](https://en.wikipedia.org/wiki/Strong_typing) typically do little implicit conversion and discourage the **reinterpretation of representations**, while languages with [weak typing](https://en.wikipedia.org/wiki/Weak_typing) perform many **implicit conversions** between data types. Weak typing language often allow forcing the [compiler](https://en.wikipedia.org/wiki/Compiler) to arbitrarily interpret（解释） a data item as having different representations—this can be a non-obvious programming error, or a technical method to directly deal with underlying hardware.

> NOTE: 按照上述strong-typing和weak-typing的描述，c就属于weak typing了。

In most languages, the word *coercion* is used to denote an ***implicit* conversion**, either during compilation or during [run time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)). For example, in an expression mixing integer and floating point numbers (like `5 + 0.1`), the **compiler** will automatically convert **integer representation** into **floating point representation** so fractions are not lost. **Explicit type conversions** are either indicated by writing additional code (e.g. adding type identifiers or calling built-in [routines](https://en.wikipedia.org/wiki/Subroutine)) or by coding conversion routines for the compiler to use when it otherwise would halt with a type mismatch.

> NOTE: convert **integer representation** into **floating point representation**  所采用的实现方式就是在第一段中提及的：change **underlying data representation** 

In most [ALGOL](https://en.wikipedia.org/wiki/ALGOL)-like languages, such as [Pascal](https://en.wikipedia.org/wiki/Pascal_(programming_language)), [Modula-2](https://en.wikipedia.org/wiki/Modula-2), [Ada](https://en.wikipedia.org/wiki/Ada_(programming_language)) and [Delphi](https://en.wikipedia.org/wiki/Delphi_(programming_language)), *conversion* and *casting* are distinctly different concepts. In these languages, ***conversion*** refers to either implicitly or explicitly changing a value from one **data type storage format** to another, e.g. a 16-bit integer to a 32-bit integer. The storage needs may change as a result of the **conversion**, including a possible loss of precision or truncation. The word ***cast***, on the other hand, refers to explicitly changing the *interpretation* of the *bit pattern* representing a value from one type to another. For example, 32 contiguous bits may be treated as an array of 32 booleans, a 4-byte string, an unsigned 32-bit integer or an IEEE single precision floating point value. Because the **stored bits** are never changed, the programmer must know low level details such as **representation format**, **byte order**, and **alignment needs**, to meaningfully cast.

> NOTE : 在这些语言中，*conversion* and *casting* 是不同的两个概念；conversion可能涉及到数据存储的改变（如缩短位数等）；而cast则不涉及数据存储的改变，cast仅仅refers to explicitly changing the *interpretation* of the *bit pattern* representing a value from one type to another. 显然，在 [ALGOL](https://en.wikipedia.org/wiki/ALGOL)-like languages 中，*conversion* 所对应的是第一段中提及的第一种实现方式，而*casting* 则对应的第二种实现方式；

> NOTE: 那c中到底采用的是哪种方式呢？下面这一段进行了解释，它的意思是c中是存在歧义的；一句我目前的认知，c采用的是change **underlying data representation**来实现的；

In the C family of languages and [ALGOL 68](https://en.wikipedia.org/wiki/ALGOL_68), the word *cast* typically refers to an *explicit* type conversion (as opposed to an implicit conversion), causing some ambiguity about whether this is a **re-interpretation of a bit-pattern** or a **real data representation conversion**. More important is the multitude（大量的） of ways and rules that apply to what data type (or class) is located by a pointer and how a pointer may be adjusted by the compiler in cases like object (class) inheritance.



### Language comparison

#### C-like languages

##### Implicit type conversion

Implicit type conversion, also known as *coercion*, is an automatic type conversion by the [compiler](https://en.wikipedia.org/wiki/Compiler). Some [programming languages](https://en.wikipedia.org/wiki/Programming_language) allow compilers to provide coercion; others require it.



In a mixed-type expression, data of one or more [subtypes](https://en.wikipedia.org/wiki/Subtyping) can be [converted](https://en.wikipedia.org/w/index.php?title=Type_conversion_in_C&action=edit&redlink=1) to a **supertype** as needed at [runtime](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)) so that the program will run correctly. For example, the following is legal [C language](https://en.wikipedia.org/wiki/C_(programming_language)) code:

```c
double  d;
long    l;
int     i;

if (d > i)   d = i;
if (i > l)   l = i;
if (d == l)  d *= 2;
```

Although **d**, **l**, and **i** belong to different data types, they will be automatically converted to equal data types each time a **comparison** or **assignment** is executed. This behavior should be used with caution, as [unintended consequences](https://en.wikipedia.org/wiki/Unintended_consequences) can arise. Data can be lost when converting representations from floating-point to integer, as the fractional components of the floating-point values will be truncated (rounded toward zero). Conversely, precision can be lost when converting representations from integer to floating-point, since a floating-point type may be unable to exactly represent an integer type. For example, `float` might be an [IEEE 754](https://en.wikipedia.org/wiki/IEEE_754) single precision type, which cannot represent the integer 16777217 exactly, while a 32-bit integer type can. This can lead to unintuitive behavior, as demonstrated by the following code:

```c
#include <stdio.h>

int main(void)
{
    int i_value   = 16777217;
    float f_value = 16777216.0;
    printf("The integer is: %d\n", i_value);
    printf("The float is:   %f\n", f_value);
    printf("Their equality: %d\n", i_value == f_value);
}
```

On compilers that implement floats as IEEE single precision, and ints as at least 32 bits, this code will give this peculiar print-out:

```c
    The integer is: 16777217
    The float is: 16777216.000000
    Their equality: 1
```

Note that 1 represents equality in the last line above. This odd behavior is caused by an implicit conversion of `i_value` to float when it is compared with `f_value`. The conversion causes loss of precision, which makes the values equal before the comparison.

Important takeaways:

1. `float` to `int` causes [truncation](https://en.wikipedia.org/wiki/Truncation), i.e., removal of the fractional part.
2. `double` to `float` causes rounding of digit.
3. `long` to `int` causes dropping of excess higher order bits.



###### Type promotion

One special case of implicit type conversion is **type promotion**, where the compiler automatically expands the binary representation of objects of integer or floating-point types. Promotions are commonly used with types smaller than the native type of the target platform's [arithmetic logic unit](https://en.wikipedia.org/wiki/Arithmetic_logic_unit) (ALU), before arithmetic and logical operations, to make such operations possible, or more efficient if the ALU can work with more than one type. C and C++ perform such promotion for objects of boolean, character, wide character, enumeration, and short integer types which are promoted to int, and for objects of type float, which are promoted to double. Unlike some other type conversions, promotions never lose precision or modify the value stored in the object.

In [Java](https://en.wikipedia.org/wiki/Java_(programming_language)):

```java
int x = 3;
double y = 3.5;
System.out.println(x+y); //The output will be 6.5
```

##### Explicit type conversion

Explicit type conversion is a type conversion which is explicitly defined within a program (instead of being done by a compiler for implicit type conversion). It is defined by the user in the program.

```c
double da = 3.3;
double db = 3.3;
double dc = 3.4;
int result = (int)da + (int)db + (int)dc; //result == 9
//if implicit conversion would be used (as with "result = da + db + dc"), result would be equal to 10
```

There are several kinds of explicit conversion.

- checked

Before the conversion is performed, a runtime check is done to see if the destination type can hold the source value. If not, an error condition is raised.

- unchecked

No check is performed. If the destination type cannot hold the source value, the result is undefined.

- bit pattern

The raw bit representation of the source is copied verbatim（一字不差的）, and it is re-interpreted according to the destination type. This can also be achieved via [aliasing](https://en.wikipedia.org/wiki/Aliasing_(computing)).

#### C# and C++

In [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)), type conversion can be made in a safe or unsafe (i.e., C-like) manner, the former called *checked type cast*.[[8\]](https://en.wikipedia.org/wiki/Type_conversion#cite_note-8)

```c#
Animal animal = new Cat();

Bulldog b = (Bulldog) animal;   // if (animal is Bulldog), stat.type(animal) is Bulldog, else an exception
b = animal as Bulldog;          // if (animal is Bulldog), b = (Bulldog) animal, else b = null

animal = null;
b = animal as Bulldog;          // b == null
```

In [C++](https://en.wikipedia.org/wiki/C%2B%2B) the similar effect can be achieved using *C++-style cast syntax*.

```c++
Animal* animal = new Cat;

Bulldog* b = static_cast<Bulldog*>(animal); // compiles only if either Animal or Bulldog is derived from the other (or same)
b = dynamic_cast<Bulldog*>(animal);         // if (animal is Bulldog), b = (Bulldog*) animal, else b = nullptr

animal = nullptr;
b = dynamic_cast<Bulldog*>(animal);         // b == nullptr
```

### Security issues

In [hacking](https://en.wikipedia.org/wiki/Hacker_(computer_security)), typecasting is the misuse of type conversion to temporarily change a [variable](https://en.wikipedia.org/wiki/Variable_(computer_science))'s data type from how it was originally defined.[[9\]](https://en.wikipedia.org/wiki/Type_conversion#cite_note-9) This provides opportunities for hackers since in type conversion after a variable is "typecast" to become a different data type, the compiler will treat that hacked variable as the new data type for that specific operation.[[10\]](https://en.wikipedia.org/wiki/Type_conversion#cite_note-10)

