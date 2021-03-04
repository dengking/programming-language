# Attribute

这是c++11引入的新特性，统一了attribute的语法。

## wikipedia [C++11#Attributes](https://en.wikipedia.org/wiki/C++11#Attributes)

C++11 provides a standardized syntax for compiler/tool extensions to the language. Such extensions were traditionally specified using `#pragma` directive or vendor-specific keywords (like `__attribute__` for GNU and `__declspec` for Microsoft). With the new syntax, added information can be specified in a form of an attribute enclosed in double square brackets. 

> NOTE: 提供uniform syntax

An attribute can be applied to various elements of source code:

```C++
int [[attr1]] i [[attr2, attr3]];

[[attr4(arg1, arg2)]] if (cond)
{
    [[vendor::attr5]] return i;
}
```

In the example above, 

attribute `attr1` applies to the type of variable `i`, 

`attr2` and `attr3` apply to the variable itself, 

`attr4` applies to the `if` statement and 

`vendor::attr5` applies to the return statement. 

In general (but with some exceptions), an attribute specified for a named entity is placed **after** the name, and before the entity otherwise, as shown above, several attributes may be listed inside one pair of double square brackets, added arguments may be provided for an attribute, and attributes may be scoped by vendor-specific attribute namespaces.

It is recommended that attributes have no language semantic meaning and do not change the sense of a program when ignored. Attributes can be useful for providing information that, for example, helps the compiler to issue better diagnostics or optimize the generated code.

C++11 provides two standard attributes itself: 

### `noreturn`

`noreturn` to specify that a function does not return, and 

### `carries_dependency`

`carries_dependency` to help optimizing multi-threaded code by indicating that function arguments or return value carry a dependency.[*[clarification needed](https://en.wikipedia.org/wiki/Wikipedia:Please_clarify)*]

## cppreference [attribute specifier sequence](https://en.cppreference.com/w/cpp/language/attributes)



### Explanation

Attributes provide the unified standard syntax for implementation-defined language extensions, such as the GNU and IBM language extensions `__attribute__((...))`, Microsoft extension `__declspec()`, etc.

### Standard attributes

Only the following attributes are defined by the C++ standard.

|                                                           |                                                              |
| --------------------------------------------------------- | ------------------------------------------------------------ |
| `[[noreturn]]`(C++11)                                     | indicates that the function does not return                  |
| `[[carries_dependency]]`(C++11)                           | indicates that dependency chain in release-consume [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) propagates in and out of the function |
| `[[deprecated]]`(C++14) `[[deprecated("reason")]]`(C++14) | indicates that the use of the name or entity declared with this attribute is allowed, but discouraged for some *reason* |
| `[[fallthrough]]`(C++17)                                  | indicates that the fall through from the previous case label is intentional and should not be diagnosed by a compiler that warns on fall-through |
| `[[nodiscard]]`(C++17) `[[nodiscard("reason")]]`(C++20)   | encourages the compiler to issue a warning if the return value is discarded |
| `[[maybe_unused]]`(C++17)                                 | suppresses compiler warnings on unused entities, if any      |
| `[[likely]]`(C++20) `[[unlikely]]`(C++20)                 | indicates that the compiler should optimize for the case where a path of execution through a statement is more or less likely than any other path of execution |
| `[[no_unique_address]]`(C++20)                            | indicates that a non-static data member need not have an address distinct from all other non-static data members of its class |
| `[[optimize_for_synchronized]]`(TM TS)                    | indicates that the function definition should be optimized for invocation from a [synchronized statement](https://en.cppreference.com/w/cpp/language/transactional_memory) |

### External links

#### clang

clang [Attributes in Clang](https://clang.llvm.org/docs/AttributeReference.html)

#### gcc

gcc [6 Extensions to the C Language Family](https://gcc.gnu.org/onlinedocs/gcc/C-Extensions.html#C-Extensions)

## microsoft [Attributes in C++](https://docs.microsoft.com/en-us/cpp/cpp/attributes?view=msvc-160)

### C++ Standard Attributes

### `[[noreturn]]`



### `[[carries_dependency]]`