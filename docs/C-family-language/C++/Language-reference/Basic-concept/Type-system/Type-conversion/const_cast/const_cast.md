# `const_cast`

drop CV是危险的，但是`C++`还是给programmer提供了这种权利，这是遵循C++ philosophy的（`C++\Philosophy.md`）。

## cppreference [`const_cast`](https://en.cppreference.com/w/cpp/language/const_cast) 

only const_cast may be used to cast away (remove) constness or volatility.

> NOTE: 唯一的方法

### NOTE

`const_cast` makes it possible to form a reference or pointer to non-const type that is actually referring to a [const object](https://en.cppreference.com/w/cpp/language/cv) or a reference or pointer to non-volatile type that is actually referring to a [volatile object](https://en.cppreference.com/w/cpp/language/cv). Modifying a const object through a non-const access path and referring to a volatile object through a non-volatile [glvalue](https://en.cppreference.com/w/cpp/language/value_category#glvalue) results in undefined behavior.

> NOTE: 虽然危险，但是给予了programmer这样的权利

https://stackoverflow.com/questions/19554841/how-to-use-const-cast



## modify a const variable

https://www.geeksforgeeks.org/how-to-modify-a-const-variable-in-c/

https://stackoverflow.com/questions/2006161/changing-the-value-of-const-variable-in-c

## Cast away CV

阅读[static_cast conversion](https://en.cppreference.com/w/cpp/language/static_cast)、[dynamic_cast conversion](https://en.cppreference.com/w/cpp/language/dynamic_cast)，可以看到两者都有这样的描述：

> except when such conversions would cast away *constness* or *volatility*.



如果cast away CV，compiler是会报错的：

```c++
static_cast from 'const clone_inherit<concrete, cloneable>' to 'concrete &' casts away qualifiers
```





[reinterpret_cast casts away qualifiers](https://stackoverflow.com/questions/27995692/reinterpret-cast-casts-away-qualifiers)