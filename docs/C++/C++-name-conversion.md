# C++ name convention

本节对C++ name convention进行描述。



## Header file

C++的header file的name convention是不带`.h`；C++是C的超集，所以它会包含C的header file，对于这种情况，name convention为`c***`:

| C          | C++                                                         |
| ---------- | ----------------------------------------------------------- |
| `string.h` | [`cstring`](http://www.cplusplus.com/reference/cstring/)    |
| `stdlib.h` | [cstdlib](https://en.cppreference.com/w/cpp/header/cstdlib) |





## `_t` and `_type` suffix

参考:

- stackoverflow [C++ type suffix _t, _type or none](https://stackoverflow.com/questions/26400352/c-type-suffix-t-type-or-none)

### `_t` suffix

stackoverflow [C++ type suffix _t, _type or none](https://stackoverflow.com/questions/26400352/c-type-suffix-t-type-or-none): 

in C++14 `_t` suffix is commonly used to indicate an alias template for nested `::type` typedef, e.g. `X_t<T>` stands for `typename X<T>::type` – [Piotr Skotnicki](https://stackoverflow.com/users/3953764/piotr-skotnicki) [Oct 16 '14 at 9:22](https://stackoverflow.com/questions/26400352/c-type-suffix-t-type-or-none#comment41451361_26400352)

[A](https://stackoverflow.com/a/26400432): 

As a C heritage the `_t` (that used to mean "*defined via* `typedef`") syntax has been inherited (they're also [SUS/POSIX-reserved](https://stackoverflow.com/a/3225396/1938163) in the global namespace).



### `_type` suffix

stackoverflow [C++ type suffix _t, _type or none](https://stackoverflow.com/questions/26400352/c-type-suffix-t-type-or-none): 

It seems `_type` is more common in template meta-programming (for template class member typedefs), whereas no suffix is used when it is not important for the outside whether the typename is a class or a typedef. For (template) class member types there just seem to be some edge cases when it isn't clear which would be more appropriate. – [tmlen](https://stackoverflow.com/users/4108376/tmlen) [Oct 16 '14 at 9:19](https://stackoverflow.com/questions/26400352/c-type-suffix-t-type-or-none#comment41451248_26400352) 



[A](https://stackoverflow.com/a/26400652):

there are a few such names (also pointed out in a comment by @jrok): `common_type`, `underlying_type`, `is_literal_type`, `true_type`, `false_type`. In the first three, `_type` is not really a suffix, it's an actual part of the name (e.g. a *metafunction* to give the common *type* or the underlying *type*). With `true_type` and `false_type`, it is indeed a suffix (since `true` and `false` are reserved words). I would say it's a *type* which represents a true/false value in the type-based metaprogramming sense.