# Namespaces



## cppreference [Namespaces](https://en.cppreference.com/w/cpp/language/namespace)





### Inline namespaces(since C++11)



### Unnamed namespaces

```C++
inline(optional) namespace attr(optional) { namespace-body }
```

This definition is treated as a definition of a namespace with **unique name** and a *using-directive* in the current scope that nominates（提名、任命） this unnamed namespace (Note: implicitly added using directive makes namespace available for the [qualified name lookup](https://en.cppreference.com/w/cpp/language/qualified_lookup) and [unqualified name lookup](https://en.cppreference.com/w/cpp/language/unqualified_lookup), but not for the [argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl) ).

> NOTE: 这个unique name是由compiler生成的。

```C++
namespace
{
int i;  // defines ::(unique)::i
}
void f()
{
	i++;  // increments ::(unique)::i
}

namespace A
{
namespace
{
int i; // A::(unique)::i
int j; // A::(unique)::j
}
void g()
{
	i++;
} // A::unique::i++
}

using namespace A;
// introduces all names from A into global namespace
void h()
{
	i++;    // error: ::(unique)::i and ::A::(unique)::i are both in scope
	A::i++; // ok, increments ::A::(unique)::i
	j++;    // ok, increments ::A::(unique)::j
}

int main()
{

}
// g++ test.cpp

```

> NOTE: 上述程序编译报错如下:
>
> ```C++
> test.cpp: In function ‘void h()’:
> test.cpp:27:2: error: reference to ‘i’ is ambiguous
>   i++;    // error: ::(unique)::i and ::A::(unique)::i are both in scope
>   ^
> test.cpp:14:5: note: candidates are: int A::{anonymous}::i
>  int i; // A::(unique)::i
>      ^
> test.cpp:3:5: note:                 int {anonymous}::i
>  int i;  // defines ::(unique)::i
> 
> ```



#### Linkage

> NOTE: C++11对name in unnamed namespace的linkage进行了重新定义，这个修改带来了一定的usage，在下面的usage章节进行了描述。

| (until C++11)                                                | (since C++11)                                                |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| Even though names in an unnamed namespace may be declared with external linkage, <br>they are never accessible from other translation units because their namespace name is unique. | Unnamed namespaces as well as all namespaces declared directly or indirectly within an unnamed namespace have [internal linkage](https://en.cppreference.com/w/cpp/language/storage_duration#Linkage), <br>which means that any name that is declared within an unnamed namespace has internal linkage. |



## Usage of unnamed namespaces: make an identifier translation unit local



### stackoverflow [Why are unnamed namespaces used and what are their benefits?](https://stackoverflow.com/questions/357404/why-are-unnamed-namespaces-used-and-what-are-their-benefits)  

[A](https://stackoverflow.com/a/357464)

Unnamed namespaces are a utility to make an identifier **translation unit local**. They behave as if you would choose a **unique name** per **translation unit** for a namespace:

```cpp
namespace unique { /* empty */ }
using namespace unique;
namespace unique { /* namespace body. stuff in here */ }
```

The extra step `using` the empty body is important, so you can already refer within the namespace body to identifiers like `::name` that are defined in that namespace, since the `using` directive already took place.

This means you can have free functions called (for example) `help` that can exist in multiple translation units, and they won't clash(冲突) at link time. The effect is almost identical to using the `static` keyword used in C which you can put in in the declaration of identifiers. **Unnamed namespaces** are a superior alternative, being able to even make a type translation unit local.

> NOTE: linker会进行ODR检查，关于此，参见`C-and-C++\Declaration-and-definition\Definition`

```cpp
namespace { int a1; }
static int a2;
```

Both `a`'s are translation unit local and won't clash at link time. But the difference is that the `a1` in the anonymous namespace gets a unique name.

Read the excellent article at comeau-computing [Why is an unnamed namespace used instead of static?](http://www.comeaucomputing.com/techtalk/#nostatic) ([Archive.org mirror](https://web.archive.org/web/20181115023158/http://www.comeaucomputing.com/techtalk/#nostatic)).

> NOTE：上述链接已经失效 

[A](https://stackoverflow.com/a/357427):

Having something in an anonymous namespace means it's local to this [translation unit](https://en.wikipedia.org/wiki/Translation_unit_(programming)) (`.cpp` file and all its includes) this means that if another symbol with the same name is defined elsewhere there will not be a violation of the [One Definition Rule](http://en.wikipedia.org/wiki/One_Definition_Rule) (ODR).

This is the same as the C way of having a static global variable or static function but it can be used for class definitions as well (and should be used rather than `static` in C++).

All anonymous namespaces in the same file are treated as the same namespace and all anonymous namespaces in different files are distinct. An anonymous namespace is the equivalent of:

```cpp
namespace __unique_compiler_generated_identifer0x42 {
    ...
}
using namespace __unique_compiler_generated_identifer0x42;
```



### stackoverflow [Unnamed/anonymous namespaces vs. static functions](https://stackoverflow.com/questions/154469/unnamed-anonymous-namespaces-vs-static-functions) 



### stackoverflow [Why an unnamed namespace is a “superior” alternative to static? [duplicate]](https://stackoverflow.com/questions/4977252/why-an-unnamed-namespace-is-a-superior-alternative-to-static)

[A](https://stackoverflow.com/a/4977525)

- As you've mentioned, namespace works for anything, not just for functions and objects.

- As Greg has pointed out, `static` means too many things already.

  > [A](https://stackoverflow.com/a/4977267): One reason may be that `static` already has too many meanings (I can count at least three). Since an anonymous namespace can encapsulate anything including types, it seems superior to the `static` solution.

- Namespaces provide a uniform and consistent way of controlling visibility at the global scope. You don't have to use different tools for the same thing.

- When using an anonymous namespace, the function/object name will get mangled properly, which allows you to see something like "`(anonymous namespace)::xyz`" in the symbol table after de-mangling, and not just "`xyz`" with static linkage.

- As pointed out in the comments below, it isn't allowed to use static things as template arguments, while with anonymous namespaces it's fine.

- More? Probably, but I can't think of anything else right now.

## compiler如何实现namespace？

name mangling将namespace添加到其中的每个name中。

> NOTE: 需要添加例子

