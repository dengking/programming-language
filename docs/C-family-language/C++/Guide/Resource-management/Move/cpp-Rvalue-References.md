# [Understanding lvalues and rvalues in C and C++](https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c)

The terms *lvalue* and *rvalue* are not something one runs into often in C/C++ programming, but when one does, it's usually not immediately clear what they mean. The most common place to run into these terms are in compiler error & warning messages. For example, compiling the following with `gcc`:

```C
int foo() {return 2;}

int main()
{
    foo() = 2;

    return 0;
}
```

You get:

```
test.c: In function 'main':
test.c:8:5: error: lvalue required as left operand of assignment
```

True, this code is somewhat perverse and not something you'd write, but the error message mentions *lvalue*, which is not a term one usually finds in C/C++ tutorials. Another example is compiling this code with `g++`:

```c++
int& foo()
{
    return 2;
}
```

Now the error is:

```
testcpp.cpp: In function 'int& foo()':
testcpp.cpp:5:12: error: invalid initialization of non-const reference
of type 'int&' from an rvalue of type 'int'
```

Here again, the error mentions some mysterious *rvalue*. So what do *lvalue* and *rvalue* mean in C and `C++`? This is what I intend to explore in this article.

### A simple definition

This section presents an intentionally simplified definition of *lvalues* and *rvalues*. The rest of the article will elaborate on this definition.

An *lvalue* (*locator value*) represents an object that occupies some **identifiable location in memory** (i.e. has an address).

*rvalues* are defined by exclusion, by saying that every expression is either an *lvalue* or an *rvalue*. Therefore, from the above definition of *lvalue*, an *rvalue* is an expression that *does not* represent an object occupying some identifiable location in memory.

### Basic examples

The terms as defined above may appear vague, which is why it's important to see some simple examples right away.

Let's assume we have an integer variable defined and assigned to:

```c++
int var;
var = 4;
```

An assignment expects an lvalue as its left operand, and `var` is an lvalue, because it is an object with an identifiable memory location. On the other hand, the following are invalid:

```c++
4 = var;       // ERROR!
(var + 1) = 4; // ERROR!
```

Neither the constant `4`, nor the expression `var + 1` are lvalues (which makes them rvalues). They're not lvalues because both are temporary results of expressions, which don't have an **identifiable memory location** (i.e. they can just reside in some **temporary register** for the duration of the computation). Therefore, assigning to them makes no semantic sense - there's nowhere to assign to.

So it should now be clear what the error message in the first code snippet means. `foo` returns a **temporary value** which is an rvalue. Attempting to assign to it is an error, so when seeing `foo() = 2;` the compiler complains that it expected to see an lvalue on the left-hand-side of the assignment statement.

Not all assignments to results of function calls are invalid, however. For example, `C++ ` references make this possible:

```c++
int globalvar = 20;

int& foo()
{
    return globalvar;
}

int main()
{
    foo() = 10;
    return 0;
}
```

Here `foo` returns a reference, *which is an lvalue*, so it can be assigned to. Actually, the ability of `C++` to return **lvalues** from functions is important for implementing some overloaded operators. One common example is overloading the brackets operator `[]` in classes that implement some kind of lookup access. `std::map` does this:

```
std::map<int, float> mymap;
mymap[10] = 5.6;
```

The assignment `mymap[10]` works because the non-const overload of `std::map::operator[]` returns a reference that can be assigned to.

### Modifiable lvalues

Initially when lvalues were defined for C, it literally meant "values suitable for left-hand-side of assignment". Later, however, when ISO C added the `const` keyword, this definition had to be refined. After all:

```
const int a = 10; // 'a' is an lvalue
a = 10;           // but it can't be assigned!
```

So a further refinement had to be added. Not all lvalues can be assigned to. Those that can are called *modifiable lvalues*. Formally, the C99 standard defines modifiable lvalues as:

> [...] an lvalue that does not have array type, does not have an incomplete type, does not have a const-qualified type, and if it is a structure or union, does not have any member (including, recursively, any member or element of all contained aggregates or unions) with a const-qualified type.

### Conversions between lvalues and rvalues

Generally speaking, language constructs operating on object values require rvalues as arguments. For example, the binary addition operator `'+'` takes two rvalues as arguments and returns an rvalue:

```c++
int a = 1;     // a is an lvalue
int b = 2;     // b is an lvalue
int c = a + b; // + needs rvalues, so a and b are converted to rvalues
               // and an rvalue is returned
```

As we've seen earlier, `a` and `b` are both lvalues. Therefore, in the third line, they undergo an implicit *lvalue-to-rvalue conversion*. All lvalues that aren't arrays, functions or of incomplete types can be converted thus to rvalues.

What about the other direction? Can rvalues be converted to lvalues? Of course not! This would violate the very nature of an lvalue according to its definition [[1\]](https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/#id6).

This doesn't mean that lvalues can't be produced from rvalues by more **explicit means**. For example, the unary `'*'` (dereference) operator takes an rvalue argument but produces an lvalue as a result. Consider this valid code:

```c++
int arr[] = {1, 2};
int* p = &arr[0];
*(p + 1) = 10;   // OK: p + 1 is an rvalue, but *(p + 1) is an lvalue
```

***SUMMARY*** : 注意，`p+1`和`*(p + 1)`是两个不同的表达式

Conversely, the unary address-of operator `'&'` takes an lvalue argument and produces an rvalue:

```c++
int var = 10;
int* bad_addr = &(var + 1); // ERROR: lvalue required as unary '&' operand
int* addr = &var;           // OK: var is an lvalue
&var = 40;                  // ERROR: lvalue required as left operand
                            // of assignment
```

The ampersand plays another role in `C++` - it allows to define reference types. These are called "lvalue references". **Non-const lvalue references** cannot be assigned rvalues, since that would require an invalid rvalue-to-lvalue conversion:

```c++
std::string& sref = std::string();  // ERROR: invalid initialization of
                                    // non-const reference of type
                                    // 'std::string&' from an rvalue of
                                    // type 'std::string'
```

**Constant lvalue references** *can* be assigned rvalues. Since they're constant, the value can't be modified through the reference and hence there's no problem of modifying an rvalue. This makes possible the very common C++ idiom of accepting values by **constant references** into functions, which avoids unnecessary copying and construction of temporary objects.

### CV-qualified rvalues

If we read carefully the portion of the C++ standard discussing lvalue-to-rvalue conversions [[2\]](https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/#id7), we notice it says:

> An lvalue (3.10) of a non-function, non-array type T can be converted to an rvalue. [...] If T is a non-class type, the type of the rvalue is the cv-unqualified version of T. Otherwise, the type of the rvalue is T.

What is this "cv-unqualified" thing? *CV-qualifier* is a term used to describe *const* and *volatile* type qualifiers.

From section 3.9.3:

> Each type which is a cv-unqualified complete or incomplete object type or is void (3.9) has three corresponding cv-qualified versions of its type: a *const-qualified* version, a *volatile-qualified* version, and a *const-volatile-qualified* version. [...] The cv-qualified or cv-unqualified versions of a type are distinct types; however, they shall have the same representation and alignment requirements (3.9)

But what has this got to do with rvalues? Well, in C, rvalues never have cv-qualified types. Only lvalues do. In `C++`, on the other hand, class rvalues can have cv-qualified types, but built-in types (like `int`) can't. Consider this example:

```c++
#include <iostream>

class A {
public:
    void foo() const { std::cout << "A::foo() const\n"; }
    void foo() { std::cout << "A::foo()\n"; }
};

A bar() { return A(); }
const A cbar() { return A(); }


int main()
{
    bar().foo();  // calls foo
    cbar().foo(); // calls foo const
}
```

The second call in `main` actually calls the `foo () const` method of `A`, because the type returned by `cbar` is `const A`, which is distinct from `A`. This is exactly what's meant by the last sentence in the quote mentioned earlier. Note also that the return value from `cbar` is an rvalue. So this is an example of a cv-qualified rvalue in action.

### Rvalue references (C++11)

Rvalue references and the related concept of *move semantics* is one of the most powerful new features the C++11 standard introduces to the language. A full discussion of the feature is way beyond the scope of this humble article [[3\]](https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/#id8), but I still want to provide a simple example, because I think it's a good place to demonstrate how an understanding of what lvalues and rvalues are aids our ability to reason about non-trivial language concepts.

I've just spent a good part of this article explaining that one of the main differences between lvalues and rvalues is that lvalues can be modified, and rvalues can't. Well, C++11 adds a crucial twist to this distinction, by allowing us to have references to rvalues and thus modify them, in some special circumstances.

As an example, consider a simplistic implementation of a dynamic "integer vector". I'm showing just the relevant methods here:

```c++
class Intvec
{
public:
    explicit Intvec(size_t num = 0)
        : m_size(num), m_data(new int[m_size])
    {
        log("constructor");
    }

    ~Intvec()
    {
        log("destructor");
        if (m_data) {
            delete[] m_data;
            m_data = 0;
        }
    }

    Intvec(const Intvec& other)
        : m_size(other.m_size), m_data(new int[m_size])
    {
        log("copy constructor");
        for (size_t i = 0; i < m_size; ++i)
            m_data[i] = other.m_data[i];
    }

    Intvec& operator=(const Intvec& other)
    {
        log("copy assignment operator");
        Intvec tmp(other);
        std::swap(m_size, tmp.m_size);
        std::swap(m_data, tmp.m_data);
        return *this;
    }
private:
    void log(const char* msg)
    {
        cout << "[" << this << "] " << msg << "\n";
    }

    size_t m_size;
    int* m_data;
};
```

So, we have the usual constructor, destructor, copy constructor and copy assignment operator [[4\]](https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/#id9) defined, all using a logging function to let us know when they're actually called.

Let's run some simple code, which copies the contents of `v1` into `v2`:

```
Intvec v1(20);
Intvec v2;

cout << "assigning lvalue...\n";
v2 = v1;
cout << "ended assigning lvalue...\n";
```

What this prints is:

```
assigning lvalue...
[0x28fef8] copy assignment operator
[0x28fec8] copy constructor
[0x28fec8] destructor
ended assigning lvalue...
```

Makes sense - this faithfully represents what's going on inside `operator=`. But suppose that we want to assign some rvalue to `v2`:

```
cout << "assigning rvalue...\n";
v2 = Intvec(33);
cout << "ended assigning rvalue...\n";
```

Although here I just assign a freshly constructed vector, it's just a demonstration of a more general case where some temporary rvalue is being built and then assigned to `v2` (this can happen for some function returning a vector, for example). What gets printed now is this:

```
assigning rvalue...
[0x28ff08] constructor
[0x28fef8] copy assignment operator
[0x28fec8] copy constructor
[0x28fec8] destructor
[0x28ff08] destructor
ended assigning rvalue...
```

Ouch, this looks like a lot of work. In particular, it has one extra pair of constructor/destructor calls to create and then destroy the temporary object. And this is a shame, because inside the **copy assignment operator**, *another* temporary copy is being created and destroyed. That's extra work, for nothing.

Well, no more. `C++11` gives us rvalue references with which we can implement "move semantics", and in particular a "move assignment operator" [[5\]](https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/#id10). Let's add another `operator=` to `Intvec`:

```c++
Intvec& operator=(Intvec&& other)
{
    log("move assignment operator");
    std::swap(m_size, other.m_size);
    std::swap(m_data, other.m_data);
    return *this;
}
```

The `&&` syntax is the new *rvalue reference*. It does exactly what it sounds it does - gives us a reference to an rvalue, which is going to be destroyed after the call. We can use this fact to just "steal" the internals of the rvalue - it won't need them anyway! This prints:

```
assigning rvalue...
[0x28ff08] constructor
[0x28fef8] move assignment operator
[0x28ff08] destructor
ended assigning rvalue...
```

What happens here is that our new move assignment operator is invoked since an rvalue gets assigned to `v2`. The constructor and destructor calls are still needed for the temporary object that's created by `Intvec(33)`, but another temporary inside the assignment operator is no longer needed. The operator simply switches the rvalue's internal buffer with its own, arranging it so the rvalue's destructor will release our object's own buffer, which is no longer used. Neat.

I'll just mention once again that this example is only the tip of the iceberg on move semantics and rvalue references. As you can probably guess, it's a complex subject with a lot of special cases and gotchas to consider. My point here was to demonstrate a very interesting application of the difference between lvalues and rvalues in `C++`. The compiler obviously knows when some entity is an rvalue, and can arrange to invoke the correct constructor at compile time.

### Conclusion

One can write a lot of C++ code without being concerned with the issue of rvalues vs. lvalues, dismissing them as weird compiler jargon in certain error messages. However, as this article aimed to show, getting a better grasp of this topic can aid in a deeper understanding of certain C++ code constructs, and make parts of the C++ spec and discussions between language experts more intelligible.

Also, in the new C++ spec this topic becomes even more important, because C++11's introduction of rvalue references and move semantics. To really grok this new feature of the language, a solid understanding of what rvalues and lvalues are becomes crucial.



| [[1\]](https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/#id1) | rvalues can be assigned to lvalues explicitly. The lack of implicit conversion means that rvalues cannot be used in places where lvalues are expected. |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
|                                                              |                                                              |

| [[2\]](https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/#id2) | That's section 4.1 in the new C++11 standard draft. |
| ------------------------------------------------------------ | --------------------------------------------------- |
|                                                              |                                                     |

| [[3\]](https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/#id3) | You can find a lot of material on this topic by simply googling "rvalue references". Some resources I personally found useful: [this one](http://www.artima.com/cppsource/rvalue.html), [and this one](http://stackoverflow.com/questions/5481539/what-does-t-mean-in-c0x), and [especially this one](http://thbecker.net/articles/rvalue_references/section_01.html). |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
|                                                              |                                                              |

| [[4\]](https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/#id4) | This a canonical implementation of a copy assignment operator, from the point of view of exception safety. By using the copy constructor and then the non-throwing `std::swap`, it makes sure that no intermediate state with uninitialized memory can arise if exceptions are thrown. |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
|                                                              |                                                              |

| [[5\]](https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/#id5) | So now you know why I was keeping referring to my `operator=` as "copy assignment operator". In C++11, the distinction becomes important. |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
|                                                              |                                                              |

