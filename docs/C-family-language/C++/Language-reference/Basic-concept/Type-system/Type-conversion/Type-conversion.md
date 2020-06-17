# [type conversions](http://www.cplusplus.com/doc/tutorial/typecasting/)

## Implicit conversion

**Implicit conversions** are automatically performed when a value is copied to a **compatible type**. For example:

```cpp
short a=2000;
int b;
b=a;

```

Here, the value of `a` is promoted(提升) from `short` to `int` without the need of any **explicit operator**. This is known as a **standard conversion**. Standard conversions affect fundamental data types, and allow the conversions between numerical types (short to int, int to float, double to int...), to or from `bool`, and some pointer conversions.

总结：**standard conversion**是**implicit conversion**，但是**implicit conversion**不一定是**standard conversion**

Converting to int from some smaller integer type, or to double from float is known as **promotion**, and is guaranteed to produce the exact same value in the destination type. Other conversions between **arithmetic types** may not always be able to represent the same value exactly:

- If a **negative integer** value is converted to an **unsigned type**, the resulting value corresponds to its 2's complement bitwise representation (i.e., -1 becomes the largest value representable by the type, -2 the second largest, ...).
- The conversions from/to `bool` consider `false` equivalent to **zero** (for numeric types) and to **null pointer** (for pointer types); `tru`e is equivalent to all other values and is converted to the equivalent of 1.
- If the conversion is from a **floating-point type** to an **integer type**, the value is truncated (the decimal part is removed). If the result lies outside the range of representable values by the type, the conversion causes **undefined behavior**.
- Otherwise, if the conversion is between numeric types of the same kind (integer-to-integer or floating-to-floating), the conversion is valid, but the value is implementation-specific (and may not be portable).

总结：上面主要描述的是**conversions between arithmetic types**

Some of these conversions may imply a loss of precision, which the compiler can signal with a warning. This warning can be avoided with an **explicit conversion**.

For **non-fundamental types**, **arrays** and **functions** **implicitly** convert to **pointers**, and **pointers** in general allow the following conversions:

- **Null pointers** can be converted to pointers of any type(null pointer能够convert to如何其他类型的pointer)
- Pointers to any type can be converted to void pointers.
- Pointer upcast: pointers to a **derived class** can be converted to a pointer of an accessible and unambiguous **base class**, without modifying its const or volatile qualification.

## Implicit conversions with classes

In the world of classes, **implicit conversions** can be controlled by means of three **member functions**:

- **Single-argument constructors:** allow **implicit conversion** from a particular type to initialize an object.
- **Assignment operator:** allow **implicit conversion** from a particular type on assignments.
- **Type-cast operator:** allow **implicit conversion** to a particular type.

For example:

```cpp
// implicit conversion of classes:
#include <iostream>
using namespace std;

class A {};

class B {
public:
  // conversion from A (constructor):
  B (const A& x) {
      std::cout<<"copy constructor"<<std::endl;
  }
  // conversion from A (assignment):
  B& operator= (const A& x) {
      std::cout<<"assignment constructor"<<std::endl;
      return *this;
  }
  // conversion to A (type-cast operator)
  operator A() {
    std::cout<<"conversion to A"<<std::endl;
    return A();
  }
};

int main ()
{
  A foo;
  B bar = foo;    // calls constructor
  bar = foo;      // calls assignment
  foo = bar;      // calls type-cast operator
  return 0;
}
```

The **type-cast operator** uses a particular syntax: it uses the `operator` keyword followed by the **destination type** and an empty set of parentheses. Notice that the return type is the **destination type** and thus is not specified before the `operator` keyword.


## Keyword explicit

On a function call, `C++` allows one **implicit conversion** to happen for each **argument**(函数的参数). This may be somewhat problematic for classes, because it is not always what is intended. For example, if we add the following function to the last example:

```cpp
void fn (B arg) {}
```

This function takes an argument of type `B`, but it could as well be called with an object of type `A` as argument:

```cpp
fn (foo);
```

总结：经过验证，这里调用的是copy constructor；那这反映的是c++的什么特性呢？

This may or may not be what was intended. But, in any case, it can be prevented by marking the affected constructor with the `explicit` keyword:

```cpp
// explicit:
#include <iostream>
using namespace std;

class A {};

class B {
public:
  explicit B (const A& x) {}
  B& operator= (const A& x) {return *this;}
  operator A() {return A();}
};

void fn (B x) {}

int main ()
{
  A foo;
  //B bar = foo;    // calls constructor
  B bar (foo);
  bar = foo;
  foo = bar;
  
//  fn (foo);  // not allowed for explicit ctor.
  fn (bar);  

  return 0;
}
```

需要对比这个例子和上一个例子的差异所在，在添加了关键字`explicit`后，现在`B bar = foo;`和`fn (foo);`这些存在隐式转换的写法都是不允许的。

Additionally, constructors marked with `explicit` cannot be called with the assignment-like syntax; In the above example, `bar` could not have been constructed with:

```cpp
B bar = foo;
```

Type-cast member functions (those described in the previous section) can also be specified as `explicit`. This prevents `implicit conversions` in the same way as `explicit`-specified constructors do for the destination type.



# pointer and its type

## [c-pointers-to-any-type](https://stackoverflow.com/questions/2935955/c-pointers-to-any-type)

Yes, you can use a `void*` to point to anything, and then cast it to the proper type when needed (that's how `malloc` and such can work).

`void*` is basically "pointer to an arbitrary block of memory".

## [Any type of pointer can point to anything?](https://stackoverflow.com/questions/19710601/any-type-of-pointer-can-point-to-anything)

Pointers *may* be interchangeable(通用的), but are not required to be(指针可以互换，但不是必需如此).

In particular, on some platforms, certain types need to be aligned to certain byte-boundaries. So while a `char` may be anywhere in memory, an `int` may need to be on a 4-byte boundary.

Another important potential difference is with function-pointers.
Pointers to functions may not be interchangeable with pointers to data-types on many platforms.

It bears repeating: **This is platform-specific**.

I believe Intel x86 architectures treat all pointers the same.
But you may well encounter other platforms where this is not true.

I strongly doubt that on any given platform, casting a pointer from one type to another will modify the **address value**. I understand that dereferencing an unaligned pointer may cause an **exception**, but I doubt the compiler would align the pointer when the cast is done. I can't see any benefits or any motive for this behavior. – [Spidey](https://stackoverflow.com/users/131326/spidey)

Every pointer is of some specific **type**. There's a special generic pointer type `void*` that can point to anything, but you have to convert a `void*` to some specific pointer type before you can **dereference** it.

You can convert a pointer value from one pointer type to another. In most cases, converting a pointer from `foo*` to `bar*` and back to `foo*` will yield the original value -- but that's not actually guaranteed in all cases.

You *can* cause a pointer of type `foo*` to point to an object of type `bar`, but (a) it's usually a bad idea, and (b) in some cases, it may not work (say, if the target types `foo` and `bar` have different sizes or **alignment requirements**).

You can get away with things like:

```cpp
int n = 42;
char *p = (char*)&n;
```

which causes `p` to point to `n` -- but then `*p` doesn't give you the value of `n`, it gives you the value of the first byte of `n` as a `char`.

The differing behavior of **pointer arithmetic** is only part of the reason for having different **pointer types**. It's mostly about *type safety*. If you have a pointer of type `int*`, you can be reasonably sure (unless you've done something unsafe) that it actually points to an `int` object. And if you try to treat it as an object of a different type, the compiler will likely complain about it.

Basically, we have distinct pointer types for the same reasons we have other distinct types: so we can keep track of what kind of value is stored in each object, with help from the compiler.

(There have been languages that only have untyped generic pointers. In such a language, it's more difficult to avoid type errors, such as storing a value of one type and accidentally accessing it as if it were of another type.)

## Type casting

C++ is a **strong-typed** language. Many conversions, specially those that imply a different interpretation of the value, require an explicit conversion, known in C++ as *type-casting*. There exist two main syntaxes for generic type-casting: *functional* and *c-like*:

```cpp
double x = 10.3;
int y;
y = int (x);    // functional notation
y = (int) x;    // c-like cast notation 
```

The functionality of these generic forms of **type-casting** is enough for most needs with fundamental data types. However, these operators can be applied indiscriminately(任意的) on classes and pointers to classes, which can lead to code that -while being syntactically correct- can cause **runtime errors**. For example, the following code compiles without errors: 

```cpp
// class type-casting
#include <iostream>
using namespace std;

class Dummy {
    double i,j;
};

class Addition {
    int x,y;
  public:
    Addition (int a, int b) { x=a; y=b; }
    int result() { return x+y;}
};

int main () {
  Dummy d;
  Addition * padd;
  padd = (Addition*) &d;
  cout << padd->result();
  return 0;
}
```

The program declares a pointer to `Addition`, but then it assigns to it a reference to an object of another unrelated type using **explicit type-casting**:

```cpp
padd = (Addition*) &d;
```

Unrestricted **explicit type-casting** allows to convert any pointer into any other pointer type, independently of the types they point to. The subsequent call to member `result` will produce either a **run-time error** or some other **unexpected results**.

In order to control these types of conversions between classes, we have four specific **casting operators**: `dynamic_cast`, `reinterpret_cast`, `static_cast` and `const_cast`. Their format is to follow the new type enclosed between angle-brackets (`<>`) and immediately after, the expression to be converted between parentheses.

```cpp
dynamic_cast <new_type> (expression)
reinterpret_cast <new_type> (expression)
static_cast <new_type> (expression)
const_cast <new_type> (expression)
```

The traditional **type-casting** equivalents to these expressions would be:

```cpp
(new_type) expression
new_type (expression)
```

but each one with its own special characteristics:

### dynamic_cast

  `dynamic_cast` can only be used with **pointers** and **references** to classes (or with `void*`). Its purpose is to ensure that the result of the **type conversion** points to a valid complete object of the destination **pointer** type.

This naturally includes *pointer upcast* (converting from pointer-to-derived to pointer-to-base), in the same way as allowed as an *implicit conversion*.

But `dynamic_cast` can also *downcast* (convert from pointer-to-base to pointer-to-derived) polymorphic classes (those with virtual members) if -and only if- the pointed object is a valid complete object of the target type. For example:  

```cpp
// dynamic_cast
#include <iostream>
#include <exception>
using namespace std;

class Base { virtual void dummy() {} };
class Derived: public Base { int a; };

int main () {
  try {
    Base * pba = new Derived;
    Base * pbb = new Base;
    Derived * pd;

    pd = dynamic_cast<Derived*>(pba);
    if (pd==0) cout << "Null pointer on first type-cast.\n";

    pd = dynamic_cast<Derived*>(pbb);
    if (pd==0) cout << "Null pointer on second type-cast.\n";

  } catch (exception& e) {cout << "Exception: " << e.what();}
  return 0;
}
```

Wikipedia上关于[downcast](https://en.wikipedia.org/wiki/Downcasting)的介绍。

**Compatibility note:** This type of `dynamic_cast` requires *Run-Time Type Information (RTTI)* to keep track of **dynamic types**. Some compilers support this feature as an option which is disabled by default. This needs to be enabled for **runtime type checking** using `dynamic_cast` to work properly with these types.

The code above tries to perform two **dynamic casts** from pointer objects of type `Base*` (`pba` and `pbb`) to a pointer object of type `Derived*`, but only the first one is successful. Notice their respective initializations:

```cpp
Base * pba = new Derived;
Base * pbb = new Base;
```

  Even though both are pointers of type `Base*`, `pba` actually points to an object of type `Derived`, while `pbb` points to an object of type `Base`. Therefore, when their respective `type-casts` are performed using `dynamic_cast`, `pba` is pointing to a full object of class `Derived`, whereas `pbb` is pointing to an object of class `Base`, which is an incomplete object of class `Derived`.

When `dynamic_cast` cannot cast a pointer because it is not a complete object of the required class -as in the second conversion in the previous example- it returns a *null pointer* to indicate the failure. If `dynamic_cast` is used to convert to a **reference type** and the conversion is not possible, an **exception** of type `bad_cast` is thrown instead.

`dynamic_cast` can also perform the other implicit casts allowed on pointers: casting null pointers between pointers types (even between unrelated classes), and casting any pointer of any type to a `void*` pointer.  

### static_cast

`static_cast` can perform conversions between pointers to related classes, not only *upcasts* (from pointer-to-derived to pointer-to-base), but also *downcasts* (from pointer-to-base to pointer-to-derived). No checks are performed during runtime to guarantee that the object being converted is in fact a full object of the destination type. Therefore, it is up to the **programmer** to ensure that the conversion is safe. On the other side, it does not incur the overhead of the type-safety checks of `dynamic_cast`.

```cpp
class Base {};
class Derived: public Base {};
Base * a = new Base;
Derived * b = static_cast<Derived*>(a);
```

This would be valid code, although `b` would point to an incomplete object of the class and could lead to **runtime errors** if dereferenced.

Therefore, `static_cast` is able to perform with pointers to classes not only the conversions allowed implicitly, but also their opposite conversions.

`static_cast` is also able to perform all conversions allowed implicitly (not only those with pointers to classes), and is also able to perform the opposite of these(static_cast还能够执行所有允许隐式进行的转换（不仅是那些类指针的转换），并且还能够执行与这些转换相反的操作). It can:

- Convert from `void*` to any pointer type. In this case, it guarantees that if the `void*` value was obtained by converting from that same pointer type, the resulting pointer value is the same.
- Convert integers, floating-point values and enum types to enum types.

Additionally, `static_cast` can also perform the following:

- Explicitly call a single-argument constructor or a conversion operator.
- Convert to *rvalue references*.
- Convert `enum class` values into integers or floating-point values.
- Convert any type to `void`, evaluating and discarding the value.

### reinterpret_cast

  `reinterpret_cast` converts any pointer type to any other pointer type, even of unrelated classes. The operation result is a simple **binary copy** of the value from one pointer to the other. All pointer conversions are allowed: neither the content pointed nor the pointer type itself is checked.

It can also cast **pointers** to or from **integer types**. The format in which this **integer value** represents a **pointer** is platform-specific. The only guarantee is that a **pointer** cast to an **integer type** large enough to fully contain it (such as `intptr_t`), is guaranteed to be able to be cast back to a valid pointer.

The conversions that can be performed by `reinterpret_cast` but not by `static_cast` are low-level operations based on reinterpreting the binary representations of the types, which on most cases results in code which is system-specific, and thus non-portable. For example:  

```cpp
class A { /* ... */ };
class B { /* ... */ };
A * a = new A;
B * b = reinterpret_cast<B*>(a);
```

This code compiles, although it does not make much sense, since now `b` points to an object of a totally unrelated and likely incompatible class. Dereferencing `b` is unsafe.

### const_cast

This type of casting manipulates the **constness** of the object pointed by a pointer, either to be set or to be removed. For example, in order to pass a `const` pointer to a function that expects a **non-const** argument:

```cpp
// const_cast
#include <iostream>
using namespace std;

void print (char * str)
{
  cout << str << '\n';
}

int main () {
  const char * c = "sample text";
  print ( const_cast<char *> (c) );
  return 0;
}
```

The example above is guaranteed to work because function `print` does not write to the pointed object(因为函数print不会对传入的参数str进行修改，所以上面的程序是可以正常运行的). Note though, that removing the **constness** of a pointed object to actually write to it causes *undefined behavior*(但请注意，删除指向对象的常量以实际写入它会导致未定义的行为).

## typeid

`typeid` allows to check the type of an expression:

```cpp
typeid (expression)
```

This **operator** returns a reference to a constant object of type `type_info` that is defined in the standard header `<typeinfo>`. A value returned by `typeid` can be compared with another value returned by `typeid` using operators `==` and `!=`or can serve to obtain a **null-terminated character sequence** representing the data type or class name by using its `name()` member.

```cpp
/ typeid
#include <iostream>
#include <typeinfo>
using namespace std;

int main () {
  int * a,b;
  a=0; b=0;
  if (typeid(a) != typeid(b))
  {
    cout << "a and b are of different types:\n";
    cout << "a is: " << typeid(a).name() << '\n';
    cout << "b is: " << typeid(b).name() << '\n';
  }
  return 0;

```

输出如下：

```
a and b are of different types:
a is: int *
b is: int 
```

When `typeid` is applied to classes, `typeid` uses the **RTTI** to keep track of the type of dynamic objects. When `typeid` is applied to an expression whose type is a **polymorphic class**, the result is the type of the most derived complete object:

```
// typeid, polymorphic class
#include <iostream>
#include <typeinfo>
#include <exception>
using namespace std;

class Base { virtual void f(){} };
class Derived : public Base {};

int main () {
  try {
    Base* a = new Base;
    Base* b = new Derived;
    cout << "a is: " << typeid(a).name() << '\n';
    cout << "b is: " << typeid(b).name() << '\n';
    cout << "*a is: " << typeid(*a).name() << '\n';
    cout << "*b is: " << typeid(*b).name() << '\n';
  } catch (exception& e) { cout << "Exception: " << e.what() << '\n'; }
  return 0;
}
```

  *Note: The string returned by member `name` of `type_info` depends on the specific implementation of your compiler and library. It is not necessarily a simple string with its typical type name, like in the compiler used to produce this output.* 

Notice how the type that `typeid` considers for **pointers** is the pointer type itself (both `a` and `b` are of type `class Base *`). However, when `typeid` is applied to objects (like `*a` and `*b`) `typeid` yields their **dynamic type** (i.e. the type of their most derived complete object).

If the type `typeid` evaluates is a pointer preceded by the dereference operator (`*`), and this pointer has a null value, `typeid` throws a `bad_typeid` exception.  


# ==TODO== When should `static_cast`, `dynamic_cast`, `const_cast` and `reinterpret_cast` be used?

https://stackoverflow.com/questions/332030/when-should-static-cast-dynamic-cast-const-cast-and-reinterpret-cast-be-used

# English
premature  过早的

ctor 构造函数

incur 招致

