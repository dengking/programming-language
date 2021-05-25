# Null pointer



## wikipedia [Null pointer](https://en.wikipedia.org/wiki/Null_pointer)



## cppreference [Pointer declaration # Null pointers](https://en.cppreference.com/w/cpp/language/pointer#Null_pointers)

Pointers of every type have a special value known as *null pointer value* of that type. A pointer whose value is null does not point to an object or a function (dereferencing a null pointer is undefined behavior), and compares equal to all pointers of the same type whose value is also *null*.

To initialize a pointer to null or to assign the null value to an existing pointer, the null pointer literal nullptr, the null pointer constant [NULL](https://en.cppreference.com/w/cpp/types/NULL), or the [implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_cast) from the integer value 0 may be used.

[Zero-](https://en.cppreference.com/w/cpp/language/zero_initialization) and [value-initialization](https://en.cppreference.com/w/cpp/language/value_initialization) also initialize pointers to their null values.

Null pointers can be used to indicate the absence of an object (e.g. [`function::target()`](https://en.cppreference.com/w/cpp/utility/functional/function/target)), or as other error condition indicators (e.g. [dynamic_cast](https://en.cppreference.com/w/cpp/language/dynamic_cast)). In general, a function that receives a pointer argument almost always needs to check if the value is null and handle that case differently (for example, the [delete expression](https://en.cppreference.com/w/cpp/language/delete) does nothing when a null pointer is passed).



## CppCoreGuidelines [ES.47: Use `nullptr` rather than `0` or `NULL`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es47-use-nullptr-rather-than-0-or-null)

### Reason

Readability. Minimize surprises: `nullptr` cannot be confused with an `int`. `nullptr` also has a well-specified (very restrictive) type, and thus works in more scenarios where type deduction might do the wrong thing on `NULL` or `0`.

### Example

Consider:

```c++
void f(int);
void f(char*);
f(0);         // call f(int)
f(nullptr);   // call f(char*)
```

### Enforcement

Flag uses of `0` and `NULL` for pointers. The transformation might be helped by simple program transformation.