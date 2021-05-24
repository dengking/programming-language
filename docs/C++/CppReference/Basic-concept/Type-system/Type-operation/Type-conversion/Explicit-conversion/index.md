# Explicit type conversion



## cppreference [Explicit type conversion](https://en.cppreference.com/w/cpp/language/explicit_cast)

Converts between types using a combination of explicit and implicit conversions.

> NOTE: 
>
> 1、其中也会使用implicit conversion

### Syntax

Returns a value of type `new_type`.

> NOTE: 
>
> 1、显然会构造一个新的object

|                                               |      |       |
| --------------------------------------------- | ---- | ----- |
| `( new_type ) expression`                     | (1)  |       |
| `( new_type ( expression )`                   | (2)  |       |
| `( new_type ( arg1, arg2, ... )`              | (3)  |       |
| `new_type ( )`                                | (4)  |       |
| `new_type { arg1, arg2, ...(optional) }`      | (5)  | C++11 |
| `template-name ( arg1, arg2, ...(optional) )` | (6)  | C++17 |
| `template-name { arg1, arg2, ...(optional) }` | (7)  | C++17 |



## CppCoreGuidelines [C.164: Avoid implicit conversion operators](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c164-avoid-implicit-conversion-operators)



```c++
struct S1 {
    string s;
    // ...
    operator char*() { return s.data(); }  // BAD, likely to cause surprises
};

struct S2 {
    string s;
    // ...
    explicit operator char*() { return s.data(); }
};

void f(S1 s1, S2 s2)
{
    char* x1 = s1;     // OK, but can cause surprises in many contexts
    char* x2 = s2;     // error (and that's usually a good thing)
    char* x3 = static_cast<char*>(s2); // we can be explicit (on your head be it)
}
```

